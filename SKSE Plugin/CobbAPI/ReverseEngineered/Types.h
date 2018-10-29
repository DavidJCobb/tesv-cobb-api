#pragma once
#include "skse/GameTypes.h"

namespace RE {
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