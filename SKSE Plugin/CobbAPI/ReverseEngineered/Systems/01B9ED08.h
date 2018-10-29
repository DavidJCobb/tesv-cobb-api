#pragma once
#include "skse/GameFormComponents.h"
#include "skse/GameForms.h"
#include "skse/Utilities.h"
#include "Shared.h"

namespace RE {
   class Unknown01B9ED08 {
      Unknown01B9ED08* GetInstance() {
         return (Unknown01B9ED08*)0x01B9ED08; // or should it be *(ClassName*)0x01B9ED08 ?
      };
      //
      MEMBER_FN_PREFIX(Unknown01B9ED08);
      DEFINE_MEMBER_FN(Subroutine00B9C440, void, 0x00B9C440, BGSSoundDescriptor*, UInt32);                 // plays a sound?
      DEFINE_MEMBER_FN(Subroutine00B9CB60, void, 0x00B9CB60, void* unk, BGSSoundDescriptor*, UInt32 zero); // plays a sound?
   };

   static DEFINE_SUBROUTINE(Unknown01B9ED08*, GetUnknown01B9ED08Instance, 0x00B9A5D0);
};