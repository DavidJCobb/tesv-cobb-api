#include "_classes.h"

namespace LuaSkyrim {
   namespace { // member functions for the class metatables
      static luastackchange_t __index(lua_State* luaVM) {
         //
         // LUA:
         //    function __index(t, k)
         //       local meta = getmetatable(t)
         //       if not meta then
         //          return nil
         //       end
         //       local a = rawget(meta, k)
         //       if a then
         //          return a
         //       end
         //       while true do
         //          local b = rawget(meta, "__superclass")
         //          if type(b) ~= "table" then
         //             return nil
         //          end
         //          meta = b
         //          local c = rawget(meta, k)
         //          if c then
         //             return c
         //          end
         //       end
         //    end
         //
         //
         // TODO: This function should return nil if the key is the name of 
         // any metamethod, or if the key is something we use internally (e.g. 
         // "__superclass"), to prevent Lua scripts from accessing superclass 
         // metatables and doing shenanigans with them.
         //
         if (!lua_getmetatable(luaVM, -2)) // STACK: [meta, k, t]
            return 0;
         lua_replace  (luaVM, -3); // STACK: [k, meta]
         lua_pushvalue(luaVM, -1); // STACK: [k, k, meta]
         lua_rawget   (luaVM, -3); // STACK: [meta[k], k, meta]
         if (!lua_isnil(luaVM, -1))
            return 1;
         lua_pop(luaVM, 1); // STACK: [k, meta]
         //
         // If we want to use fields instead of methods, then every metatable 
         // needs to have a table of getters (field name as key; CFunction as 
         // value), and we need to look up from that table at this point.
         //
         // If we go that route, then we'll want to update the documentation 
         // comment at the top of this function, which is a Lua translation 
         // of the C code.
         //
         while (true) {
            lua_pushstring(luaVM, "__superclass"); // STACK: ["__superclass",    k, meta]
            auto type = lua_rawget(luaVM, -3);     // STACK: [meta.__superclass, k, meta]
            if (type != LUA_TTABLE)
               return 0;
            lua_replace  (luaVM, -3); // STACK: [k, meta.__superclass]
            //
            // If we want to use fields instead of methods, then we need to 
            // replace the rest of this loop with code equivalent to:
            //
            //    if meta.__superclass[k].__index then
            //       return meta.__superclass[k].__index(meta.__superclass[k], k)
            //    end
            //
            // If we go that route, then we'll want to update the documentation 
            // comment at the top of this function, which is a Lua translation 
            // of the C code.
            //
            lua_pushvalue(luaVM, -1); // STACK: [k, k, meta.__superclass]
            lua_rawget   (luaVM, -3); // STACK: [meta.__superclass[k], k, meta.__superclass]
            if (!lua_isnil(luaVM, -1))
               return 1;
            lua_pop(luaVM, 1); // STACK: [k, meta.__superclass]
         }
      }
      //
      // If we want to use fields instead of methods, then we need to add a 
      // __newindex metamethod, and modify (_defineClass) to create two tables 
      // (__getters and __setters) on the metatable. Those two tables will need 
      // to be maps of field names to CFunctions; __index will need to consult 
      // the getter table (as written above) and __newindex will need to consult 
      // the setter table.
      //
      // We should probably add a __newindex method anyway to make writes silently 
      // fail.
      //
      // If we go with fields, then we should probably also modify (_defineClass) 
      // to take optional luaL_Reg*s of field-getters and field-setters, and to 
      // write those CFunctions into the relevant metatable tables.
      //
      // The issue with fields is that it becomes impossible to adjust structs 
      // without creating wrappers for them. Consider something like this:
      //
      //    local position = myReference.position -- returns {x, y, z}
      //    position[0] = 5
      //
      // That code wouldn't do anything if we just return the value. To make it 
      // work, we'd have to construct an entire wrapper just for the reference's 
      // position, and synch it with the reference. By contrast, methods make 
      // this easy:
      //
      //    local x, y, z = myReference:position()
      //    myReference:position(5, nil, nil) -- nil = unchanged
      //
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
         if (lua_isnil(luaVM, -1)) {
            _MESSAGE("WARNING: Failed to set up %s as the superclass of %s; the former's metatable is not yet defined. Are you defining your classes in the wrong order?", superclassName, className);
         }
         lua_settable(luaVM, -3); // STACK: [newmeta]
      }
      //
      // Define __metatable on the metatable, so that Lua scripts can't retrieve 
      // the metatable and do shenanigans with it:
      //
      lua_pushstring(luaVM, "__metatable");
      lua_pushboolean(luaVM, false);
      lua_settable(luaVM, -3); // metatable.__metatable = false
      //
      if (methods)
         luaL_setfuncs(luaVM, methods, 0); // import functions into the metatable
      lua_pop(luaVM, 1); // pop metatable from the stack
   }
}