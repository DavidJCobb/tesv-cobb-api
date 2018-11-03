#pragma once
#include "Papyrus/Light.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectLIGH.h"

namespace CobbPapyrus {
   namespace Light {
      namespace Setters {
         void SetFade(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* pLightRef, float fNewValue, bool bRelative = true) {
            //
            // Sets the fade. Note that changes won't take effect until the light unloads and reloads 3D (e.g. if you move 
            // the light to itself).
            //
            if (!pLightRef)
               return;
            //
            RE::ExtraLightData* extraLightData = (RE::ExtraLightData*)(pLightRef->extraData.GetByType(kExtraData_LightData));
            if (extraLightData) {
               if (!bRelative) {
                  RE::TESObjectLIGH* baseForm = (RE::TESObjectLIGH*)(pLightRef->baseForm);
                  if (baseForm)
                     fNewValue -= baseForm->fade;
                  else
                     return;
               }
               extraLightData->modifiedFade = fNewValue;
            }
         }
      }
      float GetFOV      (VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* pLightRef, bool bRelative = false) {
         //
         // If <bRelative> is false, returns the absolute FOV. Otherwise, returns (reference FOV - base form FOV).
         //
         ERROR_AND_RETURN_0_IF(pLightRef == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
         RE::ExtraLightData* extraLightData = (RE::ExtraLightData*)(pLightRef->extraData.GetByType(kExtraData_LightData));
         if (extraLightData) {
            float result = extraLightData->modifiedFOV;
            if (bRelative)
               return result;
            //
            // Get absolute value.
            //
            RE::TESObjectLIGH* baseForm = (RE::TESObjectLIGH*)(pLightRef->baseForm);
            if (baseForm)
               return result + baseForm->unk78.baseFOV;
            else
               return -1;
         }
         return 0.0;
      }
      float GetDepthBias(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* pLightRef) {
         //
         // Testing has shown that this is the Shadow Depth Bias defined on the reference.
         //
         ERROR_AND_RETURN_0_IF(pLightRef == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
         RE::ExtraLightData* extraLightData = (RE::ExtraLightData*)(pLightRef->extraData.GetByType(kExtraData_LightData));
         if (extraLightData)
            return extraLightData->depthBias;
         return 0.0;
      }
      float GetRadius   (VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* pLightRef, bool bRelative = false) {
         //
         // If <bRelative> is false, returns the absolute radius. Otherwise, returns (reference radius - base form radius).
         //
         ERROR_AND_RETURN_0_IF(pLightRef == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
         RE::ExtraRadius* data = (RE::ExtraRadius*)(pLightRef->extraData.GetByType(kExtraData_Radius));
         if (data) {
            float result = data->radiusModifier;
            if (bRelative)
               return result;
            //
            // Get absolute value.
            //
            RE::TESObjectLIGH* baseForm = (RE::TESObjectLIGH*)(pLightRef->baseForm);
            if (baseForm)
               return result + baseForm->unk78.radius;
            else
               return -1;
         } else if (!bRelative) {
            RE::TESObjectLIGH* baseForm = (RE::TESObjectLIGH*)(pLightRef->baseForm);
            if (baseForm)
               return baseForm->unk78.radius;
         }
         return 0;
      }
      float GetFade     (VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* pLightRef, bool bRelative = false) {
         //
         // If <bRelative> is false, returns the absolute Fade. Otherwise, returns (reference fade - base form fade).
         //
         ERROR_AND_RETURN_0_IF(pLightRef == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
         RE::ExtraLightData* extraLightData = (RE::ExtraLightData*)(pLightRef->extraData.GetByType(kExtraData_LightData));
         if (extraLightData) {
            float result = extraLightData->modifiedFade;
            if (bRelative)
               return result;
            //
            // Get absolute value.
            //
            RE::TESObjectLIGH* baseForm = (RE::TESObjectLIGH*)(pLightRef->baseForm);
            if (baseForm)
               return result + baseForm->fade;
            else
               return -1;
         }
         return 0;
      }
      bool GetLightsLandscape(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
         auto base = subject->baseForm;
         ERROR_AND_RETURN_0_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
         auto light = DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
         ERROR_AND_RETURN_0_IF(base == nullptr, "The reference is not a light.", registry, stackId);
         return !(subject->flags & RE::TESForm::kRefrFlag_DontLightLandscape);
      };
      bool GetLightsWater(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
         auto base = subject->baseForm;
         ERROR_AND_RETURN_0_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
         auto light = DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
         ERROR_AND_RETURN_0_IF(base == nullptr, "The reference is not a light.", registry, stackId);
         return !(subject->flags & RE::TESForm::kRefrFlag_DontLightWater);
      };
   }
}

bool CobbPapyrus::Light::Register(VMClassRegistry* registry) {
   //
   // SETTER FUNCTIONS
   //
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, void, TESObjectREFR*, float, bool>(
         "SetFade",
         PapyrusPrefixString("Light"),
         Light::Setters::SetFade,
         registry
      )
   );
   //
   // OTHER FUNCTIONS
   //
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, float, TESObjectREFR*, bool>(
         "GetFOV",
         PapyrusPrefixString("Light"),
         Light::GetFOV,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, float, TESObjectREFR*>(
         "GetDepthBias",
         PapyrusPrefixString("Light"),
         Light::GetDepthBias,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, float, TESObjectREFR*, bool>(
         "GetRadius",
         PapyrusPrefixString("Light"),
         Light::GetRadius,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, float, TESObjectREFR*, bool>(
         "GetFade",
         PapyrusPrefixString("Light"),
         Light::GetFade,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>(
         "GetLightsLandscape",
         PapyrusPrefixString("Light"),
         Light::GetLightsLandscape,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>(
         "GetLightsWater",
         PapyrusPrefixString("Light"),
         Light::GetLightsWater,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};