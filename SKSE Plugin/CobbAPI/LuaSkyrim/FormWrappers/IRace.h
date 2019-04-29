#pragma once
#include "LuaSkyrim/_includes.h"
#include "IForm.h"

namespace LuaSkyrim {
   class IRace : public IForm {
      public:
         IRace(TESForm* f) : IForm(f) {};

         static constexpr char* metatableName = "Skyrim.IRace";

         virtual const char* signature() const { return "RACE"; };

         static void   setupClass(lua_State* luaVM);
         static IRace* fromStack(lua_State* luaVM, SInt32 stackPos = -1);
   };
}