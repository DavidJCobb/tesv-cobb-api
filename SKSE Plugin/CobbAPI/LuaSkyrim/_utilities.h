#pragma once
#include "_includes.h"
#include <string>
#include <vector>

namespace LuaSkyrim {
   namespace util {
      //
      // tableKeys: Get a list of all string keys in a table.
      //
      void tableKeys(lua_State* luaVM, std::vector<std::string>& out, int stackPos);
   }
}