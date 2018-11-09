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
            mov  edx, dword ptr [ebp + 0x8]; // edx = Arg1;
            mov  eax, dword ptr [ebp + 0xC]; // eax = Arg2;
            push eax;
            push edx;
            call Inner;
            add  esp, 8;
            test al, al;
            jnz  lAbort;
            mov  eax, 0x01B11820;      // reproduce patched-over instruction piecemeal // eax = GMST:fSneakBaseValue;
            mov  eax, dword ptr [eax]; // reproduce patched-over instruction piecemeal
            mov  ecx, 0x0059897E;
            jmp  ecx;
         lAbort:
            mov  eax, -0x3E8; // -1000
            mov  ecx, 0x00598DA8;
            jmp  ecx;
         };
      };
      void Apply() {
         //
         // TODO: Do not apply if ESODeath is detected.
         //
         WriteRelJump(0x00598979, (UInt32)&Outer);
      };
   };
};