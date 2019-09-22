#pragma once
#include "skse/SafeWrite.h"
#include "ReverseEngineered\NetImmerse\objects.h"
#include "ReverseEngineered\Forms\Actor.h"
#include "ReverseEngineered\Player\PlayerCharacter.h"

namespace CobbPatches {
   namespace Exploratory {
      namespace LocationalDamageResearch {
         namespace Hook006E57D0 {
            //
            // doesn't fire consistently for all hits; not a reliable hook
            //
            void _stdcall Inner(RE::TESObjectREFR* target, RE::NiAVObject* node, RE::Actor* attacker) {
               _MESSAGE("Hook006E57D0: %08X %08X %08X", target, node, attacker);
               if (!target || !node || !attacker)
                  return;
               auto base = target->baseForm;
               if (!base || base->formType != kFormType_Character)
                  return;
               _MESSAGE("Registered apparent attack by [ACHR:%08X] on [ACHR:%08X] hitting node \"%s\".", attacker->formID, target->formID, node->m_name);
            }
            __declspec(naked) void Outer() {
               _asm {
                  push eax; // protect node
                  push eax;
                  mov  eax, 0x004D7C90;
                  call eax;
                  add  esp, 4;
                  pop  ecx; // restore node
                  push eax; // protect ref
                  push esi;
                  push ecx;
                  push eax;
                  call Inner; // stdcall
                  pop  eax; // restore ref
                  mov  ecx, 0x006E5A28;
                  jmp  ecx;
               };
            }
            void Apply() {
               WriteRelJump(0x006E5A1F, (UInt32)&Outer);
               SafeWrite8(0x006E5A1F + 5, 0x90);
            }
         }
         namespace Wrap006E57D0 {
            //
            // doesn't fire consistently for all hits; not a reliable hook
            //
            void _stdcall Inner(RE::TESObjectREFR* target, RE::Actor* attacker) {
               _MESSAGE("Wrap006E57D0: %08X %08X", target, attacker);
               if (!target || !attacker)
                  return;
               _MESSAGE("006E57D0 was called on behalf of [ACHR:%08X] and yielded [ACHR:%08X]", attacker->formID, target->formID);
            }
            __declspec(naked) void Outer() {
               _asm {
                  mov  eax, 0x006E57D0;
                  call eax;
                  push eax; // protect
                  push esi;
                  push eax;
                  call Inner; // stdcall
                  pop  eax; // restore
                  mov  ecx, 0x006E7712;
                  jmp  ecx;
               };
            }
            void Apply() {
               WriteRelJump(0x006E770D, (UInt32)&Outer);
            }
         }
         void Apply() {
            Hook006E57D0::Apply();
            Wrap006E57D0::Apply();
         }
      }
   }
}