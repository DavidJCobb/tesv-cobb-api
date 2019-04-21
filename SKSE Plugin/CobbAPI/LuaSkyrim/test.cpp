#include "test.h"
#include "_includes.h"
#include "IForm.h"
#include "IActorBase.h"
#include "skse/GameForms.h" // LookupFormByID
#include "skse/Utilities.h" // GetRuntimeDirectory

namespace LuaSkyrim {
   const std::string& GetScriptBasePath() {
      static std::string path;
      if (path.empty()) {
         std::string	runtimePath = GetRuntimeDirectory();
         if (!runtimePath.empty()) {
            path = runtimePath;
            path += "Data\\CobbAPILua\\";
         }
      }
      return path;
   };

   namespace {
      namespace _globals {
         static luastackchange_t LuaLog(lua_State* L) {
            int n = lua_gettop(L); // number of arguments
            //
            const char* out = nullptr;
            if (!lua_isstring(L, 1)) {
               lua_pushliteral(L, "incorrect argument");
               lua_error(L);
            } else {
               out = lua_tolstring(L, 1, nullptr);
               _MESSAGE("Logging message from a Lua script: %s", out);
            }
            /*// SAMPLE CODE -- Loop over the arguments:
            for (int i = 2; i <= n; i++) {
               if (!lua_isnumber(L, i)) {
                  lua_pushliteral(L, "incorrect argument");
                  lua_error(L);
               }
               lua_tonumber(L, i);
            }
            //
            // Returning two numbers:
            //
            lua_pushnumber(L, sum / n);
            lua_pushnumber(L, sum);
            return 2;
            //*/
            return 0; // return the number of return values, after pushing return values onto the Lua stack
         }
         static luastackchange_t FormByID(lua_State* L) {
            int n = lua_gettop(L); // number of arguments
            //
            if (!lua_isnumber(L, 1)) {
               lua_pushliteral(L, "incorrect argument");
               lua_error(L);
               return 0;
            }
            lua_Number num = lua_tonumber(L, 1);
            UInt32 id = num;
            TESForm* form = ::LookupFormByID(id);
            return IForm::make(L, form);
         }
      }
   }

   void Test() {
      lua_State* luaVM = luaL_newstate(); // create the entire VM
      //
      std::string file = "nativeTestScript.lua";
      file = GetScriptBasePath() + file;
      //
      int status = luaL_loadfile(luaVM, file.c_str());
      if (status) {
         _MESSAGE("Couldn't load file: %s\n", lua_tostring(luaVM, -1)); // loadfile logs error messages to the top of Lua's stack
         lua_close(luaVM); // Terminate the VM.
         return;
      }
      //
      // Set up metatables and similar for all native types that we plan to expose to Lua:
      //
      // DO NOT FORGET TO CALL THESE
      //
      IForm::setupMetatable(luaVM);
      IActorBase::setupMetatable(luaVM);
      //
      lua_register(luaVM, "logmessage", _globals::LuaLog); // make a C function available to the Lua script under the name "logmessage"
      lua_register(luaVM, "form_by_id", _globals::FormByID);
      //
      // Make the standard libraries available to scripts. There are 
      // some that we want to omit to avoid threading issues and similar.
      //
      luaL_requiref(luaVM, "_G", luaopen_base, true); // TODO: shim or block: dofile, load, loadfile, print
      lua_pop(luaVM, 1); // pop library off the stack
      luaL_requiref(luaVM, "math", luaopen_math, true);
      lua_pop(luaVM, 1); // pop library off the stack
      luaL_requiref(luaVM, "string", luaopen_string, true);
      lua_pop(luaVM, 1); // pop library off the stack
      luaL_requiref(luaVM, "table", luaopen_table, true);
      lua_pop(luaVM, 1); // pop library off the stack
      luaL_requiref(luaVM, "utf8", luaopen_utf8, true);
      lua_pop(luaVM, 1); // pop library off the stack
      //
      // Let's pass a table to Lua.
      //
      // Values are sent to Lua using a stack. We push the table, then 
      // we push a key and a value, and then we make a rawset call with 
      // the stack indices of the pushed values.
      //
      lua_newtable(luaVM); // stack[-1] == table
      for (int i = 1; i <= 5; i++) {
         lua_pushnumber(luaVM, i);     // stack[-1] == key;   stack[-2] == table
         lua_pushnumber(luaVM, i * 2); // stack[-1] == value; stack[-2] == key; stack[-3] == table
         //
         // Rawset uses the topmost two values on the stack as the value and 
         // the key to set, and takes as an argument the stack offset of the 
         // table to write to. The key and value will be removed from the 
         // stack.
         //
         lua_rawset(luaVM, -3);
      }
      lua_setglobal(luaVM, "nativeTable"); // Assign the value at the top of the stack to a global variable.
      //
      // Now let's run the script:
      //
      int result = lua_pcall(luaVM, 0, LUA_MULTRET, 0);
      if (result) {
         _MESSAGE("Failed to run script: %s\n", lua_tostring(luaVM, -1));
         lua_close(luaVM); // Terminate the VM.
         return;
      }
      //
      // Every Lua script is an anonymous function. If the script ran a top-level 
      // "return" statement, then the return value will be at the top of the Lua 
      // stack, and we can retrieve it:
      //
      double returned = lua_tonumber(luaVM, -1);
      _MESSAGE("Script returned: %.0f\n", returned);
      lua_pop(luaVM, 1); // Remove the return value from the top of the stack.
      lua_close(luaVM); // Terminate the VM.
   }
}