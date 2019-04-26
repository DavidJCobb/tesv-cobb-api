#include "IReference.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "skse/GameRTTI.h"

namespace LuaSkyrim {
   IReference::IReference(TESForm* form) : IForm(form, true) {
      if (form)
         this->formID = form->formID;
   };
   void IReference::resolve() {
      auto form = LookupFormByID(this->formID);
      if (form)
         this->wrapped = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
   };
   void IReference::abandon() {
      this->wrapped = nullptr;
   };

   namespace { // metatable methods
      namespace _methods {
         luastackchange_t baseForm(lua_State* L) {
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
         luastackchange_t parentCell(lua_State* L) {
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
         luastackchange_t position(lua_State* L) {
            //
            // TODO: Allow setting the position by way of three number arguments. 
            // If nil is specified for an axis, leave it unchanged.
            //
            // TODO: Is there a prettier way to *modify* the position? (Probably 
            // not without either making a dedicated function for the purpose, 
            // accepting a fourth argument as an enum for mod/set, or using field 
            // accessors instead of method accessors.)
            //
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
         luastackchange_t rotation(lua_State* L) {
            //
            // TODO: Allow setting the rotation by way of three number arguments. 
            // If nil is specified for an axis, leave it unchanged.
            //
            // TODO: Is there a prettier way to *modify* the rotation? (Probably 
            // not without either making a dedicated function for the purpose, 
            // accepting a fourth argument as an enum for mod/set, or using field 
            // accessors instead of method accessors.)
            //
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushnumber(L, form->rot.x);
            lua_pushnumber(L, form->rot.y);
            lua_pushnumber(L, form->rot.z);
            return 3;
         };
         luastackchange_t scale(lua_State* L) {
            //
            // TODO: Allow setting the scale by way of a number argument.
            //
            IForm* wrapper = IReference::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IReference' expected");
            auto form = (RE::TESObjectREFR*) wrapper->unwrap();
            if (!form)
               return 0;
            float scale = (float)form->scale / 100.0F;
            lua_pushnumber(L, scale);
            return 1;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "baseForm",   _methods::baseForm },
      { "parentCell", _methods::parentCell },
      { "position",   _methods::position },
      { "rotation",   _methods::rotation },
      { "scale",      _methods::scale },
      { NULL, NULL }
   };
   void IReference::setupMetatable(lua_State* luaVM) {
      static bool isDefined = false;
      if (isDefined)
         return;
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_Reference, formWrapperFactory<IReference>);
      mapFormTypeToMetatable(luaVM, kFormType_Reference, metatableName);
      //
      // TODO: It would be valuable to import a singleton for each form interface. These 
      // singletons could hold useful static functions (e.g. "for each form of this type") 
      // and relevant constants.
      //
      isDefined = true;
   };
   IReference* IReference::fromStack(lua_State* luaVM, UInt32 stackPos) {
      return (IReference*) _asClass(luaVM, stackPos, metatableName);
   };
};