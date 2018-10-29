#include "Papyrus/Shout.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h"
#include "skse/GameObjects.h"
#include "ReverseEngineered/Systems/GameData.h"

namespace PapyrusPrefix(Papyrus) {
   namespace Shout {
      TESShout* GetAnyShoutUsingSpell(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SpellItem* spell) {
         ERROR_AND_RETURN_0_IF(spell == nullptr, "Cannot search for a None spell.", registry, stackId);
         auto  data = RE::TESDataHandler::GetSingleton();
         auto& list = data->shouts;
         for (UInt32 i = 0; i < list.count; i++) {
            TESShout* current = list.arr.entries[i];
            if (!current)
               continue;
            for (UInt8 j = 0; j < 3; j++)
               if (current->words[j].spell == spell)
                  return current;
         }
         return nullptr;
      };
      VMResultArray<TESShout*> GetAllShoutsUsingSpell(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SpellItem* spell) {
         VMResultArray<TESShout*> result;
         if (spell == nullptr) {
            registry->LogError("Cannot search for a None spell.", stackId);
            return result;
         }
         auto  data = RE::TESDataHandler::GetSingleton();
         auto& list = data->shouts;
         for (UInt32 i = 0; i < list.count; i++) {
            TESShout* current = list.arr.entries[i];
            if (!current)
               continue;
            for (UInt8 j = 0; j < 3; j++) {
               if (current->words[j].spell == spell) {
                  result.push_back(current);
                  break;
               }
            }
         }
         return result;
      };
      TESShout* GetAnyShoutUsingWord(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESWordOfPower* word) {
         ERROR_AND_RETURN_0_IF(word == nullptr, "Cannot search for a None word of power.", registry, stackId);
         auto  data = RE::TESDataHandler::GetSingleton();
         auto& list = data->shouts;
         for (UInt32 i = 0; i < list.count; i++) {
            TESShout* current = list.arr.entries[i];
            if (!current)
               continue;
            for (UInt8 j = 0; j < 3; j++)
               if (current->words[j].word == word)
                  return current;
         }
         return nullptr;
      };
      VMResultArray<TESShout*> GetAllShoutsUsingWord(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESWordOfPower* word) {
         VMResultArray<TESShout*> result;
         if (word == nullptr) {
            registry->LogError("Cannot search for a None word of power.", stackId);
            return result;
         }
         auto  data = RE::TESDataHandler::GetSingleton();
         auto& list = data->shouts;
         for (UInt32 i = 0; i < list.count; i++) {
            TESShout* current = list.arr.entries[i];
            if (!current)
               continue;
            for (UInt8 j = 0; j < 3; j++) {
               if (current->words[j].word == word) {
                  result.push_back(current);
                  break;
               }
            }
         }
         return result;
      };
      //
      VMResultArray<float> GetShoutRecoveryTimes(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESShout* shout) {
         VMResultArray<float> result;
         if (shout == nullptr) {
            registry->LogError("Cannot retrieve recovery times for a None shout.", stackId);
            return result;
         }
         result.reserve(3);
         result.push_back(shout->words[0].recoverytime);
         result.push_back(shout->words[1].recoverytime);
         result.push_back(shout->words[2].recoverytime);
         return result;
      };
      VMResultArray<SpellItem*> GetShoutSpells(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESShout* shout) {
         VMResultArray<SpellItem*> result;
         if (shout == nullptr) {
            registry->LogError("Cannot retrieve spells for a None shout.", stackId);
            return result;
         }
         result.reserve(3);
         result.push_back(shout->words[0].spell);
         result.push_back(shout->words[1].spell);
         result.push_back(shout->words[2].spell);
         return result;
      };
      VMResultArray<TESWordOfPower*> GetShoutWords(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESShout* shout) {
         VMResultArray<TESWordOfPower*> result;
         if (shout == nullptr) {
            registry->LogError("Cannot retrieve words for a None shout.", stackId);
            return result;
         }
         result.reserve(3);
         result.push_back(shout->words[0].word);
         result.push_back(shout->words[1].word);
         result.push_back(shout->words[2].word);
         return result;
      };
      TESObjectSTAT* GetShoutMenuDisplayObject(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESShout* shout) {
         ERROR_AND_RETURN_0_IF(shout == nullptr, "Cannot get the menu display object of a None shout.", registry, stackId);
         return shout->menuDispObject.worldStatic;
      };
   }
}

bool PapyrusPrefix(Papyrus)::Shout::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, TESShout*, SpellItem*>(
         "GetAnyShoutUsingSpell",
         PapyrusPrefixString("Shout"),
         Shout::GetAnyShoutUsingSpell,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Shout"), "GetAnyShoutUsingSpell", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<TESShout*>, SpellItem*>(
         "GetAllShoutsUsingSpell",
         PapyrusPrefixString("Shout"),
         Shout::GetAllShoutsUsingSpell,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Shout"), "GetAllShoutsUsingSpell", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, TESShout*, TESWordOfPower*>(
         "GetAnyShoutUsingWord",
         PapyrusPrefixString("Shout"),
         Shout::GetAnyShoutUsingWord,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Shout"), "GetAnyShoutUsingWord", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<TESShout*>, TESWordOfPower*>(
         "GetAllShoutsUsingWord",
         PapyrusPrefixString("Shout"),
         Shout::GetAllShoutsUsingWord,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Shout"), "GetAllShoutsUsingWord", VMClassRegistry::kFunctionFlag_NoWait);
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<float>, TESShout*>(
         "GetShoutRecoveryTimes",
         PapyrusPrefixString("Shout"),
         Shout::GetShoutRecoveryTimes,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Shout"), "GetShoutRecoveryTimes", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<SpellItem*>, TESShout*>(
         "GetShoutSpells",
         PapyrusPrefixString("Shout"),
         Shout::GetShoutSpells,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Shout"), "GetShoutSpells", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<TESWordOfPower*>, TESShout*>(
         "GetShoutWords",
         PapyrusPrefixString("Shout"),
         Shout::GetShoutWords,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Shout"), "GetShoutWords", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, TESObjectSTAT*, TESShout*>(
         "GetShoutMenuDisplayObject",
         PapyrusPrefixString("Shout"),
         Shout::GetShoutMenuDisplayObject,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Shout"), "GetShoutMenuDisplayObject", VMClassRegistry::kFunctionFlag_NoWait);
   //
   return true;
};