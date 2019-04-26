#pragma once
#include "LuaSkyrim/_includes.h"
#include "IReference.h"

namespace LuaSkyrim {
   class IActor : public IReference {
      public:
         IActor(TESForm* form) : IReference(form) {};

         virtual const char* signature() const { return "ACHR"; };

         static constexpr char* metatableName = "Skyrim.IActor";

         static void    setupMetatable(lua_State* luaVM);
         static IActor* fromStack(lua_State* luaVM, UInt32 stackPos = 1);
   };
}