#include "OnLeveledActorRegenerated.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "skse/SafeWrite.h"

#include "Services/DetectionIntercept.h"

namespace CobbPatches {
   namespace OnLeveledActorRegenerated {
      void _stdcall Handler(RE::TESObjectREFR* actor) {
         if (!actor)
            return;
         UInt32 formID = actor->formID;
         //_MESSAGE("CobbAPI hooks detected the respawn of leveledactor %08X.", formID);
         //
         DetectionInterceptService::GetInstance().OnFormDestroyed(formID);
      };
      __declspec(naked) void Outer() {
         _asm {
            mov  eax, 0x004539F0;
            call eax; // reproduce patched-over call
            push esi;
            call Handler; // stdcall
            mov  eax, 0x0044E418;
            jmp  eax;
         };
      };
      //
      void Apply() {
         WriteRelJump(0x0044E413, (UInt32)&Outer);
      };
   };
};