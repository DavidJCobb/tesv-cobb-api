#include "SkyrimLuaService.h"
#include "_includes.h"
#include "Lua5.3.5/luauser.h"

#include "FormWrappers/IForm.h"
#include "FormWrappers/IActor.h"
#include "FormWrappers/IActorBase.h"
#include "FormWrappers/IRace.h"
#include "FormWrappers/IReference.h"

#include "skse/GameForms.h" // LookupFormByID
#include "skse/Utilities.h" // GetRuntimeDirectory

using namespace LuaSkyrim;

constexpr char* ce_luaThreadStorage = "LuaSkyrim.threads";

void SkyrimLuaService::prepForThreads() {
   auto luaVM = this->state;
   if (!luaVM)
      return;
   lua_newtable(luaVM);
   lua_setfield(luaVM, LUA_REGISTRYINDEX, ce_luaThreadStorage); // STACK: []
};
lua_State* SkyrimLuaService::getOrCreateThread(DWORD threadID) {
   auto luaVM = this->state;
   if (!luaVM)
      return nullptr;
   if (threadID == this->threadID)
      return luaVM;
   try { // If we already have this thread, don't make a state for it.
      return this->childThreads.at(threadID);
   } catch (std::out_of_range) {}
_MESSAGE("Creating Lua child thread for %08X...", threadID);
   //
   auto created = lua_newthread(luaVM); // STACK: [thread]
   lua_getfield (luaVM, LUA_REGISTRYINDEX, ce_luaThreadStorage); // STACK: [list, thread]
   lua_pushvalue(luaVM, -2);           // STACK: [thread, list, thread]
   lua_rawseti  (luaVM, -2, threadID); // STACK: [list, thread]
   lua_pop      (luaVM, 2);            // STACK: []
   this->childThreads[threadID] = created;
   return created;
};

namespace { // utility tools
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
   }
}

void _badThreadHandler(DWORD threadID) {
   _MESSAGE("Thread %08X tried to access the Lua Lock before it was readied -- this will crash!", threadID);
};

SkyrimLuaService::SkyrimLuaService() {
   CobbDebugSetBadThreadHandler(_badThreadHandler);
}

void SkyrimLuaService::StartVM() {
   if (this->state)
      return;
_MESSAGE("[THREAD %08X] Attempting to start Lua VM...", GetCurrentThreadId());
   std::lock_guard<decltype(this->setupLock)> guard(this->setupLock);
_MESSAGE(" - Lock set...");
   //
   lua_State* luaVM = luaL_newstate(); // create a VM instance
   this->state    = luaVM;
   this->threadID = GetCurrentThreadId();
_MESSAGE("[THREAD %08X] Prepping for child threads...", this->threadID);
   this->prepForThreads();
   //
   std::string file = "nativeTestScript.lua";
   file = GetScriptBasePath() + file;
   //
   int status = luaL_loadfile(luaVM, file.c_str());
   if (status) {
      _MESSAGE("Couldn't load file: %s\n", lua_tostring(luaVM, -1)); // loadfile logs error messages to the top of Lua's stack
      lua_close(luaVM); // Terminate the VM.
      this->state    = nullptr;
      this->threadID = 0;
      return;
   }
_MESSAGE(" - Starting Lua VM...");
   //
   // Set up metatables and similar for all native types that we plan to expose to Lua:
   //
   // DO NOT FORGET TO CALL THESE
   //
   IForm::setupClass(luaVM);
   IActorBase::setupClass(luaVM);
   IRace::setupClass(luaVM);
   IReference::setupClass(luaVM);
   IActor::setupClass(luaVM);
   //
   lua_register(luaVM, "logmessage", _globals::LuaLog); // make a C function available to the Lua script under the name "logmessage"
   lua_register(luaVM, "form_by_id", _globals::FormByID);
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
   //
   // Now let's run the script:
   //
   auto stackSizePrior = lua_gettop(luaVM);
   int  result = lua_pcall(luaVM, 0, LUA_MULTRET, 0);
   if (result) {
      _MESSAGE("Failed to run script: %s\n", lua_tostring(luaVM, -1));
      lua_settop(luaVM, stackSizePrior);
      return;
   }
   //
   // Every Lua script is an anonymous function. If the script ran a top-level 
   // "return" statement, then the return value will be at the top of the Lua 
   // stack, and we can retrieve it:
   //
   double returned = lua_tonumber(luaVM, -1);
   lua_settop(luaVM, stackSizePrior);
   _MESSAGE("Script returned: %.0f\n", returned);
}
void SkyrimLuaService::StopVM() {
   if (!this->state)
      return;
   std::lock_guard<decltype(this->setupLock)> guard(this->setupLock);
_MESSAGE("Stopping Lua VM...");
   lua_close(this->state); // should also kill child threads
   this->state    = nullptr;
   this->threadID = 0;
   this->childThreads.clear();
}

void SkyrimLuaService::OnReferenceDelete(UInt32 formID) {
_MESSAGE("[THREAD %08X] Lua VM was notified about the deletion of form %08X...", GetCurrentThreadId(), formID);
   if (this->setupLock.try_lock())
      this->setupLock.unlock();
   else
      return;
   //
   auto luaVM = this->getOrCreateThread(GetCurrentThreadId());
   if (!luaVM)
      return;
_MESSAGE(" - Responding...");
   if (lua_status(luaVM) != LUA_OK) {
      _MESSAGE(" - WARNING: VM thread is not OK!");
   }
   //
   lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_formWrapperReuseKey); // STACK: [list]
   if (lua_rawgeti(luaVM, -1, formID) == LUA_TUSERDATA) {  // STACK: [wrapper, list]
      IForm* a = IForm::fromStack(luaVM, -1);
      if (a) {
         a->isDeleted = true;
      }
   }
_MESSAGE(" - Done");
   lua_pop(luaVM, 2);
}