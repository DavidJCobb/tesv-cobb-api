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
      {  // Reuse.
         //
         // Check if a heavy-userdata already exists for this form, and if so, 
         // push that onto the top of the Lua stack and return 1.
         //
         // LUA:
         //    local a = LUA_REGISTRYINDEX[registryName]
         //    local b = a[form]
         //    if b then
         //       return b
         //    end
         //
         lua_getfield(luaVM, LUA_REGISTRYINDEX, registryName); // STACK: [list]
         if (lua_rawgetp(luaVM, -1, form) == LUA_TUSERDATA) {  // STACK: [wrapper, list]
            //
            // We already have a wrapper for this form; return it.
            //
            lua_remove(luaVM, -2);
            _MESSAGE("Reusing a form wrapper!");
            {  // DEBUG ONLY
               auto count = lua_gettop(luaVM);
               if (count != 1)
                  _MESSAGE("== WARNING: We are returning with %d elements in the Lua stack!", count);
            }
            return 1;
         }
         _MESSAGE("No pre-existing wrapper; stack size as of immediate check is %d.", lua_gettop(luaVM));
         lua_pop(luaVM, 2);
         {  // DEBUG ONLY
            auto count = lua_gettop(luaVM);
            if (count != 0)
               _MESSAGE("== WARNING: The stack is wrong as of the end of the reuse check!", count);
         }
         /*//
         lua_pushlightuserdata(luaVM, form); // stack: [form, list]
         lua_rawget(luaVM, -2); // stack: [list[form], list]
         lua_remove(luaVM, -2); // stack: [list[form]]
         if (lua_isnil(luaVM, -1) == 0) {
            _MESSAGE("Reusing a form wrapper!");
            //
            // We already have a wrapper for this form; return it.
            //
            return 1;
         }
         lua_remove(luaVM, -1); // stack: [] // pop nil from the stack
         //*/
      }
      IForm* a = (IForm*) lua_newuserdata(luaVM, sizeof(IForm));
      luaL_getmetatable(luaVM, metatableName);
      lua_setmetatable (luaVM, -2);
      {  // Store the new wrapper.
         //
         // Store the newly-created heavy-userdata, so that we can reuse it 
         // later per the above code.
         //
         ; // STACK: [wrapper]
         lua_getfield (luaVM, LUA_REGISTRYINDEX, registryName); // STACK: [list, wrapper]
_MESSAGE("About to persist the wrapper; got registry's wrapper list. Stack count is %d; should be 2.", lua_gettop(luaVM));
         lua_pushvalue(luaVM, -2);       // STACK: [wrapper, list, wrapper]
         lua_rawsetp  (luaVM, -2, form); // STACK: [list, wrapper]
         lua_pop      (luaVM, 1);        // STACK: [wrapper]
         {  // DEBUG ONLY
            auto count = lua_gettop(luaVM);
            if (count != 1)
               _MESSAGE("== WARNING: We are returning with %d elements in the Lua stack!", count);
         }
         /*//
         lua_getfield(luaVM, LUA_REGISTRYINDEX, registryName); // STACK: [list, wrapper]
         lua_pushlightuserdata(luaVM, form); // STACK: [form, list, wrapper]
         lua_pushvalue(luaVM, -3); // STACK: [wrapper, form, list, wrapper]
         lua_rawset(luaVM, -3); // STACK: [list, wrapper]
         lua_remove(luaVM, -1); // STACK: [wrapper] // pop the registry from the stack
         //*/
      }
      a->wrapped = form;
      return 1;
   };
};