#include "Hooks.h"
#include "SkyrimLuaService.h"
#include "FormWrappers/IForm.h"

#include "Miscellaneous/scope_control.h"
#include "_utilities.h"

#include "ReverseEngineered/Systems/BSTEvent.h"
#include "skse/GameForms.h"
#include "skse/GameRTTI.h"
#include "skse/SafeWrite.h"

#include <functional>

namespace LuaSkyrim {
   namespace EventSinks {
      namespace SpellCastStart {
         class _Sink : public RE::BSTEventSink<RE::TESSpellCastEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev = convertEvent(aEv);
                  if (ev) {
                     SpellItem* s = DYNAMIC_CAST(LookupFormByID(ev->spellFormID), TESForm, SpellItem);
                     if (s)
                        HookManager::eventSpellCastStart(ev->caster, s);
                  }
                  return EventResult::kEvent_Continue;
               };
         };
         static _Sink s_sink;
         //
         void Apply() {
            CALL_MEMBER_FN(&RE::BSTEventSourceHolder::GetOrCreate()->spellCast, AddEventSink)(&s_sink);
         }
      }
      //
      void Apply() {
         SpellCastStart::Apply();
      }
   }
   namespace Patches {
      namespace InterceptActorKill {
         bool _stdcall Inner(RE::Actor* target, RE::Actor* killer) {
            return HookManager::interceptActorKill(target, killer);
         }
         __declspec(naked) void Outer() {
            _asm {
               mov  ecx, dword ptr [esp + 0x8]; // reproduce overwritten instruction: Actor* ecx = Arg1;
               push ecx; // protect
               push ecx;
               push esi;
               call Inner; // stdcall
               pop  ecx; // restore
               test al, al;
               jz   lSkip;
               jmp  lExit;
            lSkip:
               mov  eax, 0x006AC401; // jump to original function's return statement
               jmp  eax;
            lExit:
               test ecx, ecx; // reproduce overwritten instruction
               mov  eax, 0x006AC3BC;
               jmp  eax;
            }
         }
         void Apply() {
            WriteRelJump(0x006AC3B6, (UInt32)&Outer);
            SafeWrite8  (0x006AC3BB, 0x90); // courtesy NOP
         }
      }
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
      //
      void Apply() {
         InterceptActorKill::Apply();
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
            luaL_argcheck(luaVM, lua_isstring  (luaVM, 1), 1, "unique string expected");
            luaL_argcheck(luaVM, lua_isnumber  (luaVM, 2), 2, "hook type (as number) expected");
            luaL_argcheck(luaVM, lua_isfunction(luaVM, 3), 3, "function expected");
            const char* listKey = nullptr;
            _helpers::eventConstantToListRegistryKey(luaVM, lua_tonumber(luaVM, 2), listKey);
            //
            // TODO: Lua error if (listKey) is still nullptr
            //
            lua_getfield(luaVM, LUA_REGISTRYINDEX, listKey); // STACK: [list, arg3, arg2, arg1]
            if (lua_type(luaVM, -1) != LUA_TTABLE)
               return luaL_error(luaVM, "An internal error occurred: CobbAPI was unable to access the listener list for this hook.");
            lua_pushvalue(luaVM,  1); // STACK: [arg1,       list, arg3, arg2, arg1]
            lua_rawget   (luaVM, -2); // STACK: [list[arg1], list, arg3, arg2, arg1]
            if (!lua_isnil(luaVM, -1)) {
               //
               // This event name is already in use.
               //
               return luaL_error(luaVM, "The specified listener name is already in use for this event."); // TODO: I think this should be a non-fatal warning instead
            }
            lua_pop(luaVM, 1); // STACK: [list, arg3, arg2, arg1]
            lua_pushvalue(luaVM, 1);
            lua_pushvalue(luaVM, 3); // STACK: [arg3, arg1, list, arg3, arg2, arg1]
            lua_rawset(luaVM, -3); // STACK: [list, arg3, arg2, arg1]
            return 0;
         }
         luastackchange_t unregisterForEvent(lua_State* luaVM) {
            luaL_argcheck(luaVM, lua_isstring(luaVM, 1), 1, "string expected");
            luaL_argcheck(luaVM, lua_isnumber(luaVM, 2), 2, "hook type (as number) expected");
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
      _prepHook(luaVM, "SKYRIM_EVENT_INTERCEPT_ACTOR_KILL",         kHook_InterceptActorKill,  ce_hookFunctionList_interceptActorKill,  -1);
      _prepHook(luaVM, "SKYRIM_EVENT_INTERCEPT_ACTOR_VALUE_CHANGE", kHook_InterceptAVChange,   ce_hookFunctionList_interceptAVChange,   -1);
      _prepHook(luaVM, "SKYRIM_EVENT_SPELL_CAST_START",             kHook_EventSpellCastStart, ce_hookFunctionList_eventSpellCastStart, -1);
      //
      lua_pop(luaVM, 1); // pop the constant-to-registry-name map from the stack
   }

   void HookManager::setup() {
      EventSinks::Apply();
      Patches::Apply();
   };

   void _forEachListenerOfType(lua_State* luaVM, const char* registryKey, std::function<void(lua_State*)> functor) {
      lua_getfield(luaVM, LUA_REGISTRYINDEX, registryKey); // STACK: [list]
      if (lua_type(luaVM, -1) != LUA_TTABLE) {
         lua_pop(luaVM, 1);
         return;
      }
      std::vector<std::string> listeners;
      util::tableKeys(luaVM, listeners, -1);
      if (!listeners.size()) {
         lua_pop(luaVM, 1); // STACK: []
         return;
      }
      // STACK: [list]
      for (const auto& it : listeners) {
         // STACK: [list]
         lua_pushstring(luaVM, it.c_str()); // STACK: [      key, list]
         lua_rawget    (luaVM, -2);         // STACK: [list[key], list]
         if (lua_isfunction(luaVM, -1)) {
            auto top = lua_gettop(luaVM);
            functor(luaVM);
            if (top - 1 > lua_gettop(luaVM)) { // minus one because we'll have popped the function-to-call too
               _MESSAGE("WARNING: The hook/event handler for %s managed the Lua stack incorrectly! (Difference is %d.)", registryKey, ((top - 1) - lua_gettop(luaVM)));
            }
            lua_settop(luaVM, top - 1); // STACK: [list]
         } else {
            //_MESSAGE(" - Failed to access one of the listener functions (%s). It tests as not a function.", it.c_str());
            lua_pop(luaVM, 1); // STACK: [list]
         }
      }
      lua_pop(luaVM, 1); // STACK: []
      return;
   };
   
   void HookManager::eventSpellCastStart(RE::TESObjectREFR* target, SpellItem* spell) {
      wrapped_lua_pointer luaVM;
      if (!luaVM)
         return;
      //
      // Wrap the forms outside of the loop, so that we don't do redundant 
      // processing. Capture their Lua stack indices for use when pushing 
      // args to each listener.
      //
      wrapForm(luaVM, (TESForm*)target); // STACK: [arg1]
      auto casterPos = lua_gettop(luaVM);
      wrapForm(luaVM, (TESForm*)spell); // STACK: [arg2, arg1]
      auto spellPos  = lua_gettop(luaVM);
      //
      _forEachListenerOfType(luaVM, ce_hookFunctionList_eventSpellCastStart,
         [casterPos, spellPos](lua_State* luaVM) {
            lua_pushvalue(luaVM, casterPos);
            lua_pushvalue(luaVM, spellPos);
            if (util::safeCall(luaVM, 2, 0) == LUA_OK) { // STACK: [arg2, arg1, list]
            }
         }
      );
      lua_pop(luaVM, 2); // pop forms
   }
   bool HookManager::interceptActorKill(RE::Actor* target, RE::Actor* killer) {
      wrapped_lua_pointer luaVM;
      if (!luaVM)
         return true;
      //
      // Wrap the forms outside of the loop, so that we don't do redundant 
      // processing. Capture their Lua stack indices for use when pushing 
      // args to each listener.
      //
      wrapForm(luaVM, (TESForm*)target); // STACK: [arg1]
      auto victimPos = lua_gettop(luaVM);
      wrapForm(luaVM, (TESForm*)killer); // STACK: [arg2, arg1]
      auto killerPos = lua_gettop(luaVM);
      //
      bool result  = true;
      _forEachListenerOfType(luaVM, ce_hookFunctionList_interceptActorKill,
         [victimPos, killerPos, &result](lua_State* luaVM) {
            lua_pushvalue  (luaVM, victimPos);
            lua_pushvalue  (luaVM, killerPos);
            lua_pushboolean(luaVM, result);
            if (util::safeCall(luaVM, 3, 1) == LUA_OK) { // STACK: [(list[i](target, result)) or errorText, arg2, arg1, list]
               if (lua_isboolean(luaVM, -1))
                  result = lua_toboolean(luaVM, -1);
               lua_pop(luaVM, 1); // pop return values
            }
         }
      );
      lua_pop(luaVM, 2); // pop forms
      return result;
   }
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
      // something other than Lua reacting to an actor value change (i.e. the 
      // vanilla game or a different Lua hook).
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
      wrapped_lua_pointer luaVM;
      if (!luaVM)
         return pendingChange;
      //
      // Wrap the forms outside of the loop, so that we don't do redundant 
      // processing. Capture their Lua stack indices for use when pushing 
      // args to each listener.
      //
      wrapForm(luaVM, (TESForm*)target); // STACK: [arg1]
      auto  actorPos       = lua_gettop(luaVM);
      float originalChange = pendingChange;
      //
      _forEachListenerOfType(luaVM, ce_hookFunctionList_interceptAVChange,
         [actorPos, avIndex, originalChange, &pendingChange](lua_State* luaVM) {
            lua_pushvalue  (luaVM, actorPos);
            lua_pushinteger(luaVM, avIndex);
            lua_pushnumber (luaVM, pendingChange);
            lua_pushnumber (luaVM, originalChange);
            if (util::safeCall(luaVM, 4, 1) == LUA_OK) {
               if (lua_isnumber(luaVM, -1)) {
                  pendingChange = lua_tonumber(luaVM, -1);
               } else if (lua_isinteger(luaVM, -1)) {
                  pendingChange = lua_tointeger(luaVM, -1);
               }
               lua_pop(luaVM, 1); // pop return values
            }
         }
      );
      lua_pop(luaVM, 1); // pop forms
      return pendingChange;
   }
}