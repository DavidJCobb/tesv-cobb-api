#pragma once
#include "_includes.h"

class SpellItem;
namespace RE {
   class Actor;
   class TESObjectREFR;
}

namespace LuaSkyrim {
   constexpr char* ce_hookConstantToRegistryNameMap        = "Skyrim.Hooks.constantsToLists";
   constexpr char* ce_hookFunctionList_eventSpellCastStart = "Skyrim.Hooks.eventTESSpellCastEvent";
   constexpr char* ce_hookFunctionList_interceptActorKill  = "Skyrim.Hooks.interceptActorKill";
   constexpr char* ce_hookFunctionList_interceptAVChange   = "Skyrim.Hooks.interceptAVChange";

   class HookManager {
      public:
         static void attach(lua_State*); // define needed singletons, etc., in a Lua instance
         static void setup(); // patch the engine so we can run our hooks

         static void  eventSpellCastStart(RE::TESObjectREFR* target, SpellItem* spell);
         static bool  interceptActorKill(RE::Actor* target, RE::Actor* killer);
         static float interceptAVChange(RE::Actor*, uint8_t avIndex, float pendingChange); // called by an engine patch

         enum Hook : int {
            kHook_InterceptAVChange   = 1,
            kHook_InterceptActorKill  = 2,
            kHook_EventSpellCastStart = 3,
         };
   };
}