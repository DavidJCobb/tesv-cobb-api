#pragma once
#include "Papyrus/Furniture.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/Forms/BaseForms/TESFurniture.h"

namespace PapyrusPrefix(Papyrus) {
   namespace Furniture {
      SpellItem* GetAssociatedSpell(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESFurniture* furn) {
         ERROR_AND_RETURN_0_IF(furn == nullptr, "You must specify a Furniture base form.", registry, stackId);
         return ((RE::TESFurniture*)furn)->associatedSpell;
      };
      VMResultArray<SInt32> GetEnabledMarkers(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESFurniture* furn) {
         VMResultArray<SInt32> result;
         if (furn == nullptr) {
            registry->LogError("You must specify a Furniture base form.", stackId);
            return result;
         }
         UInt32 markersAndFlags = ((RE::TESFurniture*)furn)->flags;
         for (UInt32 i = 0; i < 24; i++) {
            UInt32 bit = markersAndFlags & (1 << i);
            if (bit != 0) {
               result.push_back(i);
            }
         }
         return result;
      };
      VMResultArray<UInt32> GetMarkerEntryPoints(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESFurniture* furn, UInt32 marker) {
         //
         // NOTE: This currently returns the list of entry points that aren't explicitly disabled. Just because 
         // an entry point is listed as "true" doesn't mean that it actually exists. We won't be able to ensure 
         // accuracy in this regard until we decipher the C++-side NIF data regarding furniture.
         //
         // In fact, you could specify a totally non-existent marker (e.g. Marker 23 on a bench with only three 
         // markers) and we'd just return [true, true, true, true, true] !
         //
         // Anywho, boolean indices are: front, behind, right, left, up.
         //
         VMResultArray<UInt32> result;
         result.resize(5, false);
         if (furn == nullptr) {
            registry->LogError("You must specify a Furniture base form.", stackId);
            return result;
         }
         result[0] = 1;
         result[1] = 1;
         result[2] = 1;
         result[3] = 1;
         result[4] = 1;
         tArray<RE::TESFurniture::ModifiedMarker> markers = ((RE::TESFurniture*)furn)->markers;
         //
         for (UInt32 i = 0; i < markers.count; i++) {
            if (markers[i].furnitureMarkerIndex == marker) {
               for (UInt32 j = 1; j < 6; j++) {
                  if (markers[i].disabledEntryPoints & (1 << j))
                     result[j - 1] = 0;
               }
               return result;
            }
         }
         return result;
      };
      BGSKeyword* GetMarkerKeyword(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESFurniture* furn, UInt32 marker) {
         //
         // NOTE: We don't validate the specified marker to ensure it actually exists, but we 
         // should just return None for non-existent markers.
         //
         ERROR_AND_RETURN_0_IF(furn == nullptr, "You must specify a Furniture base form.", registry, stackId);
         return ((RE::TESFurniture*)furn)->GetMarkerKeyword(marker);
      };
      SInt32 GetWorkbenchSkill(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESFurniture* furn) {
         //
         // Refer to the WorkbenchSkill enum for the meanings of returned values.
         //
         // TODO: Is there something in Papyrus that we can mimic or map this to?
         // TODO: Should we return a string instead?
         //
         ERROR_AND_RETURN_0_IF(furn == nullptr, "You must specify a Furniture base form.", registry, stackId);
         SInt32 result = ((RE::TESFurniture*)furn)->workbench.skill;
         if (result == 0xFF)
            return -1;
         return result;
      };
      SInt32 GetWorkbenchType(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESFurniture* furn) {
         //
         // Refer to the WorkbenchSkill enum for the meanings of returned values.
         //
         // TODO: Is there something in Papyrus that we can mimic or map this to? Likely not...
         // TODO: Should we return a string instead?
         //
         ERROR_AND_RETURN_0_IF(furn == nullptr, "You must specify a Furniture base form.", registry, stackId);
         SInt32 result = ((RE::TESFurniture*)furn)->workbench.type;
         if (result == 0x00)
            return -1;
         return result;
      };
      //
      // Flags.
      //
      UInt32 GetMustExitToTalk(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESFurniture* furn) {
         ERROR_AND_RETURN_0_IF(furn == nullptr, "You must specify a Furniture base form.", registry, stackId);
         return (UInt32)((RE::TESFurniture*)furn)->flags & RE::TESFurniture::kFurnitureFlag_MustExitToTalk;
      }
   }
};

bool PapyrusPrefix(Papyrus)::Furniture::Register(VMClassRegistry* registry) {
   //
   // FUNCTIONS
   //
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, SpellItem*, TESFurniture*>(
         "GetAssociatedSpell",
         PapyrusPrefixString("Furniture"),
         Furniture::GetAssociatedSpell,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Furniture"), "GetAssociatedSpell", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<SInt32>, TESFurniture*>(
         "GetEnabledMarkers",
         PapyrusPrefixString("Furniture"),
         Furniture::GetEnabledMarkers,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Furniture"), "GetEnabledMarkers", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, VMResultArray<UInt32>, TESFurniture*, UInt32>(
         "GetMarkerEntryPoints",
         PapyrusPrefixString("Furniture"),
         Furniture::GetMarkerEntryPoints,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Furniture"), "GetMarkerEntryPoints", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, BGSKeyword*, TESFurniture*, UInt32>(
         "GetMarkerKeyword",
         PapyrusPrefixString("Furniture"),
         Furniture::GetMarkerKeyword,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Furniture"), "GetMarkerKeyword", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, SInt32, TESFurniture*>(
         "GetWorkbenchType",
         PapyrusPrefixString("Furniture"),
         Furniture::GetWorkbenchType,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Furniture"), "GetWorkbenchType", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, SInt32, TESFurniture*>(
         "GetWorkbenchSkill",
         PapyrusPrefixString("Furniture"),
         Furniture::GetWorkbenchSkill,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Furniture"), "GetWorkbenchSkill", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, UInt32, TESFurniture*>(
         "GetMustExitToTalk",
         PapyrusPrefixString("Furniture"),
         Furniture::GetMustExitToTalk,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Furniture"), "GetMustExitToTalk", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // DONE.
   //
   return true;
}