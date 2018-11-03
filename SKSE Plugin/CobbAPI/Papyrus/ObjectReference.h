#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/Base.h"

#include "skse/PapyrusVM.h"

//#define COBB_PAPYRUS_OBJECTREFERENCE_CHANGEFORMS_USE_FUNCTOR 1

namespace CobbPapyrus {
   namespace ObjectReference {
      bool Register(::VMClassRegistry* registry);
      //
      namespace Internals {
         #ifdef COBB_PAPYRUS_OBJECTREFERENCE_CHANGEFORMS_USE_FUNCTOR
         class ChangeFlagOperationFunctor : public CompoundOperationBase {
            public:
               explicit ChangeFlagOperationFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
               explicit ChangeFlagOperationFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
               //
               virtual const char*  ClassName() const { return PapyrusPrefixString("ChangeFlagOperationFunctor"); }
               virtual UInt32    ClassVersion() const { return 1; }
               //
               virtual bool OnSave(SKSESerializationInterface* intfc);
               virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
               //
               virtual void Run(VMValue& resultValue);
               //
               enum OperationType : UInt8 {
                  kOperationType_Retrieve   = 0,
                  kOperationType_SetFlags   = 1,
                  kOperationType_ClearFlags = 2,
                  kOperationType_Undefined  = 0xFF
               };
               //
               OperationType  operationType = kOperationType_Undefined;
               UInt32         formID = 0;
               UInt32         operationData = 0;
         };
         #endif
      }
      namespace Loaded3D {
         class CheckNodeForControllerManagerFunctor : public CompoundOperationBase {
            public:
               explicit CheckNodeForControllerManagerFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
               explicit CheckNodeForControllerManagerFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
               //
               virtual const char*  ClassName() const { return PapyrusPrefixString("CheckNodeForControllerManagerFunctor"); }
               virtual UInt32    ClassVersion() const { return 1; }
               //
               virtual bool OnSave(SKSESerializationInterface* intfc);
               virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
               //
               virtual void Run(VMValue& resultValue);
               //
               UInt32 referenceFormID = 0;
            private:
               static bool FindController(NiNode* node);
         };
      }
   }
}