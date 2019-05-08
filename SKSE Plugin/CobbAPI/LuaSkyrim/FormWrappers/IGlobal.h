#pragma once
#include "LuaSkyrim/_includes.h"
#include "IForm.h"

namespace LuaSkyrim {
   class IGlobal : public IForm {
      public:
         IGlobal(TESForm* f) : IForm(f) {};

         static constexpr char* metatableName = "Skyrim.IGlobal";

         virtual const char* signature() const { return "GLOB"; };

         static void     setupClass(lua_State* luaVM);
         static IGlobal* fromStack(lua_State* luaVM, SInt32 stackPos = -1);
   };
}