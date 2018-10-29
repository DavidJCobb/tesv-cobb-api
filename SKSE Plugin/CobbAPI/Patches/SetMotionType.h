#pragma once
#include "Shared.h"

#include "skse/GameReferences.h"
#include "skse/NiNodes.h"
#include "skse/SafeWrite.h"

#pragma once

#include "ReverseEngineered/Forms/TESObjectREFR.h"

namespace CobbPatches {
   namespace SetMotionType {
      //
      // Band-aid fix to prevent SetMotionType from breaking certain references. Certain references fail to move properly 
      // if the following conditions are met:
      //
      //  - Their 3D models contain NiControllerManagers with/in-addition-to yet-to-be-determined characteristics
      //  - The reference has a ChangeForm with flag 4 (HAVOK_MOVE) set (to indicate that Keyframed physics have been set)
      //
      // As it happens, most of these models are Keyframed by default, and this allows for a band-aid fix as described 
      // below. Basically, we refuse to set the changeFlag for redundant SetMotionType(4) calls. A ~proper~ fix would 
      // involve locating and patching the code that actually chokes on the changeFlag and produces broken behavior, but 
      // locating that code has been extremely difficult.
      //
      static const UInt32 targetBase = 0x00909A10;

      namespace Hook1 {
         static const UInt32 entry = targetBase + 0x51;
         static const UInt32 exit  = targetBase + 0x58;

         /*bool Check(TESObjectREFR* reference, NiNode* node, UInt32 desiredMotionType) {
            //
            // We want to AVOID marking the form as Havok-changed if:
            //  - The form isn't Havok-changed yet (and thus we can see its default motion type)
            //  - We're setting it to motion type 4, but that's its current/default motion type
            //
            if (desiredMotionType != 4)
               return false;
            if (((RE::TESObjectREFR*)reference)->GetChangeFlags() & 4)
               //
               // TODO: This check runs to completion but sometimes CAUSES A CRASH further on down the line! Find out why!
               //       Doesn't make any bloody sense; nothing we're calling should actually be changing any state. Just 
               //       to clarify, returning false isn't the problem; simply retrieving the changeFlags causes a crash.
               //
               return false;
            {
               SInt8 currentType = ((RE::TESObjectREFR*)reference)->GetMotionType();
               if (currentType != 4)
                  return false;
            }
            return true; // Conditions above are met. Return true to signal that we need to skip the MarkChanged call.
         };*/
         UInt32 __declspec(naked) Check(TESObjectREFR* reference, NiNode* node, UInt32 desiredMotionType) {
            __asm {
               push ebx;
               push ecx;
               push edx;
               //
               mov  eax, [esp + 0x18]; //
               cmp  eax, 4;            // if (desiredMotionType != 4)
               jne  lReturnFalse;      //    return false;
               //
               mov  eax, [esp + 0x10];       //
               mov  eax, [eax + 0x0C];       // eax = reference->formID;
               mov  ecx, 0x01B2E38C;         //
               mov  ecx, [ecx];              // ecx = *(0x01B2E38C);
               jz   lCheckCurrentMotionType; // if (!ecx) return false;
               mov  ecx, [ecx + 0x3E8];      // ecx = ecx->unk3E8;
               sub  esp, 0x4;                // UInt32 outFlags;
               mov  ebx, esp;                //
               push eax;                     //
               push ebx;                     //
               mov  eax, 0x00676C20;         //
               call eax;                     // ecx->GetChangeFlags(&outFlags, eax);
               mov  eax, [ebx];              // eax = outFlags;
               add  esp, 0x4;                // delete outFlags;
               and  eax, 0x4;                // if (eax & 4)
               jnz  lReturnFalse;            //    return false;
               //
               lCheckCurrentMotionType:
               mov  ecx, [esp + 0x10];                //
               call RE::TESObjectREFR::GetMotionType; // eax = (RE::TESObjectREFR)reference->GetMotionType();
               cmp  al, 0x4;                          // if ((SInt8)eax != 0x4)
               jne  lReturnFalse;                     //    return false;
               //
               lReturnTrue:
               mov  eax, 1;
               jmp lReturn;
               lReturnFalse:
               mov  eax, 0;
               lReturn:
               pop  edx;
               pop  ecx;
               pop  ebx;
               retn;
            };
         };

         __declspec(naked) void Patch() {
            __asm {
               mov eax, 0;
               push edx; // motion type
               push ebx; // node
               push edi; // reference
               call Check;
               add esp, 0xC; // clean up after last non-member call
               //
               movzx eax, al;
               mov [esp + 0x10], eax; // Save our result in a stack-allocated var that I'm ~pretty~ sure won't be used by native code after this point.
               //
               push ecx; //
               push 0;   //
               push 1;   // Reproduce the instructions we patched over.
               push edx; //
               push ebx; //
               jmp [exit];
            };
         };

         void Apply() {
            SafeWrite8(entry + 0, 0x90); // NOP
            SafeWrite8(entry + 1, 0x90); // NOP
            SafeWrite8(entry + 2, 0x90); // NOP
            SafeWrite8(entry + 3, 0x90); // NOP
            SafeWrite8(entry + 4, 0x90); // NOP
            SafeWrite8(entry + 5, 0x90); // NOP
            SafeWrite8(entry + 6, 0x90); // NOP
            //
            WriteRelJump(entry, (UInt32)Patch);
         };
      };
      namespace Hook2 {
         static const UInt32 entry = targetBase + 0x5D;
         static const UInt32 exit  = targetBase + 0x62;

         __declspec(naked) void Patch() {
            //
            // If the earlier check indicated that this is a redundant set-call, then 
            // we skip the MarkChanged call. Otherwise, we run it.
            //
            // Disposable registers:
            //  - eax
            //  - ecx
            //  - edx
            //
            // Important  registers:
            //  - ebx (node)
            //  - ebp (node refcount address)
            //  - edi (reference)
            //
            __asm {
               mov eax, [esp + 0x24]; // Load the sentinel bool from earlier into eax. (stack offset used earlier + 0x14, due to a native non-member call not being cleaned up yet.)
               test eax, eax;
               jnz lEnd;
               push 4;                //
               mov eax, [edi];        //
               mov eax, [eax + 0x28]; //
               mov ecx, edi;          //
               call eax;              // edi->MarkChanged(4);
               lEnd:
               jmp [exit];
            };
         };

         void Apply() {
            SafeWrite8(entry + 0x0, 0x90); // NOP
            SafeWrite8(entry + 0x1, 0x90); // NOP
            SafeWrite8(entry + 0x2, 0x90); // NOP
            SafeWrite8(entry + 0x3, 0x90); // NOP
            SafeWrite8(entry + 0x4, 0x90); // NOP
            // The three bytes here are used to clean up after a prior non-member call.
            SafeWrite8(entry + 0x8, 0x90); // NOP
            SafeWrite8(entry + 0x9, 0x90); // NOP
            SafeWrite8(entry + 0xA, 0x90); // NOP
            SafeWrite8(entry + 0xB, 0x90); // NOP
            SafeWrite8(entry + 0xC, 0x90); // NOP
            SafeWrite8(entry + 0xD, 0x90); // NOP
            //
            WriteRelJump(entry, (UInt32)Patch);
         };
      };

      void Apply() {
         Hook1::Apply();
         Hook2::Apply();
      };
   };
};