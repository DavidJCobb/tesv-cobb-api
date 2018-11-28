#include "LoadMoreEditorIDs.h"
#include "skse/SafeWrite.h"
#include "Services/ExtendedEditorIDService.h"

#include "ReverseEngineered/Forms/BGSAcousticSpace.h"
#include "ReverseEngineered/Forms/BGSLightingTemplate.h"
#include "ReverseEngineered/Forms/TESImageSpace.h"

namespace RE {
   class TESForm;
}
namespace CobbPatches {
   namespace LoadMoreEditorIDs {
      constexpr UInt32 ce_GetEditorID_VTBLIndex = 0x32;
      constexpr UInt32 ce_SetEditorID_VTBLIndex = 0x33;
      constexpr UInt32 ce_ptrSize = sizeof(void*);
      //
      constexpr UInt32 ce_targetVtbls[] = {
         RE::BGSAcousticSpace::kVTBL,
         RE::BGSLightingTemplate::kVTBL,
         RE::TESImageSpace::kVTBL,
      };
      //
      namespace Setter {
         void _stdcall Inner(const RE::TESForm* form, const char* editorID) {
            ExtendedEditorIDService::GetInstance().StoreEditorID(form, editorID);
         }
         __declspec(naked) void _stdcall Register(const char* editorID) {
            _asm {
               mov  eax, dword ptr [esp + 0x4];
               push eax;
               push ecx;
               call Inner; // stdcall
               mov  eax, 1;
               retn 4;
            }
         }
      }
      namespace Getter {
         const char* _stdcall Inner(const RE::TESForm* form) {
            auto& service = ExtendedEditorIDService::GetInstance();
            try {
               auto& s = service.GetEditorID(form);
               return s.c_str();
            } catch (std::out_of_range) {
               return "";
            }
         }
         __declspec(naked) const char* _stdcall Get() {
            _asm {
               push ecx;
               call Inner; // stdcall
               retn;
            }
         }
      }
      //
      void Apply() {
         //
         // Generally, a form will parse its EDID record even if it doesn't store its editor ID 
         // in memory; it will pass the parsed string to its virtual method 0x33 (SetEditorID), 
         // which will either store the editor ID (typically as extra-data) or just return true 
         // immediately. In the latter case, we can write over the VTBL entry to replace the no-
         // op with a function that will store the editor ID in our storage.
         //
         for (UInt32 i = 0; i < std::extent<decltype(ce_targetVtbls)>::value; i++) {
            SafeWrite32(ce_targetVtbls[i] + (ce_GetEditorID_VTBLIndex * ce_ptrSize), (UInt32)&Getter::Get);
            SafeWrite32(ce_targetVtbls[i] + (ce_SetEditorID_VTBLIndex * ce_ptrSize), (UInt32)&Setter::Register);
         }
      }
   }
}