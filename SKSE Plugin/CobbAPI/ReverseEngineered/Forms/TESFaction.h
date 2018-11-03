#pragma once
#include "skse/GameForms.h"

class BGSLoadFormBuffer;
namespace RE {
   class PackageLocation;
   //
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
         struct CrimeValues { // sizeof == 0x14
            bool   arrest; // 00
            bool   attackOnSight; // 01
            UInt16 murder;     // 02
            UInt16 assault;    // 04
            UInt16 trespass;   // 06
            UInt16 pickpocket; // 08
            float  stealMult;  // 0C
            UInt16 escape;     // 10
            UInt16 werewolf;   // 12
         };
         struct VendorData { // sizeof == 0x20
            struct Struct00A88CB0 { // sizeof == 0x4
               UInt32 unk00 = 0;
               //
               MEMBER_FN_PREFIX(Struct00A88CB0);
               DEFINE_MEMBER_FN(Load, void, 0x005E96D0, BGSLoadFormBuffer*);
            };
            //
            UInt16 startHour;				// 00
            UInt16 endHour;				// 02
            UInt32 radius;					// 04
            UInt8  onlyBuysStolenItems;	// 08
            UInt8  notSellBuy;				// 0A
            UInt16 pad0B;					// 0B
            PackageLocation* packageLocation;		// 0C // PLVD subrecord
            Struct00A88CB0*  unk10;					// 10 // conditions
            BGSListForm*   buySellList;			// 14
            TESObjectREFR* merchantContainer;	// 18
            UInt32 unk1C;					// 1C
         };
         struct Rank { // sizeof == 0x10
            UInt32 titleMasc;    // 00
            UInt32 titleFem;     // 04
            TESTexture insignia; // 08
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
         TESObjectREFR* jailExteriorMarker;   // 34 // JAIL subrecord
         TESObjectREFR*	followerWaitMarker;   // 38 // WAIT subrecord
         TESObjectREFR*	stolenGoodsContainer; // 3C // STOL subrecord
         TESObjectREFR*	jailPlayerInventoryContainer; // 40
         BGSListForm*   crimeGroups; // 44 // "crime group" TESFactions that this faction is a part of // CRGR subrecord
         BGSOutfit*     jailOutfit;  // 48 // JOUT subrecord
         CrimeValues crimeValues;	// 4C
         VendorData  vendorData;		// 60
         tList<Rank*> ranks;			// 80 // list of Rank instances
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