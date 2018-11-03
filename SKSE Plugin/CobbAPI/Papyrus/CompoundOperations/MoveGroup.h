#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"

#include "skse/GameReferences.h"
#include "skse/PapyrusArgs.h"
#include "skse/Serialization.h"

#include <vector>

namespace CobbPapyrus {
   namespace BatchMoveGroup {
      bool Register(::VMClassRegistry* registry);

      class BatchMoveGroupFunctor : public CompoundOperationBase {
         public:
            explicit BatchMoveGroupFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit BatchMoveGroupFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("BatchMoveGroupFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            enum { kSaveVersion = 3 };
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual void Run(VMValue& resultValue);
            //
            void AddChild(TESObjectREFR* subject);
            void AddChildren(VMArray<TESObjectREFR*> subjects);
            void SetRoot(TESObjectREFR* subject);
            void SetDestinationPosition(float x, float y, float z);
            void SetDestinationRotation(float x, float y, float z);
            void SetAlsoMoveTeleportMarkers(bool);
            void SetTargetCell(TESObjectCELL*);
         private:
            NiPoint3 _destinationPos;
            NiPoint3 _destinationRot;
            UInt32 _rootRefrHandle = *g_invalidRefHandle;
            struct OperationData {
               UInt32 subjectRefrHandle;
            };
            std::vector<OperationData> operations;
            //
            bool alsoMoveTeleportMarkers = false;
            TESObjectCELL* targetCell = NULL;
      };
   }
}