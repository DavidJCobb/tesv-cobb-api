#pragma once
#include "skse/GameRTTI.h"
#include "Utilities/Dumps/ExtraData/ExtraTeleport.h"

void CobbDump(BSExtraData* extra, const char* indent) {
   UInt8 type = extra->GetType();
   switch (type) {
      case kExtraData_Teleport:
         CobbDump((RE::ExtraTeleport*)extra, indent);
         break;
   };
}