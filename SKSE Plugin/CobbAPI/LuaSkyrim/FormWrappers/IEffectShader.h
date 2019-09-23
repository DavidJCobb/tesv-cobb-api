#pragma once
#include "LuaSkyrim/_includes.h"
#include "IForm.h"

namespace LuaSkyrim {
   class IEffectShader : public IForm {
      public:
         IEffectShader(TESForm* f) : IForm(f) {};

         static constexpr char* metatableName = "Skyrim.IEffectShader";

         virtual const char* signature() const { return "EFSH"; };

         static void setupClass(lua_State* luaVM);
         static IEffectShader* fromStack(lua_State* luaVM, SInt32 stackPos = -1);
   };
}