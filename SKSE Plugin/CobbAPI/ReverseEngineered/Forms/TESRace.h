#pragma once
#include "skse/GameForms.h"
#include "skse/GameObjects.h"

class TESObjectARMO;
namespace RE {
   enum BinaryGender { // 's how Bethesda set it up
      kBinaryGender_Male   = 0,
      kBinaryGender_Female = 1,
   };
   enum RaceDetectionSize : UInt32 {
      kDetectionSize_Small = 0,
      kDetectionSize_Medium,
      kDetectionSize_Large,
      kDetectionSize_VeryLarge
   };
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
	      struct Data { // sizeof == 0xA8
		      struct SkillBonus {
			      UInt8 skill;
			      UInt8 bonus;
		      };
            //
		      SkillBonus skillBonus[kRace_NumSkillBonuses];
		      UInt8  pad0E[2];    // 0E
		      float  height[2];   // 10 // indices: 0 = male; 1 = female
		      float  weight[2];   // 18 // indices: 0 = male; 1 = female
		      UInt32 raceFlags;   // 20 - not init'd
		      float  health;      // 24
		      float  magicka;     // 28
		      float  stamina;     // 2C
		      float  carryWeight; // 30
		      float  mass;        // 34
		      float  accelerationRate; // 38
		      float  decelerationRate; // 3C
            RaceDetectionSize	size;  // 40 // used for sneak/detection math
		      UInt32 unk44; // 44 // Head Biped Object // init'd to FFFFFFFF
		      UInt32 unk48; // 48 // Hair Biped Object // init'd to FFFFFFFF - Flags?
		      float	 injuredHealthPct; // 4C - not init'd
            UInt32 unk50; // 50 // Shield Biped Object // init'd to FFFFFFFF
		      float  healthRegen;   // 54
		      float  manaRegen;     // 58
		      float  staminaRegen;  // 5C
		      float  unarmedDamage; // 60
		      float  unarmedReach;  // 64
		      UInt32 unk68;         // 68 // Body Biped Object // init'd to FFFFFFFF - Flags?
		      float  aimAngleTolerance; // 6C
		      float  flightRadius = 0;  // 70
		      float  angleAcceleration; // 74
		      float  angleTolerance;    // 78
		      UInt32 raceFlags2; // 7C
		      UInt32 unk80;			// 80
		      UInt32 unk84;			// 84
		      float	unk88;			// 88
		      float	unk8C;
		      float	unk90;
		      float	unk94;
		      float	unk98;
		      float	unk9C;
		      float	unkA0;
		      float	unkA4;
		      float	unkA8;
	      };
         //
	      TESModel					models[2];			// 058
	      Data						data;				// 080
	      BGSTextureModel				textureModel[2];	// 12C
	      BGSBehaviorGraphModel		behaviorGraph[2];	// 154
	      StringCache::Ref			behaviorPath[2];	// 17C
	      StringCache::Ref			behaviorName[2];	// 184
	      BGSVoiceType*    voiceTypes[2]; // 18C // indices: 0 = male; 1 = female // if not specified, these default to the hardcoded voicetypes
	      BGSBodyPartData* bodyPartData;  // 190
         TESObjectARMO*   decapitateArmor[2];	// 194
	      UnkArray unk180[2]; // 180 // "Default Hair Colors" -- likely ColorForms; a set for males and a set for females
	      void* unk198[4];
	      void* unk1A8[2]; // AttackAnimationArrayMap
	      StringCache::Ref        editorId; // 1B0
	      BGSMaterialType*        impactMaterial; // 1B4
	      BGSImpactDataSet*       meleeImpact; // 1B8
	      BGSArtObject*           decapitateBloodArt; // 1BC
	      BGSSoundDescriptorForm* openCorpseSound; // 1C0
	      BGSSoundDescriptorForm* closeCorpseSound; // 1C4
         //
         // Next member is wrong? 1DC is the offset of whatever is defined in the NAM7 subrecord.
         //
	      StringCache::Ref        bipedObjectNames[0x20];
	      tArray<BGSEquipSlot*>   slotRestrictions;
	      UInt32                  unk274;
	      BGSEquipSlot*           unarmedEquipSlot; // 278
	      TESRace*                morphRace; // 27C
	      TESRace*                armorRace; // 280
	      UnkArray unk284;
	      UnkArray unk290;
	      UInt8    unk29C[0x18];

	      struct CharGenData { // sizeof == 0xAC
		      struct TintOption {
			      UInt32					unk00;			// 00
			      TESTexture				texture;		// 04
			      BGSColorForm* defaultColor;	// 0C
			      tArray<BGSColorForm*>	colors;			// 10
			      tArray<float>			alpha;			// 14
			      tArray<UInt32>			unk18;			// 18
		      };
		      UInt32	presetFlags[4][8];					// 00
		      UInt32	totalPresets[4];					// 80
		      tArray<TintOption*>*    tintData;			// 90
		      tArray<BGSTextureSet*>* detailTextureSetList;		// 94
		      BGSTextureSet* defaultTexture;	// 98
		      tArray<TESNPC*>* presets;			// 9C
		      tArray<BGSColorForm*>* hairColors;			// A0
		      BGSColorForm* defaultHairColor;		// A4
		      tArray<BGSHeadPart*>* headParts;		// A8

            MEMBER_FN_PREFIX(CharGenData);
            DEFINE_MEMBER_FN(Constructor, CharGenData*, 0x00576810);
	      };

	      CharGenData* chargenData[2]; // 2B4 // face data

         MEMBER_FN_PREFIX(TESRace);
         DEFINE_MEMBER_FN(AddFaceDetailsTextureSet, void, 0x00578490, BinaryGender which, BGSTextureSet*); // aborts if given null pointer // appends to CharGenData::detailTextureSetList
         DEFINE_MEMBER_FN(AddFaceTintLayer,         void, 0x005783C0, BinaryGender which, CharGenData::TintOption*); // aborts if given null pointer // appends to CharGenData::tintData
         DEFINE_MEMBER_FN(AddHairColor,             void, 0x005782F0, BinaryGender which, BGSColorForm*); // aborts if given null pointer // appends to CharGenData::hairColors
         DEFINE_MEMBER_FN(AddHeadPart,              void, 0x00578120, BinaryGender which, BGSHeadPart*); // aborts if given null pointer // appends to CharGenData::headParts
         DEFINE_MEMBER_FN(AddPresetNPC,             void, 0x00578220, BinaryGender which, TESNPC*); // aborts if given null pointer // appends to CharGenData::presets
         DEFINE_MEMBER_FN(SetDefaultFaceTexture,    void, 0x005768F0, BinaryGender which, BGSTextureSet*);
         DEFINE_MEMBER_FN(SetDefaultHairColor,      void, 0x00576890, BinaryGender which, BGSColorForm*);
   };
   static_assert(offsetof(TESRace, data) == 0x80, "TESRace::data is in the wrong place!");
   static_assert(offsetof(TESRace, chargenData) == 0x2B4, "TESRace::chargenData is in the wrong place!");
   STATIC_ASSERT(offsetof(TESRace::CharGenData, tintData) == 0x90);
};