#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"

#include "skse/GameReferences.h"
#include "skse/PapyrusArgs.h"
#include "skse/Serialization.h"

#include <vector>

namespace CobbPapyrus {
   namespace BatchSpawnComplex {
      bool Register(::VMClassRegistry* registry);

      class BatchSpawnComplexFunctor : public CompoundOperationBase {
         public:
            explicit BatchSpawnComplexFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit BatchSpawnComplexFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("BatchSpawnComplexFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            enum { kSaveVersion = 2 };
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual void Run(VMValue& resultValue);
            //
            virtual volatile bool DeleteWhenDone() const { return false; };
            //
            void AddSpawn(UInt32 set, TESForm* form, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot, UInt32 count, bool bForcePersist, bool bInitiallyDisabled);
            void AddSpawn(UInt32 set, TESForm* form, TESObjectREFR* anchor, const NiPoint3& targetPos, const NiPoint3& targetRot, const NiPoint3& pos, const NiPoint3& rot, bool bForcePersist, bool bInitiallyDisabled);
            bool IsCompleted() { return this->_completed; };
            VMResultArray<TESObjectREFR*> GetResultsFromSet(UInt32 set);
         protected:
            bool _deleteWhenDone = false;
         private:
            bool _completed = false;
            struct OperationData;
            struct OperationSet {
               UInt32 identifier;
               std::vector<OperationData> operations;
               std::vector<UInt32> results;
            };
            struct OperationData {
               UInt32     spawnFormId;
               UInt32     targetRefrHandle;
               NiPoint3   pos;
               NiPoint3   rot;
               UInt32     count; // technically this should always be 1
               bool       bForcePersist;
               bool       bInitiallyDisabled;
               bool       usingTargetPoint;
               NiPoint3   targetPos; // if we use a point instead of an object.
               NiPoint3   targetRot; // if we use a point instead of an object.
            };
            std::vector<OperationSet> operationSets;
      };
   }
}