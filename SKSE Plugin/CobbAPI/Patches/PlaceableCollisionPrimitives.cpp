#include "PlaceableCollisionPrimitives.h"
#include "skse/GameRTTI.h"
#include "skse/SafeWrite.h"

#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Systems/GameData.h"

static const UInt32 g_PlaceableCollisionPrimitiveFormID = 0x00C0BB00;

namespace CobbPatches {
   namespace PlaceableCollisionPrimitives {
      int constexpr ce_strlen(const char* str) {
         return *str ? 1 + ce_strlen(str + 1) : 0;
      }

      void Manager::Load() {
         #if COBB_PLACEABLE_NAVCUT_USES_MANAGER == 0
            return;
         #endif
         BGSKeyword* keyword;
         {
            auto form = LookupFormByID(0x00C0BB03);
            keyword = DYNAMIC_CAST(form, TESForm, BGSKeyword);
            if (!keyword) {
               _MESSAGE("Unable to find navcut keyword 00C0BB03!");
               return;
            }
         }
         auto   data = RE::TESDataHandler::GetSingleton();
         auto&  list = data->activators;
         size_t count = list.count;
         for (size_t i = 0; i < count; i++) {
            auto item = list.arr.entries[i];
            if (!item)
               continue;
            if (item->keyword.HasKeyword(keyword))
               this->activators.push_back(item);
         }
      };
      bool Manager::Matches(TESObjectACTI* form) const {
         auto& list = this->activators;
         auto  size = list.size();
         for (decltype(size) i = 0; i < size; i++)
            if (list[i] == form)
               return true;
         return false;
      };

      bool _stdcall Inner(RE::TESForm* base) {
         if (base->formType != kFormType_Activator)
            return false;
         auto acti = (TESObjectACTI*)base;
         if (!acti)
            return false;
         return Manager::GetInstance().Matches(acti);
      }
      namespace Hook1 {
         static const UInt32 targetBase = 0x004E4FF0;
         //
         static const UInt32 entry = targetBase + 0x99;
         static const UInt32 exit  = targetBase + 0xA1;
         static const UInt32 b_not = targetBase + 0xAA;

         namespace DebugLogging {
            void _stdcall Inner(RE::TESObjectREFR* ref) {
               if (!ref || !ref->baseForm)
                  return;
               _MESSAGE("Generating 3D for reference %08X, base %08X...", ref->formID, ref->baseForm->formID);
               {  // loaded state flag
                  auto state = ref->loadedState;
                  if (state && state->unk18 & 1)
                     _MESSAGE(" - Loaded state flag is 1.");
                  else
                     _MESSAGE(" - Loaded state flag is 0.");
               }
               {  // Unk_2A
                  _MESSAGE(" - IsWaterActivator is %d.", ref->IsWaterActivator());
               }
            };
            __declspec(naked) void Outer() {
               _asm {
                  mov  ecx, 0x004AC1B0; // bool ReferenceIsLargeStatic(TESObjectREFR*)
                  call ecx;
                  push eax; // protect
                  push esi;
                  call Inner; // stdcall
                  pop  eax; // restore
                  mov  ecx, 0x004E5004;
                  jmp  ecx;
               };
            };
            void Apply() {
               WriteRelJump(0x004E4FFF, (UInt32)Outer);
            };
         };

         void DebugLogWhenGenerating() {
            _MESSAGE(" - Generating primitive 3D...");
         };
         __declspec(naked) void Patch() {
            __asm {
               //
               // REGISTERS:
               // eax = base form; must not be changed if we don't match.
               // ebx
               // ecx = disposable
               // edx
               // edi
               // esi = essential; (TESObjectREFR*) this
               // ebp
               //
               //cmp eax, dword ptr ds:[0x0131052C]; // CollisionMarker // Visual Studio literally does not compile this properly.
               mov  ecx, 0x0131052C; // == (TESObjectSTAT**)0x0131052C // CollisionMarker
               cmp  eax, dword ptr [ecx];
               je   lFormMatches;
               push eax; // protect
               #if COBB_PLACEABLE_NAVCUT_USES_MANAGER == 0
                  mov  eax, dword ptr [eax + 0x0C]; // eax = eax->formID;
                  cmp  eax, g_PlaceableCollisionPrimitiveFormID;
               #else
                  push eax;   // arg
                  call Inner; // stdcall
                  cmp  al, 1;
               #endif
               pop  eax; // restore
               je  lFormMatches;
            lNoMatch:
               jmp  [b_not];
            lFormMatches:
//call DebugLogWhenGenerating;
               jmp  [exit];
            };
         };

         void Apply() {
            for (UInt32 i = entry; i < exit; i++)
               SafeWrite8(i, 0x90); // NOP
            WriteRelJump(entry, (UInt32) Patch);
         };
      };
      namespace Hook2 { // testing; don't know if this will help yet
         static constexpr UInt32 b_matches = 0x004DB9DD;
         static constexpr UInt32 b_not     = 0x004DB9D8;

         __declspec(naked) void Patch() {
            _asm {
               //cmp eax, dword ptr ds:[0x0131052C]; // CollisionMarker // Visual Studio literally does not compile this properly.
               mov  ecx, 0x0131052C; // == (TESObjectSTAT**)0x0131052C // CollisionMarker
               cmp  eax, dword ptr [ecx];
               je   lFormMatches;
               push eax; // protect
               #if COBB_PLACEABLE_NAVCUT_USES_MANAGER == 0
                  mov  eax, dword ptr [eax + 0x0C]; // eax = eax->formID;
                  cmp  eax, g_PlaceableCollisionPrimitiveFormID;
               #else
                  push eax;   // arg
                  call Inner; // stdcall
                  cmp  al, 1;
               #endif
               pop  eax; // restore
               je   lFormMatches;
            lNoMatch:
               jmp  [b_not];
            lFormMatches:
               jmp  [exit];
            };
         };
         void Apply() {
            WriteRelJump(0x004DB9D0, (UInt32)Patch);
            SafeWrite8(0x004DB9D5, 0x90); // NOP
         };
      };

      void OnFormsLoaded() {
         #if COBB_PLACEABLE_NAVCUT_USES_MANAGER == 1
            Manager::GetInstance().Load();
         #endif
      };
      void Apply() {
         Hook1::Apply();
         Hook2::Apply();
//Hook1::DebugLogging::Apply();
      };
   }
};