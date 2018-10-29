#pragma once
#include "skse/GameAPI.h"

namespace cobb {
   namespace skyrim {
      void dispose(void*, bool); // Calls the Dispose method on a target object, via handwritten assembly.
      void* heap_alloc(UInt32 size); // Allocate memory on Skyrim's heap.
      void* heap_create_by_vtbl(UInt32 size, UInt32 vtbl); // Create an object on Skyrim's heap, and set its VTBL.
      //
      template<typename T> T* heap_create_struct() { // Create an object on Skyrim's heap.
         T* memory = (T*) FormHeap_Allocate(sizeof(T));
         memset(memory, 0, sizeof(T));
         return memory;
      };
   };
};