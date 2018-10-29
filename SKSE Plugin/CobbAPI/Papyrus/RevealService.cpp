#pragma once
#include "Papyrus/RevealService.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "Services/PapyrusFunctorStorage.h"
#include "skse/PapyrusVM.h"

#include "skse/GameData.h"
#include "skse/GameRTTI.h"

#include "Shared.h" // for access to our object storage
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"

namespace PapyrusPrefix(Papyrus) {
   namespace RevealService {
      bool RevealServiceFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         //
         SERIALIZATION_ASSERT(WriteData(intfc, &(this->operation)), "Failed to write the operation type.");
         //
         SERIALIZATION_ASSERT(WriteData(intfc, &(this->paramBool)), "Failed to write the paramBool.");
         {  // param: vector of forms
            auto& param = this->paramForms;
            //
            UInt32 size = param.size();
            SERIALIZATION_ASSERT(WriteData(intfc, &size), "Failed to write the paramForms count.");
            for (UInt32 i = 0; i < size; i++) {
               TESForm* form = param[i];
               UInt32 formId = 0;
               if (form)
                  formId = form->formID;
               SERIALIZATION_ASSERT(WriteData(intfc, &formId), "Failed to write the paramForms (%d).", i);
            }
         }
         {  // param: marker registration
            auto& param = this->paramRegistrations;
            //
            UInt32 size = param.size();
            SERIALIZATION_ASSERT(WriteData(intfc, &size), "Failed to write the paramRegistrations count.");
            for (UInt32 i = 0; i < size; i++) {
               auto& entry = param[i];
               //
               UInt32 formId = entry.form ? entry.form->formID : 0;
               SERIALIZATION_ASSERT(WriteData(intfc, &formId), "Failed to write the paramRegistrations' entry #%d (form ID).", i);
               SERIALIZATION_ASSERT(WriteData(intfc, &entry.bounds), "Failed to write the paramRegistrations' entry #%d (bounds).", i);
               UInt32 size = entry.pathReveal.size();
               SERIALIZATION_ASSERT(WriteData(intfc, &size), "Failed to write the paramRegistrations' entry #%d (path length).", i);
               if (size)
                  SERIALIZATION_ASSERT(intfc->WriteRecordData(entry.pathReveal.data(), size), "Failed to write the paramRegistrations' entry #%d (path).", i);
            }
         }
         //
         return true;
      };
      bool RevealServiceFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         //
         if (version <= 1) {
            UInt8 discard;
            SERIALIZATION_ASSERT(ReadData(intfc, &discard), "Failed to read the (deprecated) form type.");
         }
         SERIALIZATION_ASSERT(ReadData(intfc, &(this->operation)), "Failed to read the operation type.");
         //
         SERIALIZATION_ASSERT(ReadData(intfc, &(this->paramBool)), "Failed to read the paramBool.");
         {  // param: vector of forms
            auto& param = this->paramForms;
            //
            UInt32 size = 0;
            SERIALIZATION_ASSERT(ReadData(intfc, &size), "Failed to read the paramForms count.");
            param.reserve(size);
            for (UInt32 i = 0; i < size; i++) {
               UInt32 formId = 0;
               SERIALIZATION_ASSERT(ReadData(intfc, &formId), "Failed to read the paramForms entry #%d.", i);
               {  // Handle load order changes.
                  UInt32 fixedFormId;
                  if (!intfc->ResolveFormId(formId, &fixedFormId))
                     fixedFormId = 0;
                  formId = fixedFormId;
               }
               TESForm* form = formId ? LookupFormByID(formId) : nullptr;
               param.push_back(form);
            }
         }
         if (version >= 2) { // param: marker registration
            auto& param = this->paramRegistrations;
            //
            UInt32 size = 0;
            SERIALIZATION_ASSERT(ReadData(intfc, &size), "Failed to read the paramRegistrations count.");
            param.resize(size);
            for (UInt32 i = 0; i < size; i++) {
               auto& entry = param[i];
               //
               UInt32 formId = 0;
               SERIALIZATION_ASSERT(ReadData(intfc, &formId), "Failed to read the paramRegistration form ID.");
               {  // Handle load order changes.
                  UInt32 fixedFormId;
                  if (!intfc->ResolveFormId(formId, &fixedFormId))
                     fixedFormId = 0;
                  formId = fixedFormId;
               }
               entry.form = formId ? LookupFormByID(formId) : nullptr;
               SERIALIZATION_ASSERT(ReadData(intfc, &entry.bounds), "Failed to read the paramRegistration bounds.");
               //
               UInt32 size;
               SERIALIZATION_ASSERT(ReadData(intfc, &size), "Failed to read the paramRegistration path length.");
               entry.pathReveal.resize(size);
               if (size)
                  SERIALIZATION_ASSERT(intfc->ReadRecordData((void*)entry.pathReveal.data(), size), "Failed to read the paramRegistration path.");
            }
         }
         //
         return true;
      };
      void RevealServiceFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         UInt32 result = 0;
         bool reload3D = false; // (true) if we need to reload references' 3D. (false) if the reveal handler does it for us (e.g. because it had to loop over the cell to find the target references anyway)
         //
         ::RevealService& service = ::RevealService::GetInstance();
         bool returnResult = false;
         switch (this->operation) {
            case Operation::kFunctorOperation_GetLightRevealState:
               result = (SInt32) service.GetRevealStateLights();
               returnResult = true;
               break;
            case Operation::kFunctorOperation_SetLightRevealState:
               service.SetLightRevealState(this->paramBool);
               break;
            case Operation::kFunctorOperation_SetLightModels:
               service.DefineLightModels(
                  this->paramForms[0],
                  this->paramForms[1],
                  this->paramForms[2],
                  this->paramForms[3],
                  this->paramForms[4]
               );
               break;
            case Operation::kFunctorOperation_GetMarkerRevealState:
               result = (SInt32) service.GetRevealStateMain();
               returnResult = true;
               break;
            case Operation::kFunctorOperation_SetMarkerRevealState:
               service.SetMainRevealState(this->paramBool);
               break;
            case Operation::kFunctorOperation_SetAllRevealStates:
               service.SetAllRevealStates(this->paramBool);
               break;
            case Operation::kFunctorOperation_RegisterMarkers:
               service.RegisterFormsInBulk(this->paramRegistrations);
               break;
         }
         if (returnResult)
            PackValue(&resultValue, &result, registry);
      };
      //
      // ===== Papyrus APIs =========================================================================================
      //
      bool SetLightModels(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         TESObjectACTI* omni,
         TESObjectACTI* omniShadow,
         TESObjectACTI* omniHalf,
         TESObjectACTI* spot,
         TESObjectACTI* spotShadow
      ) {
         ERROR_AND_RETURN_0_IF(omni == NULL, "You must at least specify a model for standard omni lights.", registry, stackId);
         auto func = new RevealServiceFunctor(stackId);
         func->operation = RevealServiceFunctor::Operation::kFunctorOperation_SetLightModels;
         func->paramForms.resize(5);
         func->paramForms[0] = (TESForm*)omni;
         func->paramForms[1] = (TESForm*)omniShadow;
         func->paramForms[2] = (TESForm*)omniHalf;
         func->paramForms[3] = (TESForm*)spot;
         func->paramForms[4] = (TESForm*)spotShadow;
         //
         if (func->Queue())
            return true;
         registry->LogError("Unable to queue " PapyrusPrefixString("RevealService") " operation (SetLightModels); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
         delete func;
         return false;
      };
      bool SetLightRevealState(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         bool state
      ) {
         auto func = new RevealServiceFunctor(stackId);
         func->operation = RevealServiceFunctor::Operation::kFunctorOperation_SetLightRevealState;
         func->paramBool = state;
         //
         if (func->Queue())
            return true;
         registry->LogError("Unable to queue " PapyrusPrefixString("RevealService") " operation (SetLightRevealState); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
         delete func;
         return false;
      };
      bool GetLightRevealState(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         auto func = new RevealServiceFunctor(stackId);
         func->operation = RevealServiceFunctor::Operation::kFunctorOperation_GetLightRevealState;
         //
         if (func->Queue())
            return true;
         registry->LogError("Unable to queue " PapyrusPrefixString("RevealService") " operation (GetLightRevealState); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
         delete func;
         return false;
      };
      bool GetMarkerRevealState(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         auto func = new RevealServiceFunctor(stackId);
         func->operation = RevealServiceFunctor::Operation::kFunctorOperation_GetMarkerRevealState;
         //
         if (func->Queue())
            return true;
         registry->LogError("Unable to queue " PapyrusPrefixString("RevealService") " operation (GetMarkerRevealState); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
         delete func;
         return false;
      };
      bool SetMarkerRevealState(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         bool state
      ) {
         auto func = new RevealServiceFunctor(stackId);
         func->operation = RevealServiceFunctor::Operation::kFunctorOperation_SetMarkerRevealState;
         func->paramBool = state;
         //
         if (func->Queue())
            return true;
         registry->LogError("Unable to queue " PapyrusPrefixString("RevealService") " operation (SetMarkerRevealState); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
         delete func;
         return false;
      };
      bool SetAllRevealStates(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         bool state
      ) {
         auto func = new RevealServiceFunctor(stackId);
         func->operation = RevealServiceFunctor::Operation::kFunctorOperation_SetAllRevealStates;
         func->paramBool = state;
         //
         if (func->Queue())
            return true;
         registry->LogError("Unable to queue " PapyrusPrefixString("RevealService") " operation (SetAllRevealStates); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
         delete func;
         return false;
      };
      bool RegisterMarker(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         TESForm* baseForm, BSFixedString revealPath, VMArray<SInt32> boundsArr
      ) {
         auto func = new RevealServiceFunctor(stackId);
         func->operation = RevealServiceFunctor::Operation::kFunctorOperation_RegisterMarkers;
         func->paramRegistrations.resize(1);
         auto& entry = func->paramRegistrations[0];
         entry.form = baseForm;
         entry.pathReveal = revealPath.data;
         SInt32 i;
         boundsArr.Get(&i, 0);
         entry.bounds.min.x = i;
         boundsArr.Get(&i, 1);
         entry.bounds.min.y = i;
         boundsArr.Get(&i, 2);
         entry.bounds.min.z = i;
         boundsArr.Get(&i, 3);
         entry.bounds.max.x = i;
         boundsArr.Get(&i, 4);
         entry.bounds.max.y = i;
         boundsArr.Get(&i, 5);
         entry.bounds.max.z = i;
         //
         if (func->Queue())
            return true;
         registry->LogError("Unable to queue " PapyrusPrefixString("RevealService") " operation (RegisterMarker); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
         delete func;
         return false;
      };
      bool RegisterMarkersInBulk(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<TESForm*> forms, VMArray<BSFixedString> paths, VMArray<SInt32> boundsArr
      ) {
         UInt32 count = forms.Length();
         if (count != paths.Length()) {
            registry->LogError("The number of forms and the number of paths must be equal; they are not.", stackId);
            return false;
         }
         if (count * 6 > boundsArr.Length()) {
            registry->LogError("The bounds array is the wrong length; you must specify six ints per form.", stackId);
            return false;
         }
         auto func = new RevealServiceFunctor(stackId);
         func->operation = RevealServiceFunctor::Operation::kFunctorOperation_RegisterMarkers;
         func->paramRegistrations.resize(count);
         for (UInt32 i = 0; i < count; i++) {
            auto& entry = func->paramRegistrations[i];
            forms.Get(&entry.form, i);
            BSFixedString path;
            paths.Get(&path, i);
            entry.pathReveal = path.data;
            SInt32 j;
            boundsArr.Get(&j, 0);
            entry.bounds.min.x = j;
            boundsArr.Get(&j, 1);
            entry.bounds.min.y = j;
            boundsArr.Get(&j, 2);
            entry.bounds.min.z = j;
            boundsArr.Get(&j, 3);
            entry.bounds.max.x = j;
            boundsArr.Get(&j, 4);
            entry.bounds.max.y = j;
            boundsArr.Get(&j, 5);
            entry.bounds.max.z = j;
         }
         //
         if (func->Queue())
            return true;
         registry->LogError("Unable to queue " PapyrusPrefixString("RevealService") " operation (RegisterMarkersInBulk); too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
         delete func;
         return false;
      };
   }
};

bool PapyrusPrefix(Papyrus)::RevealService::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<RevealServiceFunctor>();
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction5<StaticFunctionTag, void, TESObjectACTI*, TESObjectACTI*, TESObjectACTI*, TESObjectACTI*, TESObjectACTI*>(
         "SetLightModels",
         PapyrusPrefixString("RevealService"),
         RevealService::SetLightModels,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, void, bool>(
         "SetLightRevealState",
         PapyrusPrefixString("RevealService"),
         RevealService::SetLightRevealState,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction0<StaticFunctionTag, bool>(
         "GetLightRevealState",
         PapyrusPrefixString("RevealService"),
         RevealService::GetLightRevealState,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, void, bool>(
         "SetMarkerRevealState",
         PapyrusPrefixString("RevealService"),
         RevealService::SetMarkerRevealState,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction0<StaticFunctionTag, bool>(
         "GetMarkerRevealState",
         PapyrusPrefixString("RevealService"),
         RevealService::GetMarkerRevealState,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, void, bool>(
         "SetAllRevealStates",
         PapyrusPrefixString("RevealService"),
         RevealService::SetAllRevealStates,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction3<StaticFunctionTag, void, TESForm*, BSFixedString, VMArray<SInt32>>(
         "RegisterMarker",
         PapyrusPrefixString("RevealService"),
         RevealService::RegisterMarker,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction3<StaticFunctionTag, void, VMArray<TESForm*>, VMArray<BSFixedString>, VMArray<SInt32>>(
         "RegisterMarkersInBulk",
         PapyrusPrefixString("RevealService"),
         RevealService::RegisterMarkersInBulk,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};