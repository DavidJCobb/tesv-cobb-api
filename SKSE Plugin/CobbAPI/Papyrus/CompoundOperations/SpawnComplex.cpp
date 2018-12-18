#pragma once
#include "Papyrus/CompoundOperations/SpawnComplex.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameReferences.h"

#include "Miscellaneous/math.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "Shared.h" // for access to our object storage
#include "CobbRotation.h"
#include "Papyrus/Rotations.h"

//
// This is a variant on BatchSpawnRel. It allows you to group spawns into buckets (or "sets"), and it preserves the 
// exact order of spawns within these sets (i.e. a failed spawn produces a None rather than being silently removed 
// from its set). BatchSpawnComplex objects stick around after executing, allowing you to query the sets individually. 
// These objects should be manually deleted by the Papyrus call stack that created them, once all sets have been 
// retrieved.
//

#define ERROR_AND_RETURN_IF_WRONG_STACK(verb) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchSpawnComplex") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return; \
   }
#define ERROR_AND_RETURN_VALUE_IF_WRONG_STACK(verb, retval) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchSpawnComplex") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return retval; \
   }

namespace CobbPapyrus {
   namespace BatchSpawnComplex {
      //
      // Functor virtual methods:
      //
      bool BatchSpawnComplexFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         //
         if (!WriteData(intfc, &_completed))
            return false;
         //
         UInt32 setCount = this->operationSets.size();
         SERIALIZATION_ASSERT(WriteData(intfc, &setCount), "Failed to write the set count.");
         //
         for (UInt32 i = 0; i < setCount; i++) {
            OperationSet e = this->operationSets[i];
            SERIALIZATION_ASSERT(WriteData(intfc, &e.identifier), "Failed to write the identifier for set %d.", i);
            {  // Set operations.
               UInt32 operationCount = e.operations.size();
               SERIALIZATION_ASSERT(WriteData(intfc, &operationCount), "Failed to write the operation count for set %d (identifier %d).", i, e.identifier);
               for (UInt32 j = 0; j < operationCount; j++) {
                  SERIALIZATION_ASSERT(WriteData(intfc, &(e.operations[j])), "Failed to write operation %d for set %d (identifier %d).", j, i, e.identifier);
               }
            }
            {  // Set completed results.
               UInt32 completedCount = e.results.size();
               SERIALIZATION_ASSERT(WriteData(intfc, &completedCount), "Failed to write the completed-result count for set %d (identifier %d).", i, e.identifier);
               for (UInt32 j = 0; j < completedCount; j++) {
                  SERIALIZATION_ASSERT(WriteData(intfc, &(e.results[j])), "Failed to write completed-result %d for set %d (identifier %d).", j, i, e.identifier);
               }
            }
         }
         //
         return true;
      };
      bool BatchSpawnComplexFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         //
         SERIALIZATION_ASSERT(ReadData(intfc, &this->_completed), "Failed to read the completion status of this operation.");
         //
         UInt32 setCount;
         SERIALIZATION_ASSERT(ReadData(intfc, &setCount), "Failed to read the set count.");
         this->operationSets.reserve(setCount);
         for (UInt32 i = 0; i<setCount; i++) {
            OperationSet e;
            SERIALIZATION_ASSERT(ReadData(intfc, &e.identifier), "Failed to read the identifier for set %d.", i);
            {  // Load pending set operations.
               UInt32 operationCount;
               SERIALIZATION_ASSERT(ReadData(intfc, &operationCount), "Failed to read the operation count for set %d (identifier %d).", i, e.identifier);
               e.operations.resize(operationCount);
               for (size_t j = 0; j < operationCount; j++) {
                  OperationData f;
                  if (version < 3) {
                     UInt32 dummy;
                     SERIALIZATION_ASSERT(ReadData(intfc, &f.spawnFormId), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "form to spawn");
                     SERIALIZATION_ASSERT(ReadData(intfc, &f.targetRefrHandle), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "anchor reference");
                     SERIALIZATION_ASSERT(ReadData(intfc, &f.pos), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "position");
                     SERIALIZATION_ASSERT(ReadData(intfc, &f.rot), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "rotation");
                     SERIALIZATION_ASSERT(ReadData(intfc, &dummy), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "count (now unused)");
                     SERIALIZATION_ASSERT(ReadData(intfc, &f.bForcePersist), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "persist flag");
                     SERIALIZATION_ASSERT(ReadData(intfc, &f.bInitiallyDisabled), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "disabled flag");
                     if (version < 2) {
                        f.usingTargetPoint = false;
                     } else {
                        SERIALIZATION_ASSERT(ReadData(intfc, &f.usingTargetPoint), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "using-target-point flag");
                        SERIALIZATION_ASSERT(ReadData(intfc, &f.targetPos), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "target position");
                        SERIALIZATION_ASSERT(ReadData(intfc, &f.targetRot), "Failed to read operation %d for set %d (identifier %d) (save version: %d): %s wasn't valid.", j, i, e.identifier, version, "target rotation");
                     }
                  } else {
                     SERIALIZATION_ASSERT(ReadData(intfc, &f), "Failed to read operation %d for set %d (identifier %d).", j, i, e.identifier);
                  }
                  {  // Fix up form ID of spawn. Might change if load order was changed.
                     UInt32 fixedFormId;
                     if (!intfc->ResolveFormId(f.spawnFormId, &fixedFormId)) {
                        //_MESSAGE(__FUNCTION__ ": FormID of set %i (identifier %d) spawn %i could not be resolved; setting to invalid handle.", i, e.identifier, j);
                        fixedFormId = *g_invalidRefHandle;
                     }
                     f.spawnFormId = fixedFormId;
                  }
                  e.operations.push_back(f);
               }
            }
            {  // Load completed set operations.
               UInt32 completedCount;
               SERIALIZATION_ASSERT(ReadData(intfc, &completedCount), "Failed to read the completed-result count for set %d (identifier %d).", i, e.identifier);
               e.results.reserve(completedCount);
               for (size_t j = 0; j < completedCount; j++) {
                  UInt32 f;
                  SERIALIZATION_ASSERT(ReadData(intfc, &f), "Failed to read completed-result %d for set %d (identifier %d).", j, i, e.identifier);
                  e.results.push_back(f);
               }
            }
            //
            // Place set into the task-complex instance.
            //
            this->operationSets.push_back(e);
         }
         //
         return true;
      };
      void BatchSpawnComplexFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         UInt32 nullHandle = *g_invalidRefHandle;
         VMResultArray<TESObjectREFR*> allSpawned;
         //
         for (size_t i = 0; i < this->operationSets.size(); i++) {
            OperationSet& s = this->operationSets[i];
            for (size_t j = 0; j < s.operations.size(); j++) {
               OperationData& e = s.operations[j];
               //
               // Look-ups.
               //
               TESForm* spawnForm = LookupFormByID(e.spawnFormId);
               if (spawnForm == nullptr) {
                  s.results.push_back(nullHandle);
                  allSpawned.push_back(nullptr);
                  continue;
               }
               RE::refr_ptr target(e.targetRefrHandle);
               if (target == nullptr) {
                  s.results.push_back(nullHandle);
                  allSpawned.push_back(nullptr);
                  continue;
               }
               //
               // Spawn the new object.
               //
               TESObjectREFR* subject = PlaceAtMe_Native(registry, this->_stackId, target.get_base(), spawnForm, 1, e.bForcePersist, e.bInitiallyDisabled);
               if (subject == nullptr) {
                  s.results.push_back(nullHandle);
                  allSpawned.push_back(nullptr);
                  continue;
               }
               //
               // Position the subject.
               //
               TESObjectCELL* parentCell = subject->parentCell;
               TESWorldSpace* worldspace = CALL_MEMBER_FN(subject, GetWorldspace)();
               if (target != nullptr) {
                  parentCell = (TESObjectCELL*) target->parentCell;
                  worldspace = (TESWorldSpace*) CALL_MEMBER_FN(target, GetWorldspace)();
               }
               NiPoint3 finalPos(e.pos);
               NiPoint3 finalRot(e.rot);
               {
                  if (target || e.usingTargetPoint) {
                     Cobb::Coordinates applied;
                     if (e.usingTargetPoint) {
                        Cobb::ApplyRelativeCoordinates(applied, e.targetPos, e.targetRot, e.pos, e.rot, true, true);
                     } else if (target) {
                        Cobb::ApplyRelativeCoordinates(applied, target->pos, target->rot, e.pos, e.rot, true, true);
                     }
                     finalPos = applied.pos;
                     finalRot.x = applied.rot.x;
                     finalRot.y = applied.rot.y;
                     finalRot.z = applied.rot.z;
                  }
               }
               //
               MoveRefrToPosition(subject, &nullHandle, parentCell, worldspace, &finalPos, &finalRot);
               //
               // Save spawned for result.
               //
               s.results.push_back(GetOrCreateRefrHandle(subject));
               allSpawned.push_back(subject);
            }
         }
         this->_completed = true;
         PackValue(&resultValue, &allSpawned, registry);
      };
      void BatchSpawnComplexFunctor::AddSpawn(UInt32 set, TESForm* form, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot, bool bForcePersist, bool bInitiallyDisabled) {
         if (this->_completed)
            return;
         //
         UInt32 formId = 0;
         if (form != NULL)
            formId = form->formID;
         UInt32 targetRefrHandle = GetOrCreateRefrHandle(target);
         //
         OperationData t = { formId, targetRefrHandle, pos, rot, bForcePersist, bInitiallyDisabled };
         //
         bool found = false;
         for (size_t i = 0; i < this->operationSets.size(); i++) {
            if (this->operationSets[i].identifier == set) {
               this->operationSets[i].operations.push_back(t);
               found = true;
               break;
            }
         }
         if (!found) {
            OperationSet s;
            s.identifier = set;
            s.operations.push_back(t);
            this->operationSets.push_back(s);
         }
      };
      void BatchSpawnComplexFunctor::AddSpawn(
         UInt32 set,
         TESForm* form, TESObjectREFR* anchor,
         const NiPoint3& targetPos, const NiPoint3& targetRot,
         const NiPoint3& pos, const NiPoint3& rot,
         bool bForcePersist, bool bInitiallyDisabled
      ) {
         if (this->_completed)
            return;
         //
         UInt32 formId = 0;
         if (form != NULL)
            formId = form->formID;
         UInt32 targetRefrHandle = GetOrCreateRefrHandle(anchor);
         //
         OperationData t = { formId, targetRefrHandle, pos, rot, bForcePersist, bInitiallyDisabled, true, targetPos, targetRot };
         //
         bool found = false;
         for (size_t i = 0; i < this->operationSets.size(); i++) {
            if (this->operationSets[i].identifier == set) {
               this->operationSets[i].operations.push_back(t);
               found = true;
               break;
            }
         }
         if (!found) {
            OperationSet s;
            s.identifier = set;
            s.operations.push_back(t);
            this->operationSets.push_back(s);
         }
      };
      VMResultArray<TESObjectREFR*> BatchSpawnComplexFunctor::GetResultsFromSet(UInt32 set) {
         VMResultArray<TESObjectREFR*> result;
         if (!this->_completed)
            return result;
         for (size_t i = 0; i < this->operationSets.size(); i++) {
            OperationSet e = this->operationSets[i];
            if (e.identifier == set) {
               result.resize(e.results.size(), nullptr);
               for (size_t j = 0; j < e.results.size(); j++) {
                  //_MESSAGE(PapyrusPrefixString("BatchSpawnComplex") " operation returning a set of results... Currently on result %d in set %d.", j, i);
                  TESObjectREFR* current = nullptr;
                  LookupREFRByHandle(&e.results[j], &current);
                  result[j] = current;
               }
               break;
            }
         }
         return result;
      };
      //
      // Papyrus APIs:
      //
      SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         BatchSpawnComplexFunctor* func = new BatchSpawnComplexFunctor(stackId);
         UInt32 handle = StorableObjectStorageInstance().StoreObject(func, stackId);
         return handle;
      }
      void AddSpawn(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle, UInt32 set, TESForm* form, TESObjectREFR* target,
         VMArray<float> position, VMArray<float> rotation, bool bForcePersist, bool bInitiallyDisabled
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(position.Length() != 3, "Position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(rotation.Length() != 3, "Rotation parameter must be an array of length 3.", registry, stackId);
         //
         BatchSpawnComplexFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnComplexFunctor>(handle);
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
         func->AddSpawn(set, form, target, pos, rot, bForcePersist, bInitiallyDisabled);
      }
      void AddSpawnAroundPoint(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle, UInt32 set, TESForm* form, TESObjectREFR* target,
         VMArray<float> targetPos, VMArray<float> targetRot,
         VMArray<float> position, VMArray<float> rotation,
         bool bForcePersist, bool bInitiallyDisabled
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(targetPos.Length() != 3, "Target position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(targetRot.Length() != 3, "Target rotation parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(position.Length() != 3, "Position parameter must be an array of length 3.", registry, stackId);
         ERROR_AND_RETURN_IF(rotation.Length() != 3, "Rotation parameter must be an array of length 3.", registry, stackId);
         //
         BatchSpawnComplexFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnComplexFunctor>(handle);
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
         targetPos.Get(&posTarget.x, 0);
         targetPos.Get(&posTarget.y, 1);
         targetPos.Get(&posTarget.z, 2);
         //
         NiPoint3 rotTarget;
         targetRot.Get(&rotTarget.x, 0);
         targetRot.Get(&rotTarget.y, 1);
         targetRot.Get(&rotTarget.z, 2);
         rotTarget.x = cobb::degrees_to_radians(rotTarget.x);
         rotTarget.y = cobb::degrees_to_radians(rotTarget.y);
         rotTarget.z = cobb::degrees_to_radians(rotTarget.z);
         //
         func->AddSpawn(set, form, target, posTarget, rotTarget, pos, rot, bForcePersist, bInitiallyDisabled);
      }
      bool Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchSpawnComplexFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnComplexFunctor>(handle);
         ERROR_AND_RETURN_0_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_VALUE_IF_WRONG_STACK("executed", false);
         if (func->IsCompleted()) {
            registry->LogError("Cannot execute a " PapyrusPrefixString("BatchSpawnComplex") " operation if it has already been executed.", stackId);
            return false;
         }
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("BatchSpawnComplex") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled and its handle has been freed.", stackId);
            delete func;
            return false;
         }
      }
      void Cancel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchSpawnComplexFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnComplexFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("canceled");
         func = StorableObjectStorageInstance().TakeObject<BatchSpawnComplexFunctor>(handle); // remove it from storage
         delete func;
      }
      VMResultArray<TESObjectREFR*> GetResults(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, UInt32 set) {
         VMResultArray<TESObjectREFR*> result;
         if (handle <= 0) {
            registry->LogError("Invalid handle.", stackId);
            return result;
         }
         //
         //CobbSpawnerTaskComplexInstance* func = SKSEObjectStorageInstance().AccessObject<CobbSpawnerTaskComplexInstance>(handle);
         BatchSpawnComplexFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnComplexFunctor>(handle);
         if (func == NULL) {
            registry->LogError("Failed to lookup object for given handle.", stackId);
            return result;
         }
         ERROR_AND_RETURN_VALUE_IF_WRONG_STACK("accessed", result);
         //
         return func->GetResultsFromSet(set);
      }
      void Delete(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchSpawnComplexFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSpawnComplexFunctor>(handle);
         if (func) {
            ERROR_AND_RETURN_IF_WRONG_STACK("deleted");
            ERROR_AND_RETURN_IF(func->IsExecuting(), "Cannot delete a " PapyrusPrefixString("BatchSpawnComplex") " operation while it is executing.", registry, stackId); // not that this should be possible anyway
            BatchSpawnComplexFunctor* func = StorableObjectStorageInstance().TakeObject<BatchSpawnComplexFunctor>(handle); // remove it from storage
            delete func;
         }
      }
   }
};

bool CobbPapyrus::BatchSpawnComplex::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<BatchSpawnComplexFunctor>();
   //
   // PAPYRUS APIS
   //
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, SInt32>(
         "Create",
         PapyrusPrefixString("BatchSpawnComplex"),
         BatchSpawnComplex::Create,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnComplex"), "Create", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction8<StaticFunctionTag, void, SInt32, UInt32, TESForm*, TESObjectREFR*, VMArray<float>, VMArray<float>, bool, bool>(
         "AddSpawn",
         PapyrusPrefixString("BatchSpawnComplex"),
         BatchSpawnComplex::AddSpawn,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnComplex"), "AddSpawn", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction10<StaticFunctionTag, void, SInt32, UInt32, TESForm*, TESObjectREFR*, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, bool, bool>(
         "AddSpawnAroundPoint",
         PapyrusPrefixString("BatchSpawnComplex"),
         BatchSpawnComplex::AddSpawnAroundPoint,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnComplex"), "AddSpawnAroundPoint", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, SInt32>(
         "Cancel",
         PapyrusPrefixString("BatchSpawnComplex"),
         BatchSpawnComplex::Cancel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnComplex"), "Cancel", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32, UInt32>(
         "GetResults",
         PapyrusPrefixString("BatchSpawnComplex"),
         BatchSpawnComplex::GetResults,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnComplex"), "GetResults", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, SInt32>(
         "Delete",
         PapyrusPrefixString("BatchSpawnComplex"),
         BatchSpawnComplex::Cancel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSpawnComplex"), "Delete", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32>(
         "Run",
         PapyrusPrefixString("BatchSpawnComplex"),
         BatchSpawnComplex::Run,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};