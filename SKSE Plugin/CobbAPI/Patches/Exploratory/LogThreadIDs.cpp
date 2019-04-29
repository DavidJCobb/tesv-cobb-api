#include "LogThreadIDs.h"
#include "skse/SafeWrite.h"

namespace CobbPatches {
   namespace Exploratory {
      namespace LogThreadIDs {
         namespace ActorKillHook {
            void _stdcall Inner() {
               _MESSAGE("%08X == THREAD ID FOR Actor::Kill", GetCurrentThreadId());
            }
            __declspec(naked) void Outer() {
               _asm {
                  push ecx; // protect
                  call Inner; // stdcall
                  pop  ecx;
                  mov  eax, 0x006BD6C0; // reproduce patched-over call to TESObjectREFR::GetOrCreateRefHandle
                  call eax;             //
                  mov  ecx, 0x006AC3C8;
                  jmp  ecx;
               }
            }
            void Apply() {
               WriteRelJump(0x006AC3C3, (UInt32)&Outer);
            }
         }
         namespace InterceptActorValueChange {
            //
            // NOTES:
            //
            //  - This can run on a different thread from the main thread if run via the 
            //    console. However, I don't know if it actually does the thing, or if it 
            //    queues a BSTask, in those situations. It CAN queue a BSTask.
            //
            //     - Hm. The thread ID I observed when running from the console was the 
            //       same thread ID stored at Main::unk14.
            //
            void _stdcall Inner() {
               _MESSAGE("%08X == THREAD ID FOR Actor::InterceptActorValueChange", GetCurrentThreadId());
            }
            _declspec(naked) void Outer() {
               _asm {
                  call Inner; // stdcall
                  mov  ecx, 0x012E2BDC; // ActorValueList::GetSingleton()
                  mov  ecx, dword ptr [ecx]; // reproduce patched-over instruction
                  mov  eax, 0x006DFB5B;
                  jmp  eax;
               };
            }
            void Apply() {
               WriteRelJump(0x006DFB55, (UInt32)&Outer);
            }
         }
         namespace SuspectedMainThreadHook01 {
            //
            // NOTES:
            //
            //  - Only appears to run while the game is not paused.
            //
            //  - Appears to run on two different threads.
            //
            void _stdcall Inner() {
               _MESSAGE("%08X == THREAD ID FOR Main::Subroutine006910F0", GetCurrentThreadId());
            }
            __declspec(naked) void Outer() {
               _asm {
                  push ecx; // protect
                  call Inner; // stdcall
                  pop  ecx; // restore
                  mov  eax, 0x00515E30; // reproduce patched-over call to Main::ProcessReferenceStateQueue
                  call eax;             //
                  mov  ecx, 0x006910F5;
                  jmp  ecx;
               }
            }
            void Apply() {
               WriteRelJump(0x006910F0, (UInt32)&Outer);
            }
         }
         //
         void Apply() {
            ActorKillHook::Apply();
            InterceptActorValueChange::Apply();
            SuspectedMainThreadHook01::Apply();
         }
      }
   }
}