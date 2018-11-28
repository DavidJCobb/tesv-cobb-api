#pragma once
#include "skse/GameForms.h"
#include "skse/GameObjects.h"

namespace RE {
   class BGSAcousticSpace : public TESBoundObject {
      public:
         enum { kTypeID = kFormType_AcousticSpace };
         enum {
            kVTBL = 0x0108A08C
         };

         TESForm* loopingSound;	// 20
         TESForm* soundRegion;	// 24
         TESForm* reverbType;	// 28
   };
};