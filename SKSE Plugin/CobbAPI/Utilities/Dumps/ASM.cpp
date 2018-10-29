#include "Utilities/Dumps/ASM.h"

static char* dOut = "      %d: %08X (subroutine?)";
void __declspec(naked) VisualStudioIsReallyDumbSometimes(const UInt32* stack) {
   __asm {
      push esi;
      mov esi, [esp + 0x8];
      push eax;
      push ebx;
      push ecx;
      mov eax, 0;
   lStart:
      cmp eax, 0x1E;
      jae lReturn;
      lea ebx, [eax + eax];
      lea ebx, [ebx + ebx];
      add ebx, esi;
      mov ebx, [ebx];
      mov ecx, ebx;
      and ecx, 0xFF000000;
      jnz lContinue;
      mov ecx, ebx;
      and ecx, 0x00FF0000;
      jz lContinue;
      push eax;
      push ebx;
      push eax;
      push dOut;
      call _MESSAGE;
      add esp, 0xC;
      pop eax;
   lContinue:
      inc eax;
      jmp lStart;
   lReturn:
      pop ecx;
      pop ebx;
      pop eax;
      pop esi;
      retn 4;
   };
};

void CobbDumpAssemblyProxied(UInt32 eax, UInt32 ebx, UInt32 ecx, UInt32 edx, UInt32 ebp, UInt32 esi, const UInt32* stack, bool dumpFullStack) {
   #define DEPTH 30
   stack += 4; // account for last argument
   //
   _MESSAGE("Dumping assembly state, including this call...");
   _MESSAGE("   Registers:");
   _MESSAGE("      eax: %08X", eax);
   _MESSAGE("      ebx: %08X", ebx);
   _MESSAGE("      ecx: %08X", ecx);
   _MESSAGE("      edx: %08X", edx);
   _MESSAGE("      ebp: %08X", ebp);
   _MESSAGE("      esi: %08X", esi);
   _MESSAGE("      esp: %08X", stack);
   _MESSAGE("   Call Stack (Vanilla Subroutines Only):");
   try {
      /*for (UInt32 i = 0; i < DEPTH; i++) {
         const UInt32* entry = stack + (i * 4);
         UInt32 value = *entry;
         if (value & 0xFF000000 != 0)
            continue;
         if (value & 0x00FF0000 == 0)
            continue;
         _MESSAGE("      %d: %08X (subroutine?)", i, value);
      }*/
      //VisualStudioIsReallyDumbSometimes(stack); // because Visual Studio refuses to compile the above properly
      _MESSAGE("      Unavailable. VS refuses to compile it properly and the ASM rewrite crashes.");
   } catch (std::exception& e) {
      _MESSAGE("      Exception encountered. Stopping.");
   }
   if (dumpFullStack) {
      _MESSAGE("   Full Stack:");
      try {
         for (UInt32 i = 0; i < DEPTH; i++) {
            const UInt32* entry = stack + (i * 4);
            _MESSAGE("      %d: %08X", i, *entry);
         }
      } catch (std::exception& e) {
         _MESSAGE("      Exception encountered. Stopping.");
      }
   }
   _MESSAGE("   Dump complete.");
   //
   #undef DEPTH
};

void __declspec(naked) CobbDumpAssembly() {
   __asm {
      push 1;
      push esp;
      push esi;
      push ebp;
      push edx;
      push ecx;
      push ebx;
      push eax;
      nop; // can replace with int3 as needed
      call CobbDumpAssemblyProxied;
      add esp, 0x20;
      retn;
   };
};