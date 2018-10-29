#pragma once
#include "Shared.h"

namespace RE {
   struct RTTI {
      UInt32 unk04;
      UInt32 unk08;
      char* className;
      // ...?
      virtual void Unk_00(void);
      virtual void Unk_01(void);
      virtual void Unk_02(void);
      virtual void Unk_03(void);
   };

   struct SimpleLock {
      UInt32 owningThreadID;     // thread that has ownership of the lock
      UInt32 numberOfLocksHeld;  // number of times the owning thread has locked this mechanism
      //
      // This member function checks if the lock is owned by the calling thread. If so, it immediately increments the lock count. 
      // Otherwise, it sleeps until the lock count hits zero, assigns ownership to the calling thread, and increments the count.
      //
      DEFINE_MEMBER_FN_LONG(SimpleLock, IncrementCounterWhenSafe, void, 0x00401710, char* unusedDebugOutputIdentifyingMyCaller);
      //
      void DecrementCounter() {
         //
         // Functions that increment the counter will always decrement it when they're done. 
         // This is pretty much how they do it.
         //
         // (Bethesda actually doesn't run the thread ID check: if they just incremented the 
         // counter, then they must have ownership of the lock. I've included the check just 
         // because when it comes to these things, I like a "safety first" approach.)
         //
         if (GetCurrentThreadId() == this->owningThreadID) {
            this->numberOfLocksHeld--;
            if (!this->numberOfLocksHeld) {
               InterlockedCompareExchange(&this->owningThreadID, 0, this->owningThreadID);
            }
         }
      };
   };

   DEFINE_SUBROUTINE_EXTERN(bool, IsCellOrWorldspace, 0x00472D10, void* thingToTest, ::TESObjectCELL** outCell, ::TESWorldSpace** outWorld);
   DEFINE_SUBROUTINE_EXTERN(void, FillBufferWithByte, 0x00F52240, void* buffer, UInt8 byte, UInt32 length);
};