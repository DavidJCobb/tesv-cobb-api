#pragma once
#include "Papyrus/ImageSpace.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/Forms/TESImageSpace.h"
#include "ReverseEngineered/Systems/GameData.h"

namespace CobbPapyrus {
   namespace ImageSpace {
      VMResultArray<TESForm*> GetAll(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         VMResultArray<TESForm*> items;
         auto  data = RE::TESDataHandler::GetSingleton();
         auto& list = data->imageSpaces;
         auto  size = list.count;
         for (UInt32 i = 0; i < size; i++)
            items.push_back((TESForm*)list[i]);
         return items;
      }
   }
};

bool CobbPapyrus::ImageSpace::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(new NativeFunction0<StaticFunctionTag, VMResultArray<TESForm*>>(
      "GetAllForms",
      PapyrusPrefixString("ImageSpace"),
      GetAll,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("ImageSpace"), "GetAllForms", VMClassRegistry::kFunctionFlag_NoWait);
   return true;
}