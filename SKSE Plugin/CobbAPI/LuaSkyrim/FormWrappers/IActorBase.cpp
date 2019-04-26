#include "IActorBase.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/BaseForms/TESNPC.h"

namespace LuaSkyrim {
   IActorBase::IActorBase(TESForm* form) : IForm(form, ((form->formID & 0xFF000000) == 0xFF000000)) {};

   namespace { // metatable methods
      namespace _methods {
         luastackchange_t gender(lua_State* L) {
            IForm* wrapper = IActorBase::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActorBase' expected");
            auto form = wrapper->unwrap();
            if (!form)
               lua_pushnumber(L, -1);
            else {
               int gender = -1;
               if (form->formType == kFormType_NPC) {
                  auto  base = (RE::TESNPC*) form;
                  auto& data = base->actorData;
                  if (data.flags & RE::TESNPC::kFlag_Female)
                     gender = 1;
                  else
                     gender = 0;
               }
               lua_pushnumber(L, gender);
            }
            return 1;
         };
         luastackchange_t race(lua_State* L) {
            IForm* wrapper = IActorBase::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActorBase' expected");
            auto form = wrapper->unwrap();
            if (!form) {
               lua_pushnil(L);
               return 1;
            }
            auto base = (RE::TESNPC*) form;
            auto race = base->race.race;
            if (race)
               return wrapForm(L, race);
            lua_pushnil(L);
            return 1;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "gender", _methods::gender },
      { "race",   _methods::race },
      { NULL, NULL }
   };
   void IActorBase::setupMetatable(lua_State* luaVM) {
      static bool isDefined = false;
      if (isDefined)
         return;
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_NPC, formWrapperFactory<IActorBase>);
      mapFormTypeToMetatable(luaVM, kFormType_NPC, metatableName);
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