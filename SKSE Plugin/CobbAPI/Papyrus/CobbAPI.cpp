#include "Papyrus/CobbAPI.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "Shared.h"

namespace CobbPapyrus {
   namespace CobbAPI {
      SInt32 GetVersion(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         return g_pluginVersion;
      };
   }
}

bool CobbPapyrus::CobbAPI::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(new NativeFunction0<StaticFunctionTag, SInt32>(
      "GetVersion",
      "CobbAPI",
      CobbPapyrus::CobbAPI::GetVersion,
      registry
   ));
   registry->SetFunctionFlags("GetVersion", "GetVersion", VMClassRegistry::kFunctionFlag_NoWait);
   //
   return true;
};