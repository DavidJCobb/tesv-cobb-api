#include "ReverseEngineered/Forms/TESPackage.h"

namespace RE {
   bool TESPackage::IsExtraType() { // 005DEFA0 - 1.9.32
      switch (this->type) {
         case kPackageType_Activate:
         case kPackageType_Alarm:
         case kPackageType_Flee:
         case kPackageType_Trespass:
         case kPackageType_Spectator:
         case kPackageType_ReactToDead:
         case kPackageType_DoNothing:
         case kPackageType_InGameDialogue:
         case kPackageType_Surface:
         case kPackageType_AvoidPlayer:
         case kPackageType_ReactToDestroyedObject:
         case kPackageType_ReactToGrenadeOrMine:
         case kPackageType_StealWarning:
         case kPackageType_PickPocketWarning:
         case kPackageType_MovementBlocked:
         case kPackageType_VampireFeed:
         case kPackageType_CannibalFeed:
            return true;
      }
      return false;
   }
};