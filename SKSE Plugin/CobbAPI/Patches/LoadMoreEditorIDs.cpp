#include "LoadMoreEditorIDs.h"
#include "skse/SafeWrite.h"
#include "Services/ExtendedEditorIDService.h"

#include "ReverseEngineered/Forms/BGSLightingTemplate.h"

namespace RE {
   class TESForm;
}
namespace CobbPatches {
   namespace LoadMoreEditorIDs {
      constexpr UInt32 ce_SetEditorID_VTBLIndex = 0x33;
      constexpr UInt32 ce_ptrSize = sizeof(void*);
      //
      void _stdcall _RegisterInner(const RE::TESForm* form, const char* editorID) {
         ExtendedEditorIDService::GetInstance().StoreEditorID(form, editorID);
      }
      __declspec(naked) void _stdcall _Register(const char* editorID) {
         _asm {
            mov  eax, dword ptr [esp + 0x4];
            push eax;
            push ecx;
            call _RegisterInner; // stdcall
            mov  eax, 1;
            retn 4;
         }
      }
      //
      void Apply() {
         SafeWrite32(RE::BGSLightingTemplate::kVTBL + (ce_SetEditorID_VTBLIndex * ce_ptrSize), (UInt32)&_Register);
      }
   }
}