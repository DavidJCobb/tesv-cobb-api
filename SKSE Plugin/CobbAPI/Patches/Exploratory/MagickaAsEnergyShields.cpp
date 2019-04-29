#include "MagickaAsEnergyShields.h"
#include "ReverseEngineered/Forms/Actor.h"
#include "skse/SafeWrite.h"

namespace CobbPatches {
   namespace Exploratory {
      namespace MagickaAsEnergyShields {
         //
         // Test patch: have all damage go through magicka first, and then health.
         //
         // Notes:
         //
         //  - Killmoves still only care about health. If you regenerate some magicka, 
         //    but still have low health, then you can still be killmoved. We'd need 
         //    to fix that in order to implement "energy shields" reliably.
         //
         //  - There is no "shield regen delay." Magicka constantly regenerates even 
         //    as you take damage.
         //
         void _stdcall Inner(RE::Actor* target, float* result) {
            float currentHP = target->actorValueOwner.GetCurrent(0x18);
            float currentMG = target->actorValueOwner.GetCurrent(0x19);
            if (currentMG + *result >= 0.0F) {
               target->actorValueOwner.ModifyModifier(RE::kAVModifier_Damage, 0x19, *result);
               *result = 0.0F;
               return;
            }
            //
            // Code below does shield bleedthrough. If we don't want bleedthrough, then 
            // after setting magicka to zero, we'd need to return zero if magicka is zero, 
            // and return (result) otherwise. (Actually, magicka always regens. If we don't 
            // patch in a shield regen delay, then we'd need to return (result) if magicka 
            // is below some threshold; otherwise, it'll very rarely be at zero, so actors 
            // will be nigh immortal.)
            //
            // So: Say we have 50 magicka and are losing 70 health. 50 + -70 = -20, so we'd 
            // return -20 for bleedthrough.
            //
            float healthMod = currentMG + *result;
            target->actorValueOwner.ModifyModifier(RE::kAVModifier_Damage, 0x19, -currentMG);
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
            WriteRelJump(0x006DFB55, (UInt32)&Outer);
         };
      }
   }
}