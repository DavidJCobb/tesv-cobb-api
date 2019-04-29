#pragma once
#include "LuaSkyrim/_includes.h"
#include "IReference.h"

namespace LuaSkyrim {
   class IActor : public IReference {
      public:
         IActor(TESForm* form) : IReference(form) {};

         virtual void resolve();
         virtual const char* signature()   const { return "ACHR"; };
         virtual uint8_t     getFormType() const { return kFormType_Character; };

         static constexpr char* metatableName = "Skyrim.IActor";

         static void    setupClass(lua_State* luaVM);
         static IActor* fromStack(lua_State* luaVM, SInt32 stackPos = -1);
   };
}