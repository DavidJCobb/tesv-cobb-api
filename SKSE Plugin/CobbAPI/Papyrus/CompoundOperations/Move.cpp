#pragma once
#include "Papyrus/CompoundOperations/Move.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameReferences.h"
#include "skse/GameRTTI.h"

#include "Miscellaneous/math.h"

#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Forms/TESWorldSpace.h"
#include "Services/TeleportMarkerService.h"

#include "Shared.h" // for access to our object storage

#define ERROR_AND_RETURN_IF_WRONG_STACK(verb) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchMove") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return; \
   }
#define ERROR_AND_RETURN_VALUE_IF_WRONG_STACK(verb, retval) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchMove") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return retval; \
   }

namespace CobbPapyrus {
   namespace BatchMove {
      //
      // Functor virtual methods:
      //
      bool BatchMoveFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         //
         UInt32 operationCount = this->operations.size();
         SERIALIZATION_ASSERT(WriteData(intfc, &operationCount), "Failed to write the operation count.");
         for (UInt32 i = 0; i<operationCount; i++)
            SERIALIZATION_ASSERT(WriteData(intfc, &(this->operations[i])), "Failed to write operation %d.", i);
         //
         SERIALIZATION_ASSERT(WriteData(intfc, &(this->alsoMoveTeleportMarkers)), "Failed to write the also-move-teleport-markers flag.");
         //
         return true;
      };
      bool BatchMoveFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         //
         UInt32 operationCount;
         SERIALIZATION_ASSERT(ReadData(intfc, &operationCount), "Failed to read the operation count.");
         //
         this->operations.resize(operationCount);
         for (UInt32 i = 0; i < operationCount; i++) {
            OperationData e;
            if (version < 2) {
               SERIALIZATION_ASSERT(ReadData(intfc, &e.subjectRefrHandle), "Failed to read operation %d subject handle (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.targetRefrHandle), "Failed to read operation %d target handle (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.pos), "Failed to read operation %d position (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.rot), "Failed to read operation %d rotation (save version: %d).", i, version);
               e.operationType = kOpType_Normal;
            } else {
               SERIALIZATION_ASSERT(ReadData(intfc, &e), "Failed to read operation %d.", i);
            }
            this->operations.push_back(e);
         }
         if (version >= 2) {
            if (version >= 3) {
               SERIALIZATION_ASSERT(ReadData(intfc, &(this->alsoMoveTeleportMarkers)), "Failed to read the also-move-teleport-markers flag.");
            } else if (version == 2) {
               bool value;
               SERIALIZATION_ASSERT(ReadData(intfc, &value), "Failed to read the also-move-teleport-markers flag.");
               this->alsoMoveTeleportMarkers = 0;
               if (value)
                  this->alsoMoveTeleportMarkers = 1;
            }
         }
         //
         return true;
      };
      void BatchMoveFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         UInt32 nullHandle = *g_invalidRefHandle;
         VMResultArray<TESObjectREFR*> allSpawned;
         //
         for (size_t i = 0; i < this->operations.size(); i++) {
            OperationData& e = this->operations[i];
            RE::refr_ptr subject;
            RE::refr_ptr target;
            //
            // Look up the reference.
            //
            subject.set_from_handle(&e.subjectRefrHandle);
            if (!subject)
               continue;
            target.set_from_handle(&e.targetRefrHandle);
            //
            // Position the subject.
            //
            RE::TESObjectCELL* parentCell = nullptr;
            RE::TESWorldSpace* worldspace = nullptr;
            RE::NiPoint3 finalPos(e.pos);
            RE::NiPoint3 originalPos(subject->pos);
            RE::NiPoint3 originalRot(subject->rot);
            if (e.operationType == kOpType_MoveToEditorLocation) {
               bool success = ((RE::TESObjectREFR*)subject)->MoveToMyEditorLocation(false);
               if (!success) {
                  if (this->alsoMoveTeleportMarkers) {
                     RE::refr_ptr destination;
                     subject->GetDestinationDoor(destination);
                     if (destination)
                        TeleportMarkerService::GetInstance().ResetMarker(destination.get_base());
                  }
                  //
                  // Need to prevent the refcount from net-decreasing...
                  //
                  allSpawned.push_back((::TESObjectREFR*) subject.abandon());
               }
               continue;
            }
            parentCell = subject->parentCell;
            worldspace = CALL_MEMBER_FN(subject, GetWorldspace)();
            //
            if (target != nullptr) {
               parentCell = target->parentCell;
               worldspace = CALL_MEMBER_FN(target, GetWorldspace)();
               finalPos += target->pos;
               target = nullptr;
            }
            //
            switch (e.operationType) {
               case kOpType_TeleportMarker:
                  if (!TeleportMarkerService::GetInstance().MoveMarker(subject.get_base(), finalPos, e.rot)) {
                     #ifdef _DEBUG
                     _MESSAGE("Move functor wanted to move the teleport marker of something that isn't a load door.");
                     #endif
                  }
                  break;
               case kOpType_Normal:
               default:
                  if (e.operationType != kOpType_MoveToEditorLocation)
                     //MoveRefrToPosition(subject, &nullHandle, parentCell, worldspace, &finalPos, &e.rot);
                     subject->MoveTo(&nullHandle, parentCell, worldspace, &finalPos, (RE::NiPoint3*)&e.rot);
                  //
                  if (this->alsoMoveTeleportMarkers) {
                     RE::refr_ptr destination;
                     subject->GetDestinationDoor(destination);
                     if (destination) {
                        if (this->alsoMoveTeleportMarkers == kMoveTeleport_Yes) {
                           TeleportMarkerService::GetInstance().MoveMarkerRelativeTo(destination.get_base(), originalPos, originalRot, finalPos, e.rot);
                        } else if (this->alsoMoveTeleportMarkers == kMoveTeleport_EditorOffset) {
                           TeleportMarkerService::GetInstance().MoveMarkerToRelativeEditorLocOffset(subject.get_base(), destination.get_base());
                        }
                     }
                  }
            };
            //
            // Save spawned for result.
            //
            allSpawned.push_back((::TESObjectREFR*) subject.get_base());
         }
         PackValue(&resultValue, &allSpawned, registry);
      };
      void BatchMoveFunctor::AddOperation(TESObjectREFR* subject, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot, const UInt8& type) {
         UInt32 subjectRefrHandle = GetOrCreateRefrHandle(subject);
         UInt32 targetRefrHandle = GetOrCreateRefrHandle(target);
         if (subjectRefrHandle == *g_invalidRefHandle)
            return;
         //
         OperationData t = { subjectRefrHandle, targetRefrHandle, pos, rot, (OperationType)type };
         this->operations.push_back(t);
      };
      void BatchMoveFunctor::SetAlsoMoveTeleportMarkers(SInt32 which) {
         this->alsoMoveTeleportMarkers = which;
      };
      //
      // Papyrus APIs:
      //
      SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         BatchMoveFunctor* func = new BatchMoveFunctor(stackId);
         UInt32 handle = StorableObjectStorageInstance().StoreObject(func, stackId);
         return handle;
      }
      void AddOperation(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         TESObjectREFR* subject,
         TESObjectREFR* target,
         VMArray<float> position,
         VMArray<float> rotation,
         UInt32 operationType
      ) {
         ERROR_AND_RETURN_IF(handle <= 0,             "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(subject == NULL,         "Subject parameter must not be none.", registry, stackId);
         ERROR_AND_RETURN_IF(position.Length() != 3,  "Position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(rotation.Length() != 3,  "Rotation parameter must be an array of length 3.", registry, stackId);
         //
         BatchMoveFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         NiPoint3 pos;
         position.Get(&pos.x, 0);
         position.Get(&pos.y, 1);
         position.Get(&pos.z, 2);
         //
         NiPoint3 rot;
         rotation.Get(&rot.x, 0);
         rotation.Get(&rot.y, 1);
         rotation.Get(&rot.z, 2);
         rot.x = cobb::degrees_to_radians(rot.x);
         rot.y = cobb::degrees_to_radians(rot.y);
         rot.z = cobb::degrees_to_radians(rot.z);
         //
         func->AddOperation(subject, target, pos, rot, operationType);
      }
      void AddOperation(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* tag,
         SInt32 handle,
         TESObjectREFR* subject,
         TESObjectREFR* target,
         VMArray<float> position,
         VMArray<float> rotation
      ) {
         AddOperation(registry, stackId, tag, handle, subject, target, position, rotation, BatchMoveFunctor::kOpType_Normal);
      }
      void SetAlsoMoveTeleportMarkers(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* tag,
         SInt32 handle, SInt32 which
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->SetAlsoMoveTeleportMarkers(which);
      };
      bool Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveFunctor>(handle);
         ERROR_AND_RETURN_0_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_VALUE_IF_WRONG_STACK("executed", false);
         func = StorableObjectStorageInstance().TakeObject<BatchMoveFunctor>(handle); // remove it from storage
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("BatchMove") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled and its handle has been freed.", stackId);
            delete func;
            return false;
         }
      }
      void Cancel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("canceled");
         func = StorableObjectStorageInstance().TakeObject<BatchMoveFunctor>(handle); // remove it from storage
         delete func;
      }
   }
};

bool CobbPapyrus::BatchMove::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<BatchMoveFunctor>();
   //
   // PAPYRUS APIS
   //
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, SInt32>(
         "Create",
         PapyrusPrefixString("BatchMove"),
         BatchMove::Create,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMove"), "Create", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction5 <StaticFunctionTag, void, SInt32, TESObjectREFR*, TESObjectREFR*, VMArray<float>, VMArray<float>>(
         "AddOperation",
         PapyrusPrefixString("BatchMove"),
         BatchMove::AddOperation,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMove"), "AddOperation", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction6 <StaticFunctionTag, void, SInt32, TESObjectREFR*, TESObjectREFR*, VMArray<float>, VMArray<float>, UInt32>(
         "AddOperationSpecial",
         PapyrusPrefixString("BatchMove"),
         BatchMove::AddOperation,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMove"), "AddOperationSpecial", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, SInt32>(
         "SetAlsoMoveTeleportMarkers",
         PapyrusPrefixString("BatchMove"),
         BatchMove::SetAlsoMoveTeleportMarkers,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMove"), "SetAlsoMoveTeleportMarkers", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, SInt32>(
         "Cancel",
         PapyrusPrefixString("BatchMove"),
         BatchMove::Cancel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMove"), "Cancel", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32>(
         "Run",
         PapyrusPrefixString("BatchMove"),
         BatchMove::Run,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};