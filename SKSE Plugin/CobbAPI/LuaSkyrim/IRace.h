#pragma once
#include "_includes.h"
#include "IForm.h"

namespace LuaSkyrim {
   class IRace : public IForm {
      public:
         static constexpr char* metatableName = "Skyrim.IRace";

         static void   setupMetatable(lua_State* luaVM);
         static IRace* fromStack(lua_State* luaVM, UInt32 stackPos = 1);
   };
}