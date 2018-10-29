#pragma once
#include "ReverseEngineered/Forms/Actor.h"

#include "skse/Utilities.h"

namespace RE {
   class  PlayerCharacter;
   extern constexpr PlayerCharacter** const g_thePlayer = (PlayerCharacter**) 0x01B2E8E4;

   class PlayerCharacter : public Character { // sizeof == 0x72C
      public:
         virtual ~PlayerCharacter();
         // virtual SInt32 GetBountyViolent(TESFaction*);    // Virtual 0x12A
         // virtual SInt32 GetBountyNonViolent(TESFaction*); // Virtual 0x12B
         // virtual void   Unk_12C(TESFaction*);             // Virtual 0x12C // related to paying the player's bounty

         struct Unk494 {
            float  unk00 = -1;
            UInt32 unk04 = 0; // ref handle
            UInt32 unk08 = 0x10;
         };

         // parents
         BSTEventSink<void*>   menuOpenCloseEvent;			// 19C .?AV?$BSTEventSink@VMenuOpenCloseEvent@@@@
         BSTEventSink<void*>   menuModeChangeEvent;		// 1A0 .?AV?$BSTEventSink@VMenuModeChangeEvent@@@@
         BSTEventSink<void*>   userEventEnabledEvent;		// 1A4 .?AV?$BSTEventSink@VUserEventEnabledEvent@@@@
         BSTEventSource<void*> actorCellEventSource;		// 1A8 .?AV?$BSTEventSource@UBGSActorCellEvent@@@@
         BSTEventSource<void*> actorDeathEventSource;		// 1D8 .?AV?$BSTEventSource@UBGSActorDeathEvent@@@@
         BSTEventSource<void*> positionPlayerEventSource;	// 208 .?AV?$BSTEventSource@UPositionPlayerEvent@@@@

         UInt32	pad238[(0x2A4 - 0x238) >> 2];	// 238

         NiPoint3 unk2A4; // 2A4 - initialized from (NiPoint3*)0x01B910A4
         NiPoint3 unk2B0; // 2B0 - initialized from (NiPoint3*)0x01B910A4
         NiPoint3 unk2BC; // 2BC - initialized from (NiPoint3*)0x01B910A4
         UInt32   unk2C8;
         UInt32   unk2CC;
         UnkFormArray unk2D0; // perks?
         UnkFormArray unk2DC;
         UnkFormArray unk2E8; // perks?
         UnkFormArray unk2F4;
         UnkFormArray unk300;
         UnkFormArray unk30C;
         UnkFormArray unk318;
         UnkFormArray unk324;
         UInt32       unk330 = 0;
         UInt32       unk334 = 0;
         UnkFormArray unk338;
         float  unk344;
         float  unk348;
         UInt32 unk34C = 0;
         UInt32 unk350 = 0;
         UInt32 unk354 = 0;
         UInt32 unk358 = 0xDEADBEEF;
         float  unk35C;
         UInt32 unk360 = 0;
         UInt32 unk364;
         UInt32 unk368;
         UInt32 unk36C = 0;
         UInt32 unk370 = 0;
         UInt32 unk374 = 0;
         UInt32 unk378 = 0xDEADBEEF;
         UInt32 unk37C;
         UInt32 unk380 = 0;
         struct {
            UInt32 unk00 = 0;
            UInt32 unk04 = 0;
         } unk384;
         UInt32 unk38C;        // NiTMapBase...
         UInt32 unk390 = 0x25; //
         UInt32 unk394;        //
         UInt32 unk398 = 0;    // ...end of.
         UInt32 unk39C = 0;
         NiPoint3 wentInsideAt; // 3A0 // all values initialized to FLT_MAX; player's last-known outdoor position; updated whenever the player passes through a loading screen that takes them from an exterior to an interior
         struct Unk3AC {
            UInt32 unk00 = 0;
            TESObjectCELL* unk04 = nullptr; // 04
            NiPoint3 unk08; // initialized to (NiPoint3*)0x01B910A4
            NiPoint3 unk14; // initialized to (NiPoint3*)0x01B910A4
            UInt32 unk20 = 0;
            UInt32 unk24 = 0;
            UInt32 unk28 = 0; // ref handle
            UInt32 unk2C = 0; // ref handle
            bool   unk30 = false;
            bool   unk31 = true;
            bool   unk32 = true;
            UInt8  unk33; // padding?
            //
            MEMBER_FN_PREFIX(Unk3AC);
            DEFINE_MEMBER_FN(Subroutine007475F0, void, 0x007475F0); // reset?
         } unk3AC; // this struct layout is used elsewhere, but it doesn't have VTBL/RTTI, so I can't ID it
         UInt32 unk3E0 = -1;
         UInt32 unk3E4 = 0; // these could be individual bytes
         UInt32 unk3E8 = 0;
         UInt32 unk3EC = -1;
         UInt32 unk3F0 = 0; // these could be individual bytes
         UInt32 unk3F4 = 0;
         UInt32 unk3F8 = -1;
         UInt32 unk3FC = 0; // these could be individual bytes
         UInt32 unk400 = 0;
         UInt32 unk404 = 0;
         UInt32 unk408[(0x420 - 0x408) / 4];
         UInt32 unk420 = 0xFF7FFFFF;
         UInt32 unk424;
         UInt32 unk428;
         TESFaction* unk42C = nullptr; // 42C // faction currently arresting the player?
         UInt32 unk430 = 0;
         UInt32 unk434 = 0;
         UInt32 unk438[(0x48C - 0x438) / 4];
         UInt32 unk48C = 0;
         UInt32	unk490;							// 490 - Handle // own handle?
         Unk494 unk494[0xE];                    // 494; sizeof each is 0xC
         UInt32	pad53C[(0x54C - 0x53C) >> 2]; // 53C
         UInt32	unk54C; // 54C - Handle; PlayerCharacter::Subroutine0073D4B0 returns the actor and clears the handle
         UInt32	pad550[(0x568 - 0x550) >> 2];
         UInt32	unk568; // 568 - Handle
         UInt32	pad56C[(0x58C - 0x56C) >> 2];	// 56C
         NiNode*  firstPersonSkeleton; // 58C
         UInt32	pad590[(0x5AC - 0x590) >> 2];
         UInt32	lastRiddenHorseHandle;			// 5AC - Handle
         UInt32	pad5B0[(0x5FC - 0x5B0) >> 2];
         TESObjectCELL* unk5FC; // 5FC // redundant player cell? can be an interior.
         UInt32   unk600;
         UInt32   unk604;
         UInt32   unk608;
         UInt32   unk60C;
         UInt32   unk610;
         PlayerSkills* skills;					// 614
         UInt32	targetHandle;					// 618
         UInt32	unk61C;							// 61C
         UInt32	unk620;							// 620
         UInt32	unk624;							// 624
         UInt32	unk628;							// 628
         UInt32	unk62C;							// 62C
         tArray<UInt32>	hostileHandles;			// 630
         UInt32	unk63C;							// 63C
         UInt32	unk640;							// 640
         TESForm* tempPoison;					// 644
         UInt32	numTeammates;					// 648
         UInt32	pad64C[(0x6B0 - 0x64C) >> 2];
         BGSLocation* unk6B0; // 6B0
         UInt32	pad6B4[(0x6E0 - 0x6B4) >> 2];
         UInt8	unk6E0;							// 6E0
         UInt8	numPerkPoints;					// 6E1
         UInt16 unk6E2;							// 6E2
         UInt32	unk6E4;							// 6E4
         tArray<TintMask*>	tintMasks;			// 6E8		// These are the actual tints
         tArray<TintMask*> overlayTintMasks;	// 6F4		// These apply when overlay head parts is enabled
         UInt32 unk700 = 0;
         UInt32 unk704 = 0;
         UInt32 unk708 = 0x80000000;
         UInt32 unk70C;
         UInt32 unk710;
         UnkFormArray unk714;
         UInt32 unk720 = 0;
         UInt8  unk724; // bitmask
         bool   unk725 = true;
         bool   unk726 = true; // 726
         UInt8  unk727; // 727 // bitmask; PlayerCharacter::Unk_71 returns unk727 & 1; value is related to flags on actor's root nodes, somehow; value is related to whether the player is AI driven, too
         UInt8  unk728 = 4;
         UInt8  unk729; // 729 // bitmask
         UInt8  unk72A; // padding?
         UInt8  unk72B; // padding?

         // Overlayed tints should be the same as original tints
         // occasionally they can have no type so index matching
         // is required to set anything on the tint
         TintMask* GetOverlayTintMask(TintMask * original);

         // Confirmed - Same as ExtraContainerChanges::EntryData
         // This type is used by scaleform to extend data
         // It can be used to extend more of the "ExtraData"
         /*struct ObjDesc
         {
         TESForm					* form;
         tList<BaseExtraList>	* extraData;
         SInt32					countDelta;

         MEMBER_FN_PREFIX(ObjDesc);
         DEFINE_MEMBER_FN(GenerateName, const char *, 0x00475AA0);
         };*/

         MEMBER_FN_PREFIX(PlayerCharacter);
         DEFINE_MEMBER_FN(GetTintList, tArray<TintMask*>*, 0x0055FF90);
         DEFINE_MEMBER_FN(GetNumTints, UInt32, 0x00735F40, UInt32 tintType);
         DEFINE_MEMBER_FN(GetTintMask, TintMask*, 0x00735F00, UInt32 tintType, UInt32 index);
         DEFINE_MEMBER_FN(GetDamage, double, 0x00730810, InventoryEntryData * pForm);
         DEFINE_MEMBER_FN(GetArmorValue, double, 0x007307E0, InventoryEntryData * pForm);
         //
         DEFINE_MEMBER_FN(GetInfamy,                 SInt32, 0x00744100, TESFaction*); // get lifetime bounty for faction
         DEFINE_MEMBER_FN(GetInfamyViolent,          SInt32, 0x00744160, TESFaction*); // get lifetime bounty for faction (violent crimes)
         DEFINE_MEMBER_FN(GetInfamyNonViolent,       SInt32, 0x007441B0, TESFaction*); // get lifetime bounty for faction (non-violent crimes)
         DEFINE_MEMBER_FN(GetStolenItemValueCrime,   SInt32, 0x00744360, TESFaction*); // total value of all items the player was witnessed stealing from the faction
         DEFINE_MEMBER_FN(GetStolenItemValueNoCrime, SInt32, 0x00744310, TESFaction*); // total value of all items the player stole from the faction without being caught
         //
         DEFINE_MEMBER_FN(HandleLowHealthHeartbeat,          void, 0x00671690, float healthPercentage);
         DEFINE_MEMBER_FN(HandleLowHealthImagespaceModifier, void, 0x00733A60, float healthPercentage);
         DEFINE_MEMBER_FN(HandleLowStatTutorials, void, 0x00737C90, UInt32 avIndex, float avValuePercentage);
   };
   static_assert(sizeof(PlayerCharacter) <= 0x72C, "RE::PlayerCharacter is too large.");
   static_assert(sizeof(PlayerCharacter) >= 0x72C, "RE::PlayerCharacter is too small.");
   static_assert(offsetof(PlayerCharacter, unk42C) <= 0x42C, "RE::PlayerCharacter::unk42C is too late.");
   static_assert(offsetof(PlayerCharacter, unk42C) >= 0x42C, "RE::PlayerCharacter::unk42C is too early.");
   static_assert(offsetof(PlayerCharacter, lastRiddenHorseHandle) <= 0x5AC, "RE::PlayerCharacter::lastRiddenHorseHandle is too late.");
   static_assert(offsetof(PlayerCharacter, lastRiddenHorseHandle) >= 0x5AC, "RE::PlayerCharacter::lastRiddenHorseHandle is too early.");
   static_assert(offsetof(PlayerCharacter, skills) <= 0x614, "RE::PlayerCharacter::skills is too late.");
   static_assert(offsetof(PlayerCharacter, skills) >= 0x614, "RE::PlayerCharacter::skills is too early.");
};