#pragma once
#include "skse/GameForms.h"

namespace RE {
   enum { // per Creation Kit docs
      kFactionReaction_Neutral = 0,
      kFactionReaction_Enemy   = 1,
      kFactionReaction_Ally    = 2,
      kFactionReaction_Friend  = 3,
   };
   class TESFaction : public TESForm { // sizeof == 0x98
      public:
         enum { kTypeID = kFormType_Faction };

         // Parents:
         TESFullName     fullName; // 14
         TESReactionForm reaction; // 1C

         // Members:
         struct CrimeValues { // sizeof == 0x2C
            TESObjectREFR* unk00; // 00 // related to jail
            UInt32	unk04;				// 04
            UInt32	unk08;				// 08
            TESObjectREFR*	jailPlayerInventoryContainer; // 0C
            BGSListForm* crimeGroups; // 10 // "crime group" TESFactions that this faction is a part of
            UInt32	unk14;				// 14
            bool	arrest;				// 18
            bool	attackOnSight;		// 19
            UInt16	murder;				// 1A
            UInt16	assault;			// 1C
            UInt16	trespass;			// 20
            UInt16	pickpocket;			// 22
            float	stealMult;			// 24
            UInt16	escape;				// 28
            UInt16	werewolf;			// 2A
         };
         struct VendorData { // sizeof == 0x20
            UInt16 startHour;				// 00
            UInt16 endHour;				// 02
            UInt32 radius;					// 04
            UInt8  onlyBuysStolenItems;	// 08
            UInt8  notSellBuy;				// 0A
            UInt16 pad0B;					// 0B
            void*  packageLocation;		// 0C
            UInt32 unk10;					// 10
            BGSListForm*   buySellList;			// 14
            TESObjectREFR* merchantContainer;	// 18
            UInt32 unk1C;					// 1C
         };
         struct Data80 { // sizeof == 0x8
            UInt32 unk00; // 00
            UInt32 unk04; // 04
         };
         enum {
            kFactionFlag_HiddenFromNPC  = (1 << 0),
            kFactionFlag_SpecialCombat  = (1 << 1),
            kFactionFlag_PlayerExpelled = (1 << 2),
            kFactionFlag_Unk00000008    = (1 << 3), // 00000008 // related to GMST:iCrimeEnemyCoolDownTimer and unk94
            kFactionFlag_TrackCrime     = (1 << 4),
            kFactionFlag_IgnoreMurder   = (1 << 5),
            kFactionFlag_IgnoreAssult   = (1 << 6),
            kFactionFlag_IngoreStealing = (1 << 7),
            kFactionFlag_IgnoreTrespass = (1 << 8),
            kFactionFlag_NoReportCrime  = (1 << 9),
            kFactionFlag_CrimeGoldDefaults = (1 << 10),
            kFactionFlag_IgnorePickpocket  = (1 << 11),
            kFactionFlag_Vendor         = (1 << 12),
            kFactionFlag_CanBeOwner     = (1 << 13),
            kFactionFlag_IgnoreWerewolf = (1 << 14)
         };

         UInt32 unk2C;	// 2C
         UInt32 factionFlags;	// 30
         CrimeValues crimeValues;	// 34
         VendorData  vendorData;		// 60
         Data80 unk80;			// 80
         UInt32 unk88;			// 88
         UInt32 unk8C;			// 8C
         float  unk90;			// 90 // if FLT_MIN, it's not used // this is a time, possibly the time at which the player resisted arrest; it's related to GMST:fResistArrestTimer and to unk94
         float  unk94;			// 94 // if FLT_MAX, it's not used // related to GMST:iCrimeEnemyCoolDownTimer

         MEMBER_FN_PREFIX(TESFaction);
         DEFINE_MEMBER_FN(GetBounty,       SInt32, 0x0055D2F0, TESNPC*);
         DEFINE_MEMBER_FN(GetReaction,     SInt32, 0x0055C1B0, TESFaction*);
         DEFINE_MEMBER_FN(IsInCrimeGroup,  bool,   0x0055C540, TESFaction* crimeGroup);
         DEFINE_MEMBER_FN(ModifyFlags,     void,   0x0055C180, UInt32 flagsBitmask, bool state); // use to set or unset flags; correctly sets FACTION_FLAGS changeflag on the form
         DEFINE_MEMBER_FN(SetPlayerEnemy,  void,   0x0055CFB0, bool makeEnemy);
         DEFINE_MEMBER_FN(SetReaction,     void,   0x0055C1C0, TESFaction*, SInt32 reaction);
         DEFINE_MEMBER_FN(UpdateUnk90Time, void,   0x0055CF30); // sets unk90 to a value that appears to represent "now," currently listed as RE::g_timeNow
   };
   STATIC_ASSERT(sizeof(TESFaction) == 0x98);
   STATIC_ASSERT(offsetof(TESFaction, vendorData) == 0x60);
};