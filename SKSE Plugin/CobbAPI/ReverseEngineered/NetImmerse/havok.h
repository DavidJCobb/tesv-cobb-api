#pragma once

#include "ReverseEngineered/NetImmerse/objects.h"

#include "skse/NiNodes.h"

typedef void(*BhkCollisionIteratorFunction)(RE::bhkCollisionObject* collision, void* parameters);
static DEFINE_SUBROUTINE(void, IterateOverBhkCollisionObjects, 0x00D0E520, NiNode* root, void* parameters, BhkCollisionIteratorFunction* iterator);

//
// It would appear that hk_____ classes are native Havok, while bhk_____ are Bethesda wrappers and extensions.
//
namespace RE {
   //
   // TODO: Virtual methods for hkp_____ classes.
   // TODO: Virtual methods for bhk_____ classes.
   //

   struct Struct00D69170 { // sizeof == 0x2C
      UInt32 unk00 = 1;
      UInt32 unk04;
      UInt32 unk08;
      UInt32 unk0C;
      UInt32 unk10 = 0;
      UInt32 unk14;
      UInt32 unk18;
      UInt32 unk1C;
      UInt16 unk20 = 0x0000;
      UInt16 unk22 = 0x0000;
      UInt32 unk24 = 0;
      UInt32 unk28 = 0;
   };
   struct Struct00D57F60 { // sizeof == 0x50
      UInt8  unk00;
      UInt8  unk01;
      UInt8  unk02;
      UInt8  unk03;
      UInt32 unk04 = 0xFFFFFFFF;
      void*  unk08; // no VTBL
      UInt32 unk0C = 0;
      UInt8  unk10 = 0;
      UInt8  unk11 = 8;
      UInt16 unk12 = 0x0000;
      UInt32 unk14 = 0;
      UInt8  unk18;
      UInt8  unk19 = 0x7F;
      UInt8  unk1A = 0xFF;
      UInt8  unk1B;
      UInt32 unk1C = 0;
      Struct00D69170 unk20;
      float  unk4C = -1.0;
   };
   struct Struct00D5FC10 { // sizeof == 0x4
      UInt32 unk00 = 0;
   };

   struct Struct00DDB260 { // sizeof >= 0xA4
      struct Struct00DDB2B0 { // sizeof == 0x50?
         UInt32 unk00[0x50 / 4]; // five DQWords?
      };
      //
      UInt32 unk04[0x40 / 4]; // unk10, unk20, and unk30 are DQWords?
      Struct00DDB2B0 unk40;
      UInt32 unk90[(0xA0 - 0x90) / 4];
      float  unkA0 = 1.0;
   };
   static_assert(sizeof(RE::Struct00DDB260) == 0xA4, "RE::Struct00DDB260 is not the right size!");

   class hkpMotion { // sizeof == 0x120
      public:
         enum MotionSystem : UInt8 {
            kMotionSystem_Invalid        = 0,
            kMotionSystem_Dynamic        = 1,
            kMotionSystem_SphereInertia  = 2,
            kMotionSystem_BoxInertia     = 3,
            kMotionSystem_Keyframed      = 4,
            kMotionSystem_Fixed          = 5,
            kMotionSystem_ThinBoxInertia = 6,
            kMotionSystem_Character      = 7
         };
         //
         virtual ~hkpMotion();
         //
         UInt16 unk004;
         UInt16 unk006 = 0x0001;
         MotionSystem motionSystem; // 08
         UInt8  unk009 = 0xFF; // DeactivatorType?
         UInt8  unk00A;        // SolverDeactivation?
         UInt8  unk00B;        // MotionQuality?
         UInt16 unk00C = 0x0000;
         UInt16 unk00E;
         Struct00DDB260 unk010;
         UInt16 unk0B4;
         UInt16 unk0B6;
         UInt32 unk0B8;
         UInt8  unk0BC;
         UInt8  unk0BD;
         UInt8  unk0BE;
         UInt8  unk0BF;
         UInt32 unk0C0[(0xF0 - 0xC0) / 4]; // not initialized
         UInt32 unk0F0[0x10 / 4]; // DQWORD
         UInt32 unk100[0x10 / 4]; // DQWORD
         UInt32 unk110 = 0;
         UInt32 unk114 = 0;
         UInt32 unk118;
         UInt8  unk11C;
         UInt8  unk11D;
         UInt16 unk11E;
         //
         MEMBER_FN_PREFIX(hkpMotion);
         DEFINE_MEMBER_FN(SetUnkBC, void, 0x00D4A3C0, UInt8 value);
   };
   static_assert(sizeof(RE::hkpMotion) == 0x120, "RE::hkpMotion is not the right size!");
   class hkpMaxSizeMotion : public hkpMotion { // sizeof == 0x120
      public:
         virtual ~hkpMaxSizeMotion();
   };
   class hkpKeyframedRigidMotion : public hkpMotion { // sizeof == 0x120
      public:
         virtual ~hkpKeyframedRigidMotion();
   };
   class hkpFixedRigidMotion : public hkpKeyframedRigidMotion { // sizeof == 0x120
      public:
         virtual ~hkpFixedRigidMotion();
   };

   class hkpWorldObject { // sizeof == 0x8C
      public:
         virtual ~hkpWorldObject();
         //
         UInt16 unk004;
         UInt16 unk006 = 0x0001;
         void*  unk008 = NULL; // class with no RTTI
         void*  unk00C; // Most likely a pointer back to the owning bhkWorldObject instance.
         Struct00D57F60 unk010;
         UInt32 unk060 = 0;
         UInt32 unk064 = 0;
         UInt32 unk068 = 0;
         SInt32 unk06C = -0xF;
         UInt32 unk070;
         UInt16 unk074 = 0x8000;
         UInt16 unk076;
         Struct00D5FC10 unk078;
         UInt32 unk07C = 0;
         UInt32 unk080 = 0;
         UInt32 unk084 = 0x80000000;
         SInt32 unk088 = -1;
   };
   static_assert(sizeof(RE::hkpWorldObject) == 0x8C, "RE::hkpWorldObject is not the right size!");

   class hkpEntity : public hkpWorldObject { // sizeof >= 0x21C
      public:
         virtual ~hkpEntity();
         //
         UInt16 unk08C;
         UInt16 unk08E;
         float  unk090 = 0.5;
         float  unk094 = 0.4;
         UInt32 unk098 = 0;
         float  unk09C = 1.0;
         UInt32 unk0A0 = 0;
         UInt32 unk0A4 = 0;
         UInt16 unk0A8 = 0xFFFF;
         UInt16 unk0AA;
         UInt32 unk0AC = 0;
         UInt16 unk0B0 = 0x0000;
         UInt16 unk0B2;
         UInt32 unk0B4 = 0;
         UInt32 unk0B8 = 0;
         UInt32 unk0BC = 0x80000000;
         UInt32 unk0C0 = 0;
         UInt32 unk0C4 = 0;
         UInt32 unk0C8 = 0;
         UInt32 unk0CC = 0;
         UInt8  unk0D0;
         UInt8  unk0D1 = 0;
         UInt8  unk0D2;
         UInt8  unk0D3;
         UInt32 unk0D4 = 0xFFFFFFFF;
         UInt32 unk0D8 = 0;
         UInt16 unk0DC = 0x0000;
         UInt8  unk0DE = 3;
         UInt8  unk0DF = 1;
         hkpMaxSizeMotion unk0E0;
         UInt32 unk200 = 0;
         UInt16 unk204 = 0x0000;
         UInt16 unk206 = 0x8000;
         UInt32 unk208 = 0;
         UInt16 unk20C = 0x0000;
         UInt16 unk20E = 0x8000;
         UInt32 unk210 = 0;
         UInt32 unk214 = 0;
         UInt32 unk218 = 0xFFFFFFFF;
   };
   static_assert(sizeof(RE::hkpEntity) == 0x21C, "RE::hkpEntity is not the right size!");

   class hkpRigidBody : public hkpEntity { // sizeof == 0x220?
      public:
         virtual ~hkpRigidBody();
         //
         UInt32 unk21C;
   };
   static_assert(sizeof(RE::hkpRigidBody) == 0x220, "RE::hkpRigidBody is not the right size!");

   class bhkRefObject : public NiObject {};
   class bhkSerializable : public bhkRefObject {
      //
      // VTBL: 0x01088364
      //
      public:
         //
         // Argument signatures assumed to match bhkRigidBody, but none of these have been analyzed yet. 
         // Remember: any methods we rename in one of these classes must be renamed in all of them.
         //
         virtual void Unk_21(void);
         virtual void Unk_22(void);
         virtual UInt32 Unk_23();
         virtual UInt32 Unk_24();
         virtual bool Unk_25(void);
         virtual void Unk_26(void);
         virtual void Unk_27(bool);
         virtual UInt32 Unk_28();
         virtual void Unk_29();
         virtual void Unk_2A(void*);
         virtual void Unk_2B(void);
         virtual void Unk_2C();
         virtual void Unk_2D(UInt32 unk);
   };
   class bhkWorldObject : public bhkSerializable {
      //
      // VTBL: 0x010884FC
      //
      public:
         hkpWorldObject* myHkpObject; // 08 // mimics the owner class, e.g. hkpRigidBody* for a bhkRigidBody
         //
         // Argument signatures assumped to match bhkRigidBody, but none of these have been analyzed yet. 
         // Remember: any methods we rename in one of these classes must be renamed in all of them.
         //
         virtual void Unk_21(void);
         virtual void Unk_22(void);
         virtual UInt32 Unk_23();
         virtual UInt32 Unk_24();
         virtual bool Unk_25(void);
         virtual void Unk_26(void);
         virtual void Unk_27(bool);
         virtual UInt32 Unk_28();
         virtual void Unk_29();
         virtual void Unk_2A(void*);
         virtual void Unk_2B(void);
         virtual void Unk_2C();
         virtual void Unk_2D(UInt32 unk);
         virtual void Unk_2E(void);
   };
   class bhkEntity : public bhkWorldObject {
      //
      // VTBL: 0x010885BC
      // Doesn't add any new virtual methods, though it may override existing ones.
      //
   };
   class bhkRigidBody : public bhkEntity { // sizeof == 0x24
      //
      // VTBL: 0x01089214
      //
      public:
         //
         UInt32 unk0C; // could belong to bhkWorldObject
         UInt16 unk10;
         UInt16 unk12;
         SInt32 unk14;
         UnkArray unk18;
         //
         // Remember: if you rename these virtual methods, rename their counterparts (if any) in the superclasses.
         //
         virtual void Unk_21(void);
         virtual void Unk_22(void);
         virtual UInt32 Unk_23(); // returns this->unk08 ? this->unk08->unk08 : NULL;
         virtual UInt32 Unk_24(); // same as Unk_23, but it checks if this exists first
         virtual bool Unk_25(void);
         virtual void Unk_26(void); // does stuff (not sure what) and then recursively executes self on all unk18 elements
         virtual void Unk_27(bool);
         virtual UInt32 Unk_28(); // returns 0xF0
         virtual void Unk_29();
         virtual void Unk_2A(void*);
         virtual void Unk_2B(void);
         virtual void Unk_2C();
         virtual void Unk_2D(UInt32 unk); // returns Unk_27(1);
         virtual void Unk_2E(void);
         virtual void Unk_2F(void);
         // ...?
   };

   class NiCollisionObject : public NiObject { // sizeof == 0x14
      //
      // Reverse-engineering suggests that this only derives from NiRefObject and not NiObject, but 
      // NiObject has a cast method for bhkCollisionObject, so this obviously DOES derive from NiObject.
      //
      public:
         enum Flags : UInt32 {
            kFlag_Active = 0x00000001,
            kFlag_Notify = 0x00000004,
            kFlag_Local  = 0x00000008,
            kFlag_Reset  = 0x00000040,
         };
         //
         UInt32        unk08 = 0; // 08 // almost certainly the NiNode* that the collision object is attached to, but I'd like to confirm that
         Flags         flags;     // 0C
         bhkRigidBody* unk10;     // 10
   };
   class bhkNiCollisionObject : public NiCollisionObject { // sizeof == 0x14
   };
   class bhkCollisionObject : public bhkNiCollisionObject { // sizeof == 0x14
      //
      // VTBL: 0x010881F4
      // Could have as many as 0x89 virtual methods.
      //
      public:
         virtual ~bhkCollisionObject();
         //
         virtual void Unk_21(void);
         virtual void Unk_22(void);
         virtual void Unk_23(void);
         virtual void Unk_24(void);
         virtual void Unk_25(void);
         virtual void Unk_26(void);
         virtual void Unk_27(void);
         virtual void Unk_28(void);
         virtual void Unk_29(void);
         virtual void SetMotionSystem(UInt32 motionSystemToSet, UInt32 unkInt, UInt32 unkBool);
   };

   class hkCharacterProxy {
      // bhkRigidBody* unk2A8;
   };

   class bhkBoxShape : public NiRefObject {
      //
      // Multiple inheritance? from bhkSphereRepShape and bhkConvexShape?
      //
      public:
         UInt32 unk08 = 0;
         UInt32 unk0C = 0;
         UInt32 unk10 = 0;
         //
         // ...?
   };
   class bhkSphereShape : public NiRefObject {
      //
      // Multiple inheritance? from bhkSphereRepShape and bhkConvexShape?
      //
      public:
         UInt32 unk08 = 0;
         UInt32 unk0C = 0;
         UInt32 unk10 = 0;
         //
         // ...?
   };
};