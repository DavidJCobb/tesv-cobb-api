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
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      //
      // TODO: It would be valuable to import a singleton for each form interface. These 
      // singletons could hold useful static functions (e.g. "for each form of this type") 
      // and relevant constants.
      //
      // TODO: Define these constants on IActorBase's singleton:
      //    GENDER_MALE    =  0
      //    GENDER_FEMALE  =  1
      //    GENDER_UNKNOWN = -1
      //
      isDefined = true;
   };
   IActorBase* IActorBase::fromStack(lua_State* luaVM, UInt32 stackPos) {
      return (IActorBase*)_asClass(luaVM, stackPos, metatableName);
   };
};