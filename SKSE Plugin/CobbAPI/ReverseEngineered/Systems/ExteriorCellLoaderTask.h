#pragma once
#include "skse/GameForms.h"
#include "ReverseEngineered/Forms/TESObjectCELL.h"

namespace RE {
   // subclass of IOTask
   class ExteriorCellLoaderTask { // sizeof == 0x30
      public:
         ExteriorCellLoaderTask(void* a, SInt32 x, SInt32 y, UInt32 d) {
            this->unk18 = a;
            this->cellX = x;
            this->cellY = y;
            this->unk28 = d;
         };
         virtual ~ExteriorCellLoaderTask(); // Dispose
         virtual void Unk_01(void);
         virtual void Unk_02(); // passes self to IOManager::TESV_0042D770(void*) if InterlockedCompareExchange(&this->unk1C, 2, 3) == 2.
         virtual void Unk_03();
         virtual bool Unk_04(UInt32, UInt32); // debug logging. always returns true  // argument types unknown
         virtual bool Unk_05(); // no-op. always returns false
         virtual void Unk_06();
         virtual void Unk_07(void);
         virtual void Unk_08(UInt32); // argument types unknown
         //
         // vtbl
         UInt32 unk04;
         UInt32 unk08; // refcount?
         UInt32 unk0C;
         UInt32 unk10;
         UInt32 unk14;
         void*  unk18;
         UInt32 unk1C = 0; // actually an int
         SInt32 cellX; // 20 // NOTE: Often, this is read as a word.
         SInt32 cellY; // 24
         UInt32 unk28;
         TESObjectCELL* unk2C = NULL;
   };
}