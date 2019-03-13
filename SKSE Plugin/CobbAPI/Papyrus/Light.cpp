#pragma once
#include "Papyrus/Light.h"

#include "Papyrus/_Prefixes.h"

#include "skse/GameRTTI.h"
#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectLIGH.h"

namespace CobbPapyrus {
   namespace Light {
      namespace Getters {
         //
         // For functions that have a <bRelative> argument: if the argument is false, the function returns the absolute 
         // value. Otherwise, it returns (absolute value - base form value), which is the value that is actually stored 
         // on the reference.
         //
         float FOV(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, bool bRelative = false) {
            ERROR_AND_RETURN_0_IF(subject == NULL, "Cannot act on a None reference.", registry, stackId);
            auto base = subject->baseForm;
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
            auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference is not a light.", registry, stackId);
            float result = CALL_MEMBER_FN((RE::BaseExtraList*)(&subject->extraData), GetExtraLightFOVModifier)();
            if (bRelative)
               return result;
            return result + light->unk78.baseFOV;
         }
         float ShadowDepthBias(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            ERROR_AND_RETURN_0_IF(subject == NULL, "Cannot act on a None reference.", registry, stackId);
            auto base = subject->baseForm;
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
            auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference is not a light.", registry, stackId);
            RE::ExtraLightData* data = (RE::ExtraLightData*)(subject->extraData.GetByType(kExtraData_LightData));
            if (data == nullptr)
               return 1.0; // constructor-level default
            return data->depthBias;
         }
         float Radius(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, bool bRelative = false) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
            auto base = subject->baseForm;
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
            auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference is not a light.", registry, stackId);
            RE::ExtraRadius* data = (RE::ExtraRadius*)(subject->extraData.GetByType(kExtraData_Radius));
            if (data) {
               float result = data->radiusModifier;
               if (bRelative)
                  return result;
               return result + light->unk78.radius;
            } else if (!bRelative) {
               return light->unk78.radius;
            }
            return 0.0;
         }
         float Fade(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, bool bRelative = false) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
            auto base = subject->baseForm;
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
            auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference is not a light.", registry, stackId);
            float result = 0.0;
            {
               RE::ExtraLightData* data = (RE::ExtraLightData*)(subject->extraData.GetByType(kExtraData_LightData));
               if (data != nullptr)
                  result = data->modifiedFade;
            }
            if (bRelative)
               return result;
            return result + light->fade;
         }
         bool LightsLandscape(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
            auto base = subject->baseForm;
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
            auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference is not a light.", registry, stackId);
            return !(subject->flags & RE::TESForm::kRefrFlag_DontLightLandscape);
         }
         bool LightsWater(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check lighting data on a None object.", registry, stackId);
            auto base = subject->baseForm;
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
            auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
            ERROR_AND_RETURN_0_IF(base == nullptr, "The reference is not a light.", registry, stackId);
            return !(subject->flags & RE::TESForm::kRefrFlag_DontLightWater);
         }
      }
      namespace Setters {
         void Fade(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, float fNewValue, bool bRelative = true, bool bPersist = false) {
            //
            // Sets the fade. Note that changes won't take effect until the light unloads and reloads 3D (e.g. if you move 
            // the light to itself).
            //
            ERROR_AND_RETURN_IF(subject == nullptr, "Cannot act on a None object.", registry, stackId);
            auto base = subject->baseForm;
            ERROR_AND_RETURN_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
            auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
            ERROR_AND_RETURN_IF(base == nullptr, "The reference is not a light.", registry, stackId);
            RE::ExtraLightData* extraLightData = (RE::ExtraLightData*)(subject->extraData.GetByType(kExtraData_LightData));
            if (extraLightData) {
               if (!bRelative)
                  fNewValue -= light->fade;
               extraLightData->modifiedFade = fNewValue;
            }
         }
         void Radius(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, float value, bool bRelative = true, bool bPersist = false) {
            ERROR_AND_RETURN_IF(subject == nullptr, "Cannot act on a None object.", registry, stackId);
            auto base = subject->baseForm;
            ERROR_AND_RETURN_IF(base == nullptr, "The reference does not have a base form.", registry, stackId);
            auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(base, TESForm, TESObjectLIGH);
            ERROR_AND_RETURN_IF(base == nullptr, "The reference is not a light.", registry, stackId);
            if (!bRelative)
               value -= light->unk78.radius;
            CALL_MEMBER_FN((RE::BaseExtraList*)(&subject->extraData), SetExtraRadius)(value);
         }
      }
   }
}

bool CobbPapyrus::Light::Register(VMClassRegistry* registry) {
   {  // Setters
      registry->RegisterFunction(new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, float, bool, bool>(
         "SetFade",
         PapyrusPrefixString("Light"),
         Light::Setters::Fade,
         registry
      ));
      registry->RegisterFunction(new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, float, bool, bool>(
         "SetRadius",
         PapyrusPrefixString("Light"),
         Light::Setters::Radius,
         registry
      ));
   }
   {  // Getters
      registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, float, TESObjectREFR*, bool>(
         "GetFOV",
         PapyrusPrefixString("Light"),
         Light::Getters::FOV,
         registry
      ));
      registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, float, TESObjectREFR*>(
         "GetDepthBias",
         PapyrusPrefixString("Light"),
         Light::Getters::ShadowDepthBias,
         registry
      ));
      registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, float, TESObjectREFR*, bool>(
         "GetRadius",
         PapyrusPrefixString("Light"),
         Light::Getters::Radius,
         registry
      ));
      registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, float, TESObjectREFR*, bool>(
         "GetFade",
         PapyrusPrefixString("Light"),
         Light::Getters::Fade,
         registry
      ));
      registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>(
         "GetLightsLandscape",
         PapyrusPrefixString("Light"),
         Light::Getters::LightsLandscape,
         registry
      ));
      registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>(
         "GetLightsWater",
         PapyrusPrefixString("Light"),
         Light::Getters::LightsWater,
         registry
      ));
   }
   //
   return true;
};