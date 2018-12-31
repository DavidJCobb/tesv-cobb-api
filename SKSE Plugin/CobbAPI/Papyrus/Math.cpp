#pragma once
#include "Papyrus/Math.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "Miscellaneous/math.h"

namespace {
   enum ColorFormat {
      kColorFormat_ABGR = 0,
      kColorFormat_HTML = 1, // 0x00RRGGBB
   };
}

namespace CobbPapyrus {
   namespace Math {
      float Atan2(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, float y, float x) {
         return cobb::radians_to_degrees(atan2(y, x));
      }
      __declspec(naked) UInt32 BSwap(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 value) {
         _asm {
            mov   eax, dword ptr [esp + 0x10];
            bswap eax;
            retn;
         }
      }
      UInt32 ConvertColorFormat(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 value, UInt32 inFormat, UInt32 outFormat) {
         if (inFormat == outFormat)
            return value;
         UInt8 r;
         UInt8 g;
         UInt8 b;
         UInt8 a;
         switch (inFormat) {
            case kColorFormat_ABGR: {
               r = (value & 0xFF);
               g = (value >> 0x08) & 0xFF;
               b = (value >> 0x10) & 0xFF;
               a = (value >> 0x18) & 0xFF;
            }; break;
            case kColorFormat_HTML: {
               r = (value >> 0x10) & 0xFF;
               g = (value >> 0x08) & 0xFF;
               b = (value & 0xFF);
               a = 0;
            }; break;
            default:
               registry->LogError("An invalid input color format was specified. Returning zero.", stackId);
               return 0;
         }
         UInt32 result = 0;
         switch (outFormat) {
            case kColorFormat_ABGR:
               return r | (g << 0x08) | (b << 0x10) | (a << 0x18);
            case kColorFormat_HTML:
               return b | (g << 0x08) | (r << 0x10);
         }
         registry->LogError("An invalid output color format was specified. Returning zero.", stackId);
         return 0;
      }
      UInt32 GetByte(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 value, UInt32 byte) {
         if (byte > 3) {
            registry->LogError("Invalid byte; zero will be returned. You must specify 0, 1, 2, or 3 as the byte to get. Bytes are in order of 0x33221100.", stackId);
            return 0;
         }
         while (byte--)
            value >>= 0x08;
         return (value & 0xFF);
      }
      UInt32 JoinColor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 format, VMArray<UInt32> color) {
         if (color.Length() < 3) {
            registry->LogError("The color array must have at least three elements, one each for red, green, and blue.", stackId);
            return 0;
         }
         UInt32 r, g, b;
         UInt32 a = 0xFF;
         color.Get(&r, 0);
         color.Get(&g, 1);
         color.Get(&b, 2);
         if (color.Length() > 3)
            color.Get(&a, 3);
         switch (format) {
            case kColorFormat_ABGR:
               return (cobb::clamp(a, 0x00, 0xFF) << 0x18) | (cobb::clamp(b, 0x00, 0xFF) << 0x10) | (cobb::clamp(g, 0x00, 0xFF) << 0x08) | cobb::clamp(r, 0x00, 0xFF);
            case kColorFormat_HTML:
               return (cobb::clamp(r, 0x00, 0xFF) << 0x10) | (cobb::clamp(g, 0x00, 0xFF) << 0x08) | cobb::clamp(b, 0x00, 0xFF);
         }
         registry->LogError("An invalid output color format was specified. Returning zero.", stackId);
         return 0;
      }
      UInt32 JoinColorComponents(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 format, UInt32 r, UInt32 g, UInt32 b, UInt32 a) {
         switch (format) {
            case kColorFormat_ABGR:
               return (cobb::clamp(a, 0x00, 0xFF) << 0x18) | (cobb::clamp(b, 0x00, 0xFF) << 0x10) | (cobb::clamp(g, 0x00, 0xFF) << 0x08) | cobb::clamp(r, 0x00, 0xFF);
            case kColorFormat_HTML:
               return (cobb::clamp(r, 0x00, 0xFF) << 0x10) | (cobb::clamp(g, 0x00, 0xFF) << 0x08) | cobb::clamp(b, 0x00, 0xFF);
         }
         registry->LogError("An invalid output color format was specified. Returning zero.", stackId);
         return 0;
      }
      UInt32 SetByte(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 value, UInt32 byte, UInt32 setTo) {
         UInt32 mask = 0x000000FF;
         if (byte > 3) {
            registry->LogError("Invalid byte; the original value will be returned. You must specify 0, 1, 2, or 3 as the byte to set. Bytes are in order of 0x33221100.", stackId);
            return value;
         }
         while (byte--) {
            mask  <<= 0x08;
            setTo <<= 0x08;
         }
         return (value & ~mask) | setTo;
      }
      VMResultArray<SInt32> SplitColor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 format, UInt32 code) {
         VMResultArray<SInt32> result;
         result.resize(4);
         UInt8 c[4];
         c[0] = (code >> 0x18) & 0xFF;
         c[1] = (code >> 0x10) & 0xFF;
         c[2] = (code >> 0x08) & 0xFF;
         c[3] = code & 0xFF;
         switch (format) {
            case kColorFormat_ABGR:
               result[0] = c[3];
               result[1] = c[2];
               result[2] = c[1];
               result[3] = c[0];
               return result;
            case kColorFormat_HTML:
               result[0] = c[1];
               result[1] = c[2];
               result[2] = c[3];
               result[3] = 0xFF;
               return result;
         }
         registry->LogError("An invalid input color format was specified. Returning an array of zeroes.", stackId);
         return result;
      }
   }
};

bool CobbPapyrus::Math::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, float, float, float>(
      "Atan2",
      PapyrusPrefixString("Math"),
      Atan2,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Math"), "Atan2", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, UInt32>(
      "BSwap",
      PapyrusPrefixString("Math"),
      BSwap,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Math"), "BSwap", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, UInt32, UInt32, UInt32>(
      "ConvertColorFormat",
      PapyrusPrefixString("Math"),
      ConvertColorFormat,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Math"), "ConvertColorFormat", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, UInt32, UInt32>(
      "GetByte",
      PapyrusPrefixString("Math"),
      GetByte,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Math"), "GetByte", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, UInt32, VMArray<UInt32>>(
      "JoinColor",
      PapyrusPrefixString("Math"),
      JoinColor,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Math"), "JoinColor", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction5<StaticFunctionTag, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32>(
      "JoinColorComponents",
      PapyrusPrefixString("Math"),
      JoinColorComponents,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Math"), "JoinColorComponents", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, UInt32, UInt32, UInt32>(
      "SetByte",
      PapyrusPrefixString("Math"),
      SetByte,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Math"), "SetByte", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMResultArray<SInt32>, UInt32, UInt32>(
      "SplitColor",
      PapyrusPrefixString("Math"),
      SplitColor,
      registry
   ));
   registry->SetFunctionFlags(PapyrusPrefixString("Math"), "SplitColor", VMClassRegistry::kFunctionFlag_NoWait);
   return true;
}