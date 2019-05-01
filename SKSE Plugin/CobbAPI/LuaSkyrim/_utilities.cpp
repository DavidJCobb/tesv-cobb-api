#include "_utilities.h"

namespace LuaSkyrim {
   namespace util {
      void tableKeys(lua_State* luaVM, std::vector<std::string>& out, int stackPos) {
         stackPos = absoluteStackIndex(luaVM, stackPos);
         lua_pushnil(luaVM); // STACK: [nil, ..., table]
         while (lua_next(luaVM, stackPos) != 0) {
            // STACK: [value, key, ..., table]
            if (lua_isstring(luaVM, -2)) {
               //
               // lua_isstring returns true if the value is a string or a number, 
               // since Lua automatically casts numbers to strings as appropriate. 
               // If we call lua_tostring on a number in the stack, then the stack 
               // item is replaced with the created string. That can break traversal 
               // with lua_next, because lua_next needs to have the last key we 
               // iterated over on the stack; if we convert a number to a string, 
               // then the stack element is no longer a key in the table.
               //
               // Accordingly, we just push the key onto the stack again, and tamper 
               // with the new stack entry.
               //
               lua_pushvalue(luaVM, -2); // [key, value, key, ..., table]
               auto key = lua_tostring(luaVM, -1);
               out.push_back(key);
               lua_pop(luaVM, 1);
            }
            lua_pop(luaVM, 1); // pop value; leave key
         }
      }
      std::vector<std::string> tableKeys(lua_State* luaVM, int stackPos) {
         std::vector<std::string> out;
         tableKeys(luaVM, out, stackPos);
         return out;
      }
   }
}