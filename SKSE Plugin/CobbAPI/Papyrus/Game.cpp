#include "Papyrus/Game.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/Forms/TESWorldSpace.h"
#include "ReverseEngineered/Player/PlayerCharacter.h"
#include "ReverseEngineered/Systems/GameData.h"

static bool* bPlayerCollisionEnabled = (bool*)0x012E3563; // 00 == collision on (normal); 01 == collision off (debug on)

namespace CobbPapyrus {
   namespace Game {
      void SetPlayerCollisionEnabled(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, bool state, bool fullProcessing) {
         if (*bPlayerCollisionEnabled == state) {
            if (fullProcessing) {
               class RE_TES {
                  public:
                     DEFINE_MEMBER_FN_LONG(RE_TES, ToggleCollision, void, 0x00434CC0)
               };
               CALL_MEMBER_FN((RE_TES*)*g_TES, ToggleCollision)();
            } else
               *bPlayerCollisionEnabled = !state;
         }
      };
      bool IsPlayerCollisionEnabled(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         return !(*bPlayerCollisionEnabled);
      };
      TESForm* GetFormFromFile(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 formID, BSFixedString file) {
         //
         // Identical to the vanilla version, except it doesn't spam the log.
         //
         DataHandler* dataHandler = DataHandler::GetSingleton();
         if (!dataHandler)
            return nullptr;
         const ModInfo* mod = dataHandler->LookupModByName(file.data);
         if (!mod)
            return nullptr;
         UInt32 modIndex = mod->modIndex << 0x18;
         if (modIndex == 0xFF000000)
            return nullptr;
         return LookupFormByID((formID & 0x00FFFFFF) | modIndex);
      };
      VMResultArray<float> GetPlayerLastKnownOutdoorPosition(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         VMResultArray<float> result;
         auto player = (*RE::g_thePlayer);
         if (!player) {
            return result;
         }
         result.resize(3);
         if (CALL_MEMBER_FN(player, GetParentWorldspace)()) {
            //
            // The player's last-known outdoor position and worldspace are updated whenever 
            // they pass through a loading screen that takes them from an exterior cell to 
            // an interior cell. The last-known outdoor position isn't live-updated while 
            // the player is outside.
            //
            result[0] = player->pos.x; // just use the current position
            result[1] = player->pos.y;
            result[2] = player->pos.z;
            return result;
         }
         NiPoint3& pos = player->wentInsideAt;
         if (pos.x == pos.y && pos.y == pos.z && pos.x == (std::numeric_limits<float>::max)()) {
            //
            // If the player has never passed from an exterior space to an interior space, 
            // then all coordinates in the last-known outdoor position are FLT_MAX.
            //
            result.resize(0);
            return result;
         }
         result[0] = pos.x;
         result[1] = pos.y;
         result[2] = pos.z;
         return result;
      };
      RE::TESWorldSpace* GetPlayerLastKnownOutdoorWorldspace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         return (*RE::g_TES)->worldSpace;
      };
   }
}

bool CobbPapyrus::Game::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, void, bool, bool>(
         "SetPlayerCollisionEnabled",
         PapyrusPrefixString("Game"),
         Game::SetPlayerCollisionEnabled,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, bool>(
         "IsPlayerCollisionEnabled",
         PapyrusPrefixString("Game"),
         Game::IsPlayerCollisionEnabled,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, TESForm*, UInt32, BSFixedString>(
         "GetFormFromFile",
         PapyrusPrefixString("Game"),
         Game::GetFormFromFile,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Game"), "IsPlayerCollisionEnabled", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, VMResultArray<float>>(
         "GetPlayerLastKnownOutdoorPosition",
         PapyrusPrefixString("Game"),
         Game::GetPlayerLastKnownOutdoorPosition,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, RE::TESWorldSpace*>(
         "GetPlayerLastKnownOutdoorWorldspace",
         PapyrusPrefixString("Game"),
         Game::GetPlayerLastKnownOutdoorWorldspace,
         registry
      )
   );
   return true;
};