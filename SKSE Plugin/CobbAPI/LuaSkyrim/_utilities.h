#pragma once
#include "_includes.h"
#include <string>
#include <vector>

namespace LuaSkyrim {
   namespace util {
      luastackchange_t errorHandler(lua_State* luaVM);
      //
      // safeCall: Performs a pcall and logs any errors encountered by 
      // the called function. As with pcall itself, you should push the 
      // function to call and then the arguments (starting with the 
      // first), and then call safeCall.
      //
      // Under the hood, safeCall uses errorHandler to log a stack trace.
      //
      luastackchange_t safeCall(lua_State* luaVM, int argCount, int returnCount);
      //
      // tableKeys: Get a list of all string keys in a table.
      //
      void tableKeys(lua_State* luaVM, std::vector<std::string>& out, int stackPos);
   }
}