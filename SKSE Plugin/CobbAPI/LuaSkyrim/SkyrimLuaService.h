#pragma once
#include "_includes.h"
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class SkyrimLuaService {
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
      std::recursive_mutex setupLock; // some of our hooks can fire during setup; we lock Lua to only run on one thread at a time, but Skyrim itself is still multi-threaded
      //
      void loadAddonScript(Addon& addon, std::string path);
      bool loadAddonScriptFiles(Addon& addon);
      void loadAddonMetadata(std::string& folder);
      void loadAddons();
      //
   public:
      lua_State* getState();

      void StartVM();
      void StopVM();

      void OnReferenceDelete(UInt32 formID);
};