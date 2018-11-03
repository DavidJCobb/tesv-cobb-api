#pragma once
#include "skse/GameTypes.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
   class BGSLoadFormBuffer;
   //
   // As of this writing (November 2nd, 2018), everything we've decoded for 
   // navmeshes is based on examining how navmeshes load their data from the 
   // ESP/ESM file, and comparing what we see to known information about the 
   // data structures in the xEdit source code. This is not the most straight-
   // forward way to investigate the class. We need to figure out how to get 
   // a NavMesh pointer, given a TESObjectCELL pointer, and then write code 
   // to log all of a navmesh's fields; then, we can examine the output to 
   // see if what we've discovered is correct. (Perhaps I could write a quick 
   // bit of JavaScript to take the logged data and draw a view from above, 
   // so I can quickly and literally see whether the data matches up.)
   //
   struct BSPathingStreamRead {
      virtual bool Unk_00(UInt32) = 0;
      virtual bool Unk_01(void* readTo) = 0;
      virtual bool Unk_02(UInt32) = 0;
      virtual bool ReadDword(UInt32* readTo) = 0; // 03
      virtual bool Unk_04(UInt32) = 0;
      virtual bool ReadWord(UInt16* readTo) = 0; // 05
      virtual bool Unk_06(UInt32) = 0;
      virtual bool Unk_07(UInt32) = 0;
      virtual bool Unk_08(UInt32) = 0;
      virtual bool Unk_09(UInt32) = 0;
      virtual bool Unk_0A(UInt32, UInt32) = 0;
      virtual bool ReadFormID(UInt32* readTo) = 0;
      virtual bool Unk_0C(UInt32); // returns false
      virtual bool Unk_0D(UInt32); // returns false
      virtual bool Unk_0E(UInt32); // returns false
      virtual void Dispose(bool); // 0F
      virtual UInt32 Unk_10() = 0;
      virtual UInt32 Unk_11() = 0;
      virtual UInt32 Unk_12() = 0;
   };
   struct PathingStreamMasterFileRead : public BSPathingStreamRead {
      virtual bool Unk_00(UInt32);
      virtual bool Unk_01(void* readTo); // read a dword?
      virtual bool Unk_02(UInt32);
      virtual bool ReadDword(UInt32* readTo); // 03
      virtual bool Unk_04(UInt32);
      virtual bool ReadWord(UInt16* readTo); // 05 // read a word?
      virtual bool Unk_06(UInt32);
      virtual bool Unk_07(UInt32);
      virtual bool Unk_08(UInt32);
      virtual bool Unk_09(UInt32);
      virtual bool Unk_0A(UInt32, UInt32);
      virtual bool ReadFormID(UInt32* readTo);
      virtual bool Unk_0C(UInt32);
      virtual bool Unk_0D(UInt32);
      virtual bool Unk_0E(UInt32);
      virtual void Dispose(bool); // 0F
      virtual UInt32 Unk_10();
      virtual UInt32 Unk_11();
      virtual UInt32 Unk_12(); // returns 0
      
      void*  unk04;
      UInt32 offset; // 08 // how far into the stream are we?
      BGSLoadFormBuffer* buffer; // 0C
   };

   enum NavmeshTriFlags : UInt16 {
      kNavmeshTriFlag_EdgeLink_0_1 = 0,
      kNavmeshTriFlag_EdgeLink_1_2,
      kNavmeshTriFlag_EdgeLink_2_0,
      kNavmeshTriFlag_Unknown4,
      kNavmeshTriFlag_Unknown5,
      kNavmeshTriFlag_Unknown6,
      kNavmeshTriFlag_Preferred,
      kNavmeshTriFlag_Unknown8,
      kNavmeshTriFlag_Unknown9,
      kNavmeshTriFlag_Water,
      kNavmeshTriFlag_Door,
      kNavmeshTriFlag_Found,
      kNavmeshTriFlag_Unknown13,
      kNavmeshTriFlag_Unknown14,
      kNavmeshTriFlag_Unknown15,
      kNavmeshTriFlag_Unknown16,
   };

   class NavMesh : public TESForm { // sizeof == 0xB4
      public:
         TESChildCell childCell; // 14

         struct Struct00B4FD10 { // sizeof == 0x9C
            virtual void   Dispose(bool); // 00
            virtual UInt32 Unk_01(); // 01 // returns navmesh form ID

            struct Vertex : public NiPoint3 { // sizeof == 0xC
               MEMBER_FN_PREFIX(Vertex);
               DEFINE_MEMBER_FN(Subroutine00B95400, void, 0x00B95400, BSPathingStreamRead*);
            };
            struct Triangle { // sizeof == 0x10
               SInt16 vertices[3]; // 00
               SInt16 edges[3];    // 06
               NavmeshTriFlags flags; // 0C
               UInt16 coverFlags;  // 0E
               //
               MEMBER_FN_PREFIX(Triangle);
               DEFINE_MEMBER_FN(Subroutine00B60F40, void, 0x00B60F40, BSPathingStreamRead*);
            };
            struct EdgeLink { // sizeof == 0xC
               struct Target { // sizeof == 0x8
                  UInt32 otherNavmesh;  // 00 // formID, at least when initially read
                  SInt16 otherTriangle; // 04
                  UInt16 pad06;
                  //
                  MEMBER_FN_PREFIX(Target);
                  DEFINE_MEMBER_FN(Read, void, 0x00B56A40, BSPathingStreamRead*);
               };
               UInt32 unk00;  // 00
               Target target; // 04
               //
               MEMBER_FN_PREFIX(EdgeLink);
               DEFINE_MEMBER_FN(Read, void, 0x00B95390, BSPathingStreamRead*);
            };
            struct DoorTriangle {
               UInt32 unk00; // 00 // The door? Or the formID of the door?
               UInt16 triangleBeforeDoor; // 04
               UInt16 pad06;
               //
               MEMBER_FN_PREFIX(DoorTriangle);
               DEFINE_MEMBER_FN(Subroutine00B56B00, void, 0x00B56B00, BSPathingStreamRead*);
            };
            struct Struct00B52000 { // sizeof == 0x30
               NiPoint3 unk00; // or possibly NiQuaternion
               float    unk0C;
               void*    unk10 = nullptr;
               void*    unk14 = nullptr;
               UInt16   unk18 = 0; // a count of something
               UInt16   unk1A = 0;
               float    unk1C = 0;
               UInt32   unk20 = 0;
               UInt32   unk24 = 0;
               UInt32   unk28 = 0;
               UInt16   unk2C = 0;
               UInt8    unk2E = 0;
               UInt8    pad2F;
               //
               MEMBER_FN_PREFIX(Struct00B52000);
               DEFINE_MEMBER_FN(Subroutine005FA160, void, 0x005FA160, Struct00B4FD10* owner);
               DEFINE_MEMBER_FN(Subroutine005F9B90, void, 0x005F9B90, Struct00B4FD10* owner);
            };
            struct Struct00B52D70 { // sizeof == 0x28 // NVNM subrecord
               UInt32 unk00 = 0;
               UInt32 unk04;
               UInt32 unk08;
               float  unk0C;
               UInt32 unk10[0x14];
               UInt32 unk24 = 0;
            };
            struct Unk84 {
               UInt32 unk00;
               UInt32 unk04;
               UInt32 unk08 = 0;
               UInt32 unk0C = 0;
               UInt32 unk10 = 0;
               void*  unk14; // initial pointer points to 0xDEADBEEF
               UInt32 unk18;
               UInt32 unk1C = 0;
            };

            UInt32 unk04;
            tArray<Vertex>   vertices;  // 08
            tArray<Triangle> triangles; // 14
            tArray<EdgeLink> edgeLinks; // 20 // sizeof(item) == 0xC
            tArray<DoorTriangle> doorTriangles; // 2C // sizeof(item) == 0x8
            tArray<UInt32> unk38; // 38
            tArray<SInt16> coverTriangles; // 44 // or perhaps this is tArray<SInt16> navmeshGrid instead?
            Struct00B52D70 unk50; // 50
            tArray<UInt32> unk78; // 78
            Unk84* unk84;
            tArray<UInt32> unk88; // 88
            UInt32 unk94 = 0;
            Struct00B52000* unk98 = nullptr;

            MEMBER_FN_PREFIX(Struct00B4FD10);
            DEFINE_MEMBER_FN(Subroutine00B50080, void, 0x00B50080, PathingStreamMasterFileRead*);
         };

         Struct00B4FD10 unk18; // 18 // possibly the NVNM subrecord, but in that case, where do ONAM, PNAM, and NNAM end up?

   };
   typedef NavMesh Navmesh;
};
