#include "Hooks.h"
#include "SkyrimLuaService.h"
#include "FormWrappers/IForm.h"

#include "Miscellaneous/scope_control.h"
#include "_utilities.h"

#include "skse/SafeWrite.h"

namespace LuaSkyrim {
   namespace Patches {
      namespace InterceptActorValueChange {
         void _stdcall Inner(RE::Actor* target, uint8_t avIndex, float* result) {
            *result = HookManager::interceptAVChange(target, avIndex, *result);
         }
         _declspec(naked) void Outer() {
            _asm {
               lea  eax, [esp + 0x8];
               push eax;
               push edi;
               push esi;
               call Inner; // stdcall
            lExit:
               mov  ecx, 0x012E2BDC;      // ActorValueList::GetSingleton()
               mov  ecx, dword ptr [ecx]; // reproduce patched-over instruction
               mov  eax, 0x006DFB5B;
               jmp  eax;
            };
         }
         void Apply() {
            WriteRelJump(0x006DFB55, (UInt32)&Outer);
         }
      }
      void Apply() {
         InterceptActorValueChange::Apply();
      }
   }
   namespace { // Lua methods
      namespace _methods {
         luastackchange_t registerForEvent(lua_State* luaVM) {
            //
            // TODO: Registrations should specify a name, so they can be unregistered; 
            // we should also allow specifying filters to be applied on the C++ side 
            // of things.
            //
            luaL_argcheck(luaVM, lua_isstring(luaVM, 1),   1, "unique string expected");
            luaL_argcheck(luaVM, lua_isfunction(luaVM, 2), 2, "function expected");
            lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_hookFunctionList_interceptAVChange); // STACK: [list]
            if (lua_type(luaVM, -1) != LUA_TTABLE)
               return 0; // TODO: warn: hook unavailable (DLL didn't do setup properly or something)
            lua_pushvalue(luaVM,  1); // STACK: [arg1,       list, arg2, arg1]
            lua_rawget   (luaVM, -2); // STACK: [list[arg1], list, arg2, arg1]
            if (!lua_isnil(luaVM, -1)) {
               //
               // This event name is already in use.
               //
               // TODO: Warn.
               //
               return 0;
            }
            lua_pop(luaVM, 1); // STACK: [list, arg2, arg1]
            lua_pushvalue(luaVM, 1);
            lua_pushvalue(luaVM, 2); // STACK: [arg2, arg1, list, arg2, arg1]
            lua_rawset(luaVM, -3); // STACK: [list, arg2, arg1]
            lua_pop(luaVM, 3); // should be arg count + 1
            return 0;
         }
         luastackchange_t unregisterForEvent(lua_State* luaVM) {
            luaL_argcheck(luaVM, lua_isstring(luaVM, 1), 1, "string expected");
            lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_hookFunctionList_interceptAVChange); // STACK: [list]
            if (lua_type(luaVM, -1) != LUA_TTABLE)
               return 0; // TODO: warn: hook unavailable (DLL didn't do setup properly or something)
            lua_pushvalue(luaVM, 1);
            lua_pushnil(luaVM); // STACK: [nil, arg1, list, ...]
            lua_rawset(luaVM, -3); // STACK: [list, ...]
            return 0;
         }
      }
   }
   static const luaL_Reg _singletonMethods[] = {
      { "registerForEvent",   _methods::registerForEvent },
      { "unregisterForEvent", _methods::unregisterForEvent },
      { NULL, NULL }
   };

   void HookManager::attach(lua_State* luaVM) {
      lua_newuserdata (luaVM, 1);  // STACK: [singleton]
      lua_newtable    (luaVM);     // STACK: [meta, singleton]
      lua_pushstring  (luaVM, "__index"); // STACK: ["__index", meta, singleton]
      lua_pushvalue   (luaVM, -2); // STACK: [meta, "__index", meta, singleton]
      lua_settable    (luaVM, -3); // STACK: [meta, singleton]
      luaL_setfuncs   (luaVM, _singletonMethods, 0); // STACK: [meta, singleton]
      lua_setmetatable(luaVM, -2); // STACK: [singleton]
      lua_setglobal   (luaVM, "skyrim_hooks"); // STACK: []
      //
      // Create function lists for each hook:
      //
      lua_newtable(luaVM);
      lua_setfield(luaVM, LUA_REGISTRYINDEX, ce_hookFunctionList_interceptAVChange);
   }

   void HookManager::setup() {
      Patches::Apply();
   };

   float HookManager::interceptAVChange(RE::Actor* target, uint8_t avIndex, float pendingChange) {
      //
      // We don't want to allow recursive hooks to fire. Consider the case of one 
      // Lua script trying to reroute health damage to magicka (using SetAV calls) 
      // and a separate script trying to reroute magicka damage to health: if we 
      // allow this to run recursively, then the two scripts will deadlock and 
      // hang the game.
      //
      // We solve this by disallowing recursion in this hook. This has the side-
      // effect that Lua can only react to actor value changes that were caused by 
      // something other than Lua.
      //
      // TODO: Can the game run multiple actor value changes concurrently? There 
      // are three threads that we can intercept a change from: the main thread 
      // (for changes from the console), and the two AI linear task threads. Are 
      // these ever working at the same time? If so, then this implementation of 
      // a recursion check will cause us to miss events from simultaneous changes.
      //
      static bool s_isRunning = false;
      if (s_isRunning)
         return pendingChange;
      cobb::scoped_no_recurse guard(s_isRunning);
      //
      auto& service = SkyrimLuaService::GetInstance();
      auto  luaVM   = service.getState();
      if (!luaVM)
         return pendingChange;
      lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_hookFunctionList_interceptAVChange); // STACK: [list]
      if (lua_type(luaVM, -1) != LUA_TTABLE) {
         lua_pop(luaVM, 1);
         return pendingChange;
      }
      std::vector<std::string> listeners;
      util::tableKeys(luaVM, -1);
      float originalChange = pendingChange;
      for (const auto& it : listeners) {
         // STACK: [list]
         lua_pushstring(luaVM, it.c_str()); // STACK: [     key,  list]
         lua_rawget    (luaVM, -2);         // STACK: [list[key], list]
         if (lua_isnil(luaVM, -1)) {
            lua_pop(luaVM, 2); // STACK: []
            return pendingChange;
         }
         if (lua_isfunction(luaVM, -1)) {
            wrapForm(luaVM, (TESForm*)target); // arg1
            lua_pushinteger(luaVM, avIndex);   // arg2
            lua_pushnumber (luaVM, pendingChange); // arg3
            lua_pushnumber (luaVM, originalChange); // arg4
            if (lua_pcall(luaVM, 4, 1, 0) == 0) { // STACK: [(list[i](target, avIndex, pendingChange)) or errorText, list]
               if (lua_isnumber(luaVM, -1)) {
                  pendingChange = lua_tonumber(luaVM, -1);
               } else if (lua_isinteger(luaVM, -1)) {
                  pendingChange = lua_tointeger(luaVM, -1);
               }
            } else {
               _MESSAGE("Encountered an error in an intercept-AV-change event listener: %s", lua_tostring(luaVM, -1));
            }
            lua_pop(luaVM, 1); // STACK: [list]
         } else
            lua_pop(luaVM, 1); // STACK: [list]
      }
   }
}