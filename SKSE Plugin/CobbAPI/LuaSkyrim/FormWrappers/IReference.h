#pragma once
#include "LuaSkyrim/_includes.h"
#include "IForm.h"

class TESForm;
namespace LuaSkyrim {
   class IReference : public IForm {
      public:
         IReference(TESForm* form);

         virtual void resolve();
         virtual void abandon();
         virtual const char* signature() const { return "REFR"; };

         uint32_t formID = 0;

         static constexpr char* metatableName = "Skyrim.IReference";

         static void        setupMetatable(lua_State* luaVM);
         static IReference* fromStack(lua_State* luaVM, UInt32 stackPos = 1);
   };
}