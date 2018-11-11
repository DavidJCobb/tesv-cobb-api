#include "OnFormDestroyed.h"
#include "skse/SafeWrite.h"

namespace CobbPatches {
   namespace OnFormDestroyed {
      void _stdcall OnDestroy(UInt32 formID) {
         //
         // TODO
         //
         _MESSAGE("CobbAPI save/load hooks detected the deletion of form %08X.", formID);
      };

      namespace Unloading { // BGSSaveLoadManager::Subroutine006798B0
         namespace FormHasNoChanges {
            _declspec(naked) void Outer() {
               _asm {
                  mov  eax, 0x00676BC0; // ChangeFormManager::GetChangeData
                  call eax; // reproduce patched-over call
                  push eax; // protect
                  cmp  edi, 0xFF000000;
                  jb   lExit; // don't destroy data for non-created references, even if they have no vanilla-engine-supported changes
                  push edi;
                  call OnDestroy; // stdcall
               lExit:
                  pop  eax; // restore
                  mov  ecx, 0x0067995E;
                  jmp  ecx;
               };
            };
            void Apply() {
               WriteRelJump(0x00679959, (UInt32)&Outer);
            };
         };
         namespace FormRefusesToSave { // TESForm::Unk_0D returned false
            _declspec(naked) void Outer() {
               _asm {
                  mov  eax, 0x00686C00; // BGSSaveGameBuffer::~BGSSaveGameBuffer
                  call eax; // reproduce patched-over call
                  push edi;
                  call OnDestroy; // stdcall
                  mov  eax, 0x00679A1C;
                  jmp  eax;
               };
            };
            void Apply() {
               WriteRelJump(0x00679A17, (UInt32)&Outer);
            };
         };
         namespace FormDeletedDuringUnload { // TESForm::Unk_0D unset all remaining changeflags, so the form will not be saved
            //
            // TESForm::Unk_0D can tamper with the form's changeflags just before save, in order 
            // to avoid saving unnecessary data or to force certain data to persist. The caller 
            // will catch changes to the changeflags and reconcile with the ChangeFormManager 
            // by way of ChangeFormManager::AddFlags and ChangeFormManager::RemoveFlags. If the 
            // latterleads to all flags on a created reference being unset, then the manager 
            // will delete the change-data, signal this to the caller (by returning true from 
            // RemoveFlags), and the form will not be saved.
            //
            // We patch the branch that runs when RemoveFlags returns true.
            //
            _declspec(naked) void Outer() {
               _asm {
                  mov  eax, 0x00686C00; // BGSSaveGameBuffer::~BGSSaveGameBuffer
                  call eax; // reproduce patched-over call
                  push edi;
                  call OnDestroy; // stdcall
                  mov  eax, 0x00679A5E;
                  jmp  eax;
               };
            };
            void Apply() {
               WriteRelJump(0x00679A59, (UInt32)&Outer);
            };
         };
         //
         void Apply() {
            FormHasNoChanges::Apply();
            FormRefusesToSave::Apply();
            FormDeletedDuringUnload::Apply();
         };
      };
      namespace Saving { // BGSSaveLoadManager::SaveGame_HookTarget
         namespace FormRefusesToSave { // TESForm::Unk_0D returned false
            _declspec(naked) void Outer() {
               _asm {
                  mov  eax, 0x00676EA0; // ChangeFormManager::Subroutine00676EA0
                  call eax; // reproduce patched-over call
                  push ebp;
                  call OnDestroy; // stdcall
                  mov  eax, 0x00679476;
                  jmp  eax;
               };
            };
            void Apply() {
               WriteRelJump(0x00679471, (UInt32)&Outer);
            };
         };
         namespace FormDeletedDuringSave { // TESForm::Unk_0D unset all remaining changeflags, so the form will not be saved
            //
            // TESForm::Unk_0D can tamper with the form's changeflags just before save, in order 
            // to avoid saving unnecessary data or to force certain data to persist. The caller 
            // will catch changes to the changeflags and reconcile with the ChangeFormManager 
            // by way of ChangeFormManager::AddFlags and ChangeFormManager::RemoveFlags. If the 
            // latterleads to all flags on a created reference being unset, then the manager 
            // will delete the change-data, signal this to the caller (by returning true from 
            // RemoveFlags), and the form will not be saved.
            //
            // We patch the branch that runs when RemoveFlags returns true.
            //
            _declspec(naked) void Outer() {
               _asm {
                  mov  eax, 0x00677160; // ChangeFormManager::RemoveFlags
                  call eax; // reproduce patched-over call
                  push eax; // protect
                  test al, al;
                  jz   lExit;
                  push ebp;
                  call OnDestroy; // stdcall
               lExit:
                  pop  eax; // restore
                  mov  eax, 0x006794B9;
                  jmp  eax;
               };
            };
            void Apply() {
               WriteRelJump(0x006794B4, (UInt32)&Outer);
            };
         };
         namespace FormCouldNotBeLocated {
            _declspec(naked) void Outer() {
               _asm {
                  mov  eax, 0x00677230; // logging call
                  call eax; // reproduce patched-over call
                  cmp  ebp, 0xFF000000;
                  jb   lExit;
                  push ebp;
                  call OnDestroy; // stdcall
               lExit:
                  mov  eax, 0x00679501;
                  jmp  eax;
               };
            };
            void Apply() {
               WriteRelJump(0x006794FC, (UInt32)&Outer);
            };
         };
         //
         void Apply() {
            FormCouldNotBeLocated::Apply();
            FormRefusesToSave::Apply();
            FormDeletedDuringSave::Apply();
         };
      };

      void Apply() {
         Unloading::Apply();
         Saving::Apply();
      };
   };
};