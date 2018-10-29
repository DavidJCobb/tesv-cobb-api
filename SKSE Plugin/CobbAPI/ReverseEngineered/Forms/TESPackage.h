#pragma once
#include "Shared.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Systems/ActorAI/base.h"

namespace RE {
   class PackageLocation { // sizeof == 0x10 // subclass of IAIWorldLocationHandle?
      //
      // VTBL: 0x010C4914
      // RTTI: 0x012692B0
      // Constructor: 0x005E70C0
      //
      public:
         virtual ~PackageLocation();
         //
         UInt8  unk04 = 3; // Subroutine 0x005E7130 switch-cases on this if it's <= 0xC.
         UInt8  pad05[3];
         UInt32 unk08 = 0;
         UInt32 unk0C = 0; // reference handle for an actor (the actor executing the package?)
         //
         MEMBER_FN_PREFIX(PackageLocation);
         DEFINE_MEMBER_FN(Subroutine005E7130, void,   0x005E7130, PackageLocation*, UInt8);
         DEFINE_MEMBER_FN(Subroutine005E8110, void,   0x005E8110, Actor*); // only took a cursory look at this; found out what unk0C is from it
         DEFINE_MEMBER_FN(GetUnk08,           UInt32, 0x005E6FF0);         // gets this->unk08 only if this->unk04 is not 0xFF or 0x01; returns NULL otherwise.
         DEFINE_MEMBER_FN(SetUnk08,           void,   0x005E7010, UInt32); // sets this->unk08 only if this->unk04 is not 0xFF or 0x01; does nothing otherwise.
   };

   class TESPackage : public TESForm {
      public:
         enum { kTypeID = kFormType_Package };

         virtual bool Unk_39(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3);
         virtual bool Unk_3A(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
         virtual bool Unk_3B(UInt32 arg0, UInt32 arg1);
         virtual bool Unk_3C(UInt32 arg0, UInt32 arg1);
         virtual bool Unk_3D(UInt32 arg0);

         enum {
            kPackageType_Find = 0,
            kPackageType_Follow,
            kPackageType_Escort,
            kPackageType_Eat,
            kPackageType_Sleep,
            kPackageType_Wander,
            kPackageType_Travel,
            kPackageType_Accompany,
            kPackageType_UseItemAt,
            kPackageType_Ambush,
            kPackageType_FleeNotCombat,
            kPackageType_CastMagic,
            kPackageType_Sandbox,
            kPackageType_Patrol,
            kPackageType_Guard,
            kPackageType_Dialogue,
            kPackageType_UseWeapon,
            kPackageType_Find2,
            kPackageType_Package,
            kPackageType_PackageTemplate,
            kPackageType_Activate,
            kPackageType_Alarm,
            kPackageType_Flee,
            kPackageType_Trespass,
            kPackageType_Spectator,
            kPackageType_ReactToDead,
            kPackageType_GetUpFromChair,
            kPackageType_DoNothing,
            kPackageType_InGameDialogue,
            kPackageType_Surface,
            kPackageType_SearchForAttacker,
            kPackageType_AvoidPlayer,
            kPackageType_ReactToDestroyedObject,
            kPackageType_ReactToGrenadeOrMine,
            kPackageType_StealWarning,
            kPackageType_PickPocketWarning,
            kPackageType_MovementBlocked,
            kPackageType_VampireFeed, // 37 (0x25)
            kPackageType_CannibalFeed, // 38 (0x26)
         };

         struct Data30 { // sizeof == 0xC
            UInt8	unk00;		// 0
            UInt8	unk01;		// 1
            UInt8	unk02;		// 2
            UInt8	unk03;		// 3
            UInt8	unk04;		// 4
            UInt8	pad05[3];	// 5
            UInt32	unk08;		// 8
         };
         struct Data { // sizeof == 0x10
            UInt32	unk00;	// 00
            UInt32	unk04;	// 04
            UInt32	unk08;	// 08
            UInt32	unk0C;	// 0C
         };

         struct Unk28 {      // Constructor is at 005E88C0.
            UInt8  unk00;    // bool; related to unk04
            UInt8  pad01[3]; // padding
            UInt32 unk04;    // reference handle for an actor
            UInt32 unk08;    // void*?
            //
            MEMBER_FN_PREFIX(Unk28);
            DEFINE_MEMBER_FN(Subroutine005E8940, void, 0x005E8940, UInt8);
            DEFINE_MEMBER_FN(Subroutine005E8A50, void, 0x005E8940, UInt8, Unk28*); // apparently modifies this instance based on the passed-in instance
            DEFINE_MEMBER_FN(SetReferenceHandle, void, 0x005E8C90, Actor*); // if unk00 is false, sets unk04 to the reference handle of the passed-in actor
            DEFINE_MEMBER_FN(SetUnk08,           void, 0x00474D90, UInt32); // void*?
         };

         UInt32	packageFlags;	// 14
         UInt8  type;			// 18 - see kPackageType_ enum
         UInt8  unk19;			// 19
         UInt8  unk1A;			// 1A
         UInt8  pad1B;			// 1B
         UInt16	unk1C;			// 1C
         UInt16	unk1E;			// 1E
         UInt32	unk20;			// 20
         PackageLocation* unk24; // 24
         Unk28*	unk28;			// 28 // actor targeted by this package?
         UInt32	unk2C;			// 2C
         Data30	unk30;			// 30
         void*  unk3C;          // 3C - linked list
         UInt32	unk40;			// 40
         UInt32	unk44;			// 44
         Data   unk48;			// 48
         Data   unk58;			// 58
         Data   unk68;			// 68
         UInt32	unk78;			// 78
         UInt32	unk7C;			// 7C - incremented in dtor

         bool IsExtraType();
         //
         MEMBER_FN_PREFIX(TESPackage);
         DEFINE_MEMBER_FN(GetFlags00007F80,   UInt8, 0x005DECA0);
         DEFINE_MEMBER_FN(Subroutine005DF310, void,  0x005DF310, Unk28*);
         DEFINE_MEMBER_FN(Subroutine005E13B0, void,  0x005E13B0, UInt32); // not yet analyzed
         DEFINE_MEMBER_FN(ModifyFlag00000800, bool,  0x005E0EC0); // Only modifies the flag for packages that were created by the engine at run-time.
   };

   static DEFINE_SUBROUTINE(TESPackage*, CreatePackageOfType, 0x005E4500, UInt8);
};