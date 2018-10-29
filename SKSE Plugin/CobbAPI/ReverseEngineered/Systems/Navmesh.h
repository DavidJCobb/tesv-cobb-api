#pragma once

#include "skse/Utilities.h"

namespace RE {
   struct NavMeshObstacleManager {
      public:
         MEMBER_FN_PREFIX(NavMeshObstacleManager);
         DEFINE_MEMBER_FN(GetProcessedTaskCount,   UInt32,  0x005FCC40);
         DEFINE_MEMBER_FN(GetBackgroundTaskCount,  UInt32,  0x005FCC80);
         DEFINE_MEMBER_FN(AddProcessedTask,        void,    0x005FEF60, void* unknown);
   };
};