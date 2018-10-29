#pragma once
#include "skse/GameForms.h"
#include "skse/GameObjects.h"

class BGSLocationRefType;
class TESBoundObject;
namespace RE {
   //
   // Aliases aren't forms, but they're used like forms in Papyrus.
   //
   class BGSBaseAlias {
      public:
         virtual ~BGSBaseAlias();
         virtual void		Unk_01(void);
         virtual void		Unk_02(TESForm*); // general-purpose setter? can modify anything from the fill-form to the message to the spell and faction lists...
         virtual const char** Unk_03(void); // referencealias: &("Ref")
         //
         enum { kTypeID = kFormType_Alias };
         enum {
            kFlag_ReservesRef    = 0x0001,
            kFlag_Optional       = 0x0002,
            kFlag_QuestObject    = 0x0004,
            kFlag_AllowReuse     = 0x0008,
            kFlag_AllowDead      = 0x0010,
            kFlag_AllowDisabled  = 0x0080,
            kFlag_AllowReserved  = 0x0200,
            kFlag_AllowDestroyed = 0x1000,
            //
            // TODO: Uses Stored Text; Stores Text; Initially Disabled; Clears Name When Removed; Essential; Protected
         };
         //
         StringCache::Ref name;	// 04
         TESQuest* owner;		// 08
         UInt32 aliasId;			// 0C
         UInt32 flags;			// 10
   };
   class BGSRefAlias : public BGSBaseAlias {
      public:
         enum { kTypeID = kFormType_ReferenceAlias };
         enum FillType : UInt16 {
            kFillType_Unk0 = 0,
            kFillType_SpecificReference = 1,
            kFillType_Unk2 = 2,
            kFillType_CreatedReference = 4,
            kFillType_Unk5 = 5,
            kFillType_UniqueActor = 6,
         };
         //
         FillType unk14; // 14
         UInt16 unk16;
         union {
            UInt32    refHandle; // fill type 1
            TESBoundObject* boundObject; // fill type 2
            TESForm*  baseForm;  // fill type 4
            TESQuest* quest;     // fill type 5
            TESNPC*   actorBase; // fill type 6
         } unk18; // 18
         union {
            BGSLocationRefType* refType; // fill type 2 only?
            UInt32 aliasID; // fill type 4 (create new reference at sibling alias)
         } unk1C; // 1C
         UInt32 unk20; // 20 // unique actor: 0
         UInt32 unk24; // 24 // referenced in Unk_02

         bool IsPlayer() const {
            switch (this->unk14) {
               case kFillType_SpecificReference:
                  return this->unk18.refHandle == 0x100000;
               case kFillType_UniqueActor:
                  return (this->unk18.actorBase)->formID == 0x7;
            }
            return false;
         };
   };
   class BGSLocAlias : public BGSBaseAlias {
      public:
         enum { kTypeID = kFormType_LocationAlias };

         UInt32 unk14[0x08];
   };

   //
   // Alias flags:
   // 0x0000000A - optional, allow reuse
   // 0x0000001A - optional, allow reuse, allow dead
   // 0x00001298 (5 bits) - Unique actor; allow reuse, dead, disabled, reserved, destroyed
   //
};