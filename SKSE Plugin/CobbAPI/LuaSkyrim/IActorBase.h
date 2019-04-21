#pragma once
#include "_includes.h"
#include "IForm.h"

namespace LuaSkyrim {
   //
   // Interface for basic forms. Not suitable for forms that can 
   // be unloaded or deleted at run-time.
   //
   class IActorBase : public IForm {
      public:
         static constexpr char* metatableName = "Skyrim.IActorBase";

         static void        setupMetatable(lua_State* luaVM);
         static IActorBase* fromStack(lua_State* luaVM, UInt32 stackPos = 1);

         static luastackchange_t make(lua_State* luaVM, TESForm*);
   };
}