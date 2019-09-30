#pragma once
#include "_includes.h"
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class wrapped_lua_pointer {
   //
   // Outside callers, e.g. hooks from x86, should use this to access the Lua state and 
   // call into it. That way, we can rely on this class's destructor to know when the 
   // outside caller is done with Lua and perform cleanup tasks, e.g. abandoning wrapped  
   // form pointers between Lua call stacks. Think of it like a smart pointer.
   //
   // This also handles thread-safety, using the (accessLock) on SkyrimLuaService.
   //
   // Usage:
   //
   //    wrapped_lua_pointer luaVM;
   //    //
   //    // And now, just use it exactly as you would a lua_State*! The constructor 
   //    // automatically gets the lua_State pointer for you.
   //
   protected:
      lua_State* state = nullptr;
   public:
      wrapped_lua_pointer();
      ~wrapped_lua_pointer();
      //
      operator bool() const noexcept { return this->state != nullptr; };
      operator lua_State*() const noexcept { return this->state; };
      bool operator!() const noexcept { return !(bool)this; };
};
class SkyrimLuaService {
   friend wrapped_lua_pointer;
   //
   public:
      static SkyrimLuaService& GetInstance() {
         static SkyrimLuaService instance;
         return instance;
      };
      struct Addon {
         std::string name;
         std::string folder;
         std::string author;
         std::string description;
         std::vector<std::string> savevars; // TODO: use a set; we need these to be unique
         std::vector<std::string> dependencies; // TODO: use a set; we need these to be unique // TODO: these are case-sensitive and maybe shouldn't be
         UInt32 version = 0;
         bool   loaded  = false;
         bool   failed  = false; // whether the add-on failed to load
      };
   protected:
      SkyrimLuaService();
      //
      lua_State* state    = nullptr;
      DWORD      threadID = 0; // thread we created the VM on; mainly just here for debugging purposes
      //
      std::unordered_map<std::string, Addon> addons;
      std::string currentAddon; // add-on currently loading script files; used for cyclical dependency checks
      //
      mutable std::recursive_mutex accessLock; // used and managed by wrapped_lua_pointer
      mutable std::recursive_mutex setupLock; // some of our hooks can fire during setup; we lock Lua to only run on one thread at a time, but Skyrim itself is still multi-threaded
      //
      void loadAddonScript(Addon& addon, std::string path);
      bool loadAddonScriptFiles(Addon& addon);
      void loadAddonMetadata(std::string& folder);
      void loadAddons();
      //
      void onLuaCodeDone(); // Runs whenever a Lua call stack finishes and we are "fully exiting" to C++; called by wrapped_lua_pointer as necessary.
      //
   public:
      void StartVM();
      void StopVM();

      void OnReferenceDelete(UInt32 formID);
};