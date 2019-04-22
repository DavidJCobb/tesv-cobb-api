#include "IForm.h"
#include "IActorBase.h"
#include "_classes.h"
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
      _defineClass(luaVM, metatableName, nullptr, _metatableMethods);
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
      {  // Create a map of form types to metatables
         lua_newtable(luaVM);
         lua_setfield(luaVM, LUA_REGISTRYINDEX, subclassListName);
      }
      isDefined = true;
   };
   void IForm::mapFormTypeToMetatable(lua_State* luaVM, uint8_t formType, const char* metatableName) {
      lua_getfield(luaVM, LUA_REGISTRYINDEX, subclassListName); // STACK: [subclassList]
      if (lua_isnil(luaVM, -1)) {
         lua_pop(luaVM, 1);
         _MESSAGE("Unable to register metatable name \"%s\" for form type %02X; the list does not exist. Did you call this function before IForm::setupMetatable?", metatableName, formType);
         return;
      }
      // STACK: [registry[subclassListName]]
      lua_pushinteger  (luaVM, formType);      // STACK: [(int)formType, subclassList]
      luaL_getmetatable(luaVM, metatableName); // STACK: [metatableName, (int)formType, subclassList]
      lua_rawset       (luaVM, -3);            // STACK: [subclassList]
      lua_pop(luaVM, 1);
   }

   IForm* IForm::fromStack(lua_State* luaVM, UInt32 stackPos) {
      return (IForm*) _asClass(luaVM, stackPos, metatableName);
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
            return 1;
         }
         lua_pop(luaVM, 2);
      }
      IForm* a = (IForm*) lua_newuserdata(luaVM, sizeof(IForm));
      if (form) {
         lua_getfield(luaVM, LUA_REGISTRYINDEX, subclassListName);
         if (lua_isnil(luaVM, -1)) {
            //
            // This should never occur. It means that the map of form types to subclass metatables 
            // doesn't exist, but that shouldn't happen if IForm::setupMetatable was called.
            //
            lua_pop(luaVM, 1);
            luaL_getmetatable(luaVM, metatableName);
         } else {
            lua_pushinteger(luaVM, form->formType); // STACK: [formType, subclassList]
            lua_rawget     (luaVM, -2); // STACK: [subclassList[formType], subclassList]
            lua_remove     (luaVM, -2); // STACK: [subclassList[formType]]
            if (lua_type(luaVM, -1) != LUA_TTABLE) {
               //
               // This form type isn't mapped to a subclass. Use IForm's metatable.
               //
               lua_pop(luaVM, 1);
               luaL_getmetatable(luaVM, metatableName);
            }
         }
         /*//
         switch (form->formType) {
            case kFormType_NPC:
               luaL_getmetatable(luaVM, IActorBase::metatableName);
               break;
            default:
               luaL_getmetatable(luaVM, metatableName);
         }
         //*/
      } else {
         //
         // No form; return nil.
         //
         lua_pushnil(luaVM);
         return 1;
      }
      lua_setmetatable(luaVM, -2);
      {  // Store the new wrapper.
         //
         // Store the newly-created heavy-userdata, so that we can reuse it 
         // later per the above code.
         //
         ; // STACK: [wrapper]
         lua_getfield (luaVM, LUA_REGISTRYINDEX, registryName); // STACK: [list, wrapper]
         lua_pushvalue(luaVM, -2);       // STACK: [wrapper, list, wrapper]
         lua_rawsetp  (luaVM, -2, form); // STACK: [list, wrapper]
         lua_pop      (luaVM, 1);        // STACK: [wrapper]
      }
      a->wrapped = form;
      return 1;
   };
};