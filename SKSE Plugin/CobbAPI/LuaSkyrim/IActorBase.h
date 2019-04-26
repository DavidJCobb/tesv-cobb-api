#pragma once
#include "_includes.h"
#include "IForm.h"

class TESForm;
namespace LuaSkyrim {
   //
   // TODO: The game can create ActorBases at run-time for leveled actors; we either 
   // need to modify IActorBase to handle forms that can be deleted, or have it fail 
   // on any 0xFFxxxxxx forms.
   //
   class IActorBase : public IForm {
      public:
         IActorBase(TESForm* form);

         virtual const char* signature() const { return "NPC_"; };

         static constexpr char* metatableName = "Skyrim.IActorBase";

         static void        setupMetatable(lua_State* luaVM);
         static IActorBase* fromStack(lua_State* luaVM, UInt32 stackPos = 1);
   };
}