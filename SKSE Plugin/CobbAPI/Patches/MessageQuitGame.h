#pragma once
#include "Shared.h"
#include "skse/PluginAPI.h"
#include "skse/SafeWrite.h"

namespace CobbPatches {
   namespace MessageQuitGame {
      //
      // The subroutine we're hooking may run multiple times, redundantly, when starting a session. For 
      // example, it runs three times during the process of COCing from the main menu. However, it DOES 
      // run for COC, New Game, and Load Game, so if you don't mind that redundancy, it's a good hook 
      // for cleaning up between sessions.
      //
      //static const UInt32 targetBase = 0x00695940;
      static const UInt32 targetBase = 0x008F0AC0;
      static const UInt32 entry = targetBase + 0;

      void FireEvent() {
         const char data[] = "quit game";
         g_ISKSEMessaging->Dispatch(g_pluginHandle, 'EvCh', (void*)&data, sizeof(data), NULL);
      };

      void _declspec(naked) Patch() {
         __asm {
            call FireEvent;
            nop;            // Courtesy nops, so that anyone peeking at this in a disassembler can tell where our stuff ends, pretty much.
            nop;            //
            nop;            //
            nop;            //
            //mov eax, dword ptr ds:[0x01B2E87C]; // Reproduce the instruction we patched over.
            mov eax, dword ptr ds:[0x01B2E860]; // Reproduce the instruction we patched over.
            retn;
         };
      };
      void Apply() {
         WriteRelCall(entry, (UInt32)Patch);
      };
   };
};