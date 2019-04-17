#include "IForm.h"
#include "ReverseEngineered/Forms/TESForm.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _methods {
         luastackchange_t formID(lua_State* L) {
            IForm* form = IForm::fromStack(L);
            luaL_argcheck(L, form != nullptr, 1, "'IForm' expected");
            if (!form->wrapped)
               lua_pushnumber(L, 0);
            else
               lua_pushnumber(L, form->wrapped->formID);
            return 1;
         };
         luastackchange_t formType(lua_State* L) {
            IForm* form = IForm::fromStack(L);
            luaL_argcheck(L, form != nullptr, 1, "'IForm' expected");
            if (!form->wrapped)
               lua_pushnumber(L, 0);
            else
               lua_pushnumber(L, form->wrapped->formType);
            return 1;
         };
         luastackchange_t __tostring(lua_State* L) {
            IForm* form = IForm::fromStack(L);
            luaL_argcheck(L, form != nullptr, 1, "'IForm' expected");
            if (!form->wrapped)
               lua_pushfstring(L, "[NONE]");
            else
               lua_pushfstring(L, "[FORM:%08X]", form->wrapped->formID);
            return 1; // indicate how many things we added to the Lua stack
         }
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "formID",     _methods::formID },
      { "formType",   _methods::formType },
      { "__tostring", _methods::__tostring },
      { NULL, NULL }
   };
   void IForm::setupMetatable(lua_State* luaVM) {
      static bool isDefined = false;
      if (isDefined)
         return;
      {  // Create the metatable
         luaL_newmetatable(luaVM, metatableName);
         //
         lua_pushstring(luaVM, "__index");
         lua_pushvalue(luaVM, -2); // pushes the metatable
         lua_settable(luaVM, -3); // metatable.__index = metatable
         //
         // TODO: The metatable should also define __meta, so that it can't be 
         // retrieved using getmetatable(wrapperObj).
         //
         luaL_setfuncs(luaVM, _metatableMethods, 0); // import functions into the metatable
         //
         lua_pop(luaVM, 1); // pop metatable from the stack
      }
      {  // Create the registry
         //
         // wrapper_map = {}
         // wrapper_map_metatable = {}
         // wrapper_map_metatable["__mode"] = "v"
         // setmetatable(wrapper_map, wrapper_map_metatable)
         // LUA_REGISTRYINDEX[registryName] = wrapper_map
         //
         lua_newtable    (luaVM);
         lua_newtable    (luaVM);       // STACK: [wrapper_map_metatable, wrapper_map]
         lua_pushstring  (luaVM, "v");  // STACK: ["v", wrapper_map_metatable, wrapper_map]
         lua_setfield    (luaVM, -2, "__mode"); // STACK: [wrapper_map_metatable, wrapper_map]
         lua_setmetatable(luaVM, -2);   // STACK: [wrapper_map]
         lua_setfield    (luaVM, LUA_REGISTRYINDEX, registryName); // STACK: []
      }
      isDefined = true;
   };
   IForm* IForm::fromStack(lua_State* luaVM, UInt32 stackPos) {
      return (IForm*) luaL_checkudata(luaVM, stackPos, metatableName);
   };

   luastackchange_t IForm::make(lua_State* luaVM, TESForm* form) {
      //
      // TODO: Check if a heavy-userdata already exists for this form, and if so, 
      // push that onto the top of the Lua stack and return 1.
      //
      IForm* a = (IForm*) lua_newuserdata(luaVM, sizeof(IForm));
      luaL_getmetatable(luaVM, metatableName);
      /*
      if (lua_isnil(luaVM, -1)) { // DEBUGGING ONLY
         _MESSAGE("WARNING: The IForm metatable doesn't exist! Did you forget to call IForm::setupMetatable?");
      }
      //*/
      lua_setmetatable (luaVM, -2);
      //
      // TODO: Store the new userdata in the list of form userdata.
      //
      a->wrapped = form;
      return 1;
   };
};