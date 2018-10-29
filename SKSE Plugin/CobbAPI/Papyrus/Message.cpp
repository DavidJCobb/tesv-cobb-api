#include "Papyrus/Message.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h"
#include "ReverseEngineered/Types.h"

namespace PapyrusPrefix(Papyrus) {
   namespace Message {
      VMResultArray<BSFixedString> GetButtonLabels(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSMessage* message) {
         VMResultArray<BSFixedString> result;
         if (message == nullptr) {
            registry->LogError("Cannot get labels for a None message.", stackId);
            return result;
         }
         auto list = (RE::tList<BSFixedString>*) &message->buttons;
         for (auto item = &list->items; item; item = item->next) {
            if (item->data->data)
               result.push_back(*item->data);
         }
         return result;
      };
      bool WillCrashDueToLength(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSMessage* message) {
         ERROR_AND_RETURN_0_IF(message == nullptr, "Cannot check safety for a None message.", registry, stackId);
         return message->GetDescription().size() > 1023;
      };
   }
}

bool PapyrusPrefix(Papyrus)::Message::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<BSFixedString>, BGSMessage*>(
         "GetButtonLabels",
         PapyrusPrefixString("Message"),
         Message::GetButtonLabels,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Message"), "GetButtonLabels", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, BGSMessage*>(
         "WillCrashDueToLength",
         PapyrusPrefixString("Message"),
         Message::WillCrashDueToLength,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Message"), "WillCrashDueToLength", VMClassRegistry::kFunctionFlag_NoWait);
   //
   return true;
};