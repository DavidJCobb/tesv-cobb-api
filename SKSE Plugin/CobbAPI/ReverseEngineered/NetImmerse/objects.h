#pragma once

#include "Shared.h"

#include "skse/NiNodes.h"
#include "skse/NiObjects.h"
#include "skse/Utilities.h"

class bhkAabbPhantom;
namespace RE {
   class bhkCollisionObject;
   class bhkRigidBody;
   class NiObject : public NiRefObject {
      //
      // Notes:
      //  - NiNode->unk1C instanceof bhkCollisionObject (or anything that can cast to it)
      //
      public:
         //
         // Standard NetImmerse:
         //
         virtual NiRTTI*		GetRTTI(void); // actually returns char*
         // 
         // Casts follow. These can return NULL, so check the result.
         // 
         virtual NiNode*			GetAsNiNode(void);
         virtual NiSwitchNode*	    GetAsNiSwitchNode(void);
         virtual void*			    Unk_05(); // this is also a cast; returned object is at least 0xC4 big and its unkC0 is a float
         virtual UInt32			    Unk_06();
         virtual NiGeometry*		GetAsNiGeometry(void);
         virtual NiTriBasedGeom*	GetAsNiTriBasedGeom(void);
         virtual NiTriStrips*		GetAsNiTriStrips(void);
         virtual NiTriShape*		GetAsNiTriShape(void);
         virtual BSSegmentedTriShape* GetAsBSSegmentedTriShape(void);
         virtual UInt32			Unk_0C(void);
         virtual UInt32			Unk_0D(void);
         virtual UInt32			Unk_0E(void);
         virtual bhkCollisionObject* GetAsBhkCollisionObject(void); // or a superclass? // virtual method 0x0F
         virtual UInt32			Unk_10(void);
         virtual bhkRigidBody*		GetAsBhkRigidBody(void); // or a superclass?
         virtual UInt32			Unk_12(void);
         //
         // Back to NetImmerse:
         //
         virtual NiObject*		CreateClone(NiCloningProcess cloner);
         //
         virtual void			LoadBinary(NiStream * stream);
         virtual void			LinkObject(NiStream * stream);
         virtual bool			RegisterStreamables(NiStream * stream);
         virtual void			SaveBinary(NiStream * stream);
         virtual bool			IsEqual(NiObject * object);
         //
         virtual void			ProcessClone(NiCloningProcess * cloner);
         virtual void			PostLinkObject(NiStream * stream);
         virtual bool			StreamCanSkip(void);
         virtual const NiRTTI*	GetStreamableRTTI(void) const;
         virtual UInt32         GetBlockAllocationSize(void) const;
         virtual NiObjectGroup* GetGroup(void) const;
         virtual void			SetGroup(NiObjectGroup * group);
         //
         // Begin bethesda extensions?
         //
         virtual UInt32			Unk_20(void);
         //
         DEFINE_MEMBER_FN_LONG(NiObject, VerifyType, void*, 0x004202E0, const NiRTTI*); // If object is of type, returns self; otherwise returns NULL.
   };
   static_assert(sizeof(NiObject) == 0x08, "RE::NiObject is not the right size!");

   class NiObjectNET : public NiObject { // sizeof == 0x18
      public:
         const char*       m_name;
         NiTimeController* m_controller; // 0C next pointer at +0x30
         NiExtraData**     m_extraData;  // 10 extra data
         UInt16 m_extraDataLen;      // 14 max valid entry
         UInt16 m_extraDataCapacity; // 16 array len

         // UNTESTED:
         void AddExtraData(NiExtraData * extraData);
         bool RemoveExtraData(NiExtraData * extraData);
         SInt32 GetIndexOf(NiExtraData * extraData);
         NiExtraData* GetExtraData(BSFixedString name);
   };
   static_assert(sizeof(NiObjectNET) == 0x18, "RE::NiObjectNET is not the right size!");

   class NiAVObject : public NiObjectNET { // sizeof == 0xA8
      public:
         enum {
            kFlag_SelectiveUpdate             = 0x00000002,
            kFlag_UpdatePropertyControllers   = 0x00000004,
            kFlag_SelectiveUpdateRigid        = 0x00000010,
            kFlag_OverrideSelectiveTransforms = 0x00000080,
         };
         //
         struct ControllerUpdateContext {
            enum {
               kDirty = 1 << 0,
            };
            //
            float  delta;
            UInt32 flags;
         };
         //
         virtual void UpdateControllers(ControllerUpdateContext * ctx);	// calls controller vtbl+0x8C
         virtual void UpdateNodeBound(ControllerUpdateContext * ctx);
         virtual void ApplyTransform(NiMatrix33 * mtx, NiPoint3 * translate, bool postTransform);
         virtual void SetPropertyState(NiProperty * prop);
         virtual void Unk_25(UInt32 arg0);
         virtual void Unk_26(UInt32 arg0);
         virtual NiAVObject* GetObjectByName(const char ** name);	// BSFixedString? alternatively BSFixedString is a typedef of a netimmerse type
         virtual void SetSelectiveUpdateFlags(bool * selectiveUpdate, bool selectiveUpdateTransforms, bool * rigid);
         virtual void UpdateDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
         virtual void UpdateSelectedDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
         virtual void UpdateRigidDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
         virtual void UpdateWorldBound(void);
         virtual void UpdateWorldData(ControllerUpdateContext * ctx);
         virtual void UpdateNoControllers(ControllerUpdateContext * ctx);
         virtual void UpdateDownwardPassTempParent(NiNode * parent, ControllerUpdateContext * ctx);
         virtual void Unk_30(void);	// calls virtual function on parent
         virtual void Unk_31(UInt32 arg0);
         virtual void Unk_32(UInt32 arg0);
         //
         NiNode*     m_parent;			 // 18
         bhkCollisionObject* collision; // 1C
         NiTransform	m_localTransform;  // 20
         NiTransform	m_worldTransform;  // 54
         float		unk88;				// 88
         float		unk8C;				// 8C
         float		unk90;				// 90
         float		unk94;				// 94
         UInt32   m_flags;			   // 98 - bitfield
         float		unk9C;				// 9C
         UInt32   unkA0;				// A0
         UInt8		unkA4;				// A4
         UInt8		unkA5;				// A5 - bitfield
         //
         MEMBER_FN_PREFIX(NiAVObject);
         DEFINE_MEMBER_FN(UpdateNode, void, 0x00AAF320, ControllerUpdateContext * ctx);
   };
   static_assert(sizeof(NiAVObject) == 0xA8, "RE::NiAVObject is not the right size!");

   class LoadedAreaBound : public NiRefObject {
      public:
         virtual ~LoadedAreaBound();
         //
         bhkAabbPhantom* unk08; // bhkAabbPhantom*
         bhkAabbPhantom* unk0C; // bhkAabbPhantom*
         bhkAabbPhantom* unk10; // bhkAabbPhantom*
         bhkAabbPhantom* unk14; // bhkAabbPhantom*
         bhkAabbPhantom* unk18; // bhkAabbPhantom*
         bhkAabbPhantom* unk1C; // bhkAabbPhantom*
         TESObjectCELL* unk20; // TESObjectCELL*
         TESObjectCELL* cell;  // 24
         UInt32 unk28;
         UInt32 unk2C;
         UInt32 unk30;
         UInt32 unk34;
         UInt32 unk38; // inited 0xDEADBEEF
         UInt32 unk3C;
         UInt32 unk40;
         NiPoint3 boundsMax; // 44
         NiPoint3 boundsMin; // 50
         float  unk5C; // ==   20.0 when observed
         float  unk60; // ==  600.0 when observed
         float  unk64; // == 1000.0 when observed
         float  unk68; // ==    0.3 when observed
   };
   STATIC_ASSERT(sizeof(LoadedAreaBound) == 0x6C);
   STATIC_ASSERT(offsetof(LoadedAreaBound, boundsMax) == 0x44);
};