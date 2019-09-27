#pragma once
#include "_includes.h"
#include <string>
#include <vector>

namespace LuaSkyrim {
   namespace util {
      extern luastackchange_t errorHandler(lua_State* luaVM);
      //
      extern void loadPartialLibrary(lua_State* luaVM, const char* lib, lua_CFunction func, std::initializer_list<const char*> allowedFunctions);
      //
      extern void warn(lua_State* luaVM, const char* message);
      extern void warn(lua_State* luaVM, const char* fmt, ...);
      //
      // safeCall: Performs a pcall and logs any errors encountered by 
      // the called function. As with pcall itself, you should push the 
      // function to call and then the arguments (starting with the 
      // first), and then call safeCall.
      //
      // Under the hood, safeCall uses errorHandler to log a stack trace.
      //
      extern luastackchange_t safeCall(lua_State* luaVM, int argCount, int returnCount);
      //
      // tableKeys: Get a list of all string keys in a table.
      //
      extern void tableKeys(lua_State* luaVM, std::vector<std::string>& out, int stackPos);
      //
      extern inline void argwarn(lua_State* L, int arg, const char* message);
      extern uint8_t getAVIndexArg(lua_State* L, int stackPos, int argIndex);
      extern float getColorComponentArg(lua_State* L, int stackPos, int argIndex); // gets an arg as a color component
      extern float getNumberArg(lua_State* L, int stackPos, int argIndex); // gets an arg as a float, even if the arg is an integer
   }
}