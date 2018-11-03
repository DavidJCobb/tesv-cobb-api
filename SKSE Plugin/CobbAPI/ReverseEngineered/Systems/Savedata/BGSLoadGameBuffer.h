#pragma once
#include "skse/GameTypes.h"
#include "skse/Utilities.h"

namespace RE {
   class BGSLoadGameBuffer {
      public:
         BGSLoadGameBuffer();
         virtual ~BGSLoadGameBuffer(); // 00 // virtual void Dispose(bool)
         virtual UInt8 Unk_01(); // 01 // returns a byte from a singleton

         UInt32 unk04 = 0;
         UInt32 unk08 = 0;
         UInt32 unk0C = 0;
         UInt32 unk10 = 0;
         UInt32 unk14 = 0;

         MEMBER_FN_PREFIX(BGSLoadGameBuffer);
         DEFINE_MEMBER_FN(LoadActorHandle, bool, 0x005ACDF0, UInt32* outHandle); // reads a form ID from the savegame, looks the form up by ID, and gets a handle
         DEFINE_MEMBER_FN(ReadData,        bool, 0x00685E20, void* writeTo, UInt32 size, UInt32 size_redundant_unused);
         DEFINE_MEMBER_FN(ReadFormID,      bool, 0x00685C90, UInt32* outFormID);
   };
   class BGSLoadFormBuffer : public BGSLoadGameBuffer {
      public:
         BGSLoadFormBuffer();
         virtual ~BGSLoadFormBuffer(); // 00 // virtual void Dispose(bool)
         virtual UInt8 Unk_01(); // 01 // returns this->unk18.unk1B
         
         struct Struct00685920 { // constructor at 0x00685920
            UInt32 unk00 = 0; // 00 // form ID
            UInt32 unk04 = 0;
            UInt32 unk08 = 0;
            UInt32 unk0C = 0;
            UInt32 unk10 = 0;
            UInt32 unk14 = 0;
            UInt16 unk18 = 0;
            UInt8  unk1A = 0;
            UInt8  unk1B;
         };

         Struct00685920 unk18;
         //
         // ...
         // unk240 == signature of top-level record (e.g. RACE, NONE, TES4)
         // ...
         // unk248 == bitmask
         // ...
         // unk258 == signature of current subrecord (e.g. NAM0, DFTF)
         // unk25C == size of current subrecord?
         //
         // bool unk299 == byte order? form-loading code often checks it, and when it's true, form-loading code calls struct member functions that bswap all values
         //
         // Highest observed field: unk424

         MEMBER_FN_PREFIX(BGSLoadFormBuffer);
         DEFINE_MEMBER_FN(OpenNextSubrecord,        UInt32, 0x00445B70); // returns record signature, or zero
         DEFINE_MEMBER_FN(VerifyTopLevelRecordType, UInt32, 0x00445E30); // returns top-level record type index, e.g. 0xE for RACE or 0x0 for NONE; also synchronizes some static vars / global state
   };
};