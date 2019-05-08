#include "IActorBase.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/BaseForms/TESActorBase.h"
#include "ReverseEngineered/Forms/BaseForms/TESNPC.h"
#include "ReverseEngineered/Systems/GameData.h"

namespace LuaSkyrim {
   IActorBase::IActorBase(TESForm* form) : IForm(form, ((form->formID & 0xFF000000) == 0xFF000000)) {};

   namespace { // metatable methods
      namespace _methods {
         luastackchange_t getDeathCount(lua_State* L) {
            IForm* wrapper = IActorBase::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActorBase' expected");
            auto form = (RE::TESActorBase*) wrapper->unwrap();
            luaL_argcheck(L, form != nullptr, 1, "the actor-base cannot be null");
            lua_pushinteger(L, CALL_MEMBER_FN((RE::TES*) *g_TES, GetActorBaseDeathCount)(form));
            return 1;
         };
         luastackchange_t getGender(lua_State* L) {
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
         luastackchange_t getRace(lua_State* L) {
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
         luastackchange_t isUnique(lua_State* L) {
            IForm* wrapper = IActorBase::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActorBase' expected");
            auto form = (RE::TESActorBase*) wrapper->unwrap();
            if (!form)
               lua_pushnil(L);
            else {
               if (form->actorData.flags & RE::TESActorBaseData::kFlag_Unique)
                  lua_pushboolean(L, true);
               else
                  lua_pushboolean(L, false);
            }
            return 1;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "getDeathCount", _methods::getDeathCount },
      { "getGender",     _methods::getGender },
      { "getRace",       _methods::getRace },
      { "isUnique",      _methods::isUnique },
      { NULL, NULL }
   };
   void IActorBase::setupClass(lua_State* luaVM) {
      if (_isClassDefined(luaVM, metatableName))
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
   };
   IActorBase* IActorBase::fromStack(lua_State* luaVM, SInt32 stackPos) {
      return (IActorBase*)_asClass(luaVM, stackPos, metatableName);
   };
};