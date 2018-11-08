#pragma once
#include "skse/GameTypes.h"
#include "skse/Utilities.h"

namespace RE {
   class Actor;
   class BGSSaveFormBuffer {
      public:
         BGSSaveFormBuffer();
         virtual ~BGSSaveFormBuffer(); // 00 // virtual void Dispose(bool)
         virtual void*    Unk_01(); // returns this->unk1C
         virtual TESForm* Unk_02();
         virtual Actor*   Unk_03(); // returns the result of Unk_02 if it's an actor, or nullptr otherwise

         struct BGSSaveLoadFormInfo { // sizeof == 9
            //
            // This exists in BGSSaveFormBuffer, but it's also created near 00687340, a tArray<?> method. 
            // We only know its name because of a debug log statement.
            //
            UInt8  unk00 = 0;
            UInt8  unk01 = 0;
            UInt8  unk02 = 0;
            __declspec(align(1)) UInt32 changeflags = 0; // 03 // not aligned as is usual
            UInt8  formType = 0; // 07
            UInt8  unk08 = 0;
         };

         // void** _vtbl; // 00
         UInt32 unk04;
         UInt32 unk08;
         UInt32 unk0C;
         BGSSaveLoadFormInfo info; // 10
         void*  unk1C = 0;

         MEMBER_FN_PREFIX(BGSSaveFormBuffer);
         DEFINE_MEMBER_FN(Destructor, void, 0x00686C00);
         DEFINE_MEMBER_FN(Subroutine006869C0, void, 0x006869C0, UInt32 formID, UInt32, UInt32 formType, UInt32);
         DEFINE_MEMBER_FN(Subroutine00686AB0, void, 0x00686AB0, void*);
         DEFINE_MEMBER_FN(Subroutine00686C70, void, 0x00686C70, UInt32);
         DEFINE_MEMBER_FN(WriteData, void, 0x00686E30, void* source, UInt32 size, UInt32 size_redundant_unused, UInt32 unk_unused_usuallyZero);
   };
   //
   static_assert(sizeof(BGSSaveFormBuffer::BGSSaveLoadFormInfo) >= 0x9, "BGSSaveFormBuffer::BGSSaveLoadFormInfo is too small.");
   static_assert(sizeof(BGSSaveFormBuffer::BGSSaveLoadFormInfo) <= 0x9, "BGSSaveFormBuffer::BGSSaveLoadFormInfo is too large, or MSVC is trying to align its members.");
   static_assert(offsetof(BGSSaveFormBuffer::BGSSaveLoadFormInfo, unk03) == 0x3, "BGSSaveFormBuffer::BGSSaveLoadFormInfo::unk03 is at the wrong offset, or MSVC is trying to align it.");
};