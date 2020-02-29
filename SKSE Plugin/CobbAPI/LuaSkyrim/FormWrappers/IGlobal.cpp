#include "IGlobal.h"
#include "LuaSkyrim/_classes.h"
#include "LuaSkyrim/_utilities.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/TESGlobal.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _methods {
         luastackchange_t forceValue(lua_State* L) {
            IForm* wrapper = IGlobal::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IGlobal' expected");
            float value = util::getNumberArg(L, 2, 2);
            auto form = (RE::TESGlobal*) wrapper->unwrap();
            if (form)
               form->value = value;
            return 0;
         };
         luastackchange_t getName(lua_State* L) {
            IForm* wrapper = IGlobal::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IGlobal' expected");
            auto form = (RE::TESGlobal*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushstring(L, form->editorID.Get());
            return 1;
         };
         luastackchange_t getValue(lua_State* L) {
            IForm* wrapper = IGlobal::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IGlobal' expected");
            auto form = (RE::TESGlobal*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushnumber(L, form->value);
            return 1;
         };
         luastackchange_t isConstant(lua_State* L) {
            IForm* wrapper = IGlobal::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IGlobal' expected");
            auto form = (RE::TESGlobal*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushboolean(L, form->IsConstant());
            return 1;
         };
         luastackchange_t setValue(lua_State* L) {
            IForm* wrapper = IGlobal::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IGlobal' expected");
            float value = util::getNumberArg(L, 2, 2);
            auto form = (RE::TESGlobal*) wrapper->unwrap();
            if (form) {
               if (form->IsConstant())
                  luaL_error(L, "Cannot set the value of a constant global-variable form. If you need to change a constant global-variable form's value at run-time, use forceValue.");
               form->value = value;
            }
            return 0;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "forceValue", _methods::forceValue },
      { "getName",    _methods::getName },
      { "getValue",   _methods::getValue },
      { "isConstant", _methods::isConstant },
      { "setValue",   _methods::setValue },
      { NULL, NULL }
   };
   void IGlobal::setupClass(lua_State* luaVM) {
      if (_isClassDefined(luaVM, metatableName))
         return;
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_Global, formWrapperFactory<IGlobal>);
      mapFormTypeToMetatable(luaVM, kFormType_Global, metatableName);
   };
   IGlobal* IGlobal::fromStack(lua_State* luaVM, SInt32 stackPos) {
      return (IGlobal*) _asClass(luaVM, stackPos, metatableName);
   };
};