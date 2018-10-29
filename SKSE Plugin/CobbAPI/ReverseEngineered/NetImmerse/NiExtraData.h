#pragma once

#include "skse/NiExtraData.h"
#include "skse/NiObjects.h"

namespace RE {
   class BSFurnitureMarkerNode : public NiExtraData {
      public:
         enum MarkerEntry : UInt16 {
            None = 0,
            Front,
            Behind,
            Right,
            Left,
            Up
         };
         enum MarkerType : UInt16 {
            Unknown = 0, // Lean?
            Sit,
            Sleep
         };
         // 14 bytes:
         struct FurnitureMarker {
            NiPoint3   position;
            float      yaw; // rotation in radians.
            MarkerType type;
            UInt16     directions; // MarkerEntry values OR'd together.
         };
         tArray<FurnitureMarker> markers;
         //
         // ...?
         //
   };

   class BSInvMarker : public NiExtraData {
      public:
         float zoom;
         NiPoint3 rotation;
         //
         // ...?
         //
   };
};