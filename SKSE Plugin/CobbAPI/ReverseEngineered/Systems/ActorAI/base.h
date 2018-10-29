#pragma once
#include "skse/Utilities.h"

namespace RE {
   class IAIWorldLocationHandle { // superclass of PackageLocation?
      //
      // VTBL: 0x010C0198
      // RTTI: 0x0125DABC
      //
      public:
         virtual ~IAIWorldLocationHandle();
         // No more than three more virtual methods.
         //
         UInt8  unk04; // if equal to 0xFF, this object is inactive?
         UInt8  pad05[3];
         UInt32 unk08;
         UInt32 unk0C;
         //
         MEMBER_FN_PREFIX(IAIWorldLocationHandle);
   };
};