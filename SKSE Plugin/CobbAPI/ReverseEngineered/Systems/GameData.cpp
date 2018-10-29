#pragma once
#include "ReverseEngineered/Systems/GameData.h"
#include "ReverseEngineered/Miscellaneous.h"

namespace RE {
   TES** g_TES = (TES**) 0x01B2E864;

   /*GridCellArray::GridCellArray() { // Decoded from ASM and listed here purely for my own reference.
      static UInt32* gridsToLoad = (UInt32*)0x01241000;
      {  // Handle grids value.
         UInt32 grids = *gridsToLoad;
         if (grids > 5) {
            grids = 5;
         } else {
            if (grids & 1 == 0)
               grids++;
         }
         *gridsToLoad = grids;
         this->length = grids;
         //
         grids *= grids;
         grids *= 4;
         this->cells = (TESObjectCELL**)CALL_MEMBER_FN(g_formHeap, Allocate)(grids, 0, 0);
         FillBufferWithByte(this->cells, 0, grids);
      }
   };*/

   ::TESObjectCELL** TES::CopyGridCells(UInt32* count) const {
      if (!this->gridCellArray->cells)
         return NULL;
      UInt32 length = this->gridCellArray->length;
      *count = length * length;
      auto list = (::TESObjectCELL**) malloc(*count * 4);
      memcpy(list, this->gridCellArray->cells, *count * 4);
      return list;
   };

   DataHandler* TESDataHandler::GetSingleton() {
      return *((TESDataHandler**)0x012E2CB8);
   };
};