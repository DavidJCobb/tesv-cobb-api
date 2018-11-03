#include "Papyrus/Utility.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h" // TESForm
#include "skse/GameObjects.h" // ActiveEffect

namespace CobbPapyrus {
   namespace Utility {
      template<typename T> bool ScriptObjectWrappedPointerExists(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, T* entity) {
         return entity != nullptr;
      };
   }
}

bool CobbPapyrus::Utility::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, TESForm*>(
         "FormExists",
         PapyrusPrefixString("Utility"),
         Utility::ScriptObjectWrappedPointerExists<TESForm>,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Utility"), "FormExists", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, ActiveEffect*>(
         "ActiveEffectExists",
         PapyrusPrefixString("Utility"),
         Utility::ScriptObjectWrappedPointerExists<ActiveEffect>,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Utility"), "ActiveEffectExists", VMClassRegistry::kFunctionFlag_NoWait);
   //
   return true;
};