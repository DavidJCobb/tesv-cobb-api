#include "_utilities.h"

namespace LuaSkyrim {
   namespace util {
      luastackchange_t errorHandler(lua_State* luaVM) {
         auto original = lua_tostring(luaVM, -1);
         luaL_traceback(luaVM, luaVM, original, 1);
         _MESSAGE("Intercepted an error from Lua:\n%s\n\n--", lua_tostring(luaVM, -1));
         return 1;
      }
      luastackchange_t safeCall(lua_State* luaVM, int argCount, int returnCount) {
         int handlerPos = lua_gettop(luaVM) - argCount;
         //
         // Add the error handler to the stack, and then move it before the Lua 
         // function and arguments:
         //
         lua_pushcfunction(luaVM, errorHandler);
         lua_insert(luaVM, handlerPos);
         //
         auto ret = lua_pcall(luaVM, argCount, returnCount, handlerPos);
         lua_remove(luaVM, handlerPos); // remove error handler from stack
         return ret;
      }
      void tableKeys(lua_State* luaVM, std::vector<std::string>& out, int stackPos) {
         stackPos = lua_absindex(luaVM, stackPos);
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
               // then the stack element no longer matches the key in the table.
               //
               // Accordingly, we just push the key onto the stack again, and tamper 
               // with the new stack entry.
               //
               lua_pushvalue(luaVM, -2); // [key, value, key, ..., table]
               auto key = lua_tostring(luaVM, -1);
               out.push_back(key);
               lua_pop(luaVM, 1); // pop duplicate key, potentially altered from number to string
            }
            lua_pop(luaVM, 1); // pop value; leave key
         }
      }
   }
}