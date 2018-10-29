#pragma once
#include "skse/GameObjects.h"

class Actor;
class BGSLoadGameBuffer;
class NiNode;
namespace RE {
   class ActiveEffect {
      public:
         enum { kTypeID = kFormType_ActiveMagicEffect };

         enum {
            kFlag_Inactive = 0x8000
         };

         virtual void ApplyPerkEffects(UInt32 arg1, MagicTarget* arg2); // 00 // modifies duration and magnitude based on the following perk entry points: mod spell duration; mod spell magnitude; mod incoming spell duration; mod incoming spell magnitude
         virtual void Unk_01(UInt32 arg1); // 01 // conditionally modifies flags at this->item->unk54
         virtual void Unk_02();   // 02 // no-op on base class
         virtual Actor* Unk_03(); // 03 // returns this->actorTarget->Unk_02();
         virtual void Unk_04(UInt32 arg1); // 04 // no-op on base class
         virtual void Unk_05(float arg1, UInt32 arg2); // 05 // related to GMST:fActiveEffectConditionUpdateInterval and StopHitEffectsVisitor; arg1 type assumed
         virtual bool Unk_06(); // 06 // no-op on base class; returns false
         virtual void Unk_07(UInt32 arg1); // 07 // no-op on base class
         virtual void Unk_08(void* arg1); // 08 // related to saving data
         virtual void Unk_09(BGSLoadGameBuffer* arg1); // 09
         virtual void Unk_0A(UInt32 arg1); // 0A
         virtual void Unk_0B(UInt32 arg1); // 0B // on the base class, just sets unk60 to 4
         virtual void Unk_0C(UInt32 arg1); // 0C // seems to print data about the effect somewhere; uses 1000.0F as defaults for duration, magnitude, etc., if some flags are set
         virtual void Unk_0D(UInt32 arg1); // 0D // no-op on base class
         virtual void Unk_0E(UInt32 arg1, UInt32 arg2); // 0E // no-op on base class
         virtual void Unk_0F(); // 0F // no-op on base class
         virtual bool Unk_10(); // 10 // checks a flag on the effect
         virtual bool Unk_11(); // 11 // no-op on base class; returns false
         virtual void Unk_12(); // 12 // no-op on base class
         virtual void Dispose(bool); // 13
         virtual void Unk_14(); // 14 // no-op on base class // judging by StaggerEffect, this is responsible for actually making the effect do a thing
         virtual void Unk_15(); // 15 // no-op on base class
         virtual bool Unk_16(); // 16
         virtual bool Unk_17(); // 17 // no-op on base class; returns true
         virtual float Unk_18(); // 18 // no-op on base class; returns 1.0

         //	void** _vtbl; // 00
         ActiveEffectReferenceEffectController controller; // 04
         UInt32   unk0C[7];    // 0C
         UInt32   actorCasterHandle; // 28
         NiNode*  niNode;      // 2C
         MagicItem* item;      // 30
         MagicItem::EffectItem* effect; // 34
         MagicTarget* actorTarget; // 38
         TESForm* sourceItem;  // 3C
         void*    unk40;       // 40 // pointer to struct UnkLinkedList { void* data; UnkLinkedList* next }
         UInt32   unk44;       // 44
         float    elapsed;     // 48
         float    duration;    // 4C
         float    magnitude;   // 50
         UInt32   flags;			// 54
         UInt32   unk58;			// 58
         UInt32   effectNum;		// 5C - Somekind of counter used to determine whether the ActiveMagicEffect handle is valid
         UInt32   unk60;			// 60
         UInt32   actorValue;		// 64 - Only seems to appear on value modifiers
         UInt32   unk68;			// 68
         UInt32   unk6C;			// 6C
   };
   class StaggerEffect : public ActiveEffect {
      public:
         virtual void Unk_14(); // 14 // the only override
   };
};