#pragma once

#include "Shared.h"
#include "skse/Utilities.h"

namespace RE {
   class ChangeForm {
      public:
         enum ChangeFlagsFORM {
            FORM_FLAGS           = 0x00000001,
            BASE_OBJECT_VALUE    = 0x00000002,
            BASE_OBJECT_FULLNAME = 0x00000004,
         };
         enum ChangeFlagsBOOK {
            TEACHES_SKILL = 0x00000020,
            HAS_BEEN_READ = 0x00000040,
         };
         enum ChangeFlagsCELL { // All valid flags are listed.
            //FORM_FLAGS     = 0x00000001, // this flag has the same meaning for all forms.
            CELL_FLAGS     = 0x00000002,
            FULLNAME       = 0x00000004,
            OWNERSHIP      = 0x00000008,
            EXTERIOR_SHORT = 0x10000000,
            EXTERIOR_CHAR  = 0x20000000,
            DETACHTIME     = 0x40000000,
            SEENDATA       = 0x80000000,
         };
         enum ChangeFlagsECZN { // encounter zone
            ZONE_FLAGS = 0x00000002,
            GAME_DATA  = 0x80000000,
         };
         enum ChangeFlagsFACT {
            FACTION_FLAGS     = 0x00000002,
            FACTION_REACTIONS = 0x00000004,
            CRIME_COUNTS      = 0x80000000,
         };
         enum ChangeFlagsFLST {
            ADDED_FORM = 0x80000000,
         };
         enum ChangeFlagsINGR {
            INGREDIENT_USE = 0x80000000
         };
         enum ChangeFlagsLCTN { // location
            KEYWORDDATA = 0x40000000,
            CLEARED = 0x80000000,
         };
         enum ChangeFlagsLeveledList {
            ADDED_OBJECT = 0x80000000
         };
         enum ChangeFlagsNPC_ {
            //FORM_FLAGS     = 0x00000001, // this flag has the same meaning for all forms.
            BASE_DATA      = 0x00000002, // on ActorBase?
            // ?           = 0x00000004,
            BASE_AIDATA    = 0x00000008, // on ActorBase?
            BASE_SPELLLIST = 0x00000010, // on ActorBase?
            BASE_FULLNAME  = 0x00000020, // on ActorBase?
            BASE_FACTIONS  = 0x00000040, // on ActorBase?
            // ?           = 0x00000080,
            // ?           = 0x00000100,
            SKILLS         = 0x00000200, // on NPC_
            // ?           = 0x00000400,
            FACE           = 0x00000800, // on NPC_
            DEFAULT_OUTFIT = 0x00001000, // on NPC_
            SLEEP_OUTFIT   = 0x00002000,
            // ?           = 0x00004000,
            // ?           = 0x00008000,
            // ?           = 0x00010000,
            // ?           = 0x00020000,
            // ?           = 0x00040000,
            // ?           = 0x00080000,
            // ?           = 0x00100000,
            // ?           = 0x00200000,
            // ?           = 0x00400000,
            // ?           = 0x00800000,
            GENDER         = 0x01000000,
            RACE           = 0x02000000, // on NPC_
            // ?           = 0x04000000,
            // ?           = 0x08000000,
            // ?           = 0x10000000,
            // ?           = 0x20000000,
            // ?           = 0x40000000,
            // ?           = 0x80000000
         };
         enum ChangeFlagsPACK {
            FLAG_WAITING   = 0x40000000,
            FLAG_NEVER_RUN = 0x80000000,
         };
         enum ChangeFlagsQUST { // All valid flags are listed.
            QUEST_FLAGS  = 0x00000001, // overrides FORM_FLAGS, apparently
            SCRIPT_DELAY = 0x00000004,
            ALREADY_RUN  = 0x04000000,
            INSTANCES    = 0x08000000,
            RUNDATA      = 0x10000000,
            OBJECTIVES   = 0x20000000,
            SCRIPT       = 0x40000000,
            STAGES       = 0x80000000
         };
         enum ChangeFlagsREFR {
            //FORM_FLAGS           = 0x00000001, // applies to any form?
            MOVE                 = 0x00000002,
            HAVOK_MOVE           = 0x00000004,
            CELL_CHANGED         = 0x00000008,
            SCALE                = 0x00000010,
            INVENTORY            = 0x00000020,
            EXTRA_OWNERSHIP      = 0x00000040,
            BASEOBJECT           = 0x00000080,
            // ?                 = 0x00000100,
            // ?                 = 0x00000200,
            EXTRA_ITEM_DATA      = 0x00000400,
            EXTRA_AMMO           = 0x00000800,
            EXTRA_LOCK           = 0x00001000,
            // ?                 = 0x00002000,
            // ?                 = 0x00004000,
            // ?                 = 0x00008000,
            // ?                 = 0x00010000,
            EXTRA_TELEPORT       = 0x00020000, // used for ADDED extra-data, not CHANGED extra-data. if this changeflag is set on a load door, that door will be stripped of its ExtraTeleport when switching to another savegame.
            // ?                 = 0x00040000,
            // ?                 = 0x00080000,
            // ?                 = 0x00100000,
            EMPTY                = 0x00200000,
            OPEN_DEFAULT_STATE   = 0x00400000,
            OPEN_STATE           = 0x00800000,
            // ?                 = 0x01000000,
            PROMOTED             = 0x02000000,
            EXTRA_ACTIVATING_CHILDREN = 0x04000000,
            LEVELED_INVENTORY    = 0x08000000,
            ANIMATION            = 0x10000000,
            EXTRA_ENCOUNTER_ZONE = 0x20000000,
            // ?                 = 0x40000000,
            EXTRA_GAME_ONLY      = 0x80000000
         };
         enum ChangeFlagsTopic {
            SAIDONCE = 0x80000000
         };
         enum ChangeFlagsACHR { // same as REFR; overrides
            LIFESTATE    = 0x00000400,
            PACKAGE_DATA = 0x00000800,
            MERCHANT_CONTAINER = 0x00001000,
            EXTRA_DISMEMBERED_LIMBS = 0x00020000,
            LEVELED_ACTOR = 0x00040000,
            DISPOSITION_MODIFIERS = 0x00080000,
            OVERRIDE_MODIFIERS = 0x00400000,
            PERMANENT_MODIFIERS = 0x00800000,
         };
         //
         struct Data {
            UInt32 flags = 0;
            UInt32 unk04 = 0; // error log strings in the game refer to this as a "buffer" or an "unloaded buffer" // it's passed as Arg1 to 0067ECF0
         };
         UInt32      formID = 0;
         Data        data;
         ChangeForm* next = nullptr;
   };

   typedef UInt32 UnknownChangeFormHashOrKey;

   static DEFINE_SUBROUTINE(void, Subroutine00A4A570, 0x00A4A570, UnknownChangeFormHashOrKey* outUnknown, UInt32 formID); // Apparently returns a hash, used to access a ChangeForm.

   class ChangeFormManager {
      public:
         struct Unk04 {
            UInt32 unk00;
            UInt32 unk04; // some kind of hash or key used to find where ChangeForms are stored?
            UInt32 count; // 08 // number of ChangeForms stored? or number available?
            UInt32 unk0C;
            ChangeForm* unk10; // 10
            UInt32 unk14;
            UInt32 unk18;
            //
            MEMBER_FN_PREFIX(Unk04);
            DEFINE_MEMBER_FN(HasFormID, bool, 0x00676B70, UInt32* formID); // do not call directly
            DEFINE_MEMBER_FN(Subroutine007385A0, bool, 0x007385A0, UInt32 myUnk18, UnknownChangeFormHashOrKey, UInt32* formID, ChangeForm::Data* outData);
         };
         struct Lock {
            MEMBER_FN_PREFIX(Lock);
            DEFINE_MEMBER_FN(lock,   void, 0x00A4AF50);
            DEFINE_MEMBER_FN(unlock, void, 0x00A4B100);
         };
         //
         UInt32 pad00;
         Unk04  unk04;
         Lock   unk20;
         //
         MEMBER_FN_PREFIX(ChangeFormManager);
         DEFINE_MEMBER_FN(GetChangeFlags, UInt32*, 0x00676C20, UInt32* outFlags, UInt32 formID); // modifies *outFlags; returns outFlags.
         DEFINE_MEMBER_FN(GetChangeData,  bool,    0x00676BC0, UInt32 formID, ChangeForm::Data* outData); // returns true if successful
         DEFINE_MEMBER_FN(HasFormID,      bool,    0x00676DB0, UInt32 formID);
         //
         // These are called during SaveGame_HookTarget:
         //
         DEFINE_MEMBER_FN(AddFlags,       void,    0x00677050, UInt32 formID, UInt32 flagsToAdd);
         DEFINE_MEMBER_FN(RemoveFlags,    bool,    0x00677160, UInt32 formID, UInt32 flagsToRemove); // returns true if the changeform was wholly removed (i.e. all flags 0 and form is created)
   };
   static_assert(offsetof(ChangeFormManager, unk20) == 0x20, "ChangeFormManager::unk20 is at the wrong offset!");
};

/*namespace _____research {
   class Unknown { // == g_thePlayer? or == *g_thePlayer?
      public:
         UInt32 pad000[(0x264 - 0x000) / 4];
         RE::ChangeFormManager::Unk04* unk264;
         //
         MEMBER_FN_PREFIX(Unknown);
         DEFINE_MEMBER_FN(Subroutine0074B710,   UInt32, 0x0074B710, UInt32 formID, UInt32 valueToAddToFlags);
         DEFINE_MEMBER_FN(GetChangeFlags,       UInt32, 0x00744310, UInt32 formID);
         DEFINE_MEMBER_FN(Subroutine0074B6A0,   UInt32, 0x0074B6A0, UInt32 formID, UInt32 flags); // called by 0x0074B710; probably saves the modified flags
   };
};*/