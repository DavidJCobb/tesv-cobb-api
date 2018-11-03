#include "Papyrus/SharedFunctors/ExtraDataServiceFunctor.h"

#include "Papyrus/_Prefixes.h"
#include "Services/ExtraDataService.h"

namespace CobbPapyrus {
   void ExtraDataServiceFunctor::Run(VMValue& resultValue) {
      CobbExtraDataService* service = CobbExtraDataService::GetSingleton();
      if (!service) {
         //
         // TODO: log error
         //
         return;
      }
      service->ModifyValue(this->referenceFormID, this->extraDataType, this->memberOffset, this->memberSize, this->memberValue);
   };
}