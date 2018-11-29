#pragma once
#include "skse/GameAPI.h"
#include "skse/GameTypes.h"

namespace RE {
   class BSString { // sizeof == 0x8
      public:
         ~BSString() { // assumes heap allocation
            if (this->m_data) {
               FormHeap_Free(this->m_data);
               this->m_data = nullptr;
            }
            this->m_dataLen = this->m_bufLen = 0;
         }
         //
         char*  m_data    = nullptr; // 00
         UInt16 m_dataLen = 0; // 04
         UInt16 m_bufLen  = 0; // 06

         MEMBER_FN_PREFIX(BSString);
         DEFINE_MEMBER_FN(assign, bool, 0x00402C00, const char*, UInt32 maxLength); // zero maxLength will copy the entire input string
   };
   template<class T> struct tList {
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
   };
   class SimpleLockReversed {
      private:
         UInt32 lockCount = 0;
         UInt32 threadID;
      public:
         MEMBER_FN_PREFIX(SimpleLockReversed);
         DEFINE_MEMBER_FN(lock,   void, 0x00A4AEE0);
         DEFINE_MEMBER_FN(unlock, void, 0x00A4B0F0);
   };
   class UnknownLock01 {
      //
      // lockCount is handled differently from SimpleLockReversed.
      //
      private:
         UInt32 lockCount = 0;
         UInt32 threadID;
      public:
         MEMBER_FN_PREFIX(UnknownLock01);
         DEFINE_MEMBER_FN(lock,   void, 0x00A4AF50);
         DEFINE_MEMBER_FN(unlock, void, 0x00A4B100);
         //
         class guard {
            private:
               UnknownLock01* lock = nullptr;
            public:
               inline guard(UnknownLock01* l) : lock(l) { // must not be nullptr
                  CALL_MEMBER_FN(this->lock, lock)();
               };
               inline guard(UnknownLock01& l) : lock(&l) {
                  CALL_MEMBER_FN(this->lock, lock)();
               };
               inline ~guard() {
                  CALL_MEMBER_FN(this->lock, unlock)();
               };
         };
   };
   //
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