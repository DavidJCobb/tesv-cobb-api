#include "IReference.h"
#include "LuaSkyrim/_classes.h"
#include "LuaSkyrim/_utilities.h"
#include "LuaSkyrim/FormWrappers/IEffectShader.h"
#include "ReverseEngineered/Forms/TESEffectShader.h"
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Systems/012E32E8.h"
#include "Miscellaneous/math.h"
#include "skse/GameRTTI.h"

namespace LuaSkyrim {
   IReference::IReference(TESForm* form) : IForm(form, true) {
      if (form) {
         auto ref = (RE::TESObjectREFR*) form;
         this->formID    = form->formID;
         this->refHandle = ref->handleRefObject.GetRefHandle();
      }
   };
   void IReference::resolve() {
      auto form = LookupFormByID(this->formID);
      if (form) {
         auto ref = (RE::TESObjectREFR*) DYNAMIC_CAST(form, TESForm, TESObjectREFR);
         if (ref && !this->ruleOutForm((TESForm*)ref))
            this->wrapped = form;
      }
   };
   void IReference::abandon() {
      this->wrapped = nullptr;
   };
   bool IReference::ruleOutForm(TESForm* form) {
      auto ref = (RE::TESObjectREFR*) form;
      if (ref->handleRefObject.GetRefHandle() != this->refHandle)
         return true;
      return false;
   };

   namespace { // metatable methods
      namespace _methods {
         luastackchange_t containsQuestItem(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushboolean(L, CALL_MEMBER_FN(form, ContainsQuestItem)());
            return 1;
         };
         luastackchange_t disable(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (form)
               form->Disable();
            return 0;
         };
         luastackchange_t enable(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (form)
               CALL_MEMBER_FN(form, Enable)();
            return 0;
         };
         luastackchange_t getBaseForm(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            auto base = form->baseForm;
            if (base)
               return wrapForm(L, (TESForm*)base);
            lua_pushnil(L);
            return 1;
         };
         luastackchange_t getBounds(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            RE::NiPoint3 a;
            RE::NiPoint3 b;
            form->GetMinBounds(&a);
            form->GetMaxBounds(&b);
            lua_pushnumber(L, a.x);
            lua_pushnumber(L, a.y);
            lua_pushnumber(L, a.z);
            lua_pushnumber(L, b.x);
            lua_pushnumber(L, b.y);
            lua_pushnumber(L, b.z);
            return 6;
         };
         luastackchange_t getLockLevel(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushinteger(L, form->GetLockLevel());
            return 1;
         };
         luastackchange_t getName(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushstring(L, CALL_MEMBER_FN(form, GetReferenceName)());
            return 1;
         };
         luastackchange_t getParentCell(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            auto cell = form->parentCell;
            if (cell)
               return wrapForm(L, (TESForm*)cell);
            lua_pushnil(L);
            return 1;
         };
         luastackchange_t getPosition(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushnumber(L, form->pos.x);
            lua_pushnumber(L, form->pos.y);
            lua_pushnumber(L, form->pos.z);
            return 3;
         };
         luastackchange_t getRotation(lua_State* L) {
            auto argCount = lua_gettop(L);
            //
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            float x = form->rot.x;
            float y = form->rot.y;
            float z = form->rot.z;
            if (argCount < 1 || !lua_isboolean(L, 2) || !lua_toboolean(L, 2)) { // if arg1 ~= true, return degrees
               x = cobb::radians_to_degrees(x);
               y = cobb::radians_to_degrees(y);
               z = cobb::radians_to_degrees(z);
            }
            lua_pushnumber(L, x);
            lua_pushnumber(L, y);
            lua_pushnumber(L, z);
            return 3;
         };
         luastackchange_t getScale(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            float scale = (float)form->scale / 100.0F;
            lua_pushnumber(L, scale);
            return 1;
         };
         luastackchange_t isDead(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushboolean(L, form->IsDead(0));
            return 1;
         };
         luastackchange_t isLocked(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushboolean(L, CALL_MEMBER_FN(form, IsLocked)());
            return 1;
         };
         luastackchange_t isQuestObject(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushboolean(L, CALL_MEMBER_FN(form, IsQuestObject)());
            return 1;
         };
         luastackchange_t playEffectShader(lua_State* L) {
            IForm* wrapperRef    = IReference::fromStack(L, 1);
            IForm* wrapperShader = IEffectShader::fromStack(L, 2);
            luaL_argcheck(L, wrapperRef    != nullptr, 1, "'IReference' expected");
            luaL_argcheck(L, wrapperShader != nullptr, 2, "'IEffectShader' expected");
            float duration = -1.0F; // internally, negative values mean "play indefinitely..."
            if (!lua_isnoneornil(L, 3)) // ...and we want nil values to mean the same thing
               duration = util::getNumberArg(L, 3, 3);
            auto  ref      = (RE::TESObjectREFR*)   wrapperRef->unwrap();
            auto  shader   = (RE::TESEffectShader*) wrapperShader->unwrap();
            if (ref && shader) {
               if (!ref->parentCell)
                  luaL_error(L, "Cannot play EffectShaders on a reference with no parent cell.");
               if (!ref->parentCell->IsLoaded())
                  luaL_error(L, "Cannot play EffectShaders on a reference that is not in a loaded cell.");
               if (!ref->GetNiNode())
                  luaL_error(L, "Cannot play EffectShaders on a reference with no 3D.");
               RE::PlayEffectShaderOn(ref, shader, duration, 0, 0, 0, 0);
            }
            return 0;
         };
         luastackchange_t setDestroyed(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            luaL_argcheck(L, lua_isboolean(L, 2), 2, "boolean expected");
            auto ref = (RE::TESObjectREFR*) wrapper->unwrap();
            if (ref) {
               bool destroyed = lua_toboolean(L, 2);
               CALL_MEMBER_FN(ref, SetDestroyed)(destroyed);
            }
            return 0;
         };
         luastackchange_t setPosition(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto ref = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!ref)
               return 0;
            //
            RE::NiPoint3 to = ref->pos;
            if (!lua_isnoneornil(L, 2))
               to.x = util::getNumberArg(L, 2, 2);
            if (!lua_isnoneornil(L, 3))
               to.y = util::getNumberArg(L, 3, 3);
            if (!lua_isnoneornil(L, 4))
               to.z = util::getNumberArg(L, 4, 4);
            CALL_MEMBER_FN(ref, SetPosition)(&to);
            return 0;
         };
         luastackchange_t setRotation(lua_State* L) {
            IForm* wrapper = IReference::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto ref = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!ref)
               return 0;
            //
            RE::NiPoint3 to = ref->rot;
            if (!lua_isnoneornil(L, 2))
               to.x = util::getNumberArg(L, 2, 2);
            if (!lua_isnoneornil(L, 3))
               to.y = util::getNumberArg(L, 3, 3);
            if (!lua_isnoneornil(L, 4))
               to.z = util::getNumberArg(L, 4, 4);
            CALL_MEMBER_FN(ref, SetRotation)(&to);
            return 0;
         };
         luastackchange_t stopEffectShader(lua_State* L) {
            IForm* wrapperRef    = IReference::fromStack(L, 1);
            IForm* wrapperShader = IEffectShader::fromStack(L, 2);
            luaL_argcheck(L, wrapperRef    != nullptr, 1, "'IReference' expected");
            luaL_argcheck(L, wrapperShader != nullptr, 2, "'IEffectShader' expected");
            auto  ref    = (RE::TESObjectREFR*)   wrapperRef->unwrap();
            auto  shader = (RE::TESEffectShader*) wrapperShader->unwrap();
            if (ref && shader) {
               auto singleton = RE::Unknown012E32E8::GetInstance();
               if (singleton) {
                  CALL_MEMBER_FN(singleton, StopEffectShader)(ref, shader);
               }
            }
            return 0;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "containsQuestItem", _methods::containsQuestItem },
      { "disable",           _methods::disable },
      { "enable",            _methods::enable },
      { "getBaseForm",       _methods::getBaseForm },
      { "getBounds",         _methods::getBounds },
      { "getLockLevel",      _methods::getLockLevel },
      { "getName",           _methods::getName },
      { "getParentCell",     _methods::getParentCell },
      { "getPosition",       _methods::getPosition },
      { "getRotation",       _methods::getRotation }, // if arg1 == true, then returns radians; otherwise, degrees
      { "getScale",          _methods::getScale },
      { "isDead",            _methods::isDead },
      { "isLocked",          _methods::isLocked },
      { "isQuestObject",     _methods::isQuestObject },
      { "playEffectShader",  _methods::playEffectShader },
      { "setDestroyed",      _methods::setDestroyed },
      { "setPosition",       _methods::setPosition }, // TODO: do we need to limit this to the main thread?
      { "setRotation",       _methods::setRotation }, // TODO: do we need to limit this to the main thread?
      { "stopEffectShader",  _methods::stopEffectShader },
      { NULL, NULL }
   };
   void IReference::setupClass(lua_State* luaVM) {
      if (_isClassDefined(luaVM, metatableName))
         return;
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_Reference, formWrapperFactory<IReference>);
      mapFormTypeToMetatable(luaVM, kFormType_Reference, metatableName);
      //
      // TODO: It would be valuable to import a singleton for each form interface. These 
      // singletons could hold useful static functions (e.g. "for each form of this type") 
      // and relevant constants.
      //
   };
   IReference* IReference::fromStack(lua_State* luaVM, SInt32 stackPos) {
      return (IReference*) _asClass(luaVM, stackPos, metatableName);
   };
};