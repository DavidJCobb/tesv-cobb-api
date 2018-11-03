#pragma once
#include "Papyrus/CompoundOperations/SpawnRel.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameReferences.h"

#include "Miscellaneous/math.h"
#include "Shared.h" // for access to our object storage
#include "CobbRotation.h"
#include "Papyrus/Rotations.h"

#define ERROR_AND_RETURN_IF_WRONG_STACK(verb) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchSpawnRel") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return; \
   }
#define ERROR_AND_RETURN_VALUE_IF_WRONG_STACK(verb, retval) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchSpawnRel") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return retval; \
   }

namespace CobbPapyrus {
   namespace BatchSpawnRel {
      //
      // Functor virtual methods:
      //
      bool BatchSpawnRelFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         //
         UInt32 operationCount = this->operations.size();
         SERIALIZATION_ASSERT(WriteData(intfc, &operationCount), "Failed to write the operation count.");
         for (UInt32 i = 0; i < operationCount; i++)
            SERIALIZATION_ASSERT(WriteData(intfc, &(this->operations[i])), "Failed to write operation %d.", i);
         //
         return true;
      };
      bool BatchSpawnRelFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         //
         UInt32 operationCount;
         SERIALIZATION_ASSERT(ReadData(intfc, &operationCount), "Failed to read the operation count.");
         //
         this->operations.reserve(operationCount);
         for (UInt32 i = 0; i < operationCount; i++) {
            OperationData e;
            if (version < 2) {
               SERIALIZATION_ASSERT(ReadData(intfc, &e.spawnFormId), "Failed to read operation %d form-to-spawn (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.targetRefrHandle), "Failed to read operation %d target handle (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.pos), "Failed to read operation %d position (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.rot), "Failed to read operation %d rotation (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.count), "Failed to read operation %d count (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.bForcePersist), "Failed to read operation %d persist flag (save version: %d).", i, version);
               SERIALIZATION_ASSERT(ReadData(intfc, &e.bInitiallyDisabled), "Failed to read operation %d disabled flag (save version: %d).", i, version);
               e.usingTargetPoint = false;
            } else {
               SERIALIZATION_ASSERT(ReadData(intfc, &e), "Failed to read operation %d.", i);
            }
            //
            // Fix up form ID of spawn. Might change if load order was changed.
            //
            UInt32 fixedFormId;
            if (!intfc->ResolveFormId(e.spawnFormId, &fixedFormId)) {
               _MESSAGE(__FUNCTION__ ": form ID of spawn %i could not be resolved; skipping.");
               continue;
            }
            e.spawnFormId = fixedFormId;
            //
            this->operations.push_back(e);
         }
         //
         return true;
      };
      void BatchSpawnRelFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         UInt32 nullHandle = *g_invalidRefHandle;
         VMResultArray<TESObjectREFR*> allSpawned;
         //
         for (size_t i = 0; i < this->operations.size(); i++) {
            OperationData& e = this->operations[i];
            //
            // Look-ups.
            //
            TESForm* spawnForm = LookupFormByID(e.spawnFormId);
            if (spawnForm == NULL)
               continue;
            //
            TESObjectREFR* target = NULL;
            LookupREFRByHandle(&e.targetRefrHandle, &target);
            if (target == NULL)
               continue;
            //
            // Spawn the new object.
            //
            TESObjectREFR* subject = PlaceAtMe_Native(registry, this->_stackId, target, spawnForm, e.count, e.bForcePersist, e.bInitiallyDisabled);
            if (subject == NULL)
               continue;
            //
            // Position the subject...
            //
            TESObjectCELL* parentCell = subject->parentCell;
            TESWorldSpace* worldspace = CALL_MEMBER_FN(subject, GetWorldspace)();
            if (target != NULL) {
               parentCell = target->parentCell;
               worldspace = CALL_MEMBER_FN(target, GetWorldspace)();
            }
            //
            NiPoint3 finalPos(e.pos);
            NiPoint3 finalRot(e.rot);
            {
               if (target != NULL || e.usingTargetPoint) {
                  Cobb::Coordinates applied;
                  if (e.usingTargetPoint) {
                     Cobb::ApplyRelativeCoordinates(applied, e.targetPos, e.targetRot, e.pos, e.rot, true, true);
                  } else if (target != NULL) {
                     Cobb::ApplyRelativeCoordinates(applied, target->pos, target->rot, e.pos, e.rot, true, true);
                  }
                  finalPos = applied.pos;
                  finalRot.x = applied.rot.x;
                  finalRot.y = applied.rot.y;
                  finalRot.z = applied.rot.z;
               }
            }
            MoveRefrToPosition(subject, &nullHandle, parentCell, worldspace, &finalPos, &finalRot);
            //
            if (target) {
               target->handleRefObject.DecRef(); target = NULL; // LookupREFRByHandle incremented the refcount
            }
            //
            // Save spawned for result.
            //
            allSpawned.push_back(subject);
         }
         PackValue(&resultValue, &allSpawned, registry);
      };
      void BatchSpawnRelFunctor::AddSpawn(TESForm* form, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot, UInt32 count, bool bForcePersist, bool bInitiallyDisabled) {
         UInt32 formId = form->formID;
         UInt32 targetRefrHandle = GetOrCreateRefrHandle(target);
         if (targetRefrHandle == *g_invalidRefHandle)
            return;
         //
         NiPoint3 dummy(0, 0, 0);
         OperationData t = { formId, targetRefrHandle, pos, rot, count, bForcePersist, bInitiallyDisabled, false, dummy, dummy };
         this->operations.push_back(t);
      };
      void BatchSpawnRelFunctor::AddSpawn(TESForm* form, TESObjectREFR* anchor, const NiPoint3& targetPos, const NiPoint3& targetRot, const NiPoint3& pos, const NiPoint3& rot, UInt32 count, bool bForcePersist, bool bInitiallyDisabled) {
         UInt32 formId = form->formID;
         UInt32 targetRefrHandle = GetOrCreateRefrHandle(anchor);
         if (targetRefrHandle == *g_invalidRefHandle)
            return;
         //
         OperationData t = { formId, targetRefrHandle, pos, rot, count, bForcePersist, bInitiallyDisabled, true, targetPos, targetRot };
         this->operations.push_back(t);
      };
      //
      // Papyrus APIs:
      //
      SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         BatchSpawnRelFunctor* func = new BatchSpawnRelFunctor(stackId);
         UInt32 handle = StorableObjectStorageInstance().StoreObject(func, stackId);
         return handle;
      }
      void AddSpawn(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, TESForm* form, TESObjectREFR* target,
         VMArray<float> position, VMArray<float> rotation, SInt32 count, bool bForcePersist, bool bInitiallyDisabled
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(form == NULL, "FormToPlace parameter must not be none.", registry, stackId);
         ERROR_AND_RETURN_IF(target == NULL, "Target parameter must not be none.", registry, stackId);
         ERROR_AND_RETURN_IF(count <= 0, "Count parameter must be greater zero.", registry, stackId);
         ERROR_AND_RETURN_IF(position.Length() != 3, "Position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(rotation.Length() != 3, "Rotation parameter must be an array of length 3.", registry, stackId);
         //
         BatchSpawnRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnRelFunctor>(handle);
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
         func->AddSpawn(form, target, pos, rot, count, bForcePersist, bInitiallyDisabled);
      }
      void AddSpawnAroundPoint(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         TESForm* form, TESObjectREFR* target,
         VMArray<float> position, VMArray<float> rotation,
         VMArray<float> targetPosition, VMArray<float> targetRotation,
         SInt32 count, bool bForcePersist, bool bInitiallyDisabled
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(form == NULL, "FormToPlace parameter must not be none.", registry, stackId);
         ERROR_AND_RETURN_IF(target == NULL, "Target parameter must not be none.", registry, stackId);
         ERROR_AND_RETURN_IF(count <= 0, "Count parameter must be greater zero.", registry, stackId);
         ERROR_AND_RETURN_IF(position.Length() != 3, "Position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(rotation.Length() != 3, "Rotation parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(targetPosition.Length() != 3, "Target position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(targetRotation.Length() != 3, "Target rotation parameter must be an array of length 3.", registry, stackId);
         //
         BatchSpawnRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnRelFunctor>(handle);
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
         NiPoint3 posTarget;
         targetPosition.Get(&posTarget.x, 0);
         targetPosition.Get(&posTarget.y, 1);
         targetPosition.Get(&posTarget.z, 2);
         //
         NiPoint3 rotTarget;
         targetRotation.Get(&rotTarget.x, 0);
         targetRotation.Get(&rotTarget.y, 1);
         targetRotation.Get(&rotTarget.z, 2);
         rotTarget.x = cobb::degrees_to_radians(rotTarget.x);
         rotTarget.y = cobb::degrees_to_radians(rotTarget.y);
         rotTarget.z = cobb::degrees_to_radians(rotTarget.z);
         //
         func->AddSpawn(form, target, pos, rot, posTarget, rotTarget, count, bForcePersist, bInitiallyDisabled);
      }
      bool Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchSpawnRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnRelFunctor>(handle);
         ERROR_AND_RETURN_0_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_VALUE_IF_WRONG_STACK("executed", false);
         func = StorableObjectStorageInstance().TakeObject<BatchSpawnRelFunctor>(handle); // remove it from storage
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("BatchSpawnRel") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled and its handle has been freed.", stackId);
            delete func;
            return false;
         }
      }
      void Cancel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchSpawnRelFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnRelFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("canceled");
         func = StorableObjectStorageInstance().TakeObject<BatchSpawnRelFunctor>(handle); // remove it from storage
         delete func;
      }
   }
};

bool CobbPapyrus::BatchSpawnRel::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<BatchSpawnRelFunctor>();
   //
   // PAPYRUS APIS
   //
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, SInt32>(
         "Create",
         PapyrusPrefixString("BatchSpawnRel"),
         BatchSpawnRel::Create,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnRel"), "Create", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction8<StaticFunctionTag, void, SInt32, TESForm*, TESObjectREFR*, VMArray<float>, VMArray<float>, SInt32, bool, bool>(
         "AddSpawn",
         PapyrusPrefixString("BatchSpawnRel"),
         BatchSpawnRel::AddSpawn,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnRel"), "AddSpawn", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction10<StaticFunctionTag, void, SInt32, TESForm*, TESObjectREFR*, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, SInt32, bool, bool>(
         "AddSpawnAroundPoint",
         PapyrusPrefixString("BatchSpawnRel"),
         BatchSpawnRel::AddSpawnAroundPoint,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnRel"), "AddSpawnAroundPoint", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, SInt32>(
         "Cancel",
         PapyrusPrefixString("BatchSpawnRel"),
         BatchSpawnRel::Cancel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnRel"), "Cancel", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32>(
         "Run",
         PapyrusPrefixString("BatchSpawnRel"),
         BatchSpawnRel::Run,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};