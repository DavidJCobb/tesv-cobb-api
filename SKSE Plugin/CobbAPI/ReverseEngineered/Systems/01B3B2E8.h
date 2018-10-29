#pragma once
#include "ReverseEngineered/Forms/Actor.h"
#include "skse/GameTypes.h"

namespace RE {
   struct Singleton01B3B2E8 { // sizeof == 0x2C
      tArray<ActorKnowledge*> unk00;
      SimpleLock     unk0C;
      tArray<UInt32> unk14;
      tArray<UInt32> unk20;
      //
      inline Singleton01B3B2E8* GetInstance() {
         return *(Singleton01B3B2E8**)0x01B3B2E8;
      };
   };
};