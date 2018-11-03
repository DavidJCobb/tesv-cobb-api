#pragma once
#include "skse/GameReferences.h"
#include "skse/Utilities.h"
#include "Shared.h"

namespace RE {
   class bhkSimpleShapePhantom;
   class Unknown01B115D8 { // constructor at 0x0058BD40 // size unknown; probably has three more padding bytes
      static Unknown01B115D8* GetInstance() {
         return (Unknown01B115D8*)0x01B115D8; // == 0x01B39A80, always // or should it be *(ClassName*)0x01B9ED08 ?
      };
      //
      UInt32   unk00;
      UInt32   crosshairRefHandle = *g_invalidRefHandle; // 04
      UInt32   unk08 = *g_invalidRefHandle; // fallback crosshair ref handle; used if crosshairRefHandle == *g_invalidRefHandle
      UInt32   unk0C = 0;
      NiPoint3 unk10;
      void*    unk1C = nullptr;
      bhkSimpleShapePhantom* unk20 = nullptr;
      UInt32   unk24 = 0; // word, extended
      UInt8    unk28 = false;
      //
      MEMBER_FN_PREFIX(Unknown01B115D8);
      DEFINE_MEMBER_FN(Reset, void, 0x0058AC50); // resets all handles and the NiPoint3
      DEFINE_MEMBER_FN(Subroutine0058AC90, void, 0x0058AC90); // destructor?
   };
};