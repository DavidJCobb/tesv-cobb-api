#include "Papyrus/Form.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h"
#include "skse/GameReferences.h"
#include "skse/GameRTTI.h"

#include "ReverseEngineered/Forms/BaseForms/base.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectLIGH.h"

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
   //
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