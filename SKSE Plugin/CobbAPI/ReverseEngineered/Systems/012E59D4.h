#pragma once
#include "skse/GameFormComponents.h"
#include "skse/GameForms.h"
#include "skse/Utilities.h"
#include "Shared.h"

namespace RE {
   class Unknown012E59D4 {
      struct UnkFormEntry {
         UInt32        formID;
         TESForm*      form;
         UnkFormEntry* nextEntry;
      };

      Unknown012E59D4* GetInstance() {
         return (Unknown012E59D4*)0x012E59D4; // or should it be *(ClassName*)0x012E59D4 ?
      };
      //
      void*       unk00;
      void*       unk04;
      UInt32      unk08;
      UInt32      unk0C;
      SimpleLock  unk10;
      SimpleLock  unk18;
      //
      MEMBER_FN_PREFIX(Unknown012E59D4);
   };

   // Dword at *(0x012E59D0) is incremented by what LOOKS like a TESForm constructor
};