#pragma once
#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
   class TESObjectLAND : public TESForm {
      public:
         operator ::TESObjectLAND*() const { return (::TESObjectLAND*) this; }
         enum { kTypeID = kFormType_Land };
         //
         // Parents:
         TESChildCell childCell; // 14
         //
         // Members:
         UInt32         unk18; // 18 // low byte, at the very least, is flags or a bool
         TESObjectCELL* unk1C; // 1C
         UInt32         unk20; // 20
         void*          unk24[4]; // 24 // quadrants // each is over 0x9000 bytes large. i'm not joking
         //
         MEMBER_FN_PREFIX(TESObjectLAND);
         DEFINE_MEMBER_FN(Subroutine004BAFB0, void, 0x004BAFB0, void* argOutTwoFloats);
         DEFINE_MEMBER_FN(Subroutine004BAF40, void, 0x004BAF40, void* out);
         DEFINE_MEMBER_FN(Subroutine004BBA70, void*, 0x004BBA70, UInt32 quadrantNumber);
   };
};