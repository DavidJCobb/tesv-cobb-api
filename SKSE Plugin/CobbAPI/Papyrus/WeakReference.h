#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"

#include "skse/GameReferences.h"
#include "skse/Serialization.h"

#include <vector>

namespace PapyrusPrefix(Papyrus) {
   namespace WeakReference {
      bool Register(::VMClassRegistry* registry);

      class WeakReferenceServiceFunctor : public CompoundOperationBase {
         public:
            explicit WeakReferenceServiceFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit WeakReferenceServiceFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("WeakReferenceServiceFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual void Run(VMValue& resultValue);
            //
            enum Operation : UInt8 {
               kFunctorOperation_Unspecified,
               kFunctorOperation_GetReference,
               kFunctorOperation_SetReference,
               kFunctorOperation_DeleteReference,
               kFunctorOperation_DeleteAllReferences
            };
            //
            Operation      operation = kFunctorOperation_Unspecified;
            TESObjectREFR* referenceHolder;
            UInt32         referenceTarget;
      };
   }
}