#pragma once
#include "skse/Utilities.h"

namespace RE {
   class Unknown01B2E724 { // this is InputManager; see skse/gameinput.h
      Unknown01B2E724* GetInstance() {
         return (Unknown01B2E724*)0x01B2E724; // or should it be *(ClassName*)0x01B2E724 ?
      };
      //
      MEMBER_FN_PREFIX(Unknown01B2E724);
      DEFINE_MEMBER_FN(UsingGamepad, bool, 0x00A68EB0);
   };
}