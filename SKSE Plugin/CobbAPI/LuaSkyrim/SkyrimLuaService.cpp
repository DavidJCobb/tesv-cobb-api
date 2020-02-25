#include "SkyrimLuaService.h"
#include <fstream>
#include "_includes.h"
#include "_utilities.h"
#include "Miscellaneous/strings.h"

#include "FormWrappers/IForm.h"
#include "FormWrappers/IActor.h"
#include "FormWrappers/IActorBase.h"
#include "FormWrappers/IEffectShader.h"
#include "FormWrappers/IGlobal.h"
#include "FormWrappers/IRace.h"
#include "FormWrappers/IReference.h"
#include "Hooks.h"

#include "skse/GameForms.h" // LookupFormByID
#include "skse/Utilities.h" // GetRuntimeDirectory
#include "ReverseEngineered/Systems/BSTEvent.h"
#include "ReverseEngineered/UI/Miscellaneous.h"

using namespace LuaSkyrim;

wrapped_lua_pointer::wrapped_lua_pointer() {
   auto& service = SkyrimLuaService::GetInstance();
   if (!service.accessLock.try_lock()) {
      _MESSAGE("WARNING: Attempted to access the SkyrimLuaService state when something else is already accessing it!");
      return;
   }
   this->state = service.state;
}
wrapped_lua_pointer::~wrapped_lua_pointer() {
   auto& service = SkyrimLuaService::GetInstance();
   service.accessLock.unlock();
   if (service.accessLock.try_lock()) { // if we managed to unlock it, then this will lock it again and return true
      service.accessLock.unlock();
      service.onLuaCodeDone();
   }
}

namespace { // utility tools
   const std::string& GetAddonBasePath() {
      static std::string path;
      if (path.empty()) {
         std::string	runtimePath = GetRuntimeDirectory();
         if (!runtimePath.empty()) {
            path = runtimePath;
            path += "Data\\scripts\\CobbAPILua\\";
         }
      }
      return path;
   };
   const std::string& GetScriptBasePath() {
      static std::string path;
      if (path.empty()) {
         std::string	runtimePath = GetRuntimeDirectory();
         if (!runtimePath.empty()) {
            path = runtimePath;
            path += "Data\\CobbAPILua\\";
         }
      }
      return path;
   };
}
namespace { // APIs provided to Lua; we're gonna change how these work in the future
   namespace _globals {
      static luastackchange_t LuaLog(lua_State* L) {
         int n = lua_gettop(L); // number of arguments
         //
         const char* out = nullptr;
         if (!lua_isstring(L, 1)) {
            lua_pushliteral(L, "incorrect argument");
            lua_error(L);
         } else {
            out = lua_tolstring(L, 1, nullptr);
            _MESSAGE("Logging message from a Lua script: %s", out);
         }
         /*// SAMPLE CODE -- Loop over the arguments:
         for (int i = 2; i <= n; i++) {
            if (!lua_isnumber(L, i)) {
               lua_pushliteral(L, "incorrect argument");
               lua_error(L);
            }
            lua_tonumber(L, i);
         }
         //
         // Returning two numbers:
         //
         lua_pushnumber(L, sum / n);
         lua_pushnumber(L, sum);
         return 2;
         //*/
         return 0; // return the number of return values, after pushing return values onto the Lua stack
      }
      static luastackchange_t FormByID(lua_State* L) {
         int n = lua_gettop(L); // number of arguments
         //
         if (!lua_isnumber(L, 1)) {
            lua_pushliteral(L, "incorrect argument");
            lua_error(L);
            return 0;
         }
         lua_Number num = lua_tonumber(L, 1);
         UInt32 id = num;
         TESForm* form = ::LookupFormByID(id);
         return wrapForm(L, form);
      }
      static luastackchange_t ShowNotification(lua_State* L) {
         int n = lua_gettop(L); // number of arguments
         //
         if (!lua_isstring(L, 1)) {
            lua_pushliteral(L, "incorrect argument");
            lua_error(L);
            return 0;
         }
         const char* message = lua_tostring(L, 1);
         const char* soundID = nullptr;
         if (lua_isstring(L, 2)) {
            soundID = lua_tostring(L, 2);
         }
         RE::ShowNotification(message, soundID, 1);
      }
   }
}

namespace {
   //
   // SKSE's form-delete hook fires on a bunch of stuff that *isn't* strictly 
   // form deletion. The Papyrus VM uses the form-delete BSTEvent sink to do 
   // some processing -- clearing script handles and I think other stuff -- 
   // but that same processing is done in response to other events as well, 
   // and that's what SKSE hooked. As such, we can't use SKSE's form-delete 
   // hook; if we do, we get lots of false-positives e.g. the player being 
   // repeatedly "deleted" during play. The form-delete BSTEvent source is 
   // upstream from SKSE's hook site and is reliable for our purposes.
   //
   class LuaFormDeleteSink : public RE::BSTEventSink<RE::TESFormDeleteEvent> {
      public:
         virtual EventResult Handle(void* aEv, void* aSource) override {
            auto ev = convertEvent(aEv);
            if (ev)
               SkyrimLuaService::GetInstance().OnReferenceDelete(ev->refrFormID);
            return EventResult::kEvent_Continue;
         };
   };
   static LuaFormDeleteSink s_luaFormDeleteSink;
}
SkyrimLuaService::SkyrimLuaService() {
   CALL_MEMBER_FN(&RE::BSTEventSourceHolder::GetOrCreate()->formDelete, AddEventSink)(&s_luaFormDeleteSink);
}

void SkyrimLuaService::loadAddonScript(SkyrimLuaService::Addon& addon, std::string path) {
   //_MESSAGE("Lua: Loading script <%s> for addon <%s>", path.c_str(), addon.name.c_str());
   auto luaVM = this->state;
   int status = luaL_loadfile(luaVM, path.c_str());
   if (status) {
      _MESSAGE("Missing script file for add-on <%s>: %s", addon.name.c_str(), lua_tostring(luaVM, -1)); // loadfile logs error messages to the top of Lua's stack
      lua_pop(luaVM, 1);
      return;
   }
   //
   // The script file is now an anonymous function loaded at stack position -1.
   //
   auto stackSizePrior = lua_gettop(luaVM); // scripts are anonymous functions and can return an arbitrary number of values, which we must discard
   int  result = util::safeCall(luaVM, 0, LUA_MULTRET); // boilerplate for pcall
   if (result) {
      _MESSAGE("Failed to run script for add-on <%s>: %s", addon.name.c_str(), lua_tostring(luaVM, -1));
      lua_settop(luaVM, stackSizePrior);
      return;
   }
}
bool SkyrimLuaService::loadAddonScriptFiles(Addon& addon) {
   enum Section {
      kSection_None = 0, // add-on metadata goes here
      kSection_Unknown = 1,
      kSection_Files = 2,
   };
   //
   if (addon.loaded || addon.failed)
      return addon.loaded;
   if (!this->currentAddon.size())
      this->currentAddon = addon.name;
   else if (this->currentAddon == addon.name) {
      _MESSAGE("Unable to load add-on <%s> or any add-ons that depend on it (directly or indirectly) due to a circular dependency.", addon.name.c_str());
      return false;
   }
   if (addon.dependencies.size()) {
      //
      // Force dependencies to load first. Recurse as necessary so that the 
      // entire dependency tree loads first.
      //
      for (auto it = addon.dependencies.begin(); it != addon.dependencies.end(); it++) {
         auto& dname = *it;
         try {
            auto& target = this->addons.at(dname);
            if (!this->loadAddonScriptFiles(target)) {
               addon.failed = true;
               return false;
            }
         } catch (std::out_of_range) { // dependency not present
            addon.failed = true;
            return false;
         }
      }
   }
   this->currentAddon = "";
   //
//_MESSAGE("Lua: Loading script files for add-on <%s>.", addon.name.c_str());
   std::ifstream file;
   file.open(addon.folder + "manifest.txt");
   if (!file) {
      _MESSAGE("Lua: The manifest file for add-on <%s> went missing during load.", addon.name.c_str());
      addon.failed = true;
      return false;
   }
   Section section = kSection_None;
   while (!file.bad() && !file.eof()) {
      char buffer[1024];
      file.getline(buffer, sizeof(buffer));
      buffer[1023] = '\0';
      //
      UInt32 i = 0;
      char   c = buffer[0];
      if (!c)
         continue;
      bool whitespace = true;
      do {
         if (!isspace(c))
            break;
      } while (++i < sizeof(buffer) && (c = buffer[i]));
      if (!c) // no non-whitespace content on line
         continue;
      // i == index of first non-whitespace character
      {  // Deal with comments.
         auto delim = strchr(buffer + i, ';');
         if (delim)
            *delim = '\0';
      }
      if (c == '[') {
         std::string header;
         auto delim = strchr(buffer + i, ']');
         if (!delim)
            continue;
         header.assign(buffer + i + 1, delim - (buffer + i + 1));
         if (!header.size())
            continue;
         auto h = header.c_str();
         auto s = header.size();
         if (_strnicmp(h, "files", s) == 0) {
            section = kSection_Files;
         } else {
            section = kSection_Unknown;
         }
         continue;
      }
      if (section == kSection_None) {
         continue;
      } else if (section == kSection_Files) {
         if (c == '/' || c == '\\') // absolute paths are not allowed
            continue;
         if (strpbrk(buffer + i, "<>:\"|?*")) // characters with disallowed special meanings
            continue;
         std::string relpath = buffer + i;
         cobb::rtrim(relpath); // left-hand side is already trimmed since we skipped whitespace
         if (!relpath.size())
            continue;
         if (_strnicmp(relpath.c_str() + relpath.size() - 4, ".lua", 4) != 0) // make sure it's a Lua file
            continue;
         {  // check for relative directory segments
            bool   found = false;
            UInt32 i    = 0;
            UInt32 last = 0;
            UInt32 size = relpath.size() - 4; // no need to iterate up to the file extension
            do {
               char c = relpath[i];
               if (c == '/' || c == '\\')
                  last = i;
               else if (c == '.') {
                  if (i + 2 >= size)
                     break;
                  char d = relpath[i + 1];
                  char e = relpath[i + 2];
                  if (d == '.' && (e == '/' || e == '\\')) { // ".." == up one level
                     found = true;
                     continue;
                  }
                  if (d == '/' || d == '\\') { // "." == current level
                     found = true;
                     continue;
                  }
               }
            } while (++i < size);
            if (found) {
               _MESSAGE("Rejected suspicious script file path (contains relative directory segments e.g. \"..\"): <%s>", relpath.c_str());
               continue;
            }
         }
         this->loadAddonScript(addon, addon.folder + relpath);
      }
   }
   //
   file.close();
   addon.loaded = true;
   //
   // TODO: fire an "OnAddonLoaded" event in Lua so that add-ons have a convenient way to 
   // run code when all of their script files are loaded (i.e. without having to put a 
   // file at the end of their lists that just exists to fire such an event manually).
   //
   return true;
}
void SkyrimLuaService::loadAddonMetadata(std::string& folder) {
   enum Section {
      kSection_None     = 0, // add-on metadata goes here
      kSection_Unknown  = 1,
      kSection_Files    = 2,
   };
   //
   std::ifstream file;
//_MESSAGE("Lua: loading manifest at <%s>...", folder + "manifest.txt");
   file.open(folder + "manifest.txt");
   if (!file) { // empty folder or not an add-on folder
      return;
   }
   Section section = kSection_None;
   Addon   addon;
   addon.folder = folder;
   while (!file.bad() && !file.eof()) {
      char buffer[1024];
      file.getline(buffer, sizeof(buffer));
      buffer[1023] = '\0';
      //
      UInt32 i = 0;
      char   c = buffer[0];
      if (!c)
         continue;
      bool whitespace = true;
      do {
         if (!isspace(c))
            break;
      } while (++i < sizeof(buffer) && (c = buffer[i]));
      if (!c) // no non-whitespace content on line
         continue;
      // i == index of first non-whitespace character
      {  // Deal with comments.
         auto delim = strchr(buffer + i, ';');
         if (delim)
            *delim = '\0';
      }
      if (c == '[') {
         if (!addon.name.size()) {
            //
            // TODO: log error: no name given
            //
            return;
         }
         std::string header;
         auto delim = strchr(buffer + i, ']');
         if (!delim)
            continue;
         header.assign(buffer + i + 1, delim - (buffer + i + 1));
         if (!header.size())
            continue;
         auto h = header.c_str();
         auto s = header.size();
         if (_strnicmp(h, "files", s) == 0) {
            section = kSection_Files;
         } else {
            section = kSection_Unknown;
         }
         continue;
      }
      if (section == kSection_None) {
         if (c == '=') // zero-length key
            continue;
         std::string key;
         std::string value;
         auto delim = strchr(buffer + i, '=');
         if (!delim)
            continue;
         key.assign(buffer + i, delim - (buffer + i));
         //value.assign(delim + 1);
         //
         auto k = key.c_str();
         auto s = key.size();
         if (_strnicmp(k, "name", s) == 0) {
            addon.name = delim + 1;
         } else if (_strnicmp(k, "author", s) == 0) {
            addon.author = delim + 1;
         } else if (_strnicmp(k, "description", s) == 0) {
            addon.description = delim + 1;
         } else if (_strnicmp(k, "version", s) == 0) {
            char* end = nullptr;
            addon.version = (delim + 1, &end, 10);
            if (!end) {
               //
               // TODO: bad version
               //
            }
         } else if (_strnicmp(k, "dependencies", s) == 0) {
            std::string v = delim + 1;
            UInt32 s = v.size();
            UInt32 last = 0;
            for (UInt32 i = 0; i <= s; i++) {
               if (v[i] == ',' || v[i] == '\0') { // handling the end-of-string means we don't need to duplicate code to get everything after the last ','
                  std::string var = v.substr(last, i - last);
                  last = i + 1;
                  cobb::trim(var);
                  if (!var.size())
                     continue;
                  addon.dependencies.push_back(var);
               }
            }
         } else if (_strnicmp(k, "savevars", s) == 0) { // comma-separated list of globals
            std::string v = delim + 1;
            UInt32 s    = v.size();
            UInt32 last = 0;
            for (UInt32 i = 0; i <= s; i++) {
               if (v[i] == ',' || v[i] == '\0') { // handling the end-of-string means we don't need to duplicate code to get everything after the last ','
                  std::string var = v.substr(last, i - last);
                  last = i + 1;
                  cobb::trim(var);
                  if (!var.size())
                     //
                     // TODO: warn
                     //
                     continue;
                  //
                  // TODO: Should we limit these to names allowed in global variables? 
                  // You *can* use any non-null glyph in a variable name e.g. _G[" myVar"]
                  //
                  addon.savevars.push_back(var);
               }
            }
         } else {
            //
            // TODO: warn on unrecognized metadata key
            //
         }
         continue;
      } else if (section == kSection_Files) {
         continue; // we load the script files on the second pass
      }
   }
   this->addons[addon.name] = addon;
   //
   file.close();
}
void SkyrimLuaService::loadAddons() {
   //
   // The only way to do things like a "DependsOn" metadata key would be to:
   //
   //  - Open each addon's manifest file once, to load the metadata and save it 
   //    into memory. Do not load script files.
   //
   //  - Loop over all add-ons and ditch any whose dependencies are not available. 
   //    Sort add-ons, also, so that each add-on comes after its dependencies in 
   //    the list.
   //
   //  - Open each remaining addon's manifest file again and load the script files.
   //
   _MESSAGE("Lua: Loading add-ons...");
   clock_t begin = clock();
   this->addons.clear();
   //
   WIN32_FIND_DATA state;
   HANDLE handle;
   {
      std::string a = GetAddonBasePath() + '*';
      handle = FindFirstFileA(a.c_str(), &state);
   }
   if (handle == INVALID_HANDLE_VALUE) {
      auto e = GetLastError();
      switch (e) {
         case ERROR_FILE_NOT_FOUND:
            break;
         case ERROR_PATH_NOT_FOUND:
            _MESSAGE("The add-ons path doesn't exist.");
            break;
         default:
            _MESSAGE("FindFirstFile failed (%d)", e);
      }
      _MESSAGE(" - Done.");
      return;
   }
   do {
      if (state.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
         if (state.cFileName[0] == '.') // FindNextFileA returns the "." and ".." pseudo-directories; really dumb
            continue;
         this->loadAddonMetadata(GetAddonBasePath() + state.cFileName + "\\");
      }
   } while (FindNextFileA(handle, &state));
   FindClose(handle);
   for (auto it = this->addons.begin(); it != this->addons.end(); ++it) {
      this->loadAddonScriptFiles(it->second); // if the addon has dependencies, those are forced to load first
   }
   clock_t end = clock();
   _MESSAGE(" - Loaded all add-ons. Time taken: %fs", (double)(end - begin) / CLOCKS_PER_SEC);
}

void SkyrimLuaService::onLuaCodeDone() {
   auto luaVM = this->state; // Don't use wrapped_lua_pointer here; the wrapper's destructor is what calls this function in the first place.
   if (!luaVM) // some hooks can fire before we've actually fired up the Lua VM
      return;
   lua_checkstack(luaVM, 3);
   //
   // Iterate over all form wrappers (IForm instances) kept in Lua. If a wrapper 
   // represents a form that can unload at run-time, then tell it to abandon its 
   // pointer to that form; the wrapper will re-obtain that pointer the next time 
   // Lua is running and a script code uses it.
   //
   // TODO: The table we're iterating over here is a list of ALL form wrappers; 
   // wouldn't it be more efficient to have two lists which we keep in synch, one 
   // for all form wrappers and another for just the ones that can unload? If 
   // we use two lists, then OnReferenceDeleted should use the can-unload list as 
   // well.
   //
   lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_formWrapperReuseKey); // STACK: [list]
   if (lua_isnil(luaVM, -1)) {
      _MESSAGE("LUA: INTERNAL WARNING: The form wrapper reuse table is missing!");
      return;
   }
   lua_pushnil(luaVM);
   while (lua_next(luaVM, -2) != 0) {
      // STACK: [value:userdata<IForm>, key:formID, list]
      void* userdata = lua_touserdata(luaVM, -1);
      if (userdata) {
         auto wrap = (IForm*)userdata;
         if (wrap->wrapped && wrap->canUnload)
            wrap->abandon();
      } else if (!lua_isnoneornil(luaVM, -1))
         _MESSAGE("LUA: INTERNAL WARNING: onLuaCodeDone: Failed to lookup IForm for form ID %08X", lua_tonumber(luaVM, -2));
      lua_pop(luaVM, 1); // pop only the value; key is used by lua_next to iterate
   }
}

void SkyrimLuaService::StartVM() {
   std::lock_guard<decltype(this->setupLock)> guard(this->setupLock);
   if (this->state)
      return;
   _MESSAGE("Lua: VM is starting...");
   //
   lua_State* luaVM = luaL_newstate(); // create a VM instance
   this->state    = luaVM;
   this->threadID = GetCurrentThreadId();
   wrapped_lua_pointer luaVMWrapper; // Create this just to ensure we run onLuaCodeDone when we're done here.
   //
   // Set up metatables and similar for all native types that we plan to expose to Lua:
   //
   // DO NOT FORGET TO CALL THESE
   //
   IForm::setupClass(luaVM);
   /**/IActorBase::setupClass(luaVM);
   /**/IEffectShader::setupClass(luaVM);
   /**/IGlobal::setupClass(luaVM);
   /**/IRace::setupClass(luaVM);
   /**/IReference::setupClass(luaVM);
   /**//**/IActor::setupClass(luaVM);
   //
   HookManager::attach(luaVM);
   //
   // TODO: all APIs should be stored in the registry and made available to loaded 
   // scripts via lua_setlocal, to ensure that one script cannot clobber the globals 
   // and thus interfere with other scripts' API access
   //
   lua_register(luaVM, "logmessage", _globals::LuaLog); // make a C function available to the Lua script under the name "logmessage"
   lua_register(luaVM, "form_by_id", _globals::FormByID);
   lua_register(luaVM, "show_notification", _globals::ShowNotification);
   //
   // Make the standard libraries available to scripts. There are 
   // some that we want to omit to avoid threading issues and similar.
   //
   luaL_requiref(luaVM, "_G", luaopen_base, true); // TODO: shim or block: dofile, load, loadfile, print
   lua_pop(luaVM, 1); // pop library off the stack
   luaL_requiref(luaVM, "math", luaopen_math, true);
   lua_pop(luaVM, 1); // pop library off the stack
   luaL_requiref(luaVM, "string", luaopen_string, true);
   lua_pop(luaVM, 1); // pop library off the stack
   luaL_requiref(luaVM, "table", luaopen_table, true);
   lua_pop(luaVM, 1); // pop library off the stack
   luaL_requiref(luaVM, "utf8", luaopen_utf8, true);
   lua_pop(luaVM, 1); // pop library off the stack
   util::loadPartialLibrary(luaVM, "os", luaopen_os, { "clock", "difftime", "time" });
   lua_pop(luaVM, 1); // pop library off the stack
   util::loadPartialLibrary(luaVM, "debug", luaopen_debug, { "getmetatable", "setmetatable", "traceback" });
   lua_pop(luaVM, 1); // pop library off the stack
   //
   // load add-ons
   //
   _MESSAGE("Lua: Attempting to load add-ons...");
   this->loadAddons();
   _MESSAGE("Lua: Add-ons loaded.");
}
void SkyrimLuaService::StopVM() {
   std::lock_guard<decltype(this->setupLock)> guard(this->setupLock);
   if (!this->state)
      return;
   _MESSAGE("Lua: VM is shutting down...");
   lua_close(this->state); // should also kill child threads
   //lua_unlock(this->state); // uncomment if using the lua_lock and lua_unlock macros // lua_close doesn't call this on its own, when it REALLY SHOULD
   this->state    = nullptr;
   this->threadID = 0;
}

void SkyrimLuaService::OnReferenceDelete(UInt32 formID) {
//_MESSAGE("[THREAD %08X] Lua VM was notified about the deletion of form %08X...", GetCurrentThreadId(), formID);
   if (formID == 0x14)
      _MESSAGE("Lua VM was notified about the deletion of the player-ref.");
   if (this->setupLock.try_lock())
      this->setupLock.unlock();
   else
      return;
   //
   auto luaVM = this->state;
   if (!luaVM)
      return;
//_MESSAGE(" - Responding...");
   if (lua_status(luaVM) != LUA_OK) {
      _MESSAGE(" - WARNING: VM thread is not OK!");
   }
   //
   //_MESSAGE("SkyrimLuaService::OnReferenceDelete: Deleting reference with form ID %08X", formID);
   lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_formWrapperReuseKey); // STACK: [list]
   if (lua_rawgeti(luaVM, -1, formID) == LUA_TUSERDATA) {  // STACK: [wrapper, list]
      IForm* a = IForm::fromStack(luaVM, -1);
      if (a) {
         //
         // BUG: This can run on the PLAYER after SKSE receives the save-file-loaded message. 
         // Until we know exactly when this runs -- particularly relative to loading saves and 
         // whatnot -- we can't use it.
         //
         a->isDeleted = true;
      }
   }
//_MESSAGE(" - Done");
   lua_pop(luaVM, 2);
}