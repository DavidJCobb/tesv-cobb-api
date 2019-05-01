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
      namespace _helpers {
         void eventConstantToListRegistryKey(lua_State* luaVM, lua_Number i, const char*& out) {
            lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_hookConstantToRegistryNameMap);
            if (lua_type(luaVM, -1) != LUA_TTABLE)
               luaL_error(luaVM, "An internal error occurred: CobbAPI was unable to map the hook type constant to the hook listener list name.");
            lua_rawgeti(luaVM, -1, i); // STACK: [map[i], map]
            out = lua_tostring(luaVM, -1);
            if (!out)
               luaL_error(luaVM, "specified hook type does not exist");
            lua_pop(luaVM, 2);
         }
      }
      namespace _methods {
         luastackchange_t registerForEvent(lua_State* luaVM) {
            //
            // TODO: Registrations should specify a name, so they can be unregistered; 
            // we should also allow specifying filters to be applied on the C++ side 
            // of things.
            //
            luaL_argcheck(luaVM, lua_isstring  (luaVM, 1), 1, "unique string expected");
            luaL_argcheck(luaVM, lua_isnumber  (luaVM, 2), 2, "hook type (as number) expected");
            luaL_argcheck(luaVM, lua_isfunction(luaVM, 3), 3, "function expected");
            const char* listKey = nullptr;
            _helpers::eventConstantToListRegistryKey(luaVM, lua_tonumber(luaVM, 2), listKey);
            lua_getfield(luaVM, LUA_REGISTRYINDEX, listKey); // STACK: [list, arg3, arg2, arg1]
            if (lua_type(luaVM, -1) != LUA_TTABLE)
               return luaL_error(luaVM, "An internal error occurred: CobbAPI was unable to access the listener list for this hook.");
            lua_pushvalue(luaVM,  1); // STACK: [arg1,       list, arg3, arg2, arg1]
            lua_rawget   (luaVM, -2); // STACK: [list[arg1], list, arg3, arg2, arg1]
            if (!lua_isnil(luaVM, -1)) {
               //
               // This event name is already in use.
               //
               return luaL_error(luaVM, "The specified event name is already in use for this hook."); // TODO: I think this should be a non-fatal warning instead
            }
            lua_pop(luaVM, 1); // STACK: [list, arg3, arg2, arg1]
            lua_pushvalue(luaVM, 1);
            lua_pushvalue(luaVM, 2); // STACK: [arg2, arg1, list, arg3, arg2, arg1]
            lua_rawset(luaVM, -3); // STACK: [list, arg3, arg2, arg1]
            return 0;
         }
         luastackchange_t unregisterForEvent(lua_State* luaVM) {
            luaL_argcheck(luaVM, lua_isstring(luaVM, 1), 1, "string expected");
            luaL_argcheck(luaVM, lua_isnumber(luaVM, 2), 2, "hook type (as number) expected");
            luaL_argcheck(luaVM, lua_isfunction(luaVM, 3), 3, "function expected");
            const char* listKey = nullptr;
            _helpers::eventConstantToListRegistryKey(luaVM, lua_tonumber(luaVM, 2), listKey);
            lua_getfield(luaVM, LUA_REGISTRYINDEX, listKey); // STACK: [list]
            if (lua_type(luaVM, -1) != LUA_TTABLE)
               return luaL_error(luaVM, "An internal error occurred: CobbAPI was unable to access the listener list for this hook.");
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

   void _prepHook(lua_State* luaVM, const char* constantName, HookManager::Hook hook, const char* hookRegistryName, int mapIndex) {
      mapIndex = lua_absindex(luaVM, mapIndex);
      lua_newtable(luaVM);
      lua_setfield(luaVM, LUA_REGISTRYINDEX, hookRegistryName);
      lua_pushnumber(luaVM, hook);
      lua_pushstring(luaVM, hookRegistryName);
      lua_rawset    (luaVM, mapIndex);
      lua_pushnumber(luaVM, hook);
      lua_setglobal (luaVM, constantName);
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
      // Create the constant-to-registry map:
      //
      lua_newtable (luaVM);
      lua_pushvalue(luaVM, -1);
      lua_setfield (luaVM, LUA_REGISTRYINDEX, ce_hookConstantToRegistryNameMap);
      //
      // Set up the various hooks' Lua stuff:
      //
      _prepHook(luaVM, "SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE", kHook_InterceptAVChange, ce_hookFunctionList_interceptAVChange, -1);
      //
      lua_pop(luaVM, 1); // pop the constant-to-registry-name map from the stack
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
_MESSAGE("LUA: INTERNAL: Unable to get the list of listeners for the InterceptAVChange hook!");
         lua_pop(luaVM, 1);
         return pendingChange;
      }
      //
      // We can't use lua_next to iterate the list of listeners, because a listener 
      // could conceivably unregister itself (or any other listener), which would 
      // potentially break lua_next iteration. Instead, we get a list of all keys 
      // using a helper function (which uses lua_next), and then iterate those keys 
      // manually.
      //
      // Note that our helper function only retrieves string (and number) keys, but 
      // since every listener must identify itself with a unique string anyway, that 
      // isn't a major issue.
      //
      std::vector<std::string> listeners;
      util::tableKeys(luaVM, listeners, -1);
      float originalChange = pendingChange;
_MESSAGE("LUA: INTERNAL: InterceptAVChange hook is going to execute %d listeners...", listeners.size());
      for (const auto& it : listeners) {
         // STACK: [list]
         lua_pushstring(luaVM, it.c_str()); // STACK: [     key,  list]
         lua_rawget    (luaVM, -2);         // STACK: [list[key], list]
         if (lua_isfunction(luaVM, -1)) {
            auto top = lua_gettop(luaVM);
            wrapForm(luaVM, (TESForm*)target); // arg1
            lua_pushinteger(luaVM, avIndex);   // arg2
            lua_pushnumber (luaVM, pendingChange); // arg3
            lua_pushnumber (luaVM, originalChange); // arg4
            if (util::safeCall(luaVM, 4, 1) == LUA_OK) { // STACK: [(list[i](target, avIndex, pendingChange)) or errorText, list]
               if (lua_isnumber(luaVM, -1)) {
                  pendingChange = lua_tonumber(luaVM, -1);
               } else if (lua_isinteger(luaVM, -1)) {
                  pendingChange = lua_tointeger(luaVM, -1);
               }
            }
            lua_settop(luaVM, top);
         } else {
_MESSAGE(" - Failed to access one of the listener functions (%s). It tests as not a function.", it.c_str());
            lua_pop(luaVM, 1); // STACK: [list]
         }
      }
      return pendingChange;
   }
}