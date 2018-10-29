#pragma once
#include "skse/GameData.h"

namespace RE {
   class EquipManager {
      public:
         virtual ~EquipManager();

         static inline EquipManager* GetSingleton(void) {
            return *((EquipManager**)0x012E5FAC);
         };

         struct EquipRequest {
            BaseExtraList* extraData; // 00
            SInt32         count;     // 04
            BGSEquipSlot*  equipSlot; // 08
            UInt32         unk0C = 0; // 0C
            bool           withEquipSound; // 10
            bool           preventUnequip; // 11
            bool           showMessage;    // 12
            UInt8          unk23;     // 23
            UInt32         unk24 = 0; // 24
         };

         MEMBER_FN_PREFIX(EquipManager);
         //
         // EQUIPITEM
         //  - equipSlot can be nullptr, in which case a default is selected.
         //
         DEFINE_MEMBER_FN(EquipItem,   void, 0x006EF3E0, Actor* actor, TESForm* item, BaseExtraList* extraData, SInt32 count, BGSEquipSlot* equipSlot, bool withEquipSound, bool preventUnequip, bool showMsg, void* unk);
         DEFINE_MEMBER_FN(UnequipItem, bool, 0x006EE560, Actor* actor, TESForm* item, BaseExtraList* extraData, SInt32 count, BGSEquipSlot* equipSlot, bool unkFlag1, bool preventEquip, bool unkFlag2, bool unkFlag3, void* unk);
         //
         DEFINE_MEMBER_FN(EquipItem_Internal, void, 0x006EEE20, Actor* actor, TESForm* item, EquipRequest*); // do not call; this doesn't use the lock and doesn't supply defaults; listed for documentation only
         //
         DEFINE_MEMBER_FN(GetDefaultEquipSlot, BGSEquipSlot*, 0x006EDD00, Actor* actor, TESForm* item);
   };
};