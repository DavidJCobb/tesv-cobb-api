#pragma once
#include "Papyrus/WeakReference.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "Services/PapyrusFunctorStorage.h"
#include "skse/PapyrusVM.h"

#include "skse/GameRTTI.h"

#include "Shared.h" // for access to our object storage
#include "Services/WeakReferenceService.h"

namespace PapyrusPrefix(Papyrus) {
   namespace WeakReference {
      bool WeakReferenceServiceFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         SERIALIZATION_ASSERT(WriteData(intfc, &(this->operation)), "Failed to write the operation type.");
         SERIALIZATION_ASSERT(WriteData(intfc, &(this->referenceHolder)), "Failed to write reference holder ID.");
         SERIALIZATION_ASSERT(WriteData(intfc, &(this->referenceTarget)), "Failed to write reference holder variant.");
         return true;
      };
      bool WeakReferenceServiceFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         SERIALIZATION_ASSERT(ReadData(intfc, &(this->operation)), "Failed to read the operation type.");
         SERIALIZATION_ASSERT(ReadData(intfc, &(this->referenceHolder)), "Failed to read reference holder ID.");
         SERIALIZATION_ASSERT(ReadData(intfc, &(this->referenceTarget)), "Failed to read reference target variant.");
         return true;
      };
      void WeakReferenceServiceFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         //
         //UInt32 result = 0;
         //bool returnResult = false;
         //
         WeakReferenceService& service = WeakReferenceService::GetInstance();
         switch (this->operation) {
            case Operation::kFunctorOperation_GetReference:
               {
                  TESObjectREFR* target = service.GetReference(this->referenceHolder, this->referenceTarget);
                  PackValue(&resultValue, &target, registry);
                  return;
               }
            case Operation::kFunctorOperation_SetReference:
               {
                  TESObjectREFR* target = DYNAMIC_CAST(LookupFormByID(this->referenceTarget), TESForm, TESObjectREFR);
                  SInt32 result = service.AddReference(this->referenceHolder, target);
                  PackValue(&resultValue, &result, registry);
                  return;
               }
            case Operation::kFunctorOperation_DeleteReference:
               {
                  service.RemoveReferenceByIndex(this->referenceHolder, this->referenceTarget);
                  return;
               }
            case Operation::kFunctorOperation_DeleteAllReferences:
               {
                  service.RemoveAllReferences(this->referenceHolder);
                  return;
               }
         }
         //if (returnResult)
         //   PackValue(&resultValue, &result, registry);
      };
      //
      // Papyrus APIs:
      //
      bool GetReference(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* holder, UInt32 handle
      ) {
         WeakReferenceServiceFunctor* func = new WeakReferenceServiceFunctor(stackId);
         func->operation = WeakReferenceServiceFunctor::Operation::kFunctorOperation_GetReference;
         func->referenceHolder = holder;
         func->referenceTarget = handle;
         //
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("WeakReferenceService") " operation (Get); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
            delete func;
            return false;
         }
         return true;
      };
      bool SetReference(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* holder, TESObjectREFR* target
      ) {
         WeakReferenceServiceFunctor* func = new WeakReferenceServiceFunctor(stackId);
         func->operation = WeakReferenceServiceFunctor::Operation::kFunctorOperation_SetReference;
         func->referenceHolder = holder;
         func->referenceTarget = target->formID;
         //
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("WeakReferenceService") " operation (Set); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
            delete func;
            return false;
         }
         return true;
      };
      bool RemoveReference(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* holder, UInt32 handle
      ) {
         WeakReferenceServiceFunctor* func = new WeakReferenceServiceFunctor(stackId);
         func->operation = WeakReferenceServiceFunctor::Operation::kFunctorOperation_DeleteReference;
         func->referenceHolder = holder;
         func->referenceTarget = handle;
         //
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("WeakReferenceService") " operation (Delete); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
            delete func;
            return false;
         }
         return true;
      };
      bool RemoveAllReferences(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* holder
      ) {
         WeakReferenceServiceFunctor* func = new WeakReferenceServiceFunctor(stackId);
         func->operation = WeakReferenceServiceFunctor::Operation::kFunctorOperation_DeleteAllReferences;
         func->referenceHolder = holder;
         func->referenceTarget = 0;
         //
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("WeakReferenceService") " operation (DeleteAll); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
            delete func;
            return false;
         }
         return true;
      };
   }
};

bool PapyrusPrefix(Papyrus)::WeakReference::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<WeakReferenceServiceFunctor>();
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction2<StaticFunctionTag, TESObjectREFR*, TESObjectREFR*, UInt32>(
         "Get",
         PapyrusPrefixString("WeakReference"),
         WeakReference::GetReference,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction2<StaticFunctionTag, SInt32, TESObjectREFR*, TESObjectREFR*>(
         "Set",
         PapyrusPrefixString("WeakReference"),
         WeakReference::SetReference,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction2<StaticFunctionTag, void, TESObjectREFR*, UInt32>(
         "Delete",
         PapyrusPrefixString("WeakReference"),
         WeakReference::RemoveReference,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, void, TESObjectREFR*>(
         "DeleteAll",
         PapyrusPrefixString("WeakReference"),
         WeakReference::RemoveAllReferences,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};