#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"

#include "skse/GameReferences.h"
#include "skse/Serialization.h"

#include <vector>

namespace PapyrusPrefix(Papyrus) {
   namespace BatchMove {
      bool Register(::VMClassRegistry* registry);

      class BatchMoveFunctor : public CompoundOperationBase {
         public:
            explicit BatchMoveFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit BatchMoveFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("BatchMoveFunctor"); }
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
               kOpType_MoveToEditorLocationOffset = 3, // Defined for consistency with MoveRel; not currently implemented
               kOpType_kOpType_TeleportMarkerToEditorOffset = 4, // Defined for consistency with MoveRel; not currently implemented
            };
            void AddOperation(TESObjectREFR* subject, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot, const UInt8& type);
            //
            void SetAlsoMoveTeleportMarkers(SInt32);
         private:
            struct OperationData {
               UInt32        subjectRefrHandle;
               UInt32        targetRefrHandle;
               NiPoint3      pos;
               NiPoint3      rot;
               OperationType operationType;
            };
            std::vector<OperationData> operations;
            //
            UInt8 alsoMoveTeleportMarkers = 0;
      };
   }
}