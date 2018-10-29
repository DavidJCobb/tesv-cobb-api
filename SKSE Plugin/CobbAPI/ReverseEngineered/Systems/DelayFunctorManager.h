#pragma once
#include "Shared.h"
#include "ReverseEngineered/Systems/ThreadLocalStorage.h"

#include "skse/NiTypes.h"

namespace RE {
   static UInt8* g_dfmOperationsAreBlocked = (UInt8*)0x0127441C;

   struct DelayFunctor {
      //
      // Superclass for functors used by certain Papyrus script APIs.
      //
      // VTBL: 0x010EA1DC
      // RTTI: 0x012B64A4
      //
      UInt32 refcount; // counter of some kind, modified with interlocked (de|in)crements.
      UInt32 unk08; // papyrus stack ID?
      //
      virtual void* Dispose(bool);        // virtual method 0x00 // Explanation: https://msdn.microsoft.com/en-us/library/fs2xkftw.aspx
      virtual void* Execute(void*);       // virtual method 0x01 // arg is VMValue?
      virtual bool ShouldResumeStack();   // virtual method 0x02
      virtual bool ShouldReschedule();    // virtual method 0x03 // takes precedence over ShouldResumeStack
      virtual bool Unk_04(void*);         // Almost certainly a function to save the functor.
      virtual UInt8 Unk_05();             // Returning a value greater than 0x19 will alter the behavior of Unk_04.
      virtual bool Unk_06(void*, void*);  // Almost certainly a function to load the functor. // ASM at 0x008AF5B0. // Second arg not used by this class?
      //
      // Unk_04 and Unk_06 mention this->unk08, using it similarly. Unk_04 calls Unk_05 and receives 
      // a numeric value from it.
      //
   };

   struct MoveToFunctor : DelayFunctor {
      //
      // Used by MoveTo (and presumably others) to queue up a move call.
      // 
      // VTBL: 0x010F5FB4
      // RTTI: 0x012BF120
      // 
      UInt32 refHandleTarget;  // 0C
      UInt32 refHandleSubject; // 10
      NiPoint3 pos; // 14
      NiPoint3 rot; // 20
      bool   matchRotation; // 2C // whether to offset from target reference coords by our coords
      bool   unk2D;         // 2D // whether to use our coords or those of the target reference
      UInt8  pad2E; // unused
      UInt8  pad2F; // unused
      void*  unk30; // unkPapyrus1
      //
      virtual void* Dispose(bool);  // Normal VTBL: 0x009067F0 (resolves the call?).
      virtual void* Execute(void*); // Normal VTBL: 0x0090CC60 (executes the call and returns argument it received).
      // ...
      virtual UInt8 Unk_05(); // Returns 0x0 immediately. Located at 0x005EADD0, which is also used as a no-op in various places.
      virtual bool Unk_06(void*, void*); // Arg types not known. Calls DelayFunctor::Unk_06 on self, with same arguments. Does other stuff; not sure what.
      //
      MEMBER_FN_PREFIX(MoveToFunctor);
      DEFINE_MEMBER_FN(Constructor, MoveToFunctor*, 0x00904590, UInt32* subjectRefhandle, NiPoint3* pos, NiPoint3* rot, bool matchRotation, bool unk, void* unkPapyrus1, void* unkPapyrus2);
      DEFINE_MEMBER_FN(Subroutine008AF3A0, void, 0x008AF3A0); // Sets VTBL to that of DelayFunctor. Called by Unk_00.
   };
   struct SetMotionTypeFunctor : DelayFunctor {
      //
      // VTBL: 0x010F629C
      // RTTI: 0x0118E204
      //
      UInt32 refHandleSubject; // 0C
      UInt32 newMotionType;    // 10
      void*  unk14; // unkPapyrus1
      UInt8  unk18; // Wait... I forgot about SetMotionType's optional bool argument. Is this it?
      //
      virtual void* Dispose(void); // Behaves very similarly to MoveToFunctor::Unk_00, right down to swapping VTBL out for DelayFunctor's VTBL.
      virtual void* Execute(void*); // ASM at 0x00909A10. // Arguments and return value not checked thoroughly. Useful; analyze.
      //
      virtual UInt8 Unk_05(); // Returns 0x8 immediately.
      //
      MEMBER_FN_PREFIX(SetMotionTypeFunctor);
      DEFINE_MEMBER_FN(Constructor, SetMotionTypeFunctor*, 0x00904E30, UInt32* subjectRefHandle, UInt32 motionType, bool unk, void* unkPapyrus1, void* unkPapyrus2);
      DEFINE_MEMBER_FN(Subroutine008AF3A0, void, 0x008AF3A0); // Sets VTBL to that of DelayFunctor. Called by Unk_00.
   };
   struct SetPositionFunctor : DelayFunctor {
      //
      // VTBL: 0x010F6094
      // RTTI: 0x012BF2C0
      //
   };

   struct Unknown012E568C {
      //
      // Does not have a VTBL or RTTI.
      //
      struct Unk457C { // NiFloatData?
         //
         // VTBL: 0x0108732C
         // RTTI: 0x01243F14 (NiFloatData)
         //
         UInt32 pad04;
         UInt32 pad08;
         UInt32 unk0C; // Referenced in Subroutine00A77A40.
         UInt32 unk10; // Referenced in Subroutine00A77A40. // Known not to be a pointer.
         // ...?
         //
         // Virtual functions can vary; this object replaces its own vtbl for unknown reasons.
         //
         virtual void* Unk_01(DelayFunctor*); // At 0x004B6D40. // Calls this->Subroutine00A77A40(); conditionally calls *(0x01B418B0)->TESV_00A487B0(this, 0); and then returns this;
         //
         DEFINE_MEMBER_FN_LONG(Unk457C, Subroutine00A77A40, void, 0x00A77A40); // Replaces vtbl, runs a switch-case based on this->unk10 (which acts on unk0C), and then interlocked-decrements variable at 0x01B91294. Replaces vtbl again afterward.
      };
      UInt32     pad0000[0x115C];
      SimpleLock unk4570;
      Unk457C    unk457C;
      // ...?
      //
      MEMBER_FN_PREFIX(Unknown012E568C);
      DEFINE_MEMBER_FN(Subroutine008C7810, bool, 0x008C7810, void* out); // Accepts a newly-created MoveToFunctor* argument; presumably, queues it.
   };

   struct Unknown01B418A0 {
      UInt32 unk00; // flags or bitmask?
   };
   struct Unknown01B418B0 {
      struct A {
         //
         // NOTE: NOT ALL OF THESE VIRTUAL METHODS MAY BELONG TO THIS CLASS. IT SEEMS THAT 
         // SOME VTBLS ARE STORED CONTIGUOUSLY.
         //
         virtual void   Unk_01(void);           // ASM at 0x00A4DCB0.
         virtual UInt32 Unk_02(UInt32);         // ASM at 0x00A4DAB0.
         virtual void   Unk_03(void);           // ASM at 0x00A4DA70.
         virtual void   Unk_04(void);           // ASM at 0x00A4EF10.
         virtual void   Unk_05(UInt32, UInt32); // ASM at 0x00A4EEE0. // Sets Arg2 to zero and calls this->Unk_09(Arg1, Arg2), returning its result (if any).
         virtual void   Unk_06(void);           // ASM at 0x00A4EEF0. // Calls this->Unk_0A(*Arg1, 0).
         virtual void*  Unk_07(UInt32, UInt32); // ASM at 0x00A4EE90. // No-op. Returns NULL.
         virtual UInt32 Unk_08();               // ASM at 0x00A4D9B0. // Returns this->unk0C;
         virtual void*  Unk_09(UInt32);         // ASM at 0x00A4DBC0.
         virtual void   Unk_0A(UInt32, UInt32); // ASM at 0x00A4D9C0. // No-op.
         virtual bool   Unk_0B(UInt32);         // ASM at 0x00A4D9D0. // Returns (this->unk10 < Arg1 < this->unk08 + this->unk10).
         virtual UInt32 Unk_0C(UInt32);         // ASM at 0x00A4DAC0. // Returns this->Unk_02(Arg1) + 4;
         virtual void   Unk_0D(void*, UInt32);  // ASM at 0x00A4DA00. Modifies first argument. Second argument apparently unused.
         virtual bool   Unk_0E();               // ASM at 0x00A4DAF0. // No-op. Returns false.
         virtual void*  Unk_0F();               // ASM at 0x00A4DAE0. // No-op. Returns NULL.
         //
         // 00 // vtbl at 0x0110D6F4; no RTTI
         Unknown01B418B0* unk04; // Type assumed. Points back to the parent?
         UInt32 unk08; // size of some kind?
         UInt32 unk0C; // Type should be same as return type of Unk_08().
         UInt32 unk10; // bound of some kind?
         UInt32 unk14; // related to unk10 somehow?
         UInt32 pad18;
         Unknown01B38334* unk1C; // Referenced in Unk_09(...).
         //
         DEFINE_MEMBER_FN_LONG(A, Subroutine00C328E0, void, 0x00C328E0);
      };
      struct B { // this might be a subclass of A*
         virtual void*  Unk_01(void);           // ASM at 0x00A4E180. // Constructor?
         virtual UInt32 Unk_02(void*);          // ASM at 0x00A4DFA0.
         virtual void   Unk_03(void*);          // ASM at 0x00A4EFB0.
         virtual bool   Unk_04(UInt32);         // ASM at 0x00A4EF10. // Returns this->Unk_0B(Arg1);
         virtual void   Unk_05(UInt32, UInt32); // ASM at 0x00A4FC70. // Returns this->TESV_00A4F490(Arg1, Arg2, 1);
         virtual void   Unk_06(void);           // ASM at 0x00A4EEF0. // Calls this->Unk_0A(*Arg1, 0).
         virtual void   Unk_07(UInt32, UInt32); // ASM at 0x00A4FC90. // Returns this->TESV_00A4F490(Arg1, Arg2, 0);
         virtual UInt32 Unk_08();               // ASM at 0x00A4EF30. // Returns this->unk10.
         virtual void   Unk_09(UInt32, UInt32); // ASM at 0x00A4DD60. // Returns this->TESV_00A4F490(Arg1, Arg2, 1);
         virtual void   Unk_0A(void);           // ASM at 0x00A4DD80.
         virtual bool   Unk_0B(UInt32);         // ASM at 0x00A4E150.
         virtual UInt32 Unk_0C(void*);          // ASM at 0x00A4E020.
         virtual void   Unk_0D(void);           // ASM at 0x00A4DDC0.
         virtual bool   Unk_0E(UInt32, UInt32); // ASM at 0x00A4EFF0. // No-op with two arguments. Returns true.
         virtual void   Unk_0F(void);
         //
         // 00 // vtbl at 0x0110D730; no RTTI
         UInt32 pad04;
         UInt32 pad08;
         UInt32 pad0C;
         UInt32 pad10;
         UInt32 pad14;
         UInt32 unk18;
         UInt32 pad1C;
         UInt32 unk20;
         UInt32 pad24;
         UInt32 unk28;
         UInt32 pad2C;
         UInt32 unk30;
         UInt32 pad34;
         UInt32 unk38;
         //
         DEFINE_MEMBER_FN_LONG(B, Subroutine00A4F490, void, 0x00A4F490, UInt32, UInt32, bool);
         DEFINE_MEMBER_FN_LONG(B, Subroutine00A4EAC0, void, 0x00A4EAC0, UInt32, UInt32, bool, bool);
      };
      struct C {
         virtual void   Unk_01(void); // ASM at 0x00A4EE20.
         virtual void   Unk_02(void); // ASM at 0x00A4E5E0.
         virtual void   Unk_03(void); // ASM at 0x00A4E9E0.
         virtual void   Unk_04(void); // ASM at 0x00A4E610.
         virtual void   Unk_05(void); // ASM at 0x00A4EDC0.
         virtual void   Unk_06(void); // ASM at 0x00A4EC90.
         virtual void   Unk_07(void); // ASM at 0x00A4EDE0.
         //
         // 00 // vtbl at 0x0110D784; no RTTI
      };
      UInt32 pad000; // looks like two words, actually
      UInt32 pad004;
      UInt32 unk008; // Number of active As, starting at unk014? Looking at it in memory indicates a **unknown where unknown has a vtbl.
      UInt32 pad00C;
      A*     unk010; // ExtendedTLSData->unk4AC is used as an index for data starting here. This member can be NULL.
      A*     unk014;
      A*     unk018; // Default used if the thing selected by ExtendedTLSData->unk4AC is null. // Could be a B* instead.
      UInt32 pad01C[0x7E];
      C*     unk214;
      UInt32 pad218[0x8];
      UInt8  unk220;
      UInt8  unk221;
      UInt8  unk222;
      UInt8  unk223;
      UInt32 unk224;
      UInt32 unk228;
      UInt32 unk22C;
      UInt32 unk230;
      A*     unk234; // Assumed to be an A*, but this is, at the very least, a void*.
      //
      MEMBER_FN_PREFIX(Unknown01B418B0);
      DEFINE_MEMBER_FN(Subroutine00A48D60, void*, 0x00A48D60, UInt32, void*, void*); // Used to allocate memory, to create queued-call objects.
      DEFINE_MEMBER_FN(Subroutine00A48C80, void*, 0x00A48C80, A*, void*, void*, void*); // Called by above. Edits TLS. Returns TLS->unk4A4. // Maybe only takes 3 args?
      DEFINE_MEMBER_FN(Subroutine00687B10, void*, 0x00687B10, A*, UInt32, void*, void*); // Called by above. Caller funnels args through except for arg2, which can vary.
      DEFINE_MEMBER_FN(Subroutine00A487B0, void,  0x00A487B0, void*, UInt32);
      DEFINE_MEMBER_FN(Subroutine00C3A0A0, void,  0x00C3A0A0); // Apparently calls x->Subroutine00C328E0 on every other A* starting at unk014.
   };

   struct Unknown01B38308 {
      //
      // Unknown object. Does not have RTTI or a VTBL.
      //
      // Apparently allows certain functions to defer/reschedule calls made to them based on some unknown 
      // condition. This may be the native delay functor manager or something very similar in purpose.
      //
      public:
      struct BSTCommonScrapHeapMessageQueue {
         struct ScheduledCall { // scheduled call?
            struct Data {
               UInt32 unk00[0x10];
            };
            ScheduledCall* nextCall; // 00 // pointer to next call, or null
            Data           data;     // 04 // call data?
         };
         //
         virtual void Unk01(void);
         virtual bool Unk02(ScheduledCall::Data*); // ASM at 0x008C71A0.
         virtual void Unk03(void);
         virtual void Unk04(void);
         virtual void Unk05(void);
         virtual bool Unk06(ScheduledCall::Data*); // ASM at 0x00693170. // Unk02 calls appears to wrap this with a lock.
         virtual void Unk07(void);
         virtual void Unk08(void);
         virtual void Unk09(void);
         virtual void Unk0A(void);
         virtual void Unk0B(void);
         //
         struct A {
            DEFINE_MEMBER_FN_LONG(A, Subroutine00A49530, ScheduledCall*, 0x00A49530, UInt32, void*); // creates a ScheduledCall?
            //
            UInt32 unk00;
            UInt32 unk04;
            UInt32 unk08; // bitmask?
            UInt32 unk0C;
            UInt32 unk10;
            void*  unk14;
         };
         struct B {
            UInt32 unk00;
         };
         //
         // 00 vtbl at 0x010CDF80
         UInt32 unk04; // 04 // a lock?
         A*     unk08; // 08 // some sort of object what makes scheduled calls
         UInt32 unk0C; // 0C // number of scheduled calls in (this->scheduledCallLinkedList), maybe?
         ScheduledCall* scheduledCallLinkedList; // 10 // linked list
      };
      //
      UInt32     unk00;
      UInt32     unk04;
      BSTCommonScrapHeapMessageQueue* unk08;

      /*inline static Unknown0* GetKnownInstance() { // Should be correct, but the stupid compiler compiles it as {xor eax, 01B38308} which is obviously WRONG
      return *((Unknown0**)0x01B38308);
      }*/
   };
   __declspec(naked) Unknown01B38308* GetUnknown01B38308Instance() { // asm getter, because the compiler craps out wrong asm when I try to do it literally any other way
      __asm {
         mov eax, 0x01B38308;
         mov eax, [eax];
         ret;
      }
   }

   struct Unknown01B38334 {
      //
      // TODO: This is what we call a BethesdaThreadedLock, and what SKSE calls a SimpleLock.
      //
      public:
         DEFINE_MEMBER_FN_LONG(Unknown01B38334, Subroutine00401710, void, 0x00401710, UInt32);
         //
         // ^ If the current thread ID matches our thread ID, increment our counter and 
         //   return; else sleep until a condition is met, and set our counter to 1.
         //
         unsigned long unk00; // a thread ID?
         UInt32 unk04;        // a counter?
         //
         inline static Unknown01B38334* GetInstance() { // untested; compiler could spit out wrong ASM
            return (Unknown01B38334*)0x01B38334;
         };
   };
}