#include "IRace.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/TESRace.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _methods {
         luastackchange_t getName(lua_State* L) {
            IForm* wrapper = IRace::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IRace' expected");
            auto form = (RE::TESRace*) wrapper->unwrap();
            if (!form) {
               lua_pushnil(L);
            } else {
               lua_pushstring(L, form->fullName.name.data);
            }
            return 1;
         };
         luastackchange_t isChild(lua_State* L) {
            IForm* wrapper = IRace::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IRace' expected");
            auto form = (RE::TESRace*) wrapper->unwrap();
            if (!form) {
               lua_pushnil(L);
            } else {
               bool flag = form->data.raceFlags & RE::TESRace::kRace_Child;
               lua_pushboolean(L, flag);
            }
            return 1;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "getName", _methods::getName },
      { "isChild", _methods::isChild },
      { NULL, NULL }
   };
   void IRace::setupClass(lua_State* luaVM) {
      if (_isClassDefined(luaVM, metatableName))
         return;
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_Race, formWrapperFactory<IRace>);
      mapFormTypeToMetatable(luaVM, kFormType_Race, metatableName);
   };
   IRace* IRace::fromStack(lua_State* luaVM, SInt32 stackPos) {
      return (IRace*) _asClass(luaVM, stackPos, metatableName);
   };
};