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
      DWORD      threadID = 0;
      //
      std::recursive_mutex setupLock;
      std::unordered_map<DWORD, lua_State*> childThreads; // TODO: put a lock on this
      //
      void       prepForThreads();
      lua_State* getOrCreateThread(DWORD threadID);
      //
   public:
      void StartVM();
      void StopVM();

      void OnReferenceDelete(UInt32 formID);
};