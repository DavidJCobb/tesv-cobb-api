#pragma once
#include "Papyrus/Alias.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/Objects/Alias.h"

class BGSRefAlias;
namespace PapyrusPrefix(Papyrus) {
   namespace Alias {
      BOOL CreatesReference(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSRefAlias* alias) {
         ERROR_AND_RETURN_0_IF(alias == nullptr, "You must specify a ReferenceAlias.", registry, stackId);
         return ((RE::BGSRefAlias*)alias)->unk14 == RE::BGSRefAlias::kFillType_CreatedReference;
      }
      SInt32 GetRefAliasCreateAtAliasID(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSRefAlias* alias) {
         ERROR_AND_RETURN_0_IF(alias == nullptr, "You must specify a ReferenceAlias.", registry, stackId);
         auto extended = (RE::BGSRefAlias*) alias;
         ERROR_AND_RETURN_0_IF(extended->unk14 != RE::BGSRefAlias::kFillType_CreatedReference, "The specified alias doesn't create a reference.", registry, stackId);
         return extended->unk1C.aliasID;
      }
      SInt32 GetRefAliasFillType(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSRefAlias* alias) {
         ERROR_AND_RETURN_0_IF(alias == nullptr, "You must specify a ReferenceAlias.", registry, stackId);
         return ((RE::BGSRefAlias*)alias)->unk14;
      };
      BOOL IsPlayerAlias(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSRefAlias* alias) {
         ERROR_AND_RETURN_0_IF(alias == nullptr, "You must specify a ReferenceAlias.", registry, stackId);
         return ((RE::BGSRefAlias*)alias)->IsPlayer();
      };
   }
};

bool PapyrusPrefix(Papyrus)::Alias::Register(VMClassRegistry* registry) {
   //
   // FUNCTIONS
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, BOOL, BGSRefAlias*>(
         "CreatesReference",
         PapyrusPrefixString("Alias"),
         Alias::CreatesReference,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Alias"), "CreatesReference", VMClassRegistry::kFunctionFlag_NoWait);
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, SInt32, BGSRefAlias*>(
         "GetRefAliasCreateAtAliasID",
         PapyrusPrefixString("Alias"),
         Alias::GetRefAliasCreateAtAliasID,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Alias"), "GetRefAliasCreateAtAliasID", VMClassRegistry::kFunctionFlag_NoWait);
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, SInt32, BGSRefAlias*>(
         "GetRefAliasFillType",
         PapyrusPrefixString("Alias"),
         Alias::GetRefAliasFillType,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Alias"), "GetRefAliasFillType", VMClassRegistry::kFunctionFlag_NoWait);
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, BOOL, BGSRefAlias*>(
         "IsPlayerAlias",
         PapyrusPrefixString("Alias"),
         Alias::IsPlayerAlias,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Alias"), "IsPlayerAlias", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // DONE.
   //
   #undef REGISTER_NOWAIT_GETTER
   return true;
}