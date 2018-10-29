#pragma once
#include "Shared.h"

namespace RE {
   class BGSPrimitive {
      public:
         BGSPrimitive();
         virtual ~BGSPrimitive();
         operator ::BGSPrimitive*() const { return (::BGSPrimitive*) this; }
         //
         enum {
            kPrimitive_None   = 0,
            kPrimitive_Box    = 1,
            kPrimitive_Sphere = 2,
            kPrimitive_Plane  = 3,
            kPrimitive_Line   = 4
         };
         //
         UInt32   type;
         NiPoint3 halfwidths;
   };

   class BGSPrimitiveBox : public BGSPrimitive {
      public:
         BGSPrimitiveBox();
         virtual ~BGSPrimitiveBox();
         operator ::BGSPrimitiveBox*() const { return (::BGSPrimitiveBox*) this; }
         //
         static BGSPrimitiveBox* Create(NiPoint3* halfwidths);
   };

   class BGSPrimitiveLine : public BGSPrimitiveBox {
      public:
         BGSPrimitiveLine();
         virtual ~BGSPrimitiveLine();
   };

   class BGSPrimitivePlane : public BGSPrimitive {
      public:
         BGSPrimitivePlane();
         virtual ~BGSPrimitivePlane();
   };

   class BGSPrimitiveSphere : public BGSPrimitive {
      public:
         BGSPrimitiveSphere();
         virtual ~BGSPrimitiveSphere();
   };
}