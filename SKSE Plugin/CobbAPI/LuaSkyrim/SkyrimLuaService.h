#pragma once
#include "_includes.h"

class SkyrimLuaService {
   public:
      static SkyrimLuaService& GetInstance() {
         static SkyrimLuaService instance;
         return instance;
      };
   private:
      lua_State* state = nullptr;
   public:
      void OnReferenceDelete(UInt32 formID);
};