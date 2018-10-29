#pragma once

#include "ReverseEngineered/Forms/BaseForms/base.h"

namespace RE {
   class TESObjectLIGH : public TESBoundAnimObject {
      public:
         enum { kTypeID = kFormType_Light };
         enum Flags { // seems to match the ESP/ESM flags
            kFlag_Dynamic        = 0x00000001,
            kFlag_Carryable      = 0x00000002,
            kFlag_Negative       = 0x00000004,
            kFlag_Flicker        = 0x00000008,
            kFlag_Unk00000010    = 0x00000010,
            kFlag_OffByDefault   = 0x00000020,
            kFlag_FlickerSlow    = 0x00000040,
            kFlag_Pulse          = 0x00000080,
            kFlag_PulseSlow      = 0x00000100,
            kFlag_TypeSpot       = 0x00000200,
            kFlag_TypeSpotShadow = 0x00000400,
            kFlag_TypeHemiShadow = 0x00000800,
            kFlag_TypeOmniShadow = 0x00001000,
            kFlag_PortalStrict   = 0x00002000,
            //
            kFlag_TypeOmni = 0,
            kFlags_Type    = kFlag_TypeSpot | kFlag_TypeSpotShadow | kFlag_TypeHemiShadow | kFlag_TypeOmniShadow,
         };
         //
         // Parents:
         TESFullName                fullName;      // 20
         TESModelTextureSwap        texSwap;       // 28
         TESIcon                    icon;          // 44
         BGSMessageIcon             messageIcon;   // 4C
         TESWeightForm              weight;        // 58
         TESValueForm               value;         // 60
         BGSDestructibleObjectForm  destructible;  // 68
         BGSEquipType               equipType;     // 70
         //
         // Members:
         struct Data78 { // sizeof() == 0x28 // light data, probably
            struct Color {
               UInt8 pad03; // 03
               UInt8 blue;  // 02
               UInt8 green; // 01
               UInt8 red;   // 00
            };
            UInt32	unk00;	// 00
            UInt32	radius;	// 04
            Color   color;  // 08
            UInt32 flags;	// 0C // This is a bitmask. AND with 0x1C00 and then compare to the enum above to see the light type.
            float 	unk10;	// falloff exponent or near clip
            float	baseFOV;// 14
            float	unk18;	// falloff exponent or near clip
            UInt32	unk1C;	// 1C
            UInt32	unk20;	// 20
            UInt32	unk24;	// 24
         };
         struct DataA8 {
            UInt32	unk0;	// 0
            UInt32	unk4;	// 4
            UInt32	unk8;	// 8
         };
         //
         Data78   unk78;   // 78
         float    fade;    // A0 // are we sure this isn't part of the previous member? minor quibble, though
         UInt32   unkA4;   // A4
         DataA8   unkA8;   // A8
   };
   static_assert(sizeof(TESObjectLIGH) == 0xB4, "RE::TESObjectLIGH is not the right size.");
}