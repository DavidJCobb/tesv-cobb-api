#pragma once
#include "Papyrus/CompoundOperations/MoveRel.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameReferences.h"

#include "Miscellaneous/math.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "Services/TeleportMarkerService.h"

#include "Shared.h" // for access to our object storage
#include "CobbRotation.h"
#include "Papyrus/Rotations.h"

#define ERROR_AND_RETURN_IF_WRONG_STACK(verb) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchMoveRel") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return; \
   }
#define ERROR_AND_RETURN_VALUE_IF_WRONG_STACK(verb, retval) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchMoveRel") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return retval; \
   }

namespace CobbPapyrus {
   namespace BatchMoveRel {
      //
      // Functor virtual methods:
      //
      bool BatchMoveRelFunctor::OnSave(SKSESerializationInterface* intfc) {
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
      bool BatchMoveRelFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         //
         UInt32 operationCount;
         SERIALIZATION_ASSERT(ReadData(intfc, &operationCount), "Failed to read the operation count.");
         //
         this->operations.resize(operationCount);
         for (UInt32 i = 0; i<operationCount; i++) {
            OperationData e;
            if (version < 2) {
               SERIALIZATION_ASSERT(ReadData(intfc, &e.subjectRefrHandle), "Failed to read operation %d subject handle (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.targetRefrHandle), "Failed to read operation %d target handle (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.pos), "Failed to read operation %d position (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.rot), "Failed to read operation %d rotation (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.usingTargetPoint), "Failed to read operation %d target-point flag (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.targetPos), "Failed to read operation %d target-position (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.targetRot), "Failed to read operation %d target-rotation (save version: %d).", i, version);
               e.operationType = kOpType_Normal;
            } else {
               SERIALIZATION_ASSERT(ReadData(intfc, &e), "Failed to read operation %d.", i);
            }
            this->operations.push_back(e);
         }
         //
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
      void BatchMoveRelFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         UInt32 nullHandle = *g_invalidRefHandle;
         VMResultArray<TESObjectREFR*> allMoved;
         //
         for (size_t i = 0; i < this->operations.size(); i++) {
            OperationData& e = this->operations[i];
            RE::refr_ptr subject;
            RE::refr_ptr target;
            subject.set_from_handle(&e.subjectRefrHandle);
            if (!subject)
               continue;
            //
            // Position the subject.
            //
            TESObjectCELL* parentCell = nullptr;
            TESWorldSpace* worldspace = nullptr;
            NiPoint3 finalPos(e.pos);
            NiPoint3 finalRot(e.rot);
            NiPoint3 originalPos(subject->pos);
            NiPoint3 originalRot(subject->rot);
            //
            // If this is a "move to editor location" operation, then we run this code 
            // block and then skip to the next iteration.
            //
            if (e.operationType == kOpType_MoveToEditorLocation) {
               if (!subject->MoveToMyEditorLocation(false)) {
                  DEBUG_ONLY_MESSAGE("MoveRel functor failed to return reference 0x%08X to its editor location.", subject->formID);
                  continue;
               }
               if (this->alsoMoveTeleportMarkers) {
                  RE::refr_ptr destination = RE::refr_ptr::make_from_already_incremented(subject->GetDestinationDoor());
                  if (destination)
                     TeleportMarkerService::GetInstance().ResetMarker(destination.get_base());
               }
               allMoved.push_back((::TESObjectREFR*) subject.abandon());
               continue;
            }
            //
            target.set_from_handle(&e.targetRefrHandle);
            parentCell = (::TESObjectCELL*) subject->parentCell;
            worldspace = (::TESWorldSpace*) CALL_MEMBER_FN(subject, GetWorldspace)();
            if (target != nullptr) {
               parentCell = (::TESObjectCELL*) target->parentCell;
               worldspace = (::TESWorldSpace*) CALL_MEMBER_FN(target, GetWorldspace)();
            }
            //
            // Get the coordinates to move to.
            //
            if (e.operationType == kOpType_MoveToEditorLocationOffset) {
               //
               // Given a (subject) and (target) that aren't created references: take the offset from the 
               // editor position of (target) to the editor position of (subject) and apply that offset to 
               // the current position of (target). If either of these references do not have an editor 
               // position, it will be because they haven't been moved yet, in which case their current 
               // position should be used.
               //
               if (subject->formID >> 0x18 == 0xFF)
                  continue;
               if (target == nullptr || target->formID >> 0x18 == 0xFF)
                  continue;
               Cobb::Coordinates editorOffset;
               {  // Get offset.
                  NiPoint3 editorPosSubject;
                  NiPoint3 editorRotSubject;
                  NiPoint3 editorPosTarget;
                  NiPoint3 editorRotTarget;
                  subject->GetEditorCoordinateDataAlways(&editorPosSubject, &editorRotSubject, &worldspace, &parentCell);
                  {
                     void* dummy;
                     bool success = target->GetEditorCoordinates(&editorPosTarget, &editorRotTarget, &dummy, nullptr);
                     if (!success) {
                        DEBUG_ONLY_MESSAGE("MoveRel functor couldn't get editor position for 0x%08X.", target->formID);
                        editorPosTarget = target->pos;
                        editorRotTarget = target->rot;
                     }
                  }
                  Cobb::Coordinates applied;
                  Cobb::GetRelativeCoordinates(editorOffset, editorPosTarget, editorRotTarget, editorPosSubject, editorRotSubject);
                  Cobb::ApplyRelativeCoordinates(applied, target->pos, target->rot, editorOffset.pos, (NiPoint3)(editorOffset.rot), true, editorOffset.rot.isRadians);
                  applied.rot.ConvertToRadians();
                  finalPos = applied.pos;
                  finalRot.x = applied.rot.x;
                  finalRot.y = applied.rot.y;
                  finalRot.z = applied.rot.z;
               }
            } else {
               //
               // Normal handling: move (subject) relative to (target).
               //
               if (target != nullptr || e.usingTargetPoint) {
                  Cobb::Coordinates applied;
                  if (e.usingTargetPoint) {
                     Cobb::ApplyRelativeCoordinates(applied, e.targetPos, e.targetRot, e.pos, e.rot, true, true);
                  } else if (target != nullptr) {
                     Cobb::ApplyRelativeCoordinates(applied, target->pos, target->rot, e.pos, e.rot, true, true);
                  }
                  finalPos = applied.pos;
                  finalRot.x = applied.rot.x;
                  finalRot.y = applied.rot.y;
                  finalRot.z = applied.rot.z;
               }
            }
            //
            // Carry out the actual operation, now that we have the coordinates we want.
            //
            switch (e.operationType) {
               case kOpType_TeleportMarker:
                  if (!TeleportMarkerService::GetInstance().MoveMarker(subject.get_base(), finalPos, finalRot)) {
                     DEBUG_ONLY_MESSAGE("MoveRel functor wanted to move the teleport marker of something that isn't a load door.");
                  }
                  break;
               case kOpType_TeleportMarkerToEditorOffset:
                  {
                     RE::refr_ptr destination = RE::refr_ptr::make_from_already_incremented(subject->GetDestinationDoor());
                     if (!TeleportMarkerService::GetInstance().MoveMarkerToRelativeEditorLocOffset(destination.get_base(), subject.get_base())) {
                        DEBUG_ONLY_MESSAGE("MoveRel functor failed to move a teleport marker to its editor offset.");
                     }
                  }
                  break;
               case kOpType_Normal:
               default:
                  if (e.operationType != kOpType_MoveToEditorLocation)
                     subject->MoveTo(&nullHandle, parentCell, worldspace, &finalPos, &finalRot);
                  //
                  if (this->alsoMoveTeleportMarkers) {
                     RE::refr_ptr destination = RE::refr_ptr::make_from_already_incremented(subject->GetDestinationDoor());
                     if (destination) {
                        if (this->alsoMoveTeleportMarkers == kMoveTeleport_Yes)
                           TeleportMarkerService::GetInstance().MoveMarkerRelativeTo(destination.get_base(), originalPos, originalRot, finalPos, finalRot);
                        else if (this->alsoMoveTeleportMarkers == kMoveTeleport_EditorOffset)
                           TeleportMarkerService::GetInstance().MoveMarkerToRelativeEditorLocOffset(subject.get_base(), destination.get_base());
                     }
                  }
            }
            //
            // Save spawned for result.
            //
            allMoved.push_back((::TESObjectREFR*) subject.abandon());
         }
         PackValue(&resultValue, &allMoved, registry);
      };
      void BatchMoveRelFunctor::AddOperation(TESObjectREFR* subject, TESObjectREFR* anchor, const NiPoint3& targetPos, const NiPoint3& targetRot, const NiPoint3& pos, const NiPoint3& rot, UInt8 operationType) {
         UInt32 subjectRefrHandle = GetOrCreateRefrHandle(subject);
         UInt32 targetRefrHandle  = GetOrCreateRefrHandle(anchor);
         if (subjectRefrHandle == *g_invalidRefHandle)
            return;
         //
         OperationData t = { subjectRefrHandle, targetRefrHandle, pos, rot, true, targetPos, targetRot, (OperationType)operationType };
         this->operations.push_back(t);
      };
      void BatchMoveRelFunctor::AddOperation(TESObjectREFR* subject, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot, UInt8 operationType) {
         UInt32 subjectRefrHandle = GetOrCreateRefrHandle(subject);
         UInt32 targetRefrHandle  = GetOrCreateRefrHandle(target);
         if (subjectRefrHandle == *g_invalidRefHandle)
            return;
         //
         NiPoint3 dummy(0, 0, 0);
         OperationData t = { subjectRefrHandle, targetRefrHandle, pos, rot, false, dummy, dummy, (OperationType)operationType };
         this->operations.push_back(t);
      };
      void BatchMoveRelFunctor::SetAlsoMoveTeleportMarkers(SInt32 which) {
         this->alsoMoveTeleportMarkers = which;
      };
      //
      // Papyrus APIs:
      //
      SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         BatchMoveRelFunctor* func = new BatchMoveRelFunctor(stackId);
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
         BatchMoveRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveRelFunctor>(handle);
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
         AddOperation(registry, stackId, tag, handle, subject, target, position, rotation, BatchMoveRelFunctor::OperationType::kOpType_Normal);
      }
      void AddOperationAroundPoint(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         TESObjectREFR* subject,
         TESObjectREFR* anchor,
         VMArray<float> targetPosition, VMArray<float> targetRotation,
         VMArray<float> position, VMArray<float> rotation,
         UInt32 operationType
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(subject == NULL, "Subject parameter must not be none.", registry, stackId);
         ERROR_AND_RETURN_IF(targetPosition.Length() != 3, "Target position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(targetRotation.Length() != 3, "Target rotation parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(position.Length() != 3, "Position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(rotation.Length() != 3, "Rotation parameter must be an array of length 3.", registry, stackId);
         //
         BatchMoveRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveRelFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "CobbSKSE internal error: CobbMoverTaskRel: Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         NiPoint3 targetPos;
         targetPosition.Get(&targetPos.x, 0);
         targetPosition.Get(&targetPos.y, 1);
         targetPosition.Get(&targetPos.z, 2);
         //
         NiPoint3 targetRot;
         targetRotation.Get(&targetRot.x, 0);
         targetRotation.Get(&targetRot.y, 1);
         targetRotation.Get(&targetRot.z, 2);
         targetRot.x = cobb::degrees_to_radians(targetRot.x);
         targetRot.y = cobb::degrees_to_radians(targetRot.y);
         targetRot.z = cobb::degrees_to_radians(targetRot.z);
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
         func->AddOperation(subject, anchor, targetPos, targetRot, pos, rot, operationType);
      }
      void AddOperationAroundPoint(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* tag,
         SInt32 handle,
         TESObjectREFR* subject,
         TESObjectREFR* anchor,
         VMArray<float> targetPosition, VMArray<float> targetRotation,
         VMArray<float> position, VMArray<float> rotation
      ) {
         AddOperationAroundPoint(registry, stackId, tag, handle, subject, anchor, targetPosition, targetRotation, position, rotation, BatchMoveRelFunctor::OperationType::kOpType_Normal);
      }
      void SetAlsoMoveTeleportMarkers(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* tag,
         SInt32 handle, SInt32 which
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveRelFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->SetAlsoMoveTeleportMarkers(which);
      };
      bool Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveRelFunctor>(handle);
         ERROR_AND_RETURN_0_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_VALUE_IF_WRONG_STACK("executed", false);
         func = StorableObjectStorageInstance().TakeObject<BatchMoveRelFunctor>(handle); // remove it from storage
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("BatchMoveRel") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled and its handle has been freed.", stackId);
            delete func;
            return false;
         }
      }
      void Cancel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveRelFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("canceled");
         func = StorableObjectStorageInstance().TakeObject<BatchMoveRelFunctor>(handle); // remove it from storage
         delete func;
      }
   }
};

bool CobbPapyrus::BatchMoveRel::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<BatchMoveRelFunctor>();
   //
   // PAPYRUS APIS
   //
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, SInt32>(
         "Create",
         PapyrusPrefixString("BatchMoveRel"),
         BatchMoveRel::Create,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveRel"), "Create", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction5 <StaticFunctionTag, void, SInt32, TESObjectREFR*, TESObjectREFR*, VMArray<float>, VMArray<float>>(
         "AddOperation",
         PapyrusPrefixString("BatchMoveRel"),
         BatchMoveRel::AddOperation,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveRel"), "AddOperation", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction7<StaticFunctionTag, void, SInt32, TESObjectREFR*, TESObjectREFR*, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<float>>(
         "AddOperationAroundPoint",
         PapyrusPrefixString("BatchMoveRel"),
         BatchMoveRel::AddOperationAroundPoint,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveRel"), "AddOperationAroundPoint", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction6<StaticFunctionTag, void, SInt32, TESObjectREFR*, TESObjectREFR*, VMArray<float>, VMArray<float>, UInt32>(
         "AddOperationSpecial",
         PapyrusPrefixString("BatchMoveRel"),
         BatchMoveRel::AddOperation,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveRel"), "AddOperationSpecial", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction8<StaticFunctionTag, void, SInt32, TESObjectREFR*, TESObjectREFR*, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, UInt32>(
         "AddOperationSpecialAroundPoint",
         PapyrusPrefixString("BatchMoveRel"),
         BatchMoveRel::AddOperationAroundPoint,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveRel"), "AddOperationSpecialAroundPoint", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, SInt32>(
         "SetAlsoMoveTeleportMarkers",
         PapyrusPrefixString("BatchMoveRel"),
         BatchMoveRel::SetAlsoMoveTeleportMarkers,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveRel"), "SetAlsoMoveTeleportMarkers", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, SInt32>(
         "Cancel",
         PapyrusPrefixString("BatchMoveRel"),
         BatchMoveRel::Cancel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveRel"), "Cancel", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32>(
         "Run",
         PapyrusPrefixString("BatchMoveRel"),
         BatchMoveRel::Run,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};