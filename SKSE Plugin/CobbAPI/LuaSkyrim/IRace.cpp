#include "IRace.h"
#include "_classes.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/TESRace.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _methods {
         luastackchange_t isChild(lua_State* L) {
            IForm* form = IRace::fromStack(L);
            luaL_argcheck(L, form != nullptr, 1, "'IRace' expected");
            if (!form->wrapped) {
               lua_pushnil(L);
            } else {
               auto race = (RE::TESRace*) form->wrapped;
               bool flag = race->data.raceFlags & RE::TESRace::kRace_Child;
               lua_pushboolean(L, flag);
            }
            return 1;
         };
         luastackchange_t name(lua_State* L) {
            IForm* form = IRace::fromStack(L);
            luaL_argcheck(L, form != nullptr, 1, "'IRace' expected");
            if (!form->wrapped) {
               lua_pushnil(L);
            } else {
               auto race = (RE::TESRace*) form->wrapped;
               lua_pushstring(L, race->fullName.name.data);
            }
            return 1;
         };
         luastackchange_t __tostring(lua_State* L) {
            IForm* form = IForm::fromStack(L);
            luaL_argcheck(L, form != nullptr, 1, "'IActorBase' expected");
            if (!form->wrapped)
               lua_pushfstring(L, "[NONE]");
            else
               lua_pushfstring(L, "[RACE:%08X]", form->wrapped->formID);
            return 1; // indicate how many things we added to the Lua stack
         }
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "isChild",    _methods::isChild },
      { "name",       _methods::name },
      { "__tostring", _methods::__tostring },
      { NULL, NULL }
   };
   void IRace::setupMetatable(lua_State* luaVM) {
      static bool isDefined = false;
      if (isDefined)
         return;
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      mapFormTypeToMetatable(luaVM, kFormType_Race, metatableName);
      isDefined = true;
   };
   IRace* IRace::fromStack(lua_State* luaVM, UInt32 stackPos) {
      return (IRace*) _asClass(luaVM, stackPos, metatableName);
   };
};