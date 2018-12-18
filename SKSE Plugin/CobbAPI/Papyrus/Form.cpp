#include "Papyrus/Form.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h"
#include "skse/GameReferences.h"
#include "skse/GameRTTI.h"

#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/BaseForms/base.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectLIGH.h"
#include "Services/ExtendedEditorIDService.h"

namespace CobbPapyrus {
   namespace Form {
      VMResultArray<SInt32> GetBounds(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESForm* target) {
         VMResultArray<SInt32> result;
         result.resize(6, 0.0);
         //
         if (target == nullptr) {
            registry->LogError("You must specify a form.", stackId);
            return result;
         }
         if (target->formType == kFormType_Reference) {
            target = ((TESObjectREFR*) target)->baseForm;
            if (target == nullptr) {
               registry->LogError("The specified reference has no base form.", stackId);
               return result;
            }
         }
         //
         RE::TESBoundObject* baseForm = (RE::TESBoundObject*) DYNAMIC_CAST(target, TESForm, TESBoundObject);
         if (baseForm) {
            result[0] = baseForm->boundsMin.x;
            result[1] = baseForm->boundsMin.y;
            result[2] = baseForm->boundsMin.z;
            result[3] = baseForm->boundsMax.x;
            result[4] = baseForm->boundsMax.y;
            result[5] = baseForm->boundsMax.z;
         }
         return result;
      };
      VMResultArray<float> GetBoundsToHalfwidthOffsets(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESForm* form) {
         VMResultArray<float> result;
         if (form == nullptr) {
            registry->LogError("Invalid or None form specified.", stackId);
            return result;
         }
         RE::TESBoundObject* bounded = (RE::TESBoundObject*) DYNAMIC_CAST(form, TESForm, TESBoundObject);
         if (bounded == nullptr) {
            registry->LogError("Invalid form specified.", stackId);
            return result;
         }
         auto offset = [](SInt32 low, SInt32 high) { return ((high - low) / 2) + low; };
         result.resize(3);
         result[0] = offset(bounded->boundsMax.x, bounded->boundsMin.x);
         result[1] = offset(bounded->boundsMax.y, bounded->boundsMin.y);
         result[2] = offset(bounded->boundsMax.z, bounded->boundsMin.z);
         return result;
      };
      VMResultArray<float> GetBoundsToHalfwidths(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESForm* form) {
         VMResultArray<float> result;
         if (form == nullptr) {
            registry->LogError("Invalid or None form specified.", stackId);
            return result;
         }
         RE::TESBoundObject* bounded = (RE::TESBoundObject*) DYNAMIC_CAST(form, TESForm, TESBoundObject);
         if (bounded == nullptr) {
            registry->LogError("Invalid form specified.", stackId);
            return result;
         }
         result.resize(3);
         result[0] = (float)abs((SInt32)bounded->boundsMax.x - (SInt32)bounded->boundsMin.x) / 2.0;
         result[1] = (float)abs((SInt32)bounded->boundsMax.y - (SInt32)bounded->boundsMin.y) / 2.0;
         result[2] = (float)abs((SInt32)bounded->boundsMax.z - (SInt32)bounded->boundsMin.z) / 2.0;
         return result;
      };
      BSFixedString GetEditorID(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESForm* subject) {
         BSFixedString result = subject->GetEditorID();
         if (result.data && strlen(result.data))
            return result;
         std::string editorID;
         ExtendedEditorIDService::GetInstance().GetEditorID(subject, editorID);
         return editorID.c_str();
      }
      VMResultArray<BSFixedString> GetEditorIDs(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<RE::TESForm*> forms) {
         VMResultArray<BSFixedString> result;
         UInt32 size = forms.Length();
         result.reserve(size);
         std::string editorID;
         for (UInt32 i = 0; i < size; i++) {
            RE::TESForm* form;
            forms.Get(&form, i);
            if (form) {
               ExtendedEditorIDService::GetInstance().GetEditorID(form, editorID);
               result.push_back(editorID.c_str());
            } else {
               result.push_back("");
            }
         }
         return result;
      }
      bool IsDefiniteItem(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESForm* subject) {
         if (!subject)
            return false;
         UInt8 formType = subject->formType;
         if (formType == kFormType_Reference) {
            subject = ((TESObjectREFR*) subject)->baseForm;
            if (!subject)
               return false;
            formType = subject->formType;
         }
         if (formType < kFormType_ScrollItem || formType > kFormType_LeveledItem)
            return false;
         switch (formType) {
            case kFormType_ScrollItem:
            case kFormType_Armor:
            case kFormType_Book:
            case kFormType_Ingredient:
            case kFormType_Misc:
            case kFormType_Apparatus:
            case kFormType_Weapon:
            case kFormType_Ammo:
            case kFormType_Key:
            case kFormType_Potion:
            case kFormType_Note:
            case kFormType_SoulGem:
               return true;
            case kFormType_Light:
               {
                  RE::TESObjectLIGH* light = (RE::TESObjectLIGH*) DYNAMIC_CAST(subject, TESForm, TESObjectLIGH);
                  if (light) {
                     return !!(light->unk78.flags & RE::TESObjectLIGH::kFlag_Carryable);
                  }
               }
            default:
               return false;
         }
      };
   }
}

bool CobbPapyrus::Form::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<SInt32>, TESForm*>(
         "GetBounds",
         PapyrusPrefixString("Form"),
         Form::GetBounds,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Form"), "GetBounds", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<float>, TESForm*>(
         "GetBoundsToHalfwidths",
         PapyrusPrefixString("Form"),
         Form::GetBoundsToHalfwidths,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Form"), "GetBoundsToHalfwidths", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<float>, TESForm*>(
         "GetBoundsToHalfwidthOffsets",
         PapyrusPrefixString("Form"),
         Form::GetBoundsToHalfwidthOffsets,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Form"), "GetBoundsToHalfwidthOffsets", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, BSFixedString, RE::TESForm*>(
         "GetEditorID",
         PapyrusPrefixString("Form"),
         Form::GetEditorID,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Form"), "GetEditorID", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<BSFixedString>, VMArray<RE::TESForm*>>(
         "GetEditorIDs",
         PapyrusPrefixString("Form"),
         Form::GetEditorIDs,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Form"), "GetEditorIDs", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, bool, TESForm*>(
         "IsDefiniteItem",
         PapyrusPrefixString("Form"),
         Form::IsDefiniteItem,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Form"), "IsDefiniteItem", VMClassRegistry::kFunctionFlag_NoWait);
   //
   return true;
}