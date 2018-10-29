#include "Papyrus/Worldspace.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h"
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Forms/TESWorldSpace.h"

namespace PapyrusPrefix(Papyrus) {
   namespace Worldspace {
      RE::TESWorldSpace* GetParentWorldspace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESWorldSpace* world) {
         ERROR_AND_RETURN_0_IF(world == nullptr, "Cannot retrieve the parent of a None worldspace.", registry, stackId);
         return world->parentWorld;
      };
      RE::TESWorldSpace* GetRootWorldspaceOrSelf(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESWorldSpace* world) {
         ERROR_AND_RETURN_0_IF(world == nullptr, "Cannot retrieve the root of a None worldspace.", registry, stackId);
         return world->GetRootWorldspace();
      };
      float GetWaterLevel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESWorldSpace* world) {
         ERROR_AND_RETURN_0_IF(world == nullptr, "Cannot get the water level of a None worldspace.", registry, stackId);
         return CALL_MEMBER_FN(world, GetWaterLevel)();
      };
   }
}

bool PapyrusPrefix(Papyrus)::Worldspace::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, RE::TESWorldSpace*, RE::TESWorldSpace*>(
         "GetParentWorldspace",
         PapyrusPrefixString("Worldspace"),
         Worldspace::GetParentWorldspace,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Worldspace"), "GetParentWorldspace", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, RE::TESWorldSpace*, RE::TESWorldSpace*>(
         "GetRootWorldspaceOrSelf",
         PapyrusPrefixString("Worldspace"),
         Worldspace::GetRootWorldspaceOrSelf,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Worldspace"), "GetRootWorldspaceOrSelf", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, float, RE::TESWorldSpace*>(
         "GetWaterLevel",
         PapyrusPrefixString("Worldspace"),
         Worldspace::GetWaterLevel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Worldspace"), "GetWaterLevel", VMClassRegistry::kFunctionFlag_NoWait);
   //
   return true;
};