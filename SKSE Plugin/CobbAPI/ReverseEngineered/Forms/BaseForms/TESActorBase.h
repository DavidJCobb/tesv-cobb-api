#pragma once
#include "skse/GameObjects.h"

class TESActorBaseData : public BaseFormComponent { // sizeof == 0x38
   public:
      virtual void	Unk_04(UInt32 arg);
      virtual bool	GetFlag20000000(void);
      virtual bool	GetFlag80000000(void);
      virtual bool	Unk_07(void);
      virtual UInt32	GetUnk20(void);

      enum {
         kFlag_Female        = 0x00000001,
         kFlag_Essential     = 0x00000002,
         kFlag_IsPreset      = 0x00000004,
         kFlag_Respawn       = 0x00000008,
         kFlag_AutoCalcStats = 0x00000010,
         kFlag_Unique        = 0x00000020,
         kFlag_DoesntAffectStealthMeter = 0x00000040,
         kFlag_PCLevelMult   = 0x00000080,
         kFlag_UsesTemplate  = 0x00000100,
         //
         kFlag_Protected     = 0x00000800,
         //
         kFlag_Summonable    = 0x00004000,
         //
         kFlag_DoesntBleed   = 0x00010000,
         //
         kFlag_BleedoutOverride    = 0x00040000,
         kFlag_OppositeGenderAnims = 0x00080000,
         kFlag_SimpleActor         = 0x00100000,
         //
         kFlag_IsGhost = 0x20000000,
         //
         kFlag_IsInvulnerable = 0x80000000,
      };

      UInt32				flags;	// 04 - init'd to 0
      UInt16				unk08;	// 08 - init'd to 0
      UInt16				unk0A;	// 0A - init'd to 0
      UInt16				level;	// 0C - init'd to 1	(CK Multiplier * 1000 if PCLevelMult is true)
      UInt16				minLevel;	// 0E - init'd to 0
      UInt16				maxLevel;	// 10 - init'd to 0
      UInt16				unk12;	// 12 - init'd to 0x64 (100)
      UInt16				unk14;	// 14 - init'd to iBaseDisposition (35)
      UInt16				unk16;	// 16 - init'd to 0
      UInt16				unk18;	// 18 - init'd to 0
      UInt16				unk1A;	// 1A - init'd to 0
      UInt32				unk1C;	// 1C
      BGSVoiceType* voiceType;	// 20
      UInt32				unk24;	// 24
      UInt32				unk28;	// 28 - init'd to 0

      struct FactionInfo {
         TESFaction* faction;	// 00
         SInt8 rank;		// 04
         UInt8 pad[3];		// 05
      };

      tArray<FactionInfo>	factions;	// 2C

      MEMBER_FN_PREFIX(TESActorBaseData);
      DEFINE_MEMBER_FN(GetLevel, UInt16, 0x0044D4D0);
};
class TESActorBase : public TESBoundAnimObject { // sizeof == 0xC0
   public:
      // parents
      TESActorBaseData	actorData;	// 20
      TESContainer		container;	// 58
      TESSpellList		spellList;	// 64
      TESAIForm			aiForm;		// 6C
      TESFullName			fullName;	// 84
      ActorValueOwner				actorValueOwner;	// 8C
      BGSDestructibleObjectForm	destructible;		// 90
      BGSSkinForm			skinForm;	// 98
      BGSKeywordForm		keyword;	// A0
      BGSAttackDataForm	attackData;	// AC
      BGSPerkRankArray	perkRanks;	// B4

      virtual bool			Unk_52(void);
      virtual UInt32			Unk_53(void);
      virtual void			Unk_54(UInt32 arg);
      virtual TESActorBase *	Unk_55(void);
};
STATIC_ASSERT(offsetof(TESActorBase, keyword) == 0xA0);
STATIC_ASSERT(sizeof(TESActorBase) == 0xC0);