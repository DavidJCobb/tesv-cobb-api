#pragma once
#include "Papyrus/LightingTemplate.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/Forms/BGSLightingTemplate.h"
#include "ReverseEngineered/Systems/GameData.h"

namespace CobbPapyrus {
   namespace LightingTemplate {
      VMResultArray<TESForm*> GetAll(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         VMResultArray<TESForm*> items;
         auto  data = RE::TESDataHandler::GetSingleton();
         auto& list = data->lightingTemplates;
         auto  size = list.count;
         for (UInt32 i = 0; i < size; i++)
            items.push_back((TESForm*) list[i]);
         return items;
      }
   }
};

bool CobbPapyrus::LightingTemplate::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(new NativeFunction0<StaticFunctionTag, VMResultArray<TESForm*>>(
      "GetAllForms",
      PapyrusPrefixString("LightingTemplate"),
      GetAll,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("LightingTemplate"), "GetAllForms", VMClassRegistry::kFunctionFlag_NoWait);
   return true;
}