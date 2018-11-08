#pragma once

#include "skse/GameData.h"
#include "skse/GameEvents.h"
#include "skse/GameForms.h"
#include "skse/GameObjects.h"
#include "skse/GameReferences.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Systems/GameResources.h"

class bhkAabbPhantom;
class TESObjectCELL;

namespace RE {
   class PositionPlayerEvent;
   class Sky;

   class GridArray {
      public:
         GridArray();
         ~GridArray();
   };
   class GridCellArray : public GridArray { // sizeof == 0x24
      public:
         //GridCellArray(); // at 0x00474A60
         ~GridCellArray();
         //
         virtual void Unk_01(void); // sets this->unk04 = this->unk08 = -0x2710.
         virtual void Unk_02(); // Calls Unk_06 on all items
         virtual void Unk_03(void);
         virtual void Unk_04(void);
         virtual void Unk_05(void);
         virtual void Unk_06(UInt32 x, UInt32 y); // Unload or detach a cell? Removes it from the array via Unk_07 afterward.
         virtual void Unk_07(UInt32 x, UInt32 y); // Remove a cell from the array.
         virtual void Unk_08(UInt32 xSource, UInt32 ySource, UInt32 xTarget, UInt32 yTarget); // copies a cell from one slot to another
         virtual void Unk_09(UInt32 x1, UInt32 y1, UInt32 x2, UInt32 y2); // swaps the two cells within the array
         virtual void Unk_0A(bool);
         //
         UInt32 unk04;
         UInt32 unk08; // bitmask?
         UInt32 length; // 0C // As in, the length of a side -- the length of a row or column.
         TESObjectCELL** cells; // 10
         float  unk14 = 0;
         float  unk18 = 0;
         UInt32 unk1C = 0;
         bool   unk20 = false; // if true, then all cells exist?
         UInt8  pad21;
         UInt8  pad22;
         UInt8  pad23;
         //
         TESObjectCELL** GetItem(UInt32 x, UInt32 y) { // based on 0x004743F0
            UInt32 length = this->length;
            if (x >= length || y >= length)
               return nullptr;
            return &(this->cells[(x * length) + y]);
         };
         //
         MEMBER_FN_PREFIX(GridCellArray);
         DEFINE_MEMBER_FN(GetItem,        TESObjectCELL**, 0x004743F0, UInt32 x, UInt32 y);
         DEFINE_MEMBER_FN(SetItem,        TESObjectCELL**, 0x00474420, UInt32 x, UInt32 y, TESObjectCELL* cell);
         DEFINE_MEMBER_FN(IsCellAttached, bool,            0x00474470, UInt32 x, UInt32 y);
         //
         DEFINE_MEMBER_FN(Subroutine004744B0, bool, 0x004744B0); // Returns false if any cell in the array is NULL or has an unk30 != 4.
   };

   class TES {
      public:
         virtual ~TES();
         //
         struct ActorBaseDeathCount {
            struct Data {
               TESActorBase* form;
               UInt16 deathCount;
               //
               // ...?
            };
            Data* data;
            ActorBaseDeathCount* next;
         };
         //
         UInt32 unk04;
         UInt32 unk08;
         UInt32 unk0C;
         UInt32 unk10;
         UInt32 unk14;
         UInt32 unk18;
         UInt32 unk1C;
         UInt32 unk20;
         UInt32 unk24;
         UInt32 unk28;
         UInt32 unk2C;
         UInt32 unk30;
         BSTEventSink<BSResource::ArchiveStreamOpenedEvent> unk34;
         BSTEventSink<PositionPlayerEvent> unk38;
         UInt32 unk3C;
         GridCellArray* gridCellArray; // 40
         NiNode* objectLODRoot; // 44
         NiNode* landLOD; // 48
         NiNode* waterLOD; // 4C
         UInt32 tempNodeManager; // BSTempNodeManager
         UInt32 unk54;
         UInt32 unk58;
         UInt32 currentCellGridX; // 5C // == 7FFFFFFF if interior // sometimes read as an SInt16 // cell X?
         UInt32 currentCellGridY; // 60 // == 7FFFFFFF if interior // sometimes read as an SInt16 // cell Y?
         UInt32 unk64; // 7FFFFFFF
         UInt32 unk68; // 7FFFFFFF
         TESObjectCELL* currentCell; // 6C
         TESObjectCELL**  interiorCellBuffer; // idk, visited cells perhaps?
         UInt32 unk74;
         UInt32 unk78; // 0
         UInt32 unk7C; // 0
         UInt32 unk80; // 7FFFFFFF
         UInt32 unk84; // 7FFFFFFF
         UInt32 unk88;
         UInt32 unk8C;
         UInt32 unk90;
         Sky*   sky; // Sky
         UInt32 imageSpaceModifier; // ImageSpaceModifierInstanceForm // pointer?
         UInt32 unk9C; // ImageSpaceModifierInstanceDOF ** ??
         UInt32 unkA0;
         UInt32 unkA4;
         UInt8  unkA8;
         UInt8  unkA9;
         UInt8  unkAA;
         UInt8  unkAB;
         UInt8  unkAC;
         UInt8  unkAD;
         UInt8  unkAE;
         UInt8  unkAF;
         UInt8  unkB0;
         UInt8  padB1[3];
         float  unkB4; // 4579A000
         float  unkB8; // 457D2000
         TESWorldSpace* worldSpace;       // BC // TESWorldSpace
         ActorBaseDeathCount deathCounts; // C0
         UInt32 queuedFile;               // C8 // QueuedFile
         NiSourceTexture* someTexture;    // CC
         UInt32 queuedFile1;              // D0 // QueuedFile
         UInt32 queuedFile2;              // D4
         UInt32 unkD8;                    // D8 // BSFadeNode ** ??
         UInt32 unkDC;                    // DC
         UInt32 navMeshInfoMap;           // E0 // NavMeshInfoMap
         LoadedAreaBound* loadedAreaBound;
         //
         MEMBER_FN_PREFIX(TES);
         DEFINE_MEMBER_FN(CheckCellLoaded,         bool,    0x004312C0, TESObjectCELL* cellToCheck, UInt32 usuallyZero);
         DEFINE_MEMBER_FN(DeleteActorBaseDeathCounts, void, 0x00432280);
         DEFINE_MEMBER_FN(GetActorBaseDeathCount,  SInt16,  0x004322E0, TESActorBase*);
         DEFINE_MEMBER_FN(GetCurrentCell,          TESObjectCELL*, 0x00433490); // checks currentCell and currentCellGrid(X|Y); can return nullptr
         DEFINE_MEMBER_FN(ModActorBaseDeathCount,  void,    0x00433BF0, TESActorBase*, SInt16 changeByHowMuch);
         DEFINE_MEMBER_FN(GetCurrentWorldspace,    TESWorldSpace*, 0x004317A0);
         DEFINE_MEMBER_FN(Subroutine004320C0,      void*,   0x004320C0, TESObjectCELL*, UInt32);
         //
         ::TESObjectCELL** CopyGridCells(UInt32* count) const;
   };
   extern TES** g_TES;

   // Note: ModInfo is actually the wrong size. Sizeof should be 0x434.
   //       Constructor is at 0x00446AA0. An instance is created in 0x00446EC0, 
   //       which also appears to create DataHandler's unk008.
   typedef ::ModInfo TESFile;

   class TESDataHandler;
   typedef TESDataHandler DataHandler;
   class TESDataHandler {
      public:
         static DataHandler* GetSingleton();
         //
         // this + ((formType + 1) * 0xC) == address of a tArray of that form, though it might be empty
         UInt32                       unk000;
         UInt32                       unk004;             // 0004 // linked list; item->unk04 is the next entry pointer
         UnkFormArray unk008;
         UnkFormArray unk014;
         UnkFormArray unk020;
         UnkFormArray unk02C;
         tArray<BGSKeyword*>          keywords;           // 0038
         tArray<BGSLocationRefType*>  locRefTypes;        // 0044
         tArray<BGSAction*>           actions;            // 0050
         tArray<BGSTextureSet*>       textureSets;        // 005C
         tArray<BGSMenuIcon*>         menuIcons;
         tArray<TESGlobal*>           globals;
         tArray<TESClass*>            classes;            // 0080
         tArray<TESFaction*>          factions;
         tArray<BGSHeadPart*>         headParts;
         tArray<TESEyes*>             eyes;
         tArray<TESRace*>             races;              // 00B0
         tArray<TESSound*>            sounds;
         tArray<BGSAcousticSpace*>    acousticSpaces;
         UnkFormArray                 unkSkills;
         tArray<EffectSetting*>       magicEffects;       // 00E0
         tArray<Script*>              scripts;
         tArray<TESLandTexture*>      landTextures;
         tArray<EnchantmentItem*>     enchantments;
         tArray<SpellItem*>           spellItems;         // 0110
         tArray<ScrollItem*>          scrolls;            // 011C
         tArray<TESObjectACTI*>       activators;         // 0128
         tArray<BGSTalkingActivator*> talkingActivators;
         tArray<TESObjectARMO*>       armors;             // 0140
         tArray<TESObjectBOOK*>	     books;
         tArray<TESObjectCONT*>       containers;
         tArray<TESObjectDOOR*>       doors;
         tArray<IngredientItem*>      ingredients;        // 0170
         tArray<TESObjectLIGH*>       lights;
         tArray<TESObjectMISC*>       miscObjects;
         tArray<BGSApparatus*>        apparatuses;
         tArray<TESObjectSTAT*>       statics;            // 01A0
         tArray<BGSStaticCollection*> staticCollections;
         tArray<BGSMovableStatic*>    movableStatics;
         tArray<TESGrass*>            grasses;
         tArray<TESObjectTREE*>       trees;              // 01D0
         tArray<TESFlora*>            flora;
         tArray<TESFurniture*>        furniture;
         tArray<TESObjectWEAP*>       weapons;
         tArray<TESAmmo*>             ammo;               // 0200
         tArray<TESNPC*>              npcs;
         tArray<TESLevCharacter*>     levCharacters;
         tArray<TESKey*>              keys;
         tArray<AlchemyItem*>         potions;            // 0230
         tArray<BGSIdleMarker*>       idleMarkers;
         tArray<BGSNote*>             notes;
         tArray<BGSConstructibleObject*> constructibles;
         tArray<BGSProjectile*>       projectiles;        // 0260
         tArray<BGSHazard*>           bgsHazards;
         tArray<TESSoulGem*>          soulGems;
         tArray<TESLevItem*>          levItems;
         tArray<TESWeather*>          weather;            // 0290
         tArray<TESClimate*>          climates;
         tArray<BGSShaderParticleGeometryData*>	shaderParticleGeometryData;
         tArray<BGSReferenceEffect*>  referenceEffects;
         tArray<TESRegion*>           regions;            // 02C0
         tArray<NavMeshInfoMap*>      navMeshInfoMaps;
         tArray<TESObjectCELL*>       cells;
         tArray<TESObjectREFR*>       refs;	// could be actors
         tArray<Character*>           characters;         // 02F0
         tArray<MissileProjectile*>   missleProjs;
         tArray<ArrowProjectile*>     arrowProjs;
         tArray<GrenadeProjectile*>   grenadeProjs;
         tArray<BeamProjectile*>      beamProjs;          // 0320
         tArray<FlameProjectile*>     flameProjs;
         tArray<ConeProjectile*>      coneProjs;
         tArray<BarrierProjectile*>   barrierProjs;
         tArray<Hazard*>              hazards;            // 0350
         tArray<TESWorldSpace*>       worldSpaces;
         tArray<TESObjectLAND*>       lands;
         tArray<NavMesh*>             navMeshes;
         UnkFormArray                 unkTLOD;            // 0380
         tArray<TESTopic*>            topics;
         tArray<TESTopicInfo*>        topicInfos;
         tArray<TESQuest*>            quests;
         tArray<TESIdleForm*>         idleForms;          // 03B0
         tArray<TESPackage*>          packages;
         tArray<TESCombatStyle*>      combatStyles;
         tArray<TESLoadScreen*>       loadScreens;
         tArray<TESLevSpell*>         levSpells;          // 03E0
         tArray<TESObjectANIO*>       anios;
         tArray<TESWaterForm*>        waterForms;
         tArray<TESEffectShader*>     effectShaders;
         UnkFormArray                 unkTOFTs;           // 0410
         tArray<BGSExplosion*>        explosions;
         tArray<BGSDebris*>           debris;
         tArray<TESImageSpace*>       imageSpaces;
         tArray<TESImageSpaceModifier*> imageSpaceModifiers; // 0440
         tArray<BGSListForm*>         listForms;
         tArray<BGSPerk*>             perks;
         tArray<BGSBodyPartData*>     bodyPartData;
         tArray<BGSAddonNode*>        addonNodes;         // 0470
         tArray<ActorValueInfo*>      actorValueInfos;
         tArray<BGSCameraShot*>       cameraShots;
         tArray<BGSCameraPath*>       cameraPaths;
         tArray<BGSVoiceType*>        voiceTypes;         // 04A0
         tArray<BGSMaterialType*>     materialTypes;
         tArray<BGSImpactData*>       impactData;
         tArray<BGSImpactDataSet*>    impactDataSets;
         tArray<TESObjectARMA*>       armorAddons;        // 04D0
         tArray<BGSEncounterZone*>    encounterZones;
         tArray<BGSLocation*>         locations;
         tArray<BGSMessage*>          messages;
         tArray<BGSRagdoll*>          ragdolls;           // 0500
         UnkFormArray                 unkDOBJs;
         tArray<BGSLightingTemplate*> lightingTemplates;
         tArray<BGSMusicType*>        musicTypes;
         tArray<BGSFootstep*>         footsteps;          // 0530
         tArray<BGSFootstepSet*>      footstepSets;
         tArray<BGSStoryManagerBranchNode*> branchNodes;
         tArray<BGSStoryManagerQuestNode*>  questNodes;
         tArray<BGSStoryManagerEventNode*>  eventNodes;   // 0560
         tArray<BGSDialogueBranch*>         dialogBranches;
         tArray<BGSMusicTrackFormWrapper*>  musicTrackFormWrappers;
         UnkFormArray                 unkDLVWs;
         tArray<TESWordOfPower*>      wordOfPowers;       // 0590
         tArray<TESShout*>            shouts;
         tArray<BGSEquipSlot*>        equipSlots;
         tArray<BGSRelationship*>     relationships;
         tArray<BGSScene*>            scenes;             // 05C0
         tArray<BGSAssociationType*>  associationTypes;
         tArray<BGSOutfit*>           outfits;
         tArray<BGSArtObject*>        artObjects;
         tArray<BGSMaterialObject*>   materialObjects;    // 05F0
         tArray<BGSMovementType*>     movementTypes;
         tArray<BGSSoundDescriptorForm*> soundDescriptors;
         tArray<BGSDualCastData*>     dualCastData;
         tArray<BGSSoundCategory*>    soundCategories;    // 0620
         tArray<BGSSoundOutput*>      soundOutputs;
         tArray<BGSCollisionLayer*>   collisonLayers;
         tArray<BGSColorForm*>        colors;
         tArray<BGSReverbParameters*> reverbParams;       // 0650
         UInt32 unks[0x0E];                               // 065C // 03 Cell** 06 TESGlobal**
         ModList                      modList;            // 0694
         UInt32 moreunks[100];                            // 0A9C
         //
         MEMBER_FN_PREFIX(TESDataHandler);
         DEFINE_MEMBER_FN(GetFormByTypeAndIndex, TESForm*, 0x0043BCB0, UInt8 formType, UInt32 index);
         DEFINE_MEMBER_FN(GetNextID,   void,   0x0043B6D0); // Arguments and return type unexamined. Name comes from a debug logging command.
         DEFINE_MEMBER_FN(LoadScripts, UInt32, 0x0043D100);
         //
         DEFINE_MEMBER_FN(Subroutine0043EF60, TESObjectCELL*, 0x0043EF60, UInt32, UInt32, TESWorldSpace*, UInt32);
         DEFINE_MEMBER_FN(Subroutine0043FEC0, void, 0x0043FEC0, TESForm*); // register an executable-created non-temporary form?
   };
   static_assert(offsetof(DataHandler, spellItems) >= 0x110, "RE::TESDataHandler::spellItems is too early!");
   static_assert(offsetof(DataHandler, spellItems) <= 0x110, "RE::TESDataHandler::spellItems is too late!");
};