#include "Detection.h"
#include "skse/SafeWrite.h"
#include "Services/DetectionIntercept.h"

namespace RE {
   class Actor;
};
namespace CobbPatches {
   namespace Detection {
      bool Inner(RE::Actor* seeker, RE::Actor* target) {
         return DetectionInterceptService::GetInstance().ShouldCancelDetection(seeker, target);
      };
      __declspec(naked) void Outer() {
         _asm {
            push eax; // protect (the game loaded a GMST here)
            mov  edx, dword ptr [ebp + 0x8]; // edx = Arg1;
            mov  eax, dword ptr [ebp + 0xC]; // eax = Arg2;
            push eax;
            push edx;
            call Inner;
            add  esp, 8;
            test al, al;
            pop  eax; // restore
            jnz  lAbort;
            mov  edx, dword ptr [ebp + 0x8]; // reproduce patched-over instruction
            push ebx;      // reproduce patched-over instruction
            xor  ebx, ebx; // reproduce patched-over instruction
            mov  ecx, 0x00598984;
            jmp  ecx;
         lAbort:
            mov  eax, -0x3E8; // -1000
            mov  ecx, 0x00598DA7;
            jmp  ecx;
         };
      };
      void Apply() {
         if (*(UInt8*)(0x0059897E) == 0xE9) {
            //
            // Something else has patched our target site.
            //
            DetectionInterceptService::GetInstance().isActive = false;
            return;
         }
         WriteRelJump(0x0059897E, (UInt32)&Outer);
      };
   };
};