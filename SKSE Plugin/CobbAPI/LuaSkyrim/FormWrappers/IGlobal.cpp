#include "IGlobal.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/TESGlobal.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _methods {
         luastackchange_t getName(lua_State* L) {
            IForm* wrapper = IGlobal::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IRace' expected");
            auto form = (RE::TESGlobal*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushstring(L, form->editorID.Get());
            return 1;
         };
         luastackchange_t getValue(lua_State* L) {
            IForm* wrapper = IGlobal::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IRace' expected");
            auto form = (RE::TESGlobal*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushnumber(L, form->value);
            return 1;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "getName",  _methods::getName },
      { "getValue", _methods::getValue },
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