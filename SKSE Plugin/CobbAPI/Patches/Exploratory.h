#pragma once
#include "Exploratory/MagickaAsEnergyShields.h"
#include "Exploratory/LogThreadIDs.h"
#include "Exploratory/LogBSTEventData.h"

namespace CobbPatches {
   namespace Exploratory {
      void Apply() {
         //MagickaAsEnergyShields::Apply();
         //LogThreadIDs::Apply();
         LogBSTEventData::Apply();
      };
   };
};