#pragma once
#include "skse/GameTypes.h"
#include "skse/Utilities.h"

class TESObjectREFR;
namespace RE {
   class ActionInput { // sizeof >= 0x30
      //
      // Constructor: 0x00457F80
      //
      public:
         ~ActionInput() {
            CALL_MEMBER_FN(this, Destructor)();
         };
         //
         virtual void Unk_00(void) {}; // dummy
         //
         TESObjectREFR* unk04; // 04 // assumed
         TESObjectREFR* unk08; // 08 // assumed
         UInt32 unk0C; // 0C
         UInt32 unk10; // 10
         //
         MEMBER_FN_PREFIX(ActionInput);
         DEFINE_MEMBER_FN(Destructor, void, 0x00457FD0);
   };
   class TESActionData : public ActionInput {
      public:
         TESActionData() {};
         TESActionData(UInt32 a, TESObjectREFR* b, void* c, UInt32 d) {
            CALL_MEMBER_FN(this, Constructor)(a, b, c, d);
         };
         ~TESActionData() {
            CALL_MEMBER_FN(&this->unk18, Release)();
            CALL_MEMBER_FN(&this->unk14, Release)();
         };
         //
         StringCache::Ref unk14; // 14
         StringCache::Ref unk18; // 18
         UInt32 unk1C[(0x38 - 0x1C) / 4]; // 1C // might be too much space, but better safe than sorry
         //
         MEMBER_FN_PREFIX(TESActionData);
         DEFINE_MEMBER_FN(Constructor, TESActionData*, 0x00474F80, UInt32, TESObjectREFR*, void*, UInt32);
         DEFINE_MEMBER_FN(Subroutine0075FA40, void, 0x0075FA40);
   };
};