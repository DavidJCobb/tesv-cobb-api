#pragma once
#include "_includes.h"
#include <mutex>
#include <unordered_map>

class SkyrimLuaService {
   public:
      static SkyrimLuaService& GetInstance() {
         static SkyrimLuaService instance;
         return instance;
      };
   protected:
      SkyrimLuaService();
      //
      lua_State* state    = nullptr;
      DWORD      threadID = 0; // thread we created the VM on; mainly just here for debugging purposes
      //
      std::recursive_mutex setupLock; // some of our hooks can fire during setup; we lock Lua to only run on one thread at a time, but Skyrim itself is still multi-threaded
      //
   public:
      lua_State* getState();

      void StartVM();
      void StopVM();

      void OnReferenceDelete(UInt32 formID);
};