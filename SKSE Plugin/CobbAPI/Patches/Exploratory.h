#pragma once
#include "ReverseEngineered/Forms/Actor.h"
#include "skse/SafeWrite.h"

namespace CobbPatches {
   namespace Exploratory {
      //
      // Test patch: have all damage go through magicka first, and then health.
      //
      // Notes:
      //
      //  - Killmoves still only care about health. If you regenerate some magicka, 
      //    but still have low health, then you can still be killmoved. We'd need 
      //    to fix that in order to implement "energy shields" reliably.
      //
      void _stdcall Inner(RE::Actor* target, float* result) {
         float currentHP = target->actorValueOwner.GetCurrent(0x18);
         float currentMG = target->actorValueOwner.GetCurrent(0x19);
         if (currentMG + *result >= 0.0F) {
            target->actorValueOwner.Unk_06(2, 0x19, *result);
            *result = 0.0F;
            return;
         }
         //
         // Say we have 50 magicka and are losing 70 health: 50 + -70 = -20
         //
         float healthMod = currentMG + *result;
         target->actorValueOwner.Unk_06(2, 0x19, -currentMG);
         *result = healthMod;
      };
      _declspec(naked) void Outer() {
         _asm {
            cmp  edi, 0x18;
            jne  lExit;
            lea  eax, [esp + 0x8];
            push eax;
            push esi;
            call Inner; // stdcall
         lExit:
            mov  ecx, 0x012E2BDC; // ActorValueList::GetSingleton()
            mov  ecx, dword ptr [ecx]; // reproduce patched-over instruction
            mov  eax, 0x006DFB5B;
            jmp  eax;
         };
      };

      void Apply() {

         return;

         ///*
         _MESSAGE("\n\nWARNING: DEBUG EXPLORATORY PATCHES ARE BEING APPLIED\n\nDO NOT SHIP THIS IN THE FINAL BUILD\n\n");
         WriteRelJump(0x006DFB55, (UInt32)&Outer);
         //*/
      };
   };
};