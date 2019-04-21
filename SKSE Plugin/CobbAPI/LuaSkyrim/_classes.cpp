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
         if (!lua_getmetatable(luaVM, -2)) // STACK: [meta, k, t]
            return 0;
         lua_replace  (luaVM, -3); // STACK: [k, meta]
         lua_pushvalue(luaVM, -1); // STACK: [k, k, meta]
         lua_rawget   (luaVM, -3); // STACK: [meta[k], k, meta]
         if (!lua_isnil(luaVM, -1))
            return 1;
         lua_pop(luaVM, 1); // STACK: [k, meta]
         while (true) {
            lua_pushstring(luaVM, "__superclass"); // STACK: ["__superclass",    k, meta]
            auto type = lua_rawget(luaVM, -3);     // STACK: [meta.__superclass, k, meta]
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
      lua_checkstack(luaVM, 3);
      auto top = lua_gettop(luaVM);
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
         auto type = lua_rawget(luaVM, -3); // STACK: [classMeta["__superclass"], classMeta, instanceMeta, ...]
         if (type != LUA_TTABLE) {
            lua_settop(luaVM, top);
            return nullptr;
         }
         lua_replace(luaVM, -3); // STACK: [classMeta, classMeta["__superclass"], ...]
      }
      lua_settop(luaVM, top);
      return userdata;
   }
   extern void _defineClass(lua_State* luaVM, const char* className, const char* superclassName, const luaL_Reg* methods) {
      //
      // LUA:
      //    local meta = {}
      //    registry[className] = meta
      //    --
      //    meta.__index = __index -- CFunction
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
      luaL_newmetatable(luaVM, className); // STACK: [newmeta]
      //
      lua_pushstring   (luaVM, "__index"); // STACK: ["__index", newmeta]
      lua_pushcfunction(luaVM, &__index);  // STACK: [CFunction:__index, "__index", newmeta]
      lua_settable     (luaVM, -3);        // STACK: [newmeta]
      //
      if (superclassName) {
         lua_pushstring   (luaVM, "__superclass"); // STACK: ["__superclass", newmeta]
         luaL_getmetatable(luaVM, superclassName); // STACK: [supermeta, "__superclass", newmeta]
         lua_settable     (luaVM, -3); // STACK: [newmeta]
      }
      //
      if (methods)
         luaL_setfuncs(luaVM, methods, 0); // import functions into the metatable
      lua_pop(luaVM, 1); // pop metatable from the stack
   }
}