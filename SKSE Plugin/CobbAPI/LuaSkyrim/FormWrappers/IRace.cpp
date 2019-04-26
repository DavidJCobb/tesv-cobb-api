#include "IRace.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/TESRace.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _methods {
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
         luastackchange_t name(lua_State* L) {
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
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "isChild", _methods::isChild },
      { "name",    _methods::name },
      { NULL, NULL }
   };
   void IRace::setupMetatable(lua_State* luaVM) {
      static bool isDefined = false;
      if (isDefined)
         return;
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_Race, formWrapperFactory<IRace>);
      mapFormTypeToMetatable(luaVM, kFormType_Race, metatableName);
      isDefined = true;
   };
   IRace* IRace::fromStack(lua_State* luaVM, UInt32 stackPos) {
      return (IRace*) _asClass(luaVM, stackPos, metatableName);
   };
};