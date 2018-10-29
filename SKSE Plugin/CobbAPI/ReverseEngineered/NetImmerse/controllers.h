#pragma once

#include "Shared.h"
#include "ReverseEngineered/NetImmerse/objects.h"

namespace RE {
   class NiTimeController : public NiObject {
      public:
         NiTimeController();
         ~NiTimeController();
         //
         virtual void Start(float time);
         virtual void Stop(void);
         virtual void Update(float time) = 0;
         virtual void SetTarget(NiObjectNET * node);
         virtual bool Unk_17(void);
         virtual bool Unk_18(void);
         virtual void Unk_19(void);
         virtual bool Unk_1A(void);
         virtual bool Unk_1B(void);
         virtual bool Unk_1C(void) = 0;
         //
         enum {
            kAnimType_AppTime = 0 << 0,
            kAnimType_AppInit = 1 << 0,
            //
            kCycleType_Loop = 0 << 1,
            kCycleType_Reverse = 1 << 1,
            kCycleType_Clamp = 2 << 1,
            //
            kActive = 1 << 3,
            //
            kPlayBackwards = 1 << 4,
         };
         struct Unk34Item : public NiObject { // NiControllerSequence?
            char*  unk08; // "Open" or "Close"?
            UInt32 pad0C[(0x1C - 0x0C) / 4];
            float  unk1C;
            UInt32 pad20[(0x40 - 0x20) / 4];
            UInt32 unk40;
            UInt32 unk44;
            UInt32 unk48; // bitmask. flags?
            UInt32 pad4C[(0x58 - 0x4C) / 4];
            UInt32 unk58;
            //
            DEFINE_MEMBER_FN_LONG(Unk34Item, Subroutine00A72930, void, 0x00A72930, UInt32, UInt32, float, float, void*, UInt8);
         };
         //
         UInt16       flags;        // 008
         UInt8        pad00A[2];    // 00A
         float        m_fFrequency; // 00C
         float        m_fPhase;     // 010
         float        m_fLoKeyTime; // 014
         float        m_fHiKeyTime; // 018
         float        m_fStartTime; // 01C
         float        m_fLastTime;  // 020
         float        unk024;       // 024 - updated in Unk_19
         float        unk028;       // 028
         UInt8        unk02C;       // 02C
         UInt8        pad02D[3];    // 02D
         NiObjectNET* next;         // 030 - singly linked list of controllers
         NiTArray<Unk34Item*> unk34;
   };
   class NiControllerManager : public NiTimeController {
      public:
         tArray<void*> unk44; // Not sure if tArray is the right type, but we know that unk44 is the pointer to an array of void*s, and unk4C is something like a count.
         //
         MEMBER_FN_PREFIX(NiControllerManager);
         DEFINE_MEMBER_FN(Subroutine0x004D6800, void, 0x004D6800, bool); // <-- Calls {this->unk44[i]->Unk_21(0, arg);} for every i
   };
};