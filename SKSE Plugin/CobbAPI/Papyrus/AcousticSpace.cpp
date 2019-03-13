#pragma once
#include "Papyrus/AcousticSpace.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/Forms/TESImageSpace.h"
#include "ReverseEngineered/Systems/GameData.h"

namespace CobbPapyrus {
   namespace AcousticSpace {
      VMResultArray<TESForm*> GetAll(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         VMResultArray<TESForm*> items;
         auto  data = RE::TESDataHandler::GetSingleton();
         auto& list = data->acousticSpaces;
         auto  size = list.count;
         for (UInt32 i = 0; i < size; i++)
            items.push_back((TESForm*)list[i]);
         return items;
      }
   }
};

bool CobbPapyrus::AcousticSpace::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(new NativeFunction0<StaticFunctionTag, VMResultArray<TESForm*>>(
      "GetAllForms",
      PapyrusPrefixString("AcousticSpace"),
      GetAll,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("AcousticSpace"), "GetAllForms", VMClassRegistry::kFunctionFlag_NoWait);
   return true;
}