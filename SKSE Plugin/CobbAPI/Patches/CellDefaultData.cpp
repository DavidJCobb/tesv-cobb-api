#include "CellDefaultData.h"

#include "Shared.h"
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "Services/CellInteriorDataService.h"

#include "skse/PluginAPI.h"
#include "skse/SafeWrite.h"

namespace CobbPatches {
   namespace CellDefaultData {
      void _stdcall Inner(RE::TESObjectCELL* subject) {
         if (!subject)
            return;
         CellInteriorDataService::GetInstance().StoreDefaults(subject->formID, subject);
      };
      __declspec(naked) void Outer() {
         __asm {
            mov  eax, 0x004507F0; // TESForm::ModifyFlag00000008 // reproduce patched-over instruction
            call eax; // reproduce patched-over instruction
            push ebx;
            call Inner; // stdcall
            mov  eax, 0x004C4B04;
            jmp  eax;
         };
      };
      void Apply() {
         WriteRelJump(0x004C4AFF, (UInt32)&Outer);
      };
   };
};