#pragma once

#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
   class TESObject : public TESForm {
      public:
         //
         // SKSE VTBL displaced downward by two functions added to TESForm.
         //
         virtual UInt32 Unk_3B(); // 3B // no-op on TESObject; returns null
         virtual bool   Unk_3C(); // 3C // no-op on TESObject; returns false
         virtual UInt32 Unk_3D(); // 3D // no-op on TESObject; returns null
         virtual bool   Unk_3E(); // 3E // no-op on TESObject; returns false
         virtual void   Unk_3F(void* arg); // 3F // no-op on TESObject
         virtual UInt32 Unk_40(void*, void*); // 40 // no-op on TESObject; returns null
         virtual void   Unk_41(void*); // 41
         virtual bool   Unk_42(); // 42 // related to 3D?
         virtual bool   Unk_43(); // 43 // checks if the TESObject is a Static and is one of three executable-defined forms
         virtual bool   Unk_44(); // 44
         virtual void   Unk_45(UInt32 arg0, UInt32 arg1); // 45 // no-op on TESObject
         virtual UInt32 Unk_46(); // 46 // no-op on TESObject; returns null
         virtual UInt32 Unk_47(); // 47 // no-op on TESObject; returns null
         virtual void*  Unk_48(void*); // 48
   };

   // 20
   class TESBoundObject : public TESObject {
      public:
         struct Bound {
            SInt16	x;
            SInt16	y;
            SInt16	z;
         };

         // SKSE VTBL displaced downward by two functions added to TESForm.
         //
         virtual void   Unk_49(UInt32 arg); // 49
         virtual UInt32 Unk_4A(void); // 4A
         virtual UInt32 Unk_4B(UInt32 arg); // 4B	// return Unk_40(arg, 0);
         virtual bool   Unk_4C(UInt32 arg); // 4C
         virtual bool   Unk_4D(void* arg0, BSString* dst, void*); // 4D // steal/take string // assumes that it's working with a TESObjectREFR*
         virtual bool   Unk_4E(void* arg0, UInt8 arg1, UInt32 arg2, float arg3); // 4E
         virtual void   Unk_4F(UInt32 arg); // 4F
         virtual void   Unk_50(UInt32 arg); // 50
         virtual void   Unk_51(void); // 51
         virtual void   Unk_52(void); // 52
         virtual void   Unk_53(void); // 53

         Bound boundsMin; // 14
         Bound boundsMax; // 1A
   };
   static_assert(sizeof(TESBoundObject) == 0x20, "RE::TESBoundObject is the wrong size.");

   class TESBoundAnimObject : public TESBoundObject {
      public:
   };
};