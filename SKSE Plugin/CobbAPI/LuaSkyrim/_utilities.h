#pragma once
#include "_includes.h"
#include <string>
#include <vector>

namespace LuaSkyrim {
   namespace util {
      inline int absoluteStackIndex(lua_State* luaVM, int index) {
         if (index > 0 || index < LUA_REGISTRYINDEX)
            return index;
         return lua_gettop(luaVM) + index + 1;
      };
      //
      // tableKeys: Get a list of all string keys in a table.
      //
      void tableKeys(lua_State* luaVM, std::vector<std::string>& out, int stackPos);
      std::vector<std::string> tableKeys(lua_State* luaVM, int stackPos);
   }
}