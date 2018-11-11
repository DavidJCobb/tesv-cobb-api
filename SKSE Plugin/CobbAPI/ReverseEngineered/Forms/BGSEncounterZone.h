#pragma once
#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
   class BGSEncounterZone : public TESForm { // sizeof == 0x30
      public:
         enum { kTypeID = kFormType_EncounterZone };
         enum {
            kFlag_NeverResets          = 1,
            kFlag_MatchPCBelowMinLevel = 2,
            kFlag_DisableCombatBounary = 4,
         };

         TESForm*     owner;    // 14 // TESActorBase* or TESFaction*
         BGSLocation* location; // 18
         UInt8 rank;     // 1C
         UInt8 minLevel; // 1D
         UInt8 flags;    // 1E
         UInt8 maxLevel; // 1F
         UInt8 unk20[0x10]; // 20
   };
};