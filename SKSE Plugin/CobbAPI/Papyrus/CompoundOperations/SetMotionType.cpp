#pragma once
#include "Papyrus/CompoundOperations/SetMotionType.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameReferences.h"
#include "skse/GameRTTI.h"

#include "Shared.h" // for access to our object storage

#define ERROR_AND_RETURN_IF_WRONG_STACK(verb) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("SetMotionType") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return; \
   }
#define ERROR_AND_RETURN_VALUE_IF_WRONG_STACK(verb, retval) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("SetMotionType") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return retval; \
   }

namespace CobbPapyrus {
   namespace BatchSetMotionType {
      //
      // Functor virtual methods:
      //
      bool BatchSetMotionTypeFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         //
         UInt32 operationCount = this->operations.size();
         SERIALIZATION_ASSERT(WriteData(intfc, &operationCount), "Failed to write the operation count.");
         for (UInt32 i = 0; i<operationCount; i++)
            SERIALIZATION_ASSERT(WriteData(intfc, &(this->operations[i])), "Failed to write operation %d.", i);
         //
         return true;
      };
      bool BatchSetMotionTypeFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         //
         UInt32 operationCount;
         SERIALIZATION_ASSERT(ReadData(intfc, &operationCount), "Failed to read the operation count.");
         //
         this->operations.resize(operationCount);
         for (UInt32 i = 0; i < operationCount; i++) {
            OperationData e;
            SERIALIZATION_ASSERT(ReadData(intfc, &e), "Failed to read operation %d.", i);
            this->operations.push_back(e);
         }
         //
         return true;
      };
      void BatchSetMotionTypeFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         UInt32 nullHandle = *g_invalidRefHandle;
         VMResultArray<TESObjectREFR*> allSpawned;
         //
         for (size_t i = 0; i < this->operations.size(); i++) {
            OperationData& e = this->operations[i];
            TESObjectREFR* subject = NULL;
            //
            // Look up the reference.
            //
            LookupREFRByHandle(&e.subjectRefrHandle, &subject);
            if (subject == NULL)
               continue;
            //
            if (e.motionType >= 1) {
               ((RE::TESObjectREFR*)subject)->SetMotionType(e.motionType, e.allowActivate, this->markChanged);
            }
            //
            // Save spawned for result.
            //
            allSpawned.push_back(subject);
            if (subject) {
               subject->handleRefObject.DecRef(); subject = NULL; // LookupREFRByHandle incremented the refcount
            }
         }
         PackValue(&resultValue, &allSpawned, registry);
      };
      void BatchSetMotionTypeFunctor::AddOperation(TESObjectREFR* subject, SInt32 motionType, bool allowActivate) {
         UInt32 subjectRefrHandle = GetOrCreateRefrHandle(subject);
         if (subjectRefrHandle == *g_invalidRefHandle)
            return;
         //
         OperationData t = { subjectRefrHandle, motionType, allowActivate };
         this->operations.push_back(t);
      };
      void BatchSetMotionTypeFunctor::SetWhetherMarkChanged(bool markChanged) {
         this->markChanged = markChanged;
      };
      //
      // Papyrus APIs:
      //
      SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         BatchSetMotionTypeFunctor* func = new BatchSetMotionTypeFunctor(stackId);
         UInt32 handle = StorableObjectStorageInstance().StoreObject(func, stackId);
         return handle;
      }
      void AddOperation(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         TESObjectREFR* subject,
         SInt32 motionType,
         bool allowActivate
      ) {
         ERROR_AND_RETURN_IF(handle <= 0,     "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(subject == NULL, "Subject parameter must not be none.", registry, stackId);
         //
         BatchSetMotionTypeFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSetMotionTypeFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         func->AddOperation(subject, motionType, allowActivate);
      }
      void SetWhetherMarkChanged(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         bool markChanged
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchSetMotionTypeFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSetMotionTypeFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         func->SetWhetherMarkChanged(markChanged);
      };
      bool Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchSetMotionTypeFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSetMotionTypeFunctor>(handle);
         ERROR_AND_RETURN_0_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_VALUE_IF_WRONG_STACK("executed", false);
         func = StorableObjectStorageInstance().TakeObject<BatchSetMotionTypeFunctor>(handle); // remove it from storage
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("BatchSetMotionType") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled and its handle has been freed.", stackId);
            delete func;
            return false;
         }
      }
      void Cancel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchSetMotionTypeFunctor* func = StorableObjectStorageInstance().AccessObject<BatchSetMotionTypeFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("canceled");
         func = StorableObjectStorageInstance().TakeObject<BatchSetMotionTypeFunctor>(handle); // remove it from storage
         delete func;
      }
   }
};

bool CobbPapyrus::BatchSetMotionType::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<BatchSetMotionTypeFunctor>();
   //
   // PAPYRUS APIS
   //
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, SInt32>(
         "Create",
         PapyrusPrefixString("BatchSetMotionType"),
         BatchSetMotionType::Create,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSetMotionType"), "Create", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction4 <StaticFunctionTag, void, SInt32, TESObjectREFR*, SInt32, bool>(
         "AddOperation",
         PapyrusPrefixString("BatchSetMotionType"),
         BatchSetMotionType::AddOperation,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSetMotionType"), "AddOperation", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, void, SInt32, bool>(
         "SetWhetherMarkChanged",
         PapyrusPrefixString("BatchSetMotionType"),
         BatchSetMotionType::SetWhetherMarkChanged,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSetMotionType"), "SetWhetherMarkChanged", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, SInt32>(
         "Cancel",
         PapyrusPrefixString("BatchSetMotionType"),
         BatchSetMotionType::Cancel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchSetMotionType"), "Cancel", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32>(
         "Run",
         PapyrusPrefixString("BatchSetMotionType"),
         BatchSetMotionType::Run,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};