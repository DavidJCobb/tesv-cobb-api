#include "Hooks.h"
#include "SkyrimLuaService.h"
#include "FormWrappers/IForm.h"

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
            luaL_argcheck(luaVM, lua_isfunction(luaVM, 1), 1, "function expected");
            lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_hookFunctionList_interceptAVChange); // STACK: [list]
            if (lua_type(luaVM, -1) != LUA_TTABLE)
               return 0; // TODO: warn
            for (int i = 1; ; i++) {
               lua_rawgeti(luaVM, -1, i);
               if (lua_isnil(luaVM, -1)) {
                  lua_pop      (luaVM, 1); // STACK: [list]
                  lua_pushvalue(luaVM, 1); // STACK: [arg1, list]
                  lua_rawseti  (luaVM, -2, i); // STACK: [list]
                  return 0;
               }
               lua_pop(luaVM, 1);
            }
         }
         luastackchange_t unregisterForEvent(lua_State* L) {
            //
            // TODO
            //
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
      auto& service = SkyrimLuaService::GetInstance();
      auto  luaVM   = service.getOrCreateThread(GetCurrentThreadId());
      if (!luaVM)
         return pendingChange;
      lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_hookFunctionList_interceptAVChange); // STACK: [list]
      if (lua_type(luaVM, -1) != LUA_TTABLE)
         return pendingChange;
      for (int i = 1; ; i++) {
         lua_rawgeti(luaVM, -1, i);
         if (lua_isnil(luaVM, -1)) {
            lua_pop(luaVM, 2);
            return pendingChange;
         }
         if (lua_isfunction(luaVM, -1)) {
            lua_pushnumber (luaVM, pendingChange); // arg3
            lua_pushinteger(luaVM, avIndex);   // arg2
            wrapForm(luaVM, (TESForm*)target); // arg1
            lua_call(luaVM, 3, 1);
            if (lua_isnumber(luaVM, -1)) {
               pendingChange = lua_tonumber(luaVM, -1);
            } else if (lua_isinteger(luaVM, -1)) {
               pendingChange = lua_tointeger(luaVM, -1);
            }
            lua_pop(luaVM, 1);
         }
         lua_pop(luaVM, 1);
      }
   }
}