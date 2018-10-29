#pragma once
#include "skse/GameData.h"
#include "skse/GameForms.h"

class TESImageSpace;
namespace RE {
   class Sky { // sizeof == 0x238
      public:
	      virtual ~Sky();
         //
	      void* unk04;			// 04 - BSMultiBoundNode
	      NiNode* unk08;			// 08
	      UInt32 unk0C;				// 0C
	      UInt32 unk10;				// 10
	      UInt32 unk14;				// 14
	      UInt32 unk18;				// 18
	      UInt32 unk1C;				// 1C
	      UInt32 unk20;				// 20
	      UInt32 unk24;				// 24
	      TESClimate* climate;			// 28
	      TESWeather* weather;			// 2C
	      UInt32			unk30;				// 30
	      UInt32			unk34;				// 34
	      UInt32			unk38;				// 38
	      TESRegion* region;			// 3C
	      Atmosphere* atmosphere;		// 40
	      Stars* stars;			// 44
	      Sun* sun;				// 48
	      Clouds* clouds;			// 4C
	      Moon* masser;			// 50
	      Moon* secunda;			// 54
	      Precipitation* precipitation;	// 58
	      float			unk5C[6];			// 5C
	      UInt32			unk74[3];			// 74
	      float			unk80[6];			// 80
	      UInt32			unk98[6];			// 98
	      float			unkB0[9];			// B0
	      UInt32			unkD4[6];			// D4
	      float			unkEC[9];			// EC
	      UInt32			unk110[9];			// 110
	      float			unk134[7];			// 134
	      UInt32			unk150[3];			// 150
	      float			unk15C[5];			// 15C
	      UInt32			unk170;				// 170
	      void* unk174;			// 174
	      UInt32			unk178[3];			// 178
	      float			unk184;				// 184
	      UInt32			unk188[6];			// 188
	      float			unk1A0[(0x210 - 0x1A0) >> 2];	// 1A0
	      void* skyEffectController;			// 210
	      UInt32			unk214[(0x238 - 0x214) >> 2];	// 214

	      MEMBER_FN_PREFIX(Sky);
	      DEFINE_MEMBER_FN(IsSnowing, bool, 0x00504350);
         DEFINE_MEMBER_FN(Subroutine00595B80, void,           0x00595B80, float); // handles cell imagespaces; argument may be in-game time passed
         DEFINE_MEMBER_FN(Subroutine005920A0, TESImageSpace*, 0x005920A0);
         DEFINE_MEMBER_FN(Subroutine00592100, TESImageSpace*, 0x00592100);
      };
      static_assert(sizeof(Sky) == 0x238, "RE::Sky is the wrong size.");
};