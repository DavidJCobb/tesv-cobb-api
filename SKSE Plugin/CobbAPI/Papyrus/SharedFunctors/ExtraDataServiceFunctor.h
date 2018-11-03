#pragma once
#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/Base.h"

namespace CobbPapyrus {
   class ExtraDataServiceFunctor : public CompoundOperationBase {
      public:
         explicit ExtraDataServiceFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
         explicit ExtraDataServiceFunctor(SerializationTag tag) : CompoundOperationBase(tag) {};
         //
         virtual const char*  ClassName() const { return PapyrusPrefixString("ExtraDataServiceFunctor"); }
         virtual UInt32    ClassVersion() const { return 1; }
         //
         virtual bool OnSave(SKSESerializationInterface* intfc);
         virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
         //
         virtual void Run(VMValue& resultValue);
         //
         UInt32 referenceFormID;
         UInt8 extraDataType;
         UInt32 memberOffset;
         UInt32 memberSize;
         UInt32 memberValue;
         //
         static ExtraDataServiceFunctor* CreateFunctor(UInt32 stackId, UInt32 referenceFormID, UInt8 type, UInt32 offset, UInt32 size, UInt32 value) {
            ExtraDataServiceFunctor* func = new ExtraDataServiceFunctor(stackId);
            func->referenceFormID = referenceFormID;
            func->extraDataType = type;
            func->memberOffset = offset;
            func->memberSize = size;
            func->memberValue = value;
            return func;
         };
   };
}