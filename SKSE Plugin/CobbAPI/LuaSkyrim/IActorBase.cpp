#include "IActorBase.h"
#include "_classes.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/BaseForms/TESActorBase.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _methods {
         luastackchange_t gender(lua_State* L) {
            IForm* form = IActorBase::fromStack(L);
            luaL_argcheck(L, form != nullptr, 1, "'IActorBase' expected");
            if (!form->wrapped)
               lua_pushnumber(L, -1);
            else {
               int gender = -1;
               if (form->wrapped->formType == kFormType_NPC) {
                  auto  base = (RE::TESActorBase*) form->wrapped;
                  auto& data = base->actorData;
                  if (data.flags & RE::TESActorBaseData::kFlag_Female)
                     gender = 1;
                  else
                     gender = 0;
               }
               lua_pushnumber(L, gender);
            }
            return 1;
         };
         luastackchange_t __tostring(lua_State* L) {
            IForm* form = IForm::fromStack(L);
            luaL_argcheck(L, form != nullptr, 1, "'IActorBase' expected");
            if (!form->wrapped)
               lua_pushfstring(L, "[NONE]");
            else
               lua_pushfstring(L, "[NPC_:%08X]", form->wrapped->formID);
            return 1; // indicate how many things we added to the Lua stack
         }
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "gender",     _methods::gender },
      { "__tostring", _methods::__tostring },
      { NULL, NULL }
   };
   void IActorBase::setupMetatable(lua_State* luaVM) {
      static bool isDefined = false;
      if (isDefined)
         return;
      {  // Create the metatable
         _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
         /*//
         luaL_newmetatable(luaVM, metatableName);
         //
         lua_pushstring(luaVM, "__index");
         luaL_getmetatable(luaVM, IForm::metatableName);
         lua_settable(luaVM, -3); // metatable.__index = IForm_metatable
         //
         // TODO: Do we need to define __meta on every metatable, or just on 
         // the base class's metatable?
         //
         luaL_setfuncs(luaVM, _metatableMethods, 0); // import functions into the metatable
         //
         lua_pop(luaVM, 1); // pop metatable from the stack
         //*/
      }
      isDefined = true;
   };
   IActorBase* IActorBase::fromStack(lua_State* luaVM, UInt32 stackPos) {
      return (IActorBase*)_asClass(luaVM, stackPos, metatableName);
      /*//
      return (IActorBase*) luaL_checkudata(luaVM, stackPos, metatableName);
      //*/
   };

   luastackchange_t IActorBase::make(lua_State* luaVM, TESForm* form) {
      if (form && form->formType != kFormType_NPC)
         form = nullptr;
      //
      // TODO: The game can create ActorBases at run-time for leveled actors; we either 
      // need to modify IActorBase to handle forms that can be deleted, or have it fail 
      // on any 0xFFxxxxxx forms.
      //
      return IForm::make(luaVM, form);
   };
};