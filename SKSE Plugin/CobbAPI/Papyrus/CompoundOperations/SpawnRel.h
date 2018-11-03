#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"

#include "skse/GameReferences.h"
#include "skse/Serialization.h"

#include <vector>

namespace CobbPapyrus {
   namespace BatchSpawnRel {
      bool Register(::VMClassRegistry* registry);

      class BatchSpawnRelFunctor : public CompoundOperationBase {
         public:
            explicit BatchSpawnRelFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit BatchSpawnRelFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("BatchSpawnRelFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            enum { kSaveVersion = 2 };
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual void Run(VMValue& resultValue);
            //
            void AddSpawn(TESForm* form, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot, UInt32 count, bool bForcePersist, bool bInitiallyDisabled);
            void AddSpawn(TESForm* form, TESObjectREFR* anchor, const NiPoint3& targetPos, const NiPoint3& targetRot, const NiPoint3& pos, const NiPoint3& rot, UInt32 count, bool bForcePersist, bool bInitiallyDisabled);
         private:
            struct OperationData {
               UInt32     spawnFormId;
               UInt32     targetRefrHandle;
               NiPoint3   pos;
               NiPoint3   rot;
               UInt32     count;
               bool       bForcePersist;
               bool       bInitiallyDisabled;
               bool       usingTargetPoint;
               NiPoint3   targetPos; // if we use a point instead of an object.
               NiPoint3   targetRot; // if we use a point instead of an object.
            };
            std::vector<OperationData> operations;
      };
   }
}