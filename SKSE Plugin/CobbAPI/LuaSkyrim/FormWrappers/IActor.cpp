#include "IActor.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/Actor.h"
#include "ReverseEngineered/Forms/BaseForms/TESActorBase.h"
#include "ReverseEngineered/Systems/GameData.h"

#include "skse/GameRTTI.h"

namespace LuaSkyrim {
   void IActor::resolve() {
      auto form = LookupFormByID(this->formID);
      if (form) {
         auto ref = (RE::Actor*) DYNAMIC_CAST(form, TESForm, Actor);
         if (ref && this->_verifyRef(ref))
            this->wrapped = form;
      }
   };

   namespace { // metatable methods
      namespace _helpers {
         uint8_t getAVIndexArg(lua_State* L, int stackPos, int argIndex) {
            if (lua_isnumber(L, stackPos)) {
               lua_Number  f = lua_tonumber(L, stackPos);
               lua_Integer i;
               luaL_argcheck(L, lua_numbertointeger(f, &i), argIndex, "whole number or integer expected");
               luaL_argcheck(L, i >= 0 && i <= 255,         argIndex, "actor value index is out of bounds");
               return i;
            } else if (lua_isinteger(L, stackPos)) {
               lua_Integer i = lua_tointeger(L, stackPos);
               luaL_argcheck(L, i >= 0 && i <= 255, argIndex, "actor value index is out of bounds");
               return i;
            } else {
               luaL_argcheck(L, false, argIndex, "number or integer expected");
            }
         }
         float getAVValueArg(lua_State* L, int stackPos, int argIndex) {
            if (lua_isnumber(L, stackPos)) {
               return lua_tonumber(L, stackPos);
            } else if (lua_isinteger(L, stackPos)) {
               return lua_tointeger(L, stackPos);
            } else {
               luaL_argcheck(L, false, argIndex, "number or integer expected");
            }
         }
      }
      namespace _methods {
         luastackchange_t damageActorValue(lua_State* L) {
            uint8_t avIndex = _helpers::getAVIndexArg(L, 2, 2);
            float   value   = _helpers::getAVValueArg(L, 3, 3);
            IForm*  wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            form->actorValueOwner.ModifyModifier(RE::kAVModifier_Damage, avIndex, -value);
            return 0;
         };
         luastackchange_t drawWeapon(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (form)
               form->DrawSheatheWeapon(true);
            return 0;
         };
         luastackchange_t getActorValue(lua_State* L) {
            uint8_t avIndex = _helpers::getAVIndexArg(L, 2, 2);
            IForm*  wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushnumber(L, form->actorValueOwner.GetCurrent(avIndex));
            return 1;
         };
         luastackchange_t getCrimeFaction(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            return wrapForm(L, CALL_MEMBER_FN(form, GetCrimeFaction)());
         };
         luastackchange_t getDeathCount(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            luaL_argcheck(L, form != nullptr, 1, "the actor cannot be null");
            auto base = (RE::TESActorBase*) form->baseForm;
            luaL_argcheck(L, base != nullptr, 1, "the actor must have a base form");
            if (form) {
               auto extra = (RE::ExtraLeveledCreature*) CALL_MEMBER_FN((RE::BaseExtraList*) &form->extraData, GetByType)(0x2D);
               if (extra && extra->unk08)
                  base = (RE::TESActorBase*) extra->unk08;
               lua_pushinteger(L, CALL_MEMBER_FN((RE::TES*) *g_TES, GetActorBaseDeathCount)(base));
               return 1;
            }
            return 0;
         };
         luastackchange_t getLevel(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushinteger(L, CALL_MEMBER_FN(form, GetLevel)());
            return 1;
         };
         luastackchange_t getMaxCarryWeight(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushnumber(L, CALL_MEMBER_FN(form, GetMaxCarryWeight)());
            return 1;
         };
         luastackchange_t getShowsOnStealthMeter(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' caster expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushboolean(L, !(form->flags2 & RE::Actor::kFlags_DontShowOnStealthMeter));
            return 1;
         };
         luastackchange_t isAlerted(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            bool result = CALL_MEMBER_FN(form, IsAlerted)();
            lua_pushboolean(L, result);
            return 1;
         };
         luastackchange_t isInMidAir(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            bool result = CALL_MEMBER_FN(form, IsInMidAir)();
            lua_pushboolean(L, result);
            return 1;
         };
         luastackchange_t isOverencumbered(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            bool result = CALL_MEMBER_FN(form, IsOverencumbered)();
            lua_pushboolean(L, result);
            return 1;
         };
         luastackchange_t isSneaking(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            bool result = CALL_MEMBER_FN(form, IsSneaking)();
            lua_pushboolean(L, result);
            return 1;
         };
         luastackchange_t isSprinting(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            bool result = form->IsSprinting();
            lua_pushboolean(L, result);
            return 1;
         };
         luastackchange_t isSwimming(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            bool result = form->IsSwimming();
            lua_pushboolean(L, result);
            return 1;
         };
         luastackchange_t isWeaponDrawn(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            bool result = form->IsWeaponDrawn();
            lua_pushboolean(L, result);
            return 1;
         };
         luastackchange_t modifyDeathCount(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr,  1, "'IActor' expected");
            luaL_argcheck(L, lua_isinteger(L, 2), 2, "integer expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            luaL_argcheck(L, form != nullptr, 1, "the actor cannot be null");
            auto base = (RE::TESActorBase*) form->baseForm;
            luaL_argcheck(L, base != nullptr, 1, "the actor must have a base form");
            auto mod  = lua_tointeger(L, 2);
            if (form) {
               luaL_argcheck(L, (base->actorData.flags & RE::TESActorBaseData::kFlag_Unique), 1, "cannot modify death counts for non-unique actor-bases");
               auto extra = (RE::ExtraLeveledCreature*) CALL_MEMBER_FN((RE::BaseExtraList*) &form->extraData, GetByType)(0x2D);
               if (extra && extra->unk08)
                  base = (RE::TESActorBase*) extra->unk08;
               CALL_MEMBER_FN((RE::TES*) *g_TES, ModActorBaseDeathCount)(base, mod);
            }
            return 0;
         };
         luastackchange_t pushAwayFrom(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            luaL_argcheck(L, lua_isnumber(L, 2), 2, "numeric x-coordinate expected");
            luaL_argcheck(L, lua_isnumber(L, 3), 3, "numeric y-coordinate expected");
            luaL_argcheck(L, lua_isnumber(L, 4), 4, "numeric z-coordinate expected");
            luaL_argcheck(L, lua_isnumber(L, 5), 5, "numeric magnitude expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            auto pm = form->processManager;
            if (!pm)
               return 0;
            if (pm->unk9B != 0) // if actor is not in high process, then it can't be pushed, per the Papyrus func for this
               return 0;
            float x = lua_tonumber(L, 2);
            float y = lua_tonumber(L, 3);
            float z = lua_tonumber(L, 4);
            float mag = lua_tonumber(L, 5);
            CALL_MEMBER_FN(pm, PushActorAway)(form, x, y, z, mag);
            return 0;
         };
         luastackchange_t restoreActorValue(lua_State* L) {
            uint8_t avIndex = _helpers::getAVIndexArg(L, 2, 2);
            float   value   = _helpers::getAVValueArg(L, 3, 3);
            IForm*  wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            form->actorValueOwner.ModifyModifier(RE::kAVModifier_Damage, avIndex, value);
            return 0;
         };
         luastackchange_t setShowsOnStealthMeter(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr,  1, "'IActor' caster expected");
            luaL_argcheck(L, lua_isboolean(L, 2), 2, "boolean expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            CALL_MEMBER_FN(form, SetNotShowOnStealthMeter)(!lua_toboolean(L, 2));
            return 0;
         };
         luastackchange_t sheatheWeapon(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (form)
               form->DrawSheatheWeapon(false);
            return 0;
         };
         luastackchange_t soulTrap(lua_State* L) {
            IForm* wcaster = IActor::fromStack(L, 1);
            IForm* wtarget = IActor::fromStack(L, 2);
            luaL_argcheck(L, wcaster != nullptr, 1, "'IActor' caster expected");
            luaL_argcheck(L, wtarget != nullptr, 2, "'IActor' target expected");
            auto caster = (RE::Actor*) wcaster->unwrap();
            if (!caster)
               return 0;
            auto target = (RE::Actor*) wtarget->unwrap();
            if (!target)
               return 0;
            bool result = CALL_MEMBER_FN(caster, TrapSoul)(target);
            lua_pushboolean(L, result);
            return 1;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "damageActorValue",  _methods::damageActorValue },
      { "drawWeapon",        _methods::drawWeapon },
      { "getActorValue",     _methods::getActorValue },
      { "getCrimeFaction",   _methods::getCrimeFaction },
      { "getDeathCount",     _methods::getDeathCount },
      { "getLevel",          _methods::getLevel },
      { "getMaxCarryWeight", _methods::getMaxCarryWeight }, // returns CarryWeight AV influenced by the GetMaxCarryWeight perk entry point
      { "getShowsOnStealthMeter", _methods::getShowsOnStealthMeter },
      { "isAlerted",         _methods::isAlerted },
      { "isInMidAir",        _methods::isInMidAir }, // used by the game to disallow waiting/sleeping in midair; not sure how it actually works
      { "isOverencumbered",  _methods::isOverencumbered }, // always false for NPCs; always false if god mode is enabled; always false if the actor has ExtraInteraction; otherwise, compare max carry weight (with perk entry points) with inventory weight
      { "isSneaking",        _methods::isSneaking },
      { "isSprinting",       _methods::isSprinting },
      { "isSwimming",        _methods::isSwimming },
      { "isWeaponDrawn",     _methods::isWeaponDrawn },
      { "modifyDeathCount",  _methods::modifyDeathCount },
      { "pushAwayFrom",      _methods::pushAwayFrom }, // args: x,y,z to push away from; force magnitude
      { "restoreActorValue", _methods::restoreActorValue },
      { "setShowsOnStealthMeter", _methods::setShowsOnStealthMeter },
      { "sheatheWeapon",     _methods::sheatheWeapon },
      { "soulTrap",          _methods::soulTrap }, // caster:soulTrap(target)
      { NULL, NULL }
   };
   void IActor::setupClass(lua_State* luaVM) {
      if (_isClassDefined(luaVM, metatableName))
         return;
      _defineClass(luaVM, metatableName, IReference::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_Character, formWrapperFactory<IActor>);
      mapFormTypeToMetatable(luaVM, kFormType_Character, metatableName);
      //
      // TODO: It would be valuable to import a singleton for each form interface. These 
      // singletons could hold useful static functions (e.g. "for each form of this type") 
      // and relevant constants.
      //
   };
   IActor* IActor::fromStack(lua_State* luaVM, SInt32 stackPos) {
      return (IActor*) _asClass(luaVM, stackPos, metatableName);
   };
};