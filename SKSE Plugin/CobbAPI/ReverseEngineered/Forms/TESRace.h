#pragma once
#include "skse/GameForms.h"
#include "skse/GameObjects.h"

namespace RE {
   class TESRace : public TESForm { // sizeof == 0x2BC
      public:
	      enum { kTypeID = kFormType_Race };
	      enum {
		      kRace_Playable						= 0x00000001,
		      kRace_FaceGenHead					= 0x00000002,
		      kRace_Child							= 0x00000004,
		      kRace_TiltFrontBack					= 0x00000008,
		      kRace_TiltLeftRight					= 0x00000010,
		      kRace_NoShadow						= 0x00000020,
		      kRace_Swims							= 0x00000040,
		      kRace_Flies							= 0x00000080,
		      kRace_Walks							= 0x00000100,
		      kRace_Immobile						= 0x00000200,
		      kRace_NotPushable					= 0x00000400,
		      kRace_NoCombatInWater				= 0x00000800,
		      kRace_NoRotatingToHeadTrack			= 0x00001000,
		      kRace_UseHeadTrackAnim				= 0x00008000,
		      kRace_SpellsAlignWithMagicNode		= 0x00010000,
		      kRace_UseWorldRaycasts				= 0x00020000,
		      kRace_AllowRagdollCollision			= 0x00040000,
		      kRace_CantOpenDoors					= 0x00100000,
		      kRace_AllowPCDialogue				= 0x00200000,
		      kRace_NoKnockdowns					= 0x00400000,
		      kRace_AllowPickpocket				= 0x00800000,
		      kRace_AlwaysUseProxyController		= 0x01000000,
		      kRace_OverlayHeadParts				= 0x04000000,
		      kRace_OverrideHeadParts				= 0x08000000,
		      kRace_AllowMultipleMembraneShaders	= 0x20000000,
		      kRace_CanDualWield					= 0x40000000,
		      kRace_AvoidsRoads					= 0x80000000,
	      };
         //
	      // Parents:
	      TESFullName			fullName;		// 014
	      TESDescription		description;	// 01C
	      TESSpellList		spellList;		// 028
	      BGSSkinForm			skin;			// 030
	      BGSBipedObjectForm	biped;			// 038
	      BGSKeywordForm		keyword;		// 044
	      BGSAttackDataForm	attackData;		// 050
         //
	      // Members:
	      enum { kRace_NumSkillBonuses = 7 };
	      struct Data { // sizeof == 0x84
		      struct SkillBonus {
			      UInt8 skill;
			      UInt8 bonus;
		      };
            //
		      SkillBonus skillBonus[kRace_NumSkillBonuses];
		      UInt8	pad0E[2];			// 0E
		      float	height[2];			// 10
		      float	weight[2];			// 18
		      UInt32	raceFlags;			// 20 - not init'd
		      float	health;				// 24
		      float	magicka;			// 28
		      float	stamina;			// 2C
		      float	carryweight;		// 30
		      float	mass;				// 34
		      float	accelRate;			// 38
		      float	deaccelRate;		// 3C
		      UInt32	unk40; // sneak detection size: 0, 1, 2, 3 == small, normal, large, very large
		      UInt32	unk44;				// 44 - init'd to FFFFFFFF
		      UInt32	unk48;				// 48 - init'd to FFFFFFFF - Flags?
		      float	injuredHealthPct;				// 4C - not init'd
		      float	unk50;	// 50 - init'd to FFFFFFFF
		      float	healthRegen;		// 54
		      float	manaRegen;			// 58
		      float	staminaRegen;		// 5C
		      float	unarmedDamage;		// 60
		      float	handReach;			// 64
		      UInt32	unk68;				// 68 - init'd to FFFFFFFF - Flags?
		      float	aimAngleTolerance;	// 6C
		      UInt32	unk70;				// 70 - init'd to 0
		      float	angleAcceleration;	// 74
		      float	angleTolerance;		// 78
		      UInt32	unk7C;			// 7C
		      UInt32	unk80;			// 80
		      UInt32	unk84;			// 84
		      float	unk88;			// 88
		      float	unk8C;
		      float	unk90;
		      float	unk94;
		      float	unk98;
		      float	unk9C;
		      float	unk100;
		      float	unk104;
		      float	unk10C;
	      };
         //
	      TESModel					models[2];			// 058
	      Data						data;				// 080
	      BGSTextureModel				textureModel[2];	// 12C
	      BGSBehaviorGraphModel		behaviorGraph[2];	// 154
	      StringCache::Ref			behaviorPath[2];	// 17C
	      StringCache::Ref			behaviorName[2];	// 184
	      BGSVoiceType				* voiceTypes[2];	// 18C
	      BGSBodyPartData				* bodyPartData;		// 190
	      TESForm						* decapitateArmor[2];	// 194
	      UnkArray					unk180[2];
	      void						* unk198[4];
	      void						* unk1A8[2]; // AttackAnimationArrayMap
	      StringCache::Ref			editorId;
	      BGSMaterialType				* impactMaterial;
	      BGSImpactDataSet			* meleeImpact;
	      BGSArtObject				* decapitateBloodArt;
	      BGSSoundDescriptorForm		* openCorpseSound;
	      BGSSoundDescriptorForm		* closeCorpseSound;
	      StringCache::Ref			bipedObjectNames[0x20];
	      tArray<BGSEquipSlot*>		slotRestrictions;
	      UInt32						unk274;
	      BGSEquipSlot				* unarmedEquipSlot;
	      TESRace						* morphRace;
	      TESRace						* armorRace;
	      UnkArray					unk284;
	      UnkArray					unk290;
	      UInt8						unk29C[0x18];

	      struct CharGenData {
		      struct TintOption {
			      UInt32					unk00;			// 00
			      TESTexture				texture;		// 04
			      BGSColorForm			* defaultColor;	// 0C
			      tArray<BGSColorForm*>	colors;			// 10
			      tArray<float>			alpha;			// 14
			      tArray<UInt32>			unk18;			// 18
		      };
		      UInt32	presetFlags[4][8];					// 00
		      UInt32	totalPresets[4];					// 80
		      tArray<TintOption*>		* tintData;			// 90
		      tArray<BGSTextureSet*>	* textureSet;		// 94
		      BGSTextureSet			* defaultTexture;	// 98
		      tArray<TESNPC*>			* presets;			// 9C
		      tArray<BGSColorForm*>	* colors;			// A0
		      BGSColorForm			* defaultColor;		// A4
		      tArray<BGSHeadPart*>	* headParts;		// A8
	      };

	      CharGenData					* chargenData[2];
   };
   static_assert(offsetof(TESRace, data) == 0x80, "TESRace::data is in the wrong place!");
   STATIC_ASSERT(offsetof(TESRace::CharGenData, tintData) == 0x90);
};