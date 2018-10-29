#pragma once
#include "skse/GameForms.h"
#include "skse/GameReferences.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"

namespace RE {
   class ShaderReferenceEffect {
      struct UnknownTypeInformation {
         char* type;
         UnknownTypeInformation* next;
      };
      virtual ~ShaderReferenceEffect();
      //
      virtual void Unk_01(void);
      virtual UnknownTypeInformation* GetTypeInformation(); // 02
      //
      UInt32 unk00[(0x20 - 0x00) / sizeof(UInt32)];
      UInt32 refHandle; // 20
      UInt32 unk24;
      UInt8  unk28;
      UInt8  unk29; // pad
      UInt8  unk2A; // pad
      UInt8  unk2B; // pad
      UInt32 unk2C[(0x90 - 0x2C) / sizeof(UInt32)];
      TESEffectShader* shader; // 90
   };
}