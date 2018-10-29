#pragma once
#include "skse/GameForms.h"
#include "Shared.h"

namespace RE {
   class DefaultObjectList {
      public:
         enum {
            kNumDefaultObjects = 0x15A
         };
         struct DefaultObject {
            const char* description; // 00 // human-readable name
            UInt32		formType;    // 04
            UInt32		signature;   // 08 // four-character string
            UInt32		unk0C;       // 0C
         };
         //
         static DefaultObjectList* GetSingleton(void);
         //
         DefaultObject objects[kNumDefaultObjects];
         //
         SInt32 GetIndexOfSignature(UInt32 signature);
   };
   class BGSDefaultObjectManager : public TESForm {
      //
      // Constructor: 0x004F8FB0
      //
      public:
         enum { kTypeID = kFormType_DOBJ };

         // cleared to 0 in ctor
         TESForm* objects[0x15A]; // 14
         UInt8    pad[0x15A];     // just pad out the rest of the space

         static BGSDefaultObjectManager* GetSingleton(void);
   };

   static DEFINE_SUBROUTINE(TESForm*, GetDOBJByIndex, 0x00401EA0, UInt32 index);
};