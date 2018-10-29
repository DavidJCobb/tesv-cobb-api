#pragma once
#include "Shared.h"
#include "skse/SafeWrite.h"

#include "Utilities/Dumps/ASM.h"
#include "Utilities/Dumps/TESObjectREFR.h"

namespace CobbPatches {
   namespace FormRestoreState {
      static const UInt32 targetBase = 0x004DE9E0; // TESObjectREFR::Unk_0F
      static const UInt32 entry = targetBase + 0x30A;

      void Hook(TESObjectREFR* reference) {
         CobbDumpForm(reference);
      };

      void __declspec(naked) Patch() {
         __asm {
            push esi;
            call Hook;
            add esp, 4; // clean up after last non-member call
            retn 4;
         };
      };

      void Apply() {
         WriteRelJump(entry, (UInt32)Patch);
      };
   };
};