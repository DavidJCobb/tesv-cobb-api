#pragma once
#include "skse/GameForms.h"

namespace RE {
   class TESWeather : public TESForm { // sizeof == 0x760
      public:
         enum { kTypeID = kFormType_Weather };
         //
         enum { kNumColorTypes = 17 };
         enum { kNumTimeOfDay = 4 };
         //
         enum ColorTypes {
            kColorType_SkyUpper = 0,
            kColorType_FogNear = 1,
            kColorType_Unk = 2,
            kColorType_Ambient = 3,
            kColorType_Sunlight = 4,
            kColorType_Sun = 5,
            kColorType_Stars = 6,
            kColorType_SkyLower = 7,
            kColorType_Horizon = 8,
            kColorType_EffectLighting = 9,
            kColorType_CloudLODDiffuse = 10,
            kColorType_CloudLODAmbient = 11,
            kColorType_FogFar = 12,
            kColorType_SkyStatics = 13,
            kColorType_WaterMultiplier = 14,
            kColorType_SunGlare = 15,
            kColorType_MoonGlare = 16
         };
         enum TimeOfDay {
            kTime_Sunrise = 0,
            kTime_Day = 1,
            kTime_Sunset = 2,
            kTime_Night = 3
         };
         enum Classification : UInt8 {
            kClassification_Pleasant = 1,
            kClassification_Cloudy   = 2,
            kClassification_Rainy    = 4,
            kClassification_Snowy    = 8,
         };
         //
         struct ColorType { // sizeof == 0x10
            UInt32 time[kNumTimeOfDay];
         };
         struct General { // sizeof == 0x13
            UInt8	unk00[3];
            UInt8	transDelta;				// Div 1000
            UInt8	sunGlare;				// Div 256
            UInt8	sunDamage;				// Div 256
            UInt16	pad06;
            UInt8 unk08;
            UInt8 unk09;
            UInt8 unk0A;
            UInt8 classification; // 0B
            UInt32	unk0C;
            UInt8	unk10;
            UInt8	windDirection;			// Div (256/360)
            UInt8	windDirRange;			// Div (256/180)
            UInt8	pad13;
         };
         struct FogDistance { // sizeof == 0x20
            float	nearDay;
            float	farDay;				// 10
            float	nearNight;			// 14
            float	farNight;			// 18
            float	powerDay;			// 1C
            float	powerNight;
            float	maxDay;
            float	maxNight;
         };
         struct CloudAlpha {
            float	time[kNumTimeOfDay];
         };

         TESTexture1024	texture[0x20];					// 014
         UInt8			unk114[0x20];					// 114 - cleared to 0x7F
         UInt8			unk134[0x20];					// 134 - cleared to 0x7F
         UInt8			unk154[0x200];					// 154
         CloudAlpha		cloudAlpha[0x20];				// 354
         UInt32			unk554;							// 554
         General			general;						// 558
                                                //UInt8			pad56B;							// 56B
         FogDistance		fogDistance;					// 56C
         ColorType		colorTypes[kNumColorTypes];		// 58C
         TESAIForm::Data	unk69C;							// 69C
         UnkArray		unk6A4;							// 6A4
         UInt32			pad6B0;							// 6B0 - not init'd
         TESImageSpace	* imageSpaces[kNumTimeOfDay];
         BGSLightingTemplate::Data::Color	directionalAmbient[kNumTimeOfDay];	// 6C4
         TESModel		unk744;	// 744
         BGSShaderParticleGeometryData	* particleShader;	// 758
         BGSReferenceEffect				* referenceEffect;	// 75C
   };
   static_assert(sizeof(TESWeather) == 0x760, "");
};