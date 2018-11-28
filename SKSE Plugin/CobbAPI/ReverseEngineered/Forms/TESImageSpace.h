#pragma once
#include "skse/GameForms.h"
#include "skse/GameObjects.h"

namespace RE {
   class TESImageSpace : public TESForm { // sizeof == 0x64
      public:
         enum { kTypeID = kFormType_ImageSpace };
         enum {
            kVTBL = 0x010871EC
         };

         struct Data14 { // sizeof == 0x50
            float	unk00;	// 00 - init'd to 3
            float	unk04;	// 04 - init'd to 7
            float	unk08;	// 08 - init'd to .6
            float	unk0C;	// 0C - init'd to .5
            float	unk10;	// 10 - init'd to .15
            float	unk14;	// 14 - init'd to .15
            float	unk18;	// 18 - init'd to 1.8
            float	unk1C;	// 1C - init'd to 1.5
            float	unk20;	// 20 - init'd to 3.5, 3
            float	unk24;	// 24 - init'd to 1.8, .8
            float	unk28;	// 28 - init'd to 1.5
            float	unk2C;	// 2C - init'd to 1.1
            float	unk30;	// 30 - init'd to 0
            float	unk34;	// 34 - init'd to 0
            float	unk38;	// 38 - init'd to 0
            float	unk3C;	// 3C - init'd to 0
            float	unk40;	// 40 - init'd to 0
            UInt32	unk44;	// 44 - init'd to 0
            UInt32	unk48;	// 48 - init'd to 0
            float	unk4C;	// 4C - init'd to 2
         };
         Data14 data; // 14
   };
};