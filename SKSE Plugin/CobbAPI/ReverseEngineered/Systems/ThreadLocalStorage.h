#pragma once

namespace RE {
   const UInt32* g_TLSIndexPointer = (UInt32*)0x01BBEB54;
   //
   struct TLSData {
      UInt32 pad000[0x1EC / 4];            // 000
      void*  unk1EC; // instanceof ExtraRoomRefData*
      UInt32 pad1F0[(0x2F8 - 0x1F0) / 4];
      UInt8  consoleMode;					// 2F8
      UInt8  pad2F9[3];					// 2F9
      UInt32 pad2FC[0xF];
      UInt32 unk338; // Referenced in subroutine at 0x00687B10.
      UInt32 pad33C[0x4E];
      UInt8  unk474;
      UInt8  unk475;
      UInt8  unk476;
      UInt8  unk477;
      UInt8  unk478;
      UInt8  unk479; // Referenced in subroutine at 0x00A48D60.
      UInt8  unk47A;
      UInt8  unk47B;
      UInt32 pad47C[0xA];
      UInt32 unk4A4; // Referenced in subroutine at 0x00A48C80.
      UInt32 unk4A8; // Referenced in subroutine at 0x00A48C80.
      UInt32 unk4AC; // Some sort of index? Referenced all over the place, including in the DFM subroutine that SKSE hooks.
      UInt32 unk4B0; // Referenced in subroutine at 0x00A494B0. Length of something?
   };
   static_assert(offsetof(TLSData, pad33C) == 0x033C, "Data layout incorrect for RE::TLSData: bad pad33C."); // IntelliSense chokes on this; ignore the squiggly red lines.
   static_assert(offsetof(TLSData, unk476) == 0x0476, "Data layout incorrect for RE::TLSData: bad unk476."); // IntelliSense chokes on this; ignore the squiggly red lines.
   static_assert(offsetof(TLSData, unk4AC) == 0x04AC, "Data layout incorrect for RE::TLSData: bad unk4AC."); // IntelliSense chokes on this; ignore the squiggly red lines.
   //
   static TLSData* GetTLSData() {
      UInt32 TlsIndex = *g_TLSIndexPointer;
      TLSData* data = NULL;
      __asm {
         mov ecx, [TlsIndex];
         mov edx, fs:[2Ch]; // linear address of thread local storage array
         mov eax, [edx + ecx * 4];
         mov[data], eax;
      }
      return data;
   }
};