#pragma once
#include "skse/GameForms.h"

namespace RE {
   class TESEffectShader : public TESForm {
      public:
         enum { kTypeID = kFormType_EffectShader };

         struct Data { // sizeof == 0x190
            UInt8  unk000;
            UInt8  pad001; // these three padding bytes correspond to the "Unknown" entry in the DATA shown in xEdit.
            UInt8  pad002;
            UInt8  pad003;
            UInt32 unk004 = 5;
            UInt32 unk008 = 1;
            UInt32 unk00C = 3;
            UInt32 fillColorKey1            = 0;   // 010 // 0xAABBGGRR, alpha unused
            float  fillAlphaTimeFadeIn      = 0;   // 014
            float  fillAlphaTimeFull        = 0;   // 018
            float  fillAlphaTimeFadeOut     = 0;   // 01C
            float  fillAlphaRatioPersistent = 0;   // 020
            float  fullAlphaPulseAmplitude  = 0;   // 024
            float  fillAlphaPulseFrequency  = 1.0; // 028
            UInt32 fillTextureSpeedU        = 0;   // 02C
            UInt32 fillTextureSpeedV        = 0;   // 030
            float  edgeFalloff              = 1.0; // 034
            UInt32 edgeColor                = 0;   // 038 // 0xAABBGGRR, alpha unused
            float  edgeAlphaTimeFadeIn      = 0;   // 03C
            float  edgeAlphaTimeFull        = 0;   // 040
            float  edgeAlphaTimeFadeOut     = 0;   // 044
            float  edgeAlphaRatioPersistent = 0;   // 048
            float  edgeAlphaPulseAmplitude  = 0;   // 04C
            float  edgeAlphaPulseFrequency  = 1.0; // 050
            float  fillAlphaRatioFull       = 1.0; // 054
            float  edgeAlphaRatioFull       = 1.0; // 058
            UInt32 unk05C = 6;
            UInt32 unk060 = 5;
            UInt32 unk064 = 1;
            UInt32 unk068 = 4;
            UInt32 unk06C = 6;
            float  particleBirthTimeRampUp         = 0;   // 070
            float  particleBirthTimeFull           = 0;   // 074
            float  particleBirthTimeRampDown       = 0;   // 078
            float  particleBirthRatioFull          = 1.0; // 07C
            float  particleCountPersistent         = 1.0; // 080
            float  particleLifetime                = 1.0; // 084
            float  particleLifetimeVariance        = 0;   // 088
            float  particleSpeedAlongNormalInitial = 0;   // 08C
            float  particleAccelerationAlongNormal = 0;   // 090
            NiPoint3 particleVelocityInitial;             // 094
            NiPoint3 particleAcceleration;                // 0A0
            float  particleScaleKey1               = 1.0; // 0AC
            float  particleScaleKey2               = 1.0; // 0B0
            float  particleScaleKeyTime1           = 0.0; // 0B4
            float  particleScaleKeyTime2           = 1.0; // 0B8
            UInt32 particleColorKey1        = 0x00FFFFFF; // 0BC // 0xAABBGGRR, alpha unused
            UInt32 particleColorKey2        = 0x00FFFFFF; // 0C0 // 0xAABBGGRR, alpha unused
            UInt32 particleColorKey3        = 0x00FFFFFF; // 0C4 // 0xAABBGGRR, alpha unused
            float  particleColorKeyAlpha1          = 1.0; // 0C8
            float  particleColorKeyAlpha2          = 1.0; // 0CC
            float  particleColorKeyAlpha3          = 1.0; // 0D0
            float  particleColorKeyTime1           = 0.0; // 0D4
            float  particleColorKeyTime2           = 0.5; // 0D8
            float  particleColorKeyTime3           = 1.0; // 0DC
            UInt32 unk0E0 = 0;
            UInt32 unk0E4 = 0;
            UInt32 unk0E8 = 0;
            UInt32 unk0EC = 0;
            UInt32 unk0F0 = 0;
            UInt32 unk0F4 = 0;
            UInt32 unk0F8 = 0;
            float  unk0FC = 10.0;
            float  fillHolesValueStart = 255.0;     // 100
            float  fillHolesValueEnd   = 0;         // 104
            float  edgeWidthUnknown    = 0;         // 108
            UInt32 edgeColorUnknown    = 0;         // 10C // 0xAABBGGRR, alpha unused
            UInt32 unk110              = 0;         // 110 // explosion wind speed?
            UInt32 fillTextureCountU   = 1;         // 114
            UInt32 fillTextureCountV   = 1;         // 118
            float  particleAddonTimeFadeIn   = 1.0; // 11C
            float  particleAddonTimeFadeOut  = 1.0; // 120
            float  particleAddonScaleStart   = 1.0; // 124
            float  particleAddonScaleEnd     = 1.0; // 128
            float  particleAddonTimeScaleIn  = 1.0; // 12C
            float  particleAddonTimeScaleOut = 1.0; // 130
            BGSSoundDescriptorForm* ambientSound = NULL; // 134
            UInt32 fillColorKey2 = 0; // 138 // 0xAABBGGRR, alpha unused
            UInt32 fillColorKey3 = 0; // 13C // 0xAABBGGRR, alpha unused
            float  unk140 = 1.0;
            float  unk144 = 1.0;
            float  unk148 = 1.0;
            UInt32 unk14C = 0;
            UInt32 unk150 = 0;
            UInt32 unk154 = 0;
            float  unk158 = 1.0; // (fill?) color scale
            UInt32 unk15C = 0;
            UInt32 unk160 = 0;
            UInt32 unk164 = 0;
            UInt32 unk168 = 0;
            UInt32 unk16C = 0;
            UInt32 unk170 = 0;
            UInt32 unk174 = 0;
            UInt32 unk178 = 0;
            UInt32 unk17C = 0;
            UInt32 unk180 = 0;
            float  unk184 = 1.0;
            float  unk188 = 1.0;
            UInt16 unk18C = 0;
            UInt16 pad18E = 0;
         };

         Data		data;                   // 014
         TESTexture	textureMembraneFill;    // 1A4
         TESTexture	textureParticleShader;  // 1AC
         TESTexture	textureHoles;           // 1B4
         TESTexture	textureMembranePalette; // 1BC
         TESTexture	textureParticlePalette; // 1C4
         UInt32		unk1CC;	// BSIntrusiveRefCounted*
         UInt32		unk1D0;	// BSIntrusiveRefCounted*
   };
};