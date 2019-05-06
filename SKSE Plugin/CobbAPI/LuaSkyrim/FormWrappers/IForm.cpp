#include "IForm.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/TESForm.h"

namespace LuaSkyrim {
   static FormWrapperFactoryFunction _wrapperFactoriesByFormType[0xFF];

   luastackchange_t wrapForm(lua_State* luaVM, TESForm* form) {
      if (!form) {
         //
         // No form; return nil.
         //
         lua_pushnil(luaVM);
         return 1;
      }
      auto formID = form->formID;
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
         lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_formWrapperReuseKey); // STACK: [list]
         if (lua_isnil(luaVM, -1)) {
            _MESSAGE("LUA: WARNING: The form wrapper reuse table is missing!");
            return 0;
         }
         if (lua_rawgeti(luaVM, -1, formID) == LUA_TUSERDATA) {  // STACK: [wrapper, list]
            IForm* a = IForm::fromStack(luaVM, -1);
            if (a && !a->isDeleted && a->formType == form->formType && !a->ruleOutForm(form)) {
               //
               // If a wrapper already exists for this form ID, then we need to make sure 
               // it actually refers to this form. If so, then we can return it.
               //
               //  - Temporary form IDs can themselves be reused for different forms, so 
               //    we need to check the form type and make sure that we are in fact 
               //    referring to the same form.
               //
               //  - The form type alone isn't enough, though, because a new form of the 
               //    same type could be created with the same form ID (particularly 
               //    common for TESObjectREFR). We need to also check any distinguishing 
               //    characteristics about the form.
               //
               // But if those tests pass, then we already have a wrapper for this form, 
               // and we can return it.
               //
               lua_remove(luaVM, -2);
               return 1;
            }
         }
         lua_pop(luaVM, 2);
      }
      auto formType = form->formType;
      IForm* a;
      if (_wrapperFactoriesByFormType[formType]) {
         a = _wrapperFactoriesByFormType[formType](luaVM, form);
      } else {
         a = formWrapperFactory<IForm>(luaVM, form);
      }
      //
      lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_formSubclassListKey);
      if (lua_isnil(luaVM, -1)) {
         //
         // This should never occur. It means that the map of form types to subclass metatables 
         // doesn't exist, but that shouldn't happen if IForm::setupMetatable was called.
         //
         lua_pop(luaVM, 1);
         luaL_getmetatable(luaVM, IForm::metatableName);
      } else {
         lua_pushinteger(luaVM, form->formType); // STACK: [formType, subclassList]
         lua_rawget     (luaVM, -2); // STACK: [subclassList[formType], subclassList]
         lua_remove     (luaVM, -2); // STACK: [subclassList[formType]]
         if (lua_type(luaVM, -1) != LUA_TTABLE) {
            //
            // This form type isn't mapped to a subclass. Use IForm's metatable.
            //
            lua_pop(luaVM, 1);
            luaL_getmetatable(luaVM, IForm::metatableName);
         }
      }
      lua_setmetatable(luaVM, -2);
      {  // Store the new wrapper.
         //
         // Store the newly-created heavy-userdata, so that we can reuse it 
         // later per the above code.
         //
         ; // STACK: [wrapper]
         lua_getfield (luaVM, LUA_REGISTRYINDEX, ce_formWrapperReuseKey); // STACK: [list, wrapper]
         lua_pushvalue(luaVM, -2);         // STACK: [wrapper, list, wrapper]
         lua_rawseti  (luaVM, -2, formID); // STACK: [list, wrapper]
         lua_pop      (luaVM, 1);          // STACK: [wrapper]
      }
      a->wrapped = form;
      return 1;
   };

   void mapFormTypeToFactory(uint8_t formType, FormWrapperFactoryFunction func) {
      _wrapperFactoriesByFormType[formType] = func;
   }

   void mapFormTypeToMetatable(lua_State* luaVM, uint8_t formType, const char* metatableName) {
      lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_formSubclassListKey); // STACK: [subclassList]
      if (lua_isnil(luaVM, -1)) {
         lua_pop(luaVM, 1);
         _MESSAGE("Unable to register metatable name \"%s\" for form type 0x%02X; the list does not exist. Did you call this function before IForm::setupMetatable?", metatableName, formType);
         return;
      }
      // STACK: [registry[subclassListName]]
      lua_pushinteger  (luaVM, formType);      // STACK: [(int)formType, subclassList]
      luaL_getmetatable(luaVM, metatableName); // STACK: [metatableName, (int)formType, subclassList]
      lua_rawset       (luaVM, -3);            // STACK: [subclassList]
      lua_pop(luaVM, 1);
   }

   namespace { // metatable methods
      namespace _methods {
         luastackchange_t getFormID(lua_State* L) {
            IForm* wrapper = IForm::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IForm' expected");
            auto form = wrapper->unwrap();
            if (!form)
               lua_pushnumber(L, 0);
            else
               lua_pushnumber(L, form->formID);
            return 1;
         };
         luastackchange_t getFormType(lua_State* L) {
            IForm* wrapper = IForm::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IForm' expected");
            auto form = wrapper->unwrap();
            if (!form)
               lua_pushnumber(L, 0);
            else
               lua_pushnumber(L, form->formType);
            return 1;
         };
         luastackchange_t __tostring(lua_State* L) {
            IForm* wrapper = IForm::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IForm' expected");
            auto form = wrapper->unwrap();
            if (!form)
               lua_pushstring(L, "[Wrapped Form:NONE]");
            else {
               auto signature = wrapper->signature();
               if (!signature)
                  signature = "????";
               char hex[9];
               snprintf(hex, sizeof(hex), "%08X", form->formID);
               lua_pushfstring(L, "[Wrapped Form:%s:%s]", signature, hex);
            }
            return 1; // indicate how many things we added to the Lua stack
         }
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "getFormID",   _methods::getFormID },
      { "getFormType", _methods::getFormType },
      { "__tostring",  _methods::__tostring },
      { NULL, NULL }
   };
   void IForm::setupClass(lua_State* luaVM) {
      if (_isClassDefined(luaVM, metatableName)) {
         _MESSAGE("LUA: WARNING: The IForm class has already been set up!");
         return;
      }
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
         lua_setfield    (luaVM, LUA_REGISTRYINDEX, ce_formWrapperReuseKey); // STACK: []
      }
      {  // Create a map of form types to metatables
         lua_newtable(luaVM);
         lua_setfield(luaVM, LUA_REGISTRYINDEX, ce_formSubclassListKey);
      }
   };

   IForm* IForm::fromStack(lua_State* luaVM, SInt32 stackPos) {
      return (IForm*) _asClass(luaVM, stackPos, metatableName);
   };
};