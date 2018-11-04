#pragma once
#include "skse/GameTypes.h"

namespace RE {
   /*template<class T> struct tList {
      //
      // The SKSE team did an iterator-LIKE implementation for tLists. 
      // This class definition straightforwardly matches Bethesda's 
      // internal implementation; members aren't made private, so you 
      // can replicate code and processes exactly as you see them in 
      // the executable.
      //
      operator ::tList<T>*() const { return (::tList<T>*) this; } // allow use with SKSE-defined methods
      struct Node {
         T*    data;
         Node* next;
         Node* prev;
      };
      Node items;
   };*/
   class SimpleLockReversed {
      private:
         UInt32 lockCount = 0;
         UInt32 threadID;
      public:
         MEMBER_FN_PREFIX(SimpleLockReversed);
         DEFINE_MEMBER_FN(lock,   void, 0x00A4AEE0);
         DEFINE_MEMBER_FN(unlock, void, 0x00A4B0F0);
   };
   class simple_lock_rev_guard { // similar to std::lock_guard but for SimpleLock
      private:
         SimpleLockReversed* lock = nullptr;
      public:
         inline simple_lock_rev_guard(SimpleLockReversed* l) : lock(l) { // must not be nullptr
            CALL_MEMBER_FN(this->lock, lock)();
         };
         inline simple_lock_rev_guard(SimpleLockReversed& l) : lock(&l) {
            CALL_MEMBER_FN(this->lock, lock)();
         };
         inline ~simple_lock_rev_guard() {
            CALL_MEMBER_FN(this->lock, unlock)();
         };
   };
   class simple_lock_guard { // similar to std::lock_guard but for SimpleLock
      private:
         SimpleLock* lock = nullptr;
      public:
         inline simple_lock_guard(SimpleLock* l) : lock(l) { // must not be nullptr
            l->Lock();
         };
         inline simple_lock_guard(SimpleLock& l) : lock(&l) {
            l.Lock();
         };
         inline ~simple_lock_guard() {
            this->lock->Release();
         };
   };
};