#pragma once
#include "Shared.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Systems/ActorAI/base.h"

namespace RE {
   class BGSIdleCollection;
   class BGSSaveFormBuffer;
   class PackageLocation { // sizeof == 0x10 // subclass of IAIWorldLocationHandle?
      //
      // VTBL: 0x010C4914
      // RTTI: 0x012692B0
      // Constructor: 0x005E70C0
      //
      public:
         virtual ~PackageLocation();
         PackageLocation(UInt32) {
         };

         enum Type {
            kType_NearReference = 0,
            kType_InCell,
            kType_NearCurrentLocation,
            kType_NearEditorLocation,
            kType_ObjectID,
            kType_ObjectType,
            kType_NearLinkedRef,
            kType_AtPackageLocation
         };

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
   class TESPackageData { // sizeof == ?
      public:
         virtual ~TESPackageData();
         virtual void Unk_01(TESPackageData*, UInt32);
         virtual bool Unk_02(TESPackageData*);
         virtual void Unk_03(UInt32);
         virtual void Save(BGSSaveFormBuffer*); // 04
         virtual void Load(BGSLoadFormBuffer*); // 05
   };
   class TESAmbushPackageData : public TESPackageData { // sizeof == 0x8
      public:
         virtual ~TESAmbushPackageData();
         //
         PackageLocation* unk04 = nullptr; // 04
   };
   class TESCustomPackageData : public TESPackageData { // sizeof == 0x20
      public:
         virtual ~TESCustomPackageData();
         //
         struct Unk04 {
            UInt32 unk00 = 0;
            UInt32 unk04 = 0;
            UInt16 unk08 = 0;
            UInt8  unk0A = 0;
            UInt8  pad0B;
         };
         //
         Unk04  unk04;
         UInt32 unk10 = 0;
         UInt32 unk14 = 0;
         UInt32 unk18 = 0;
         UInt16 unk1C = 0;
         UInt16 pad1E;
   };
   class TESDialoguePackageData : public TESPackageData { // sizeof == 0x20
      public:
         virtual ~TESDialoguePackageData();
         //
         float  unk04; // 04 // initialized to 0x42C80000; can't check what float that is rn
         UInt32 unk08 = 0;
         PackageLocation* unk0C = nullptr;
         UInt8  unk10 = 0;
         UInt8  unk11 = 0;
         UInt16 pad12;
         UInt32 unk14 = 0;
         UInt8  unk18 = 0;
         UInt8  pad19;
         UInt16 unk1A;
         UInt32 unk1C = 0;
   };
   class TESEatPackageData : public TESPackageData { // sizeof == 0x8
      public:
         virtual ~TESEatPackageData();
         //
         PackageLocation* unk04 = nullptr; // 04
   };
   class TESEscortPackageData : public TESPackageData { // sizeof == 0xC
      public:
         virtual ~TESEscortPackageData();
         //
         PackageLocation* unk04 = nullptr; // 04
         UInt32 unk08 = 0;
         //
         TESEscortPackageData() : unk08(0xC8) {}; // at 0x005E5B40
         TESEscortPackageData(decltype(unk08)* a) : unk08(*a) {}; // at 0x005E5B60
   };
   class TESFollowPackageData : public TESPackageData { // sizeof == 0x8
      public:
         virtual ~TESFollowPackageData();
         //
         PackageLocation* unk04 = nullptr; // 04
         UInt32 unk08 = 0;
   };
   class TESPatrolPackageData : public TESPackageData {
      public:
         virtual ~TESPatrolPackageData();
         //
         UInt8 unk04;
         UInt8 unk05;
         UInt8 pad06[2];
   };
   class TESUseItemPackageData : public TESPackageData { // sizeof == 0x8
      public:
         virtual ~TESUseItemPackageData();
         //
         PackageLocation* unk04 = nullptr; // 04
   };
   class TESUseWeaponPackageData : public TESPackageData { // sizeof == 0x24
      public:
         virtual ~TESUseWeaponPackageData();
         //
         void* unk04 = nullptr; // 04
         PackageLocation* unk08 = nullptr;
         UInt8  unk0C = 0;
         UInt8  unk0D = 0;
         UInt8  unk0E = 0;
         UInt8  unk0F = 0;
         UInt8  unk10 = 0;
         UInt8  unk11 = 0;
         UInt16 unk12 = 0;
         UInt16 unk14 = 0;
         UInt16 unk16 = 6;
         float  unk18 = 1.0F;
         float  unk1C = -1.0F; // 0x40000000
         UInt32 unk20 = 0;
   };

   class TESPackage : public TESForm {
      public:
         enum { kTypeID = kFormType_Package };

         virtual bool Unk_39(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3);
         virtual bool Unk_3A(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
         virtual bool Unk_3B(UInt32 arg0, UInt32 arg1);
         virtual bool Unk_3C(UInt32 arg0, UInt32 arg1);
         virtual bool Unk_3D(UInt32 arg0);

         enum PackageType : UInt8 {
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
         enum InterruptOverride : UInt8 {
            kInterruptOverride_None,
            kInterruptOverride_Spectator,
            kInterruptOverride_ObserveDead,
            kInterruptOverride_GuardWarn,
            kInterruptOverride_Combat,
         };
         enum PreferredSpeed : UInt16 {
            kPreferredSpeed_Walk = 0,
            kPreferredSpeed_Jog,
            kPreferredSpeed_Run,
            kPreferredSpeed_FastWalk,
         };

         struct Schedule {
            SInt8 month = -1;   // 00 // 0xFF = invalid / no data
            SInt8 weekday = -1; // 01 // 0xFF = invalid / no data
            UInt8 date;         // 02
            SInt8 hour = -1;    // 03 // 0xFF = invalid / no data
            SInt8 minute = -1;  // 04 // 0xFF = invalid / no data
            UInt8 pad05[3];
            SInt32 duration = 0; // 08 // in minutes

            MEMBER_FN_PREFIX(Schedule);
            DEFINE_MEMBER_FN(CopyFrom, void, 0x005E8840, Schedule* other);
         };
         struct Data { // sizeof == 0x10
            UInt32 unk00;	// 00
            UInt32 unk04;	// 04
            UInt32 unk08;	// 08 // INAM: IdleMarker || PTDO/Type
            UInt32 unk0C;	// 0C // TNAM             || PTDO/Data

            MEMBER_FN_PREFIX(Data);
            DEFINE_MEMBER_FN(Load, void, 0x005E6B50, BGSLoadFormBuffer*);
         };

         struct Unk28 {
            enum Type : UInt8 {
               kType_SpecificReference = 0,
               kType_ObjectID,
               kType_ObjectType,
               kType_LinedRef,
               kType_ReferenceAlias,
               kType_Unknown5,
               kType_Self,
            };
            Type   type;      // 00
            UInt8  pad01[3];  // padding
            UInt32 refHandle; // 04 // reference handle for an actor
            //
            MEMBER_FN_PREFIX(Unk28);
            DEFINE_MEMBER_FN(Subroutine005E8940, void, 0x005E8940, UInt8);
         };
         struct PTDA : Unk28 { // This is the PTDA subrecord. Constructor: 0x005E88C0
            SInt32 distance;  // 08
            //
            MEMBER_FN_PREFIX(PTDA);
            DEFINE_MEMBER_FN(Subroutine005E8A50, void, 0x005E8940, UInt8, PTDA*); // apparently modifies this instance based on the passed-in instance
            DEFINE_MEMBER_FN(SetReferenceHandle, void, 0x005E8C90, Actor*); // if unk00 is false, sets unk04 to the reference handle of the passed-in actor
            DEFINE_MEMBER_FN(SetUnk08,           void, 0x00474D90, UInt32); // void*?
         };
         struct PTDT : Unk28 { // This is the PTDT subrecord. Constructor: 0x005E89B0
            UInt32 unk08;
         };

         UInt32             packageFlags;      // 14
         PackageType        type;              // 18 // see kPackageType_ enum
         InterruptOverride  interruptOverride; // 19
         UInt8              unk1A; // 1A // set to 2 by LoadForm
         UInt8              pad1B; // 1B
         PreferredSpeed     preferredSpeed; // 1C
         UInt16             interruptFlags; // 1E
         TESPackageData*    unk20; // 20 // can be any subclass
         PackageLocation*   unk24; // 24
         Unk28*             unk28; // 28 // actor targeted by this package? // PTDA or PTDT; see TESPackage::LoadForm
         BGSIdleCollection* unk2C; // 2C // IDLE, IDLA, IDLC, IDLT
         Schedule	          schedule; // 30
         void*  unk3C;       // 3C // linked list // the CTDA record; look into Struct005E9EF0
         void*  combatStyle; // 40 // CNAM
         UInt32 unk44; // 44 // ONAM
         Data   unk48; // 48 // OnBegin
         Data   unk58; // 58 // OnEnd
         Data   unk68; // 68 // OnChange
         UInt32 unk78; // 78
         UInt32 unk7C; // 7C - incremented in dtor

         bool IsExtraType();
         //
         MEMBER_FN_PREFIX(TESPackage);
         DEFINE_MEMBER_FN(GetOrCreateUseWeaponPackageData, TESPackageData*, 0x005DF2E0); // returns unk20 if it's not nullptr; if it is, sets it to (new TESUseWeaponPackageData());
         DEFINE_MEMBER_FN(GetFlags00007F80,   UInt8, 0x005DECA0);
         DEFINE_MEMBER_FN(Subroutine005DF310, void,  0x005DF310, Unk28*);
         DEFINE_MEMBER_FN(Subroutine005E13B0, void,  0x005E13B0, UInt32); // not yet analyzed
         DEFINE_MEMBER_FN(ModifyFlag00000800, bool,  0x005E0EC0); // Only modifies the flag for packages that were created by the engine at run-time.
         DEFINE_MEMBER_FN(SetSchedule,        void,  0x005DEE40, Schedule*);
   };

   static DEFINE_SUBROUTINE(TESPackage*, CreatePackageOfType, 0x005E4500, UInt8);
};