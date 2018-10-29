#pragma once
#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
   union CellColor {
      struct {
         UInt8 alpha; // typically unused, so typically zero
         UInt8 blue;
         UInt8 green;
         UInt8 red;
      } color;
      UInt32 abgr;    // 1C
                      //
      CellColor(int a) : abgr(a) {};
   };

   class BGSLightingTemplate : public TESForm { // sizeof == 0x94
      public:
         enum { kTypeID = kFormType_LightingTemplate };

         struct Color { // sizeof == 0x20 // Decoded by SKSE. This is probably what Bethesda uses, but it's not an intuitive way to expose the data.
            UInt32	x[2];				// 00 - init'd to 00FFFFFF
            UInt32	y[2];
            UInt32	z[2];
            UInt32	specular;			// 18 - init'd to 0
            float	fresnelPower;		// 1C - init'd to 1
         };
         //
         struct Data { // sizeof == 0x5C
            CellColor ambientColor     = 0; // 00
            CellColor directionalColor = 0; // 04
            CellColor fogColorNear     = 0; // 08
            float     fogPlaneNear = 0.0F; // 0C
            float     fogPlaneFar  = 0.0F; // 10
            SInt32    directionalRotXY; // 14 // directional rotation XY?
            SInt32    directionalRotZ; // 18 // directional rotation Z?
            float     directionalFade = 1.0F; // 1C // directional fade?
            float     fogClipDistance = 0.0F; // 20
            float     fogPow = 1.0F; // 24
            struct {
               CellColor xPos;
               CellColor xNeg;
               CellColor yPos;
               CellColor yNeg;
               CellColor zPos;
               CellColor zNeg;
            } directionalAmbientUNUSED; // 28
            CellColor specular; // 40
            UInt32    unk44;
            CellColor fogColorFar; // 48
            float    fogMax = 1.0F; // 4C
            float    lightFadeBegin; // 50 - fLightLODStartFade
            float    lightFadeEnd;   // 54 - fLightLODStartFade + fLightLODRange
            UInt32   inheritFromTemplate = 0x79F; // 58 // obviously unused
         };
         Data   data;  // 14
         UInt32 pad70; // 70
         union {
            struct {
               CellColor xPos;
               CellColor xNeg;
               CellColor yPos;
               CellColor yNeg;
               CellColor zPos;
               CellColor zNeg;
            } directionalAmbient;
            CellColor directionalAmbientColors[6];
         }; // 74
         UInt32 unk8C;
         float  unk90 = 1.0F; // fresnelPower?
   };
   static_assert(sizeof(BGSLightingTemplate) == 0x94, "RE::BGSLightingTemplate is the wrong size.");
};