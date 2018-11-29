#include "Papyrus/Message.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Forms/TESForm.h"

#include "common/IMemPool.h"
#include "skse/GameThreads.h"

namespace CobbPapyrus {
   namespace Message {
      void _GetDesc(BGSMessage* message, RE::BSString& out) {
         auto d = (RE::BaseFormComponent*) &message->description;
         auto f = (RE::TESForm*) DYNAMIC_CAST(d, TESDescription, TESForm);
         CALL_MEMBER_FN(d, LoadSubrecordStringContent)(&out, f, _byteswap_ulong('DESC'));
         //
         // TESDescription has an override of LoadSubrecordStringContent that handles the 
         // dynamic cast and whatnot... I'll get to speccing it at some point.
      };
      //
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
      BSFixedString GetText(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSMessage* message) {
         ERROR_AND_RETURN_0_IF(message == nullptr, "Cannot get the text of a None message.", registry, stackId);
         RE::BSString description;
         _GetDesc(message, description);
         return description.m_data;
      };
      BSFixedString GetTitle(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSMessage* message) {
         ERROR_AND_RETURN_0_IF(message == nullptr, "Cannot get the title of a None message.", registry, stackId);
         return message->fullName.name.data;
      };
      bool WillCrashDueToLength(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSMessage* message) {
         ERROR_AND_RETURN_0_IF(message == nullptr, "Cannot check safety for a None message.", registry, stackId);
         RE::BSString description;
         _GetDesc(message, description);
         return description.m_dataLen > 1023;
      };
   }
}

bool CobbPapyrus::Message::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<BSFixedString>, BGSMessage*>(
         "GetButtonLabels",
         PapyrusPrefixString("Message"),
         Message::GetButtonLabels,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Message"), "GetButtonLabels", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BGSMessage*>(
      "GetText",
      PapyrusPrefixString("Message"),
      Message::GetText,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Message"), "GetText", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BGSMessage*>(
      "GetTitle",
      PapyrusPrefixString("Message"),
      Message::GetTitle,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Message"), "GetTitle", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, BGSMessage*>(
         "WillCrashDueToLength",
         PapyrusPrefixString("Message"),
         Message::WillCrashDueToLength,
         registry
      )
   );
   //registry->SetFunctionFlags(PapyrusPrefixString("Message"), "WillCrashDueToLength", VMClassRegistry::kFunctionFlag_NoWait);
   //
   return true;
};