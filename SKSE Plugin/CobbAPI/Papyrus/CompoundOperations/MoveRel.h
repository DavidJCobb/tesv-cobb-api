#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"

#include "skse/GameReferences.h"
#include "skse/Serialization.h"

#include <vector>

namespace PapyrusPrefix(Papyrus) {
   namespace BatchMoveRel {
      bool Register(::VMClassRegistry* registry);

      class BatchMoveRelFunctor : public CompoundOperationBase {
         public:
            explicit BatchMoveRelFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit BatchMoveRelFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("BatchMoveRelFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            enum { kSaveVersion = 2 };
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual void Run(VMValue& resultValue);
            //
            enum AlsoMoveTeleportMarkers : UInt8 {
               kMoveTeleport_No = 0,
               kMoveTeleport_Yes = 1,
               kMoveTeleport_EditorOffset = 2,
            };
            enum OperationType : UInt8 {
               kOpType_Normal = 0,
               kOpType_TeleportMarker = 1,
               kOpType_MoveToEditorLocation = 2,
               kOpType_MoveToEditorLocationOffset = 3,
               kOpType_TeleportMarkerToEditorOffset = 4,
            };
            void AddOperation(TESObjectREFR* subject, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot, UInt8 operationType = kOpType_Normal);
            void AddOperation(TESObjectREFR* subject, TESObjectREFR* anchor, const NiPoint3& targetPos, const NiPoint3& targetRot, const NiPoint3& pos, const NiPoint3& rot, UInt8 operationType = kOpType_Normal);
            //
            void SetAlsoMoveTeleportMarkers(SInt32);
         private:
            struct OperationData {
               UInt32     subjectRefrHandle;
               UInt32     targetRefrHandle;
               NiPoint3   pos;
               NiPoint3   rot;
               bool       usingTargetPoint;
               NiPoint3   targetPos; // if we use a point instead of an object.
               NiPoint3   targetRot; // if we use a point instead of an object.
               OperationType operationType;
            };
            std::vector<OperationData> operations;
            //
            UInt8 alsoMoveTeleportMarkers = 0;
      };
   }
}