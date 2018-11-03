#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"
#include "Services/RevealService.h"

#include "skse/GameReferences.h"
#include "skse/Serialization.h"

#include <vector>

namespace CobbPapyrus {
   namespace RevealService {
      bool Register(::VMClassRegistry* registry);

      class RevealServiceFunctor : public CompoundOperationBase {
         public:
            explicit RevealServiceFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit RevealServiceFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("RevealServiceFunctor"); }
            virtual UInt32    ClassVersion() const { return 2; }
            //
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual void Run(VMValue& resultValue);
            //
            enum Operation : UInt8 {
               kFunctorOperation_Unspecified,
               kFunctorOperation_GetLightRevealState,
               kFunctorOperation_SetLightRevealState,
               kFunctorOperation_SetLightModels,
               kFunctorOperation_GetMarkerRevealState,
               kFunctorOperation_SetMarkerRevealState,
               kFunctorOperation_SetAllRevealStates,
               kFunctorOperation_RegisterMarkers,
            };
            //
            Operation operation = kFunctorOperation_Unspecified;
            bool      paramBool = false; // parameter for setting the reveal state
            std::vector<TESForm*> paramForms; // parameter for setting light reveal models
            ::RevealService::Transaction paramRegistrations;
      };
   }
}