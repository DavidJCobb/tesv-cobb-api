#include "_utilities.h"

namespace LuaSkyrim {
   namespace util {
      luastackchange_t errorHandler(lua_State* luaVM) {
         auto original = lua_tostring(luaVM, -1);
         luaL_traceback(luaVM, luaVM, original, 1);
         _MESSAGE("Intercepted an error from Lua:\n%s\n\n--", lua_tostring(luaVM, -1));
         return 1;
      }
      void loadPartialLibrary(lua_State* luaVM, const char* modname, lua_CFunction openf, std::initializer_list<const char*> allowedFunctions) {
         lua_checkstack(luaVM, 4);
         luaL_requiref(luaVM, modname, openf, true);
         //
         lua_pushnil(luaVM); // stack: [nil, library]
         while (lua_next(luaVM, -2) != 0) { // stack: [value, key, library]
            lua_pop(luaVM, 1); // stack: [key, library]
            const char* key = lua_tostring(luaVM, -1);
            if (!key)
               continue;
            bool found = false;
            for (auto it = allowedFunctions.begin(); it != allowedFunctions.end(); ++it) {
               auto funcName = *it;
               if (strcmp(key, funcName) == 0) {
                  found = true;
                  break;
               }
            }
            if (found)
               continue;
            lua_pushvalue(luaVM, -1); // stack: [key, key, library]
            lua_pushnil(luaVM); // stack: [nil, key, key, library]
            lua_rawset(luaVM, -4); // stack: [key, library]
         }
         // stack: [library]
      }
      void warn(lua_State* luaVM, const char* message) {
         lua_checkstack(luaVM, 1);
         luaL_traceback(luaVM, luaVM, message, 1);
         _MESSAGE("Intercepted a warning from Lua:\n%s\n\n--", lua_tostring(luaVM, -1));
         lua_pop(luaVM, 1);
      }
      void warn(lua_State* luaVM, const char* fmt, ...) {
         va_list argp;
         va_start(argp, fmt);
         lua_checkstack(luaVM, 2); // where .. pushvfstring, traceback
         luaL_where(luaVM, 1);
         lua_pushvfstring(luaVM, fmt, argp);
         va_end(argp);
         lua_concat(luaVM, 2);
         auto message = lua_tostring(luaVM, -1);
         luaL_traceback(luaVM, luaVM, message, 1);
         _MESSAGE("Intercepted a warning from Lua:\n%s\n\n--", lua_tostring(luaVM, -1));
         lua_pop(luaVM, 2); // where .. pushvfstring, traceback
      }
      luastackchange_t safeCall(lua_State* luaVM, int argCount, int returnCount) {
         int handlerPos = lua_gettop(luaVM) - argCount; // this is currently the position of the function to call, but we'll be moving the handler here
         //
         // Add the error handler to the stack, and then move it before the Lua 
         // function and arguments:
         //
         lua_pushcfunction(luaVM, errorHandler); // STACK: [error handler, args..., function]
         lua_insert(luaVM, handlerPos); // STACK: [args..., function, error handler]
         // STACK: [args..., function, error handler]
         //
         auto ret = lua_pcall(luaVM, argCount, returnCount, handlerPos);
         // STACK: [return values..., error handler]
         lua_remove(luaVM, -returnCount - 1); // remove error handler from stack
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
       inline void argwarn(lua_State* L, int arg, const char* message) {
         //
         // following code is based on luaL_argerror:
         //
         lua_Debug ar;
         if (!lua_getstack(L, 0, &ar)) // no stack frame?
            return warn(L, "bad argument #%d (%s)", arg, message);
         lua_getinfo(L, "n", &ar);
         if (strcmp(ar.namewhat, "method") == 0) {
            arg--; // don't count the (self) argument
            if (arg == 0) // the (self) argument is the one that's busted
               return warn(L, "calling '%s' on bad self (%s)", ar.name, message);
         }
         if (ar.name == NULL) // function name not identifiable
            ar.name = "?";
         return warn(L, "bad argument #%d to '%s' (%s)", arg, ar.name, message);
      }
       uint8_t getAVIndexArg(lua_State* L, int stackPos, int argIndex) {
         lua_Integer i;
         if (lua_isnumber(L, stackPos)) {
            lua_Number  f = lua_tonumber(L, stackPos);
            luaL_argcheck(L, lua_numbertointeger(f, &i), argIndex, "whole number or integer expected");
         } else if (lua_isinteger(L, stackPos)) {
            i = lua_tointeger(L, stackPos);
         } else {
            luaL_argerror(L, argIndex, "number expected");
         }
         luaL_argcheck(L, i >= 0 && i <= 255, argIndex, "actor value index is out of bounds");
         return i;
      }
      float getColorComponentArg(lua_State* L, int stackPos, int argIndex) { // gets an arg as a color component
         lua_Integer i;
         if (lua_isnumber(L, stackPos)) {
            lua_Number f = lua_tonumber(L, stackPos);
            luaL_argcheck(L, lua_numbertointeger(f, &i), argIndex, "float could not be converted to an integer");
         } else if (lua_isinteger(L, stackPos)) {
            i = lua_tointeger(L, stackPos);
         } else {
            luaL_argcheck(L, false, argIndex, "number expected");
         }
         if (i < 0)
            i = 0;
         else if (i > 255)
            i = 255;
         return i;
      }
      float getNumberArg(lua_State* L, int stackPos, int argIndex) { // gets an arg as a float, even if the arg is an integer
         if (lua_isnumber(L, stackPos)) {
            return lua_tonumber(L, stackPos);
         } else if (lua_isinteger(L, stackPos)) {
            return lua_tointeger(L, stackPos);
         } else {
            luaL_argcheck(L, false, argIndex, "number expected");
         }
      }
      void dumpStackContent(lua_State* L) {
         _MESSAGE("STACK:");
         auto top = lua_gettop(L);
         for (int i = 1; i <= top; i++) {
            auto type = lua_type(L, i);
            switch (type) {
               case LUA_TSTRING:  /* strings */
                  _MESSAGE("   \"%s\"", lua_tostring(L, i));
                  break;
               case LUA_TBOOLEAN:  /* booleans */
                  _MESSAGE("   %s", lua_toboolean(L, i) ? "true" : "false");
                  break;
               case LUA_TNUMBER:  /* numbers */
                  _MESSAGE("   %g", lua_tonumber(L, i));
                  break;
               default:  /* other values */
                  _MESSAGE("   %s", lua_typename(L, type));
                  break;
            }
         }
      }
   }
}