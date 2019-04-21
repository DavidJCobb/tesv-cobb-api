#pragma once
#include "_includes.h"

namespace LuaSkyrim {
   extern void* _asClass(lua_State* luaVM, SInt32 stackPos, const char* classKey);
   extern void  _defineClass(lua_State* luaVM, const char* className, const char* superclassName = nullptr, const luaL_Reg* methods = nullptr);
}