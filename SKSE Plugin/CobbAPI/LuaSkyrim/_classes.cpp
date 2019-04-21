#include "_classes.h"

namespace LuaSkyrim {
   namespace {
      static luastackchange_t __index(lua_State* luaVM) {
         //
         // LUA:
         //    function __index(t, k)
         //       local a = rawget(t, k)
         //       if a then
         //          return a
         //       end
         //       local meta = getmetatable(t)
         //       if not meta then
         //          return nil
         //       end
         //       local b = rawget(meta, k)
         //       if b then
         //          return b
         //       end
         //       while true do
         //          local c = rawget(meta, "__superclass")
         //          if type(c) ~= "table" then
         //             return nil
         //          end
         //          meta = c
         //          local d = rawget(meta, k)
         //          if d then
         //             return d
         //          end
         //       end
         //    end
         //
         {  // DEBUG
            auto key1 = lua_tostring(luaVM, 2);
            auto key2 = lua_tostring(luaVM, -1);
            if (key1 && key2) {
               _MESSAGE("__index accessed; key is \"%s\" (should == \"%s\")", key1, key2);
            } else {
               if (key1)
                  _MESSAGE("__index accessed; stack isn't what we expect; index 2 is \"%s\"", key1);
               else if (key2)
                  _MESSAGE("__index accessed; stack isn't what we expect; index -1 is \"%s\"", key2);
               else
                  _MESSAGE("__index accessed; stack isn't what we expect; key unknown");
            }
         }
         /*//
         lua_pushvalue(luaVM,  1); // STACK: [t,    k, t]
         lua_rawget   (luaVM, -3); // STACK: [t[k], k, t]
         if (!lua_isnil(luaVM, -1))
            return 1;
_MESSAGE("__index: key didn't exist on the userdata");
         lua_pop(luaVM, 1); // STACK: [k, t]
         //*/
         if (!lua_getmetatable(luaVM, -2)) // STACK: [meta, k, t]
            return 0;
_MESSAGE("__index: found the userdata metatable");
         lua_replace  (luaVM, -3); // STACK: [k, meta]
         lua_pushvalue(luaVM, -1); // STACK: [k, k, meta]
         lua_rawget   (luaVM, -3); // STACK: [meta[k], k, meta]
_MESSAGE("__index: key exists on the metatable? %d", !lua_isnil(luaVM, -1));
         if (!lua_isnil(luaVM, -1))
            return 1;
_MESSAGE("__index: key didn't exist on the metatable");
         lua_pop(luaVM, 1); // STACK: [k, meta]
         while (true) {
_MESSAGE("__index: new iteration; stack count is %d", lua_gettop(luaVM));
            {  // DEBUG
               lua_pushstring(luaVM, "__name"); // STACK: ["__name", k, meta]
               if (lua_rawget(luaVM, -3) == LUA_TSTRING) { // STACK: [meta.__name, k, meta]
                  auto name = lua_tostring(luaVM, -1);
                  if (name)
                     _MESSAGE(" - current metatable name is \"%s\"", name);
                  else
                     _MESSAGE(" - current metatable is nameless");
               }
               lua_pop(luaVM, 1); // STACK: [k, meta]
            }
            lua_pushstring(luaVM, "__superclass"); // STACK: ["__superclass",    k, meta]
            auto type = lua_rawget(luaVM, -3);     // STACK: [meta.__superclass, k, meta]
if (type != LUA_TTABLE) _MESSAGE("__index: key didn't exist on the base class; returning nothing");
            if (type != LUA_TTABLE)
               return 0;
            lua_replace  (luaVM, -3); // STACK: [k, meta.__superclass]
            lua_pushvalue(luaVM, -1); // STACK: [k, k, meta.__superclass]
            lua_rawget   (luaVM, -3); // STACK: [meta.__superclass[k], k, meta.__superclass]
            if (!lua_isnil(luaVM, -1))
               return 1;
            lua_pop(luaVM, 1); // STACK: [k, meta.__superclass]
         }
      }
   }

   extern void* _asClass(lua_State* luaVM, SInt32 stackPos, const char* classKey) {
      //
      // LUA:
      //    if not userdata then
      //       return nil
      //    end
      //    local instanceMeta = getmetatable(userdata)
      //    local classMeta    = getmetatable(classKey)
      //    while classMeta ~= instanceMeta do
      //       local a = instanceMeta.__superclass
      //       if type(a) ~= "table" then
      //          return nil
      //       end
      //       instanceMeta = a
      //    end
      //    return userdata
      //
      lua_checkstack(luaVM, 4);
      //
      void* userdata = lua_touserdata(luaVM, stackPos);
      if (!userdata)
         //
         // TODO: Every case in which we return nullptr should also log an error.
         //
         return nullptr;
      if (!lua_getmetatable(luaVM, stackPos))
         return nullptr;
      luaL_getmetatable(luaVM, classKey);
      // STACK: [classMeta, instanceMeta, ...]
      while (!lua_rawequal(luaVM, -1, -2)) {
         lua_pushstring(luaVM, "__superclass"); // STACK: ["__superclass", classMeta, instanceMeta, ...]
         auto type = lua_rawget(luaVM, -2); // STACK: [classMeta["__superclass"], "__superclass", classMeta, instanceMeta, ...]
         if (type != LUA_TTABLE) {
            lua_pop(luaVM, 4);
            return nullptr;
         }
         lua_replace(luaVM, -4); // STACK: ["__superclass", classMeta, classMeta["__superclass"], ...]
         lua_pop    (luaVM, 1);  // STACK: [classMeta, classMeta["__superclass"], ...]
      }
      return userdata;
   }
   extern void _defineClass(lua_State* luaVM, const char* className, const char* superclassName, const luaL_Reg* methods) {
      //
      // LUA:
      //    local meta = {}
      //    --
      //    local a
      //    if superclassName then
      //       a = registry[superclassName]
      //    else
      //       a = meta
      //    end
      //    meta.__index = a
      //    --
      //    if superclassName then
      //       meta.__superclass = registry[superclassName]
      //    end
      //    --
      //    if methods then
      //       for k, v in pairs(methods) do
      //          meta[k] = v
      //       end
      //    end
      //
      //
      lua_checkstack(luaVM, 3);
      //
      luaL_newmetatable(luaVM, className);
      //
      lua_pushstring(luaVM, "__index");
      lua_pushcfunction(luaVM, &__index);
      /*//
      if (superclassName)
         luaL_getmetatable(luaVM, superclassName);
      else
         lua_pushvalue(luaVM, -2); // pushes the metatable
      //*/
      lua_settable(luaVM, -3);
      //
      if (superclassName) {
         lua_pushstring   (luaVM, "__superclass");
         luaL_getmetatable(luaVM, superclassName);
         lua_settable     (luaVM, -3);
      }
      //
      if (methods)
         luaL_setfuncs(luaVM, methods, 0); // import functions into the metatable
      lua_pop(luaVM, 1); // pop metatable from the stack
   }
}