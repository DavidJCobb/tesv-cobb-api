#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"

#include "skse/GameReferences.h"
#include "skse/Serialization.h"

#include <vector>

namespace CobbPapyrus {
   namespace BatchSetMotionType {
      bool Register(::VMClassRegistry* registry);

      class BatchSetMotionTypeFunctor : public CompoundOperationBase {
         public:
            explicit BatchSetMotionTypeFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit BatchSetMotionTypeFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("BatchSetMotionTypeFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual void Run(VMValue& resultValue);
            //
            void AddOperation(TESObjectREFR* subject, SInt32 motionType, bool allowActivate = true);
            void SetWhetherMarkChanged(bool);
         private:
            struct OperationData {
               UInt32 subjectRefrHandle;
               SInt8  motionType;
               bool   allowActivate;
            };
            std::vector<OperationData> operations;
            bool markChanged = true;
      };
   }
}