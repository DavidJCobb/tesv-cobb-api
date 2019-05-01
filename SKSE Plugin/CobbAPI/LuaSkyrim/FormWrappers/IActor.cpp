#include "IActor.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/Actor.h"
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
            form->actorValueOwner.ModifyModifier(RE::kAVModifier_Damage, avIndex, value);
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
         luastackchange_t restoreActorValue(lua_State* L) {
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
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "damageActorValue",  _methods::damageActorValue },
      { "getActorValue",     _methods::getActorValue },
      { "isSneaking",        _methods::isSneaking },
      { "restoreActorValue", _methods::restoreActorValue },
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