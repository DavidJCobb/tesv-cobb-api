#include "BGSPrimitive.h"

__declspec(naked) BGSPrimitive* _make(UInt32 type, NiPoint3* halfwidths, NiQuaternion* probablyAQuaternion) {
   __asm {
      push ebp; // to make the stack math easier here
      mov ebp, esp;
      mov eax, [ebp + 0x10]; // arg3
      push eax;
      mov eax, [ebp + 0x0C]; // arg2
      push eax;
      mov eax, [ebp + 0x08]; // arg1
      push eax;
      mov eax, 0x0046A070;
      call eax;
      add esp, 0xC; // clean up after last non-member call
      pop ebp;
      retn;
   }
};
static NiQuaternion* defaultQuaternion = (NiQuaternion*)0x012C5C90;

namespace RE {
   BGSPrimitiveBox* BGSPrimitiveBox::Create(NiPoint3* halfwidths) {
      NiPoint3 sizes = *halfwidths;
      NiQuaternion unknown = *defaultQuaternion;
      return (BGSPrimitiveBox*) _make(kPrimitive_Box, &sizes, &unknown);
   };
}