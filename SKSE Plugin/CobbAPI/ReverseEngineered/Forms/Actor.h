#pragma once
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/NetImmerse/objects.h"
#include "ReverseEngineered/Types.h"
#include "skse/NiObjects.h"

class BGSDialogueBranch;
class BGSLoadGameBuffer;
namespace RE {
   class Actor;
   class ActorMagicCaster;
   class ActorMover;
   class CombatAreaStandard;
   class CombatTargetSelector; // extends NiRefObject
   class StandardDetectionListener;
   class TESPackage;

   enum ActorValueModifier {
      //
      // Actor values (hereafter "AVs") can have three modifiers: permanent, temporary, and damage. An AV's 
      // current value is equal to the sum of its base value and all three modifiers.
      //
      //  - The Permanent Modifier is used for any changes to an actor value that don't have the "Recover" flag 
      //    set. 
      //
      //     * Abilities and worn enchantments, typically.
      //
      //     * ForceActorValue("Stat", n) sets the permanent modifier for "Stat" to "n" minus the current value 
      //       of "Stat." Note that the current value is influenced by all modifiers, including the permanent 
      //       modifier prior to the ForceActorValue call.
      //
      //  - The Temporary Modifier is used for PeakValueModifierEffects that have the "Recover" flag set. These 
      //    would be "Fortify" and "Reduce" potions and spells that dispel after a time limit.
      //
      //  - The Damage Modifier is used for any damage taken to the stat. For example, casting a spell "damages" 
      //    your magicka. Damage modifiers are negative values for most stats, but there are two exceptions: the 
      //    MovementNoiseMult and ShoutRecoveryMult AVs "damage" upward, which makes sense, gien that increases 
      //    in those stats are "bad."
      //
      // The three modifiers for a given AV are stored as simple totals; there is no information that allows one 
      // to trace the forces that have changed them.
      //
      kAVModifier_Permanent = 0, // "permanent modifier" from CK wiki
      kAVModifier_Temporary = 1, // "temporary modifier" from CK wiki
      kAVModifier_Damage    = 2, // "damage    modifier" from CK wiki
   };

   struct Struct007AC960; // sizeof == 0x30
   struct Struct007BE6A0; // sizeof == 0x118
   struct Struct007D5990; // sizeof == 0x9C
   struct Struct007E99C0; // sizeof == 0x44
   struct Struct007AF7A0 { // sizeof == 0x7C
      Struct007D5990* unk00;
      UInt32 unk04;
      Struct007BE6A0* unk08;
      Struct007AC960* unk0C;
      Struct007E99C0* unk10;
      UInt32 unk14 = 0; // refHandle
      UInt32 unk18 = 0;
      UInt32 unk1C = 0;
      UInt8  unk20 = 0;
      UInt8  unk21 = 0;
      UInt8  pad22;
      UInt8  pad23;
      UInt32 unk24 = 0;
      UInt8  unk28 = 0;
      UInt8  unk29 = 0;
      UInt8  unk2A = 0;
      UInt8  unk2B = 0;
      float  unk2C; // initialized to *(float*)(0x01B39264)
      float  unk30 = 1.0F;
      UInt32 unk34 = 0;
      tArray<UInt32> unk38;
      UInt32 unk44 = 0;
      UInt32 unk48 = 0;
      tArray<CombatAreaStandard*> unk4C;
      UInt32 unk58 = 0;
      tArray<UInt32> unk5C;
      CombatTargetSelector* unk68;
      UInt32 unk6C = 0;
      UInt32 unk70 = 0;
      UInt32 unk74;
      UInt32 unk78;

      MEMBER_FN_PREFIX(Struct007AF7A0);
      DEFINE_MEMBER_FN(Subroutine007AE740, void,  0x007AE740, TESPackage*);
      //DEFINE_MEMBER_FN(IsHostileToActor,   bool,  0x007B4C20, Actor*); // these might actually belong to Struct007D5990
      //DEFINE_MEMBER_FN(Subroutine007B4030, void*, 0x007B4030, Actor*);
   };

   class MiddleProcess {
      //
      // Constructor at 0x0072E600 (sizeof == 0x218), assuming that's not for some other AI process level
      //
      public:
         struct EffectListNode { // sizeof == 8
            ActiveEffect*   effect; // 0
            EffectListNode* next;   // 4
         };
         struct Data30 {
            UInt32	unk00;	// 00
            UInt32	unk04;	// 04
            UInt32	unk08;	// 08
            UInt32	unk0C;	// 0C
            UInt32	unk10;	// 10
            UInt32	unk14;	// 14
            UInt32	unk18;	// 18 - (AggroRadius Violated - 14)
         };

         UInt32	unk00;	// 00
         UInt32	unk04;	// 04
         UInt32	unk08;	// 08 // reference handle // kept synched to ActorProcessManager::unk80 and TESPackage::unk08?
         UInt32	unk0C;	// 0C
         UInt32	unk10;	// 10
         UInt32	unk14;	// 14
         UInt32	unk18;	// 18
         UInt32	unk1C;	// 1C
         UInt32	unk20;	// 20
         UInt32	unk24;	// 24
         UInt32	unk28;	// 28
         float  unk2C;	// 2C
         Data30* unk30;	// 30
         UInt32	unk34;	// 34
         UInt32	unk38;	// 38
         UInt32	unk3C;	// 3C
         float	unk40;	// 40
         UInt32	unk44;	// 44
         UInt32	unk48;	// 48
         UInt32	unk4C;	// 4C
         UInt32	unk50;	// 50
         UInt32	unk54;	// 54
         UInt32	unk58;	// 58
         UInt32	unk5C;	// 5C
         NiPoint3 unk60;// 60 // There are accessor-functions (on ActorProcessManager) for both individual components and the NiPoint3 as a whole. In my notes the former are listed as Unk64, Unk68, and so on, rather than Unk60Y and Unk60Z.
            // 60 == pitch
            // 64 == ?
            // 68 == ? // no-oping the setter slows the player down considerably
         UInt32	unk6C;	// 6C
         UInt32	unk70;	// 70
         UInt32	unk74;	// 74
         UInt32	unk78;	// 78
         UInt32	unk7C;	// 7C
         UInt32	unk80;	// 80
         float	unk84;	// 84
         float	unk88;	// 88
         float	unk8C;	// 8C
         float	unk90;	// 90
         float	unk94;	// 94
         float	unk98;	// 98
         UInt32	unk9C;	// 9C
         UInt32	unkA0;	// A0
         BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>>	unkA4;	// A4
         UInt32	unkD0;	// D0 // NiNode*?
         void*	unkD4;	// D4 - NiNode?
         void*	unkD8;	// D8 - NiNode?
         UInt32	unkDC;	// D8
         void*	unkE0;	// E0 - BSFaceGenNiNode?
         void*	unkE4;	// E4
         UInt32	unkE8;	// E8
         UInt32	unkEC;	// EC
         void*	unkF0;	// F0
            // Object:
            //    UInt32 unk68; // bitmask
            //                  //  - 00000008 == last hit was a critical hit
            //    UInt32 unk74; // FO3 GetHitLocation value, though the game may not actually maintain it
            //
            //
         UInt32	unkF4;	// F4
         EffectListNode*	effectList;	// F8
         void* unkFC;	// FC
         void* unk100;	// 100
         void* unk104;	// 104
         UInt32	unk108;	// 108
         UInt32	unk10C;	// 10C
         UInt32	unk110;	// 110
         UInt32	unk114;	// 114
         UInt32	unk118;	// 118
         UInt32	unk11C;	// 11C
         UInt32	unk120;	// 120
         UInt32	unk124;	// 124
         UInt32	unk128;	// 128
         UInt32	unk12C;	// 12C - FFFFFFFF
         UInt32	unk130;	// 130
         UInt32	unk134;	// 134 - FF7FFFFF
         UInt32	furnitureHandle; // 138
         UInt32	unk13C;	// 13C
         UInt32	unk140;	// 140
         TESObjectWEAP** unk144;	// 144 // probably just a struct* whose first member is TESObjectWEAP* // matches unk164; one is left hand and one is right
         TESIdleForm*   currentIdle;	// 148
         UInt32	unk14C; // 14C
         UInt32	unk150;	// 150
         UInt32	unk154;	// 154
         UInt32	unk158;	// 158
         void*	unk15C;	// 15C - hkCharacterProxy?
         void*	unk160;	// 160
         TESObjectWEAP** unk164;	// 164 // probably just a struct* whose first member is TESObjectWEAP* // matches unk144; one is left hand and one is right
         UInt32	unk168;	// 168
         UInt32	unk16C;	// 16C
         UInt32	unk170;	// 170
         UInt32	unk174;	// 174 // last idle played?
         void*    unk178;	// 178
         UInt32	unk17C;	// 17C
         float 	unk180;	// 180 // yaw angle
         UInt32	unk184;	// 184
         UInt32	unk188;	// 188
         UInt32	unk18C;	// 18C
         UInt32	unk190;	// 190
         float  unk194;	// 194
         float  unk198;	// 198
         UInt32	unk19C;	// 19C
         UInt32	unk1A0;	// 1A0
         UInt32	unk1A4;	// 1A4
         float	unk1A8;	// 1A8
         UInt32	unk1AC;	// 1AC
         UInt32	unk1B0;	// 1B0
         float	actorAlpha;	// 1B4
         // More?...
         //
         // float unk1C4; // killmove time remaining
         // float unk1C8; // stagger time remaining?
         //       ...
         // bool  unk209; // related to soul trapping -- may indicate whether the actor has already been soul trapped -- getter 0x71FFF0, setter-to-true 0x71FFE0
         //       ...
         // bool  unk20C;
         //       ...
         // bool  unk211; // true if the actor is in combat with the player
         //       ...
         // bool  unk215  // related to death processing
         // bool  unk216  // related to death or killmove processing
         //       ...
         // float unk280;
   };

   class DetectionState : public NiRefObject { // sizeof == 0x40
      // constructor at 0x0077EF90
      public:
         SInt32 detectionAmount = -1000; // 08 // detection amount
         bool   unk0C = false;
         bool   unk0D = false; // target's current position is known?
         bool   unk0E = false;
         UInt8  unk0F;
         float  unk10 = FLT_MIN; // in-game time, in seconds, at which something occurred
         NiPoint3 unk14; // target actor position?
         float  unk20 = FLT_MIN; // in-game time, in seconds, at which something occurred
         NiPoint3 unk24; // set by Subroutine0077F030 if detectionAmount > 0;  "current position?"
         float  unk30 = FLT_MIN; // in-game time, in seconds, at which something occurred
         NiPoint3 unk34; // set by Subroutine0077F030 if detectionAmount <= 0; "last detected position?"

         MEMBER_FN_PREFIX(DetectionState);
         DEFINE_MEMBER_FN(CopyAssign, void, 0x0077F130, DetectionState*);
         DEFINE_MEMBER_FN(Load,  void, 0x0077EEF0, BGSLoadGameBuffer*);
         DEFINE_MEMBER_FN(Reset, void, 0x0077F0C0);
         DEFINE_MEMBER_FN(Save,  void, 0x0077EE50, void*);
         DEFINE_MEMBER_FN(Subroutine0077F030, void, 0x0077F030, SInt32 detectionAmount, bool unk0C, bool unk0D, bool unk0E, NiPoint3* actorPos, NiPoint3*);
   };
   class ActorKnowledge : public NiRefObject { // sizeof == 0x30
      public:
         //ActorKnowledge(RE::Actor* a, RE::Actor* b);
         //
         UInt32 unk08 = 0; // refHandle
         UInt32 unk0C = 0; // refHandle
         UInt32 unk10 = 4;
         float  unk14 = FLT_MIN;
         DetectionState* detectionState = nullptr; // 18
         tArray<UInt32> unk1C;
         UInt32 unk28 = 0; // bitmask
         float  unk2C = FLT_MIN; // a maximum for something; not used if == FLT_MIN

         MEMBER_FN_PREFIX(ActorKnowledge);
         DEFINE_MEMBER_FN(DestroyUnk18, void, 0x0077D570);
         DEFINE_MEMBER_FN(Load,         void, 0x0077D770, BGSLoadGameBuffer*);
         DEFINE_MEMBER_FN(ReplaceDetectionState, void, 0x0077DF10, DetectionState*);
         DEFINE_MEMBER_FN(Subroutine0077D630,    void, 0x0077D630, float x); // sets unk2C to x; severs connection to B ref after setting its ActorProcessManager::middleProcess::unk20C to true
         DEFINE_MEMBER_FN(Subroutine0077D6C0,    void, 0x0077D6C0); // severs connections to A and B references, and calls A->Subroutine(B, 0);
   };

   struct DetectionEvent { // sizeof == 0x18 // detection event // constructor at 0x00700CB0 // unnamed in executable
      UInt32   soundLevel = 0; // 00
      NiPoint3 position; // 04
      float    timeCreatedAt = FLT_MIN; // 08
      UInt32   unk14 = *g_invalidRefHandle; // 14 // handle of whatever reference the detection event was created at 
   };
   class ActorProcessManager { // constructor: 0x006F5820; sizeof == 0xA0
      public:
         enum {
            kEquippedHand_Left = 0,
            kEquippedHand_Right = 1
         };
         enum {
            kFlags_None = 0,
            kFlags_Unk01 = 1,
            kFlags_Unk02 = 2,
            kFlags_Unk03 = 4,
            kFlags_DrawHead = 8,
            kFlags_Mobile = 16,
            kFlags_Reset = 32
         };

         struct Struct0071A8F0 { // sizeof == 0x35C
            struct Unk019CEntry {
               UInt32 formID; // 00
               ActorKnowledge* knowledge; // 04
            };
            struct Unk019C : public tArray<Unk019CEntry> {
               MEMBER_FN_PREFIX(Unk019C);
               DEFINE_MEMBER_FN(Insert, bool, 0x00701B60, UInt32 index, UInt32 actorFormID);
               DEFINE_MEMBER_FN(ForceUndetectAll,   void, 0x00540840, void*); // used by TDETECT
               DEFINE_MEMBER_FN(Subroutine005C83E0, void, 0x005C83E0, void*); // related to warnings
               DEFINE_MEMBER_FN(Subroutine005CA760, void, 0x005CA760, void*); // related to locking doors
               DEFINE_MEMBER_FN(Subroutine006BA990, void, 0x006BA990, void*);
               DEFINE_MEMBER_FN(Subroutine006BA9F0, void, 0x006BA9F0, void*);
            };

            UInt32 unk0000 = 0;
            UInt32 unk0004 = 0;
            SInt32 shoutVariation = -1; // 08
            UInt32 unk000C = 0;
            UInt32 unk0010 = 0;
            float  avRegenDelayTimerHealth  = 0.0F; // 14 // see ActorProcessManager::SetActorValueRegenDelayTimer and similar
            float  avRegenDelayTimerStamina = 0.0F; // 18 // see ActorProcessManager::SetActorValueRegenDelayTimer and similar
            float  avRegenDelayTimerMagicka = 0.0F; // 1C // see ActorProcessManager::SetActorValueRegenDelayTimer and similar
            UInt32 unk0020 = 0;
            tArray<UInt32> unk0024;
            UInt32 unk0030 = 0;
            UInt32 unk0034 = 0;
            UInt32 unk0038 = 0;
            UInt32 unk003C = 0;
            UInt32 unk0040 = 0;
            UInt32 unk0044 = 0;
            void*  unk0048 = (void*) 0x0123DA00;
            UInt32 unk004C;
            UInt32 unk0050 = 0;
            NiPoint3 unk0054;
            NiPoint3 unk0060;
            NiPoint3 unk006C;
            NiPoint3 unk0078;
            NiPoint3 unk0084;
            NiPoint3 unk0090;
            UInt32 unk009C = 0;
            UInt32 unk00A0 = 0;
            UInt32 unk00A4 = *g_invalidRefHandle; // refHandle
            float  activationHeight = 0.0F; // A8
            UInt32 unk00AC = 0;
            UInt32 unk00B0 = 0;
            UInt32 unk00B4 = 0;
            UInt32 unk00B8 = 0;
            UInt32 unk00BC = 0;
            UInt32 unk00C0 = 0;
            UInt32 unk00C4 = 0;
            UInt32 unk00C8 = 0;
            UInt32 unk00CC = 0;
            UInt32 unk00D0 = 0;
            UInt32 unk00D4 = 0;
            UInt32 unk00D8 = 0;
            UInt32 unk00DC = 0;
            UInt32 unk00E0 = 0;
            float  unk00E4 = FLT_MAX;
            float  unk00E8 = FLT_MAX;
            float  unk00EC = FLT_MAX;
            UInt32 unk00F0 = 1;
            UInt32 unk00F4 = 0;
            UInt32 unk00F8 = 0;
            struct {
               UInt32 unk00 = 0;
               UInt32 unk04 = 0;
               UInt32 unk08 = 0;
               UInt32 unk0C = 0;
               UInt32 unk10 = 0;
               UInt32 unk14 = 0;
            } unk00FC;
            UInt8  unk0114[6] = {0,0,0,0,0,0};
            UInt16 pad011A;
            float  unk011C = -1.0F;
            NiPoint3 unk0120;
            float  unk012C;
            UInt32 unk0130;
            UInt32 unk0134 = *g_invalidRefHandle;
            UInt32 unk0138 = *g_invalidRefHandle;
            UInt32 unk013C = 0;
            UInt32 unk0140 = 0;
            UInt32 unk0144 = 0;
            UInt32 unk0148 = 0;
            struct Struct00B2D870 { // sizeof == 0x1C
               float  unk00 = FLT_MAX;
               float  unk04 = FLT_MAX;
               float  unk08 = FLT_MAX;
               UInt32 unk0C = 0;
               UInt32 unk10 = 0;
               UInt32 unk14 = 0;
               UInt16 unk18 = 0xFFFF;
               UInt8  unk1A = 0;
               UInt8  unk1B = 0;
            } unk014C;
            UInt32 unk0168 = 0;
            float  unk016C = FLT_MAX;
            UInt32 unk0170 = 0;
            UInt32 unk0174 = 0;
            UInt32 unk0178 = 0;
            UInt32 unk017C = 0;
            UInt32 unk0180 = 0;
            SInt32 unk0184 = -1;
            float  unk0188 = -1.0F;
            UInt32 unk018C = 0;
            UInt32 unk0190 = 0;
            UInt32 unk0194 = 0;
            UInt32 unk0198 = 0;
            Unk019C actorsWeDetect; // 019C
            tArray<UInt32> unk01A8;
            UInt32 unk01B4 = 0;
            NiPoint3 unk01B8;
            UInt32 unk01C4 = 0;
            float  unk01C8 = -1.0F;
            NiPoint3 unk01CC;
            float  unk01D8; // 1D8 // initialized to INI:Combat:fDeathForceCleared
            UInt32 unk01DC = 0;
            UInt32 unk01E0 = 0;
            UInt32 unk01E4 = 0;
            float  unk01E8; // 1E8 // initialized weirdly; value is vaguely based on GMST:fIdleChatterCommentTimer
            float  unk01EC; // 1EC // initialized to GMST:fAItalktosameNPCtimer
            float  unk01F0 = 1.0F;
            UInt32 unk01F4 = 0;
            UInt32 unk01F8 = 0;
            UInt32 unk01FC = 0;
            float  unk0200; // 200 // initialized vaguely based on GMST:fAISocialTimerForConversationsMax and GMST:fAISocialTimerForConversationsMin
            SInt32 unk0204 = -1;
            UInt32 unk0208 = 0;
            UInt32 unk020C = 0;
            UInt32 unk0210 = 0;
            UInt32 unk0214 = 0;
            UInt32 unk0218 = 0;
            UInt32 unk021C = 0;
            UInt16 unk0220 = 0xFFFF;
            UInt16 unk0222 = 0xFFFF;
            NiPoint3 unk0224;
            NiPoint3 unk0230;
            UInt32 unk023C = *g_invalidRefHandle;
            UInt32 unk0240 = 0;
            struct { // sizeof == 0xC
               SInt32 unk00 = -1;
               UInt8  unk04 = 0;
               UInt8  pad05[3];
               UInt32 unk08 = 0;
            } unk0244[2];
            UInt32 unk025C = 0;
            UInt32 unk0260 = 0;
            float  unk0264; // initialized to GMST:fAIIdleWaitTime
            UInt32 unk0268 = 0;
            float  unk026C = 20.0F;
            UInt32 unk0270 = 0;
            UInt32 unk0274 = 0;
            UInt32 unk0278 = 0;
            UInt32 unk027C = 0;
            UInt32 unk0280 = 0;
            UInt32 unk0284 = 0;
            UInt32 unk0288 = *g_invalidRefHandle;
            UInt32 unk028C = 0;
            UInt32 unk0290 = 0;
            UInt32 unk0294 = 0;
            UInt32 unk0298 = *g_invalidRefHandle;
            UInt32 unk029C = 0;
            UInt32 unk02A0 = 0;
            UInt32 unk02A4 = 0;
            UInt32 unk02A8 = 0;
            UInt32 unk02AC = 0;
            UInt32 unk02B0 = 0;
            UInt32 unk02B4 = 0;
            UInt32 unk02B8 = 0;
            UInt32 unk02BC = 0;
            UInt32 unk02C0 = 0;
            UInt32 unk02C4 = 0;
            UInt32 unk02C8 = 0;
            float  unk02CC; // initialized to GMST:fAIStayonScriptHeadtrack
            float  unk02D0 = 0.0F; // this is the time at which something occurred; see ActorProcessManager::Subroutine006FC110
            UInt32 unk02D4 = 0;
            UInt32 unk02D8 = 0;
            void*  unk02DC = nullptr; // {myActorProcessManager->TESV_006FC800();} returns {(bool)myActorProcessManager->unk08->unk2DC;}
            UInt32 unk02E0; // initialized to *(0x01B39170)
            DetectionEvent* currentDetectionEvent = nullptr; // 02E4
            StandardDetectionListener* unk02E8; // a new one gets constructed, though unk02E8 is set to nullptr first
            float  unk02EC = 0; // initialized to zero and then set to *(float*)(0x01B39264) by the constructor
            UInt32 unk02F0 = 0;
            UInt32 unk02F4 = 0;
            UInt16 unk02F8 = 0xFFFF;
            UInt16 unk02FA = 0xFFFF;
            UInt32 unk02FC = 0x80000000;
            tArray<UInt32> unk0300;
            NiPoint3 unk030C;
            NiPoint3 unk0318;
            UInt32 unk0324 = 0;
            UInt32 unk0328 = 0;
            UInt32 unk032C = 0;
            UInt16 unk0330 = 0;
            UInt16 unk0332 = 0;
            UInt16 unk0334 = 0;
            UInt8  unk0336 = 0;
            UInt8  unk0337 = 0;
            UInt8  unk0338 = 0;
            UInt8  unk0339 = 0;
            UInt8  unk033A = 0;
            UInt8  unk033B = 0;
            UInt8  unk033C = 0;
            bool   unk033D = true; // one of the necessary conditions for a player-vampire feed package to terminate properly
            UInt8  unk033E; // subroutine 0x006FB520 will modify unk33D only if unk33E is truthy, and it then sets unk33E to false
            UInt8  unk033F = 0;
            UInt8  isInDialogue; // 340
            UInt8  unk0341 = 0;
            UInt8  unk0342 = 0;
            UInt8  unk0343 = 0;
            UInt8  unk0344 = 0;
            UInt8  unk0345 = 0;
            UInt8  unk0346 = 1;
            UInt8  unk0347 = 0;
            UInt8  unk0348 = 0;
            UInt8  unk0349 = 0;
            bool   isArrested = false; // 034A
            UInt8  unk034B = 0;
            UInt8  unk034C = 0;
            UInt8  unk034D = 0;
            UInt8  unk034E = 0;
            UInt8  unk034F = 0;
            UInt8  unk0350 = 0;
            UInt8  unk0351 = 0;
            UInt8  unk0352 = 0;
            UInt8  unk0353 = 0;
            UInt8  unk0354 = 0;
            UInt8  unk0355 = 0;
            UInt8  unk0356 = 0;
            UInt8  unk0357 = 0;
            UInt8  unk0358 = 0;
            UInt8  unk0359 = 0;
            UInt8  unk035A = 0;
            UInt8  pad035B;
         };
         struct Struct00730290 { // sizeof == 0x48
            struct UnkAVData {
               bool  dirty; // 00
               UInt8 pad01[3];
               float unk04; // 04 // apparently: current value, for 00040000 AVs; or maximum value, for 00080000 AVs
            };
            //
            float  unk00 = -1;
            float  unk04 = -1;
            float  unk08 = -1;
            float  unk0C = -1;
            float  unk10 = -1;
            float  unk14 = -1;
            UInt32 unk18 = 0;
            UInt32 unk1C = 0;
            UInt32 unk20 = 0;
            UInt32 unk24 = 0;
            UInt32 unk28 = 0;
            UInt32 unk2C; // 2C // flags. 0x00200000 == health changing; 0x00400000 == magicka; 0x00800000 == stamina
            tArray<UnkAVData> flag00040000ActorValues; // 30
               //
               //   0: Aggression
               //   1: Morality
               //   2: Assistance
               //   3: Health
               //   4: Magicka
               //   5: Stamina
               //   6: HealRate
               //   7: MagickaRate
               //   8: StaminaRate
               //   9: SpeedMult
               //  10: CarryWeight
               //  11: VoicePoints
               //  12: VoiceRate
               //  13: OneHanded
               //  14: TwoHanded
               //  15: Paralysis
               //  16: Invisibility
               // ...
               //  33: WardPower
               //  34: WardDeflection
               //  35: EquippedItemCharge
               //  36: EquippedStaffCharge
               //  37: AbsorbChance
               //  38: WeaponSpeedMult
               //
               // ... and so on. This is combat stuff.
               //
            tArray<UnkAVData> flag00080000ActorValues; // 3C
               //
               //   0: Health
               //   1: Magicka
               //   2: Stamina
               //   3: VoicePoints
               //   4: OneHanded
               //   5: TwoHanded
               //   6: ...skills...
               // ...: ...skills...
               //  20: ...skills...
               //  21: Enchanting
               //  22: BowStaggerBonus
               //
            MEMBER_FN_PREFIX(Struct00730290);
            DEFINE_MEMBER_FN(Subroutine007300E0, void, 0x007300E0, UInt32 avIndex);
            DEFINE_MEMBER_FN(Subroutine00730130, void, 0x00730130, UInt32 avIndex);
            DEFINE_MEMBER_FN(FlagActorValuesAsDirty, void, 0x00730180);
            DEFINE_MEMBER_FN(Subroutine007300B0, void, 0x007300B0, UInt32 avIndex, UInt32); // sets a flag00040000ActorValues entry's (dirty) to false; sets unk04 to the arg
         };

         enum Flags9A {
            kFlag9A_02 = 0x02, // one of the necessary conditions for a player-vampire feed package to terminate properly
         };

         void*  unk00;                 // 00 // struct, sizeof == 0x8; created in ActorProcessManager::Subroutine0071BFA0
         MiddleProcess* middleProcess; // 04
         Struct0071A8F0* unk08;                 // 08
         TESPackage* package;          // 0C
         UInt32 unk10[(0x24 - 0x10) >> 2]; // 10
         float  unk24;                     // 24
         float  unk28; // 28
         UInt32 unk2C;
         Struct00730290* unk30;                     // 30
         UInt32 unk34[(0x38 - 0x34) >> 2]; // 34
         tList<void*>       unk38;         // 38
         tList<void*>       unk40;         // 40
         UInt32 unk48[(0x50 - 0x48) >> 2]; // 48
         float  bleedoutTimer;             // 50 // when this timer expires, the (essential) actor exits bleedout (they stop being downed)
         float  timeOfDeath;               // 54 - GetTimeDead = (GameDaysPassed*24) - timeOfDeath
         UInt32 unk58[(0x68 - 0x58) >> 2]; // 58
         TESForm* equippedObject[2];       // 68
         UInt32 unk70[(0x80 - 0x70) >> 2]; // 70
         UInt32 unk80;                     // 80 // reference handle // kept synched to MiddleProcess::unk08 and TESPackage::unk08?
         UInt32 unk84[(0x98 - 0x84) >> 2];
         UInt8  unk98;                     // 98
         UInt8  unk9A;                     // 9A // bitmask
         SInt8  unk9B = 3;                 // 9B // Indicates which of Unknown012E32E8's AI-related ref handle arrays the actor is tracked in. Actor's getter defaults this to -1 if it doesn't have an ActorProcessManager.
         UInt8  unk9C; // 9C
         bool   isIgnoringCombat; // 9D
         UInt8  unk9E[(0xA0 - 0x9E)];      // 9C

         MEMBER_FN_PREFIX(ActorProcessManager);
         //
         DEFINE_MEMBER_FN(CeaseDetectionEvent,    void, 0x006FB690);
         DEFINE_MEMBER_FN(CheckDetectionEventAge, void, 0x006FB6C0); // makes old detection events expire
         DEFINE_MEMBER_FN(CreateDetectionEvent,   void, 0x00702070, Actor* myActor, NiPoint3* position, UInt32 soundLevel, TESObjectREFR* positionOf);
         DEFINE_MEMBER_FN(GetDetectionEvent,      DetectionEvent*, 0x006FB670);
         //
         DEFINE_MEMBER_FN(GetCurrentShoutVariation, SInt32, 0x006FCB90);
         DEFINE_MEMBER_FN(SetCurrentShoutVariation, void,   0x006FCBA0, SInt32);
         //
         DEFINE_MEMBER_FN(IsAlerted, bool, 0x006F4770); // { return this->unk9A & 8; }
         DEFINE_MEMBER_FN(SetAlert,  void, 0x006F4780, bool makeAlert);
         //
         DEFINE_MEMBER_FN(IsArrested,           bool,    0x006FC260);
         DEFINE_MEMBER_FN(PushActorAway,        void,    0x00723FE0, Actor* awayFrom, float x, float y, float z, float magnitude);
         DEFINE_MEMBER_FN(SetEquipFlag,         void,    0x0071F520, UInt8 flags);
         DEFINE_MEMBER_FN(UpdateEquipment,      void,    0x007031A0, Actor* actor);
         DEFINE_MEMBER_FN(SetUnk08Unk170,       void,    0x006FD1A0, UInt32 flag);  // sets this->unk08->unk170 // but the value may actually be a float passed as a UInt32?
         DEFINE_MEMBER_FN(GetUnk08Unk341,       bool,    0x006FBAC0);               // returns (bool)this->unk08->unk341
         DEFINE_MEMBER_FN(GetUnk80,             UInt32*, 0x006F4C50, UInt32* out);
         DEFINE_MEMBER_FN(GetUnk9AFlag00000010, bool,    0x006F47C0);
         DEFINE_MEMBER_FN(SetUnk9AFlag00000002, void,    0x006F4650, bool setBitTo);
         //
         DEFINE_MEMBER_FN(GetActorValueRegenDelayTimer, float, 0x006FCC40, UInt32 avIndex); // only returns anything for health, magicka, and stamina
         DEFINE_MEMBER_FN(SetActorValueRegenDelayTimer, void,  0x006FCC90, UInt32 avIndex, float timer); // only does anything for health, magicka, and stamina
         //
         DEFINE_MEMBER_FN(GetBleedoutTimer, float, 0x00496430);
         DEFINE_MEMBER_FN(SetBleedoutTimer, void,  0x00633390, float timer);
         //
         DEFINE_MEMBER_FN(StartKillmoveTimer, void, 0x00720070);
         DEFINE_MEMBER_FN(StartStaggerTimer,  void, 0x00720090);
         DEFINE_MEMBER_FN(UpdateStaggerTimer, void, 0x007200B0, Actor* myActor, float timeElapsed);
         //
         DEFINE_MEMBER_FN(MakeAwareOf, ActorKnowledge*, 0x00709A20, Actor* myActor, Actor* otherActor);
         //
         // For documentation purposes; do not call:
         //
         DEFINE_MEMBER_FN(LoadSavedata, void, 0x007119E0, Actor*, BGSLoadGameBuffer*);
         //
         // Unknown:
         //
         DEFINE_MEMBER_FN(FlagUnk30ActorValuesAsDirty, void, 0x006F4870);
   };

   class ActorState : public IMovementState {
      public:
         virtual ~ActorState();
         //
         virtual bool Unk_14(UInt32 bitmask); // 14 // modifies flags04, but it's not a direct setter; it does weird stuff with the bits
         virtual bool Unk_15(UInt32 unk); // 15 // either bool or void; related to equipped object or possibly equipped magic specifically

         //	void	** _vtbl;	// 00

         // older versions of this class stored flags in a UInt64
         // this forced the addition of 4 useless padding bytes
         // current and future versions store flags as two UInt32s

         enum FurnitureState : UInt32 { // part of Flags04
            kFurnState_None        = 0,
            kFurnState_WantToSit_A = 1,
            kFurnState_WantToSit_B = 2,
            kFurnState_Sitting     = 3,
            kFurnState_Sitting_WantToStand = 4,
            kFurnState_WantToSleep_A = 5,
            kFurnState_WantToSleep_B = 6,
            kFurnState_Sleeping      = 7,
            kFurnState_Sleeping_WantToWake = 8,
         };

         enum Flags04 {
            //
            // Flying flags identified via Papyrus Actor::GetFlyingState.
            //
            kState_Running   = 0x00000040,
            kState_Walking   = 0x00000080,
            kState_Sprinting = 0x00000100,
            kState_Sneaking  = 0x00000200,
            kState_Swimming  = 0x00000400,
            //
            kState_Seated               = 0x0000C000, // yes, this is two bits, but they both have to be set
            kState_WantsToSit_A         = 0x00004000, // either/or
            kState_WantsToSit_B         = 0x00008000, // either/or
            kState_Seated_WantsToStand  = 0x00010000,
            //
            kState_Sleeping             = 0x0001C000, // yes, this is two bits, but they both have to be set
            kState_Sleeping_WantsToWake = 0x00020000,
            //
            kState_Flying_TakingOff    = 0x00040000,
            kState_Flying_Cruising     = 0x00080000,
            kState_Flying_Hovering     = 0x000C0000, // yes, this is two bits, but they both have to be set
            kState_Flying_Landing      = 0x00100000,
            kState_Flying_Any          = 0x00140000, // use this for checks
         };
         enum Flags08 {
            kFlag_EnableHeadtracking = 0x8,
            kFlag_Staggering = 0x2000,
         };

         UInt32	flags04;
         UInt32	flags08;

         inline FurnitureState GetFurnitureState() const {
            return (FurnitureState) ((this->flags04 >> 0xE) & 0xF);
         }
         inline bool IsUnconscious() const {
            return (this->flags04 & 0x01E00000) == 0x00600000;
         };
         bool IsWeaponDrawn() const {
            return (flags08 >> 5 & 7) >= 3;
         }
         UInt8 GetFlyingState() const { // same as the condition, but 5 seems to mean "not allowed to fly"
            return (this->flags04 >> 0x12) & 5;
         }
         //
         // For documentation purposes; do not call:
         //
         MEMBER_FN_PREFIX(ActorState);
         DEFINE_MEMBER_FN(LoadSavedata, void, 0x006F11D0, BGSLoadGameBuffer*);
   };

   class ActorValueOwner { // sizeof == 0x4 // interface
      public:
         virtual ~ActorValueOwner();

         // Argument is the ActorValue ID
         virtual float GetCurrent(UInt32 avIndex); // 01
         virtual float GetMaximum(UInt32 avIndex); // 02
         virtual float GetBase(UInt32 avIndex); // 03
         virtual void  SetBase(UInt32 avIndex, float value); // 04
         virtual void  ModBase(UInt32 avIndex, float changeBy); // 05
         virtual void  ModifyModifier(ActorValueModifier modifier, UInt32 avIndex, float changeBy); // 06
         virtual void  SetCurrent(UInt32 avIndex, float value); // 07 // just calls SetBase; wtf?
         virtual bool  Unk_08(void); // 08

         // void** _vtbl; // 00
   };

   class MovementControllerNPC;
   class Actor : public TESObjectREFR {
      //
      // Note to self: I don't think this has a VTBL. It looks like the game 
      // only ever creates instances of subclasses, like Character, so take 
      // a look at Character's VTBL when looking to decode Actor methods.
      //
      public:
         enum { kTypeID = kFormType_Character };
         operator ::Actor*() const { return (::Actor*) this; }
         operator ::TESObjectREFR*() const { return (::TESObjectREFR*) this; }

         virtual ~Actor();
         //
         virtual void Unk_A2(TESForm* someBaseForm, bool, UInt32);
         virtual float GetHeading(UInt32); // A3 // returns the yaw-rotation for actors that can't pitch/roll; otherwise, returns some mathy stuff
         virtual void Unk_A4(bool); // A4 // PlayerCharacter constructor calls this on itself
         virtual void DrawSheatheWeapon(bool draw); // A5
         virtual void Unk_A6(void);
         virtual void Unk_A7(void);
         virtual void Unk_A8(NiPoint3*); // handles movement for certain actor types?
         virtual void Unk_A9(void);
         virtual void Unk_AA(UInt32, UInt32); // a total reset -- behavior graph, spells, everything
         virtual void Unk_AB(void);
         virtual void Unk_AC(void);
         virtual void Unk_AD(float);
         virtual void Unk_AE(void*);
         virtual void Unk_AF();
         virtual void Unk_B0(void);
         virtual void Unk_B1(bool);
         virtual void Unk_B2(void);
         virtual void Unk_B3(TESForm*, UInt32, UInt32); // from what I've seen, the form is generally a weapon
         virtual void SetBounty(TESFaction*, bool forViolentCrime, SInt32 gold); // B4 // violent and non-violent bounties are tracked separately
         virtual void ModBounty(TESFaction*, bool forViolentCrime, SInt32 gold); // B5
         virtual void Unk_B6(void);
         virtual UInt32 GetBounty(TESFaction*); // B7
         virtual void Unk_B8(void*, UInt8, UInt8); // could be bools
         virtual void ServeJailTime(); // B9
         virtual void PayBounty(TESFaction*, bool removeStolenItems, bool goToJail); // BA
         virtual UInt32 IsCannibalizing(); // BB // Is in cannibal feed action? // returns int, but it's really a bool
         virtual void Unk_BC(void);
         virtual UInt32 IsVampireFeeding(); // BD // Is in vampire feed action? // returns int, but it's really a bool
         virtual void Unk_BE(void);
         virtual void Unk_BF(Actor*, TESObjectREFR*); // BF // 0x00731350? // vampire feed? first argument is target. second is furniture that the target is using. runs AI packages on all involved actors.
         virtual void Unk_C0(void);
         virtual void Unk_C1(void);
         virtual void Unk_C2(void);
         virtual void Unk_C3(void);
         virtual void Unk_C4(void);
         virtual UInt32 Unk_C5();
         virtual void Unk_C6(void);
         virtual void Unk_C7(float speed, NiPoint3* velocity); // Handles movement for typical human actors. Positive X velocity moves to the actor's right; positive Y moves forward; Z values don't seem to do anything. The velocity vector can have a non-unit length, and appears to be further scaled by the speed.
         virtual void Unk_C8(void);
         virtual void OnArmorActorValueChanged(); // C9 // called when HeavyArmor or LightArmor AV base value changes
         virtual void Unk_CA(void);
         virtual void Unk_CB(TESObjectREFR*, UInt32, UInt32, bool);
         virtual void Unk_CC(void);
         virtual void Unk_CD(void);
         virtual void Unk_CE(void);
         virtual void Unk_CF(void);
         virtual void Unk_D0(void);
         virtual void Unk_D1(void);
         virtual bool  Unk_D2();
         virtual Struct007AF7A0* Unk_D3(); // returns *(this->unkD8), or 0 if the pointer is null
         virtual void  Unk_D4(Struct007AF7A0*); // sets *(this->unkD8) to arg1, if the pointer is not null
         virtual bool  Unk_D5(UInt32);
         virtual bool  Unk_D6(UInt32);
         virtual bool  Unk_D7(UInt32, UInt32, UInt32); // constructs a DialoguePackage
         virtual void Unk_D8(void);
         virtual void Unk_D9();
         virtual void Unk_DA(void);
         virtual void Unk_DB(void);
         virtual void Unk_DC(void);
         virtual void Unk_DD();
         virtual void Unk_DE(TESPackage*, UInt32, UInt32); // DE // executes specified package? // latter args could be bools
         virtual void Unk_DF(void);
         virtual void Unk_E0(void);
         virtual void Unk_E1(void);
         virtual bool IsInCombat(); // E2
         virtual void Unk_E3(void);
         virtual void Unk_E4(); // E4 // Unknown. Related to AI.
         virtual void Unk_E5(void);
         virtual float Unk_E6(); // E6 // related to computing the actor's threat level
         virtual void Unk_E7(void);
         virtual void Unk_E8(void);
         virtual void Unk_E9(void);
         virtual void Unk_EA(void);
         virtual void Unk_EB(void);
         virtual void Unk_EC(void);
         virtual void Unk_ED(void);
         virtual void Unk_EE(void);
         virtual void Unk_EF(void);
         virtual void Unk_F0(void);
         virtual void Unk_F1(void);
         virtual void Unk_F2(void);
         virtual void Unk_F3(void);
         virtual void Unk_F4(void);
         virtual void Unk_F5(UInt32);
         virtual void AdvanceSkill(UInt32 skillId, float points, UInt32 unk1, UInt32 unk2); // F6
         virtual void Unk_F7(void);
         virtual bool Unk_F8(TESFaction*);
         virtual void VisitPerks(void); // F9 // BGSPerk::FindPerkInRanksVisitor
         virtual void AddPerk(BGSPerk* perk, UInt32 unk1); // FA
         virtual void RemovePerk(BGSPerk* perk); // FB
         //
         // Every virtual method after this point was missing from the SKSE definitions, likely because 
         // Bethesda added them after the SKSE team last checked the VTBL.
         //
         virtual void Unk_FC(UInt32); // FC // no-op for Actor
         virtual void Unk_FD(UInt32); // FC // no-op for Actor
         virtual bool Unk_FE(UInt32); // FC
         virtual UInt32 Unk_FF(); // FF
         virtual void Unk_100(); // 100
         virtual void Unk_101(); // 101 // no-op for Actor
         virtual bool Unk_102(); // 102 // no-op for Actor; returns false
         virtual void KillIfNoHealth(UInt32 unk1, float currentHealth); // 103 // checks an actor's health; if below zero, kills them
         virtual void Unk_104(UInt32); // 104
         virtual bool Unk_105();       // 105 // no-op for Actor; returns false
         virtual bool Unk_106();       // 106 // checks flags1; return strue if flags 0x02 and 0x08 are false
         virtual void ModifyFlags1_00000020(bool); // 107
         virtual void CreateUnkD0();   // 108 // == non-virtual member function 006B2630 (it's not always called through the VTBL)
         virtual void* GetUnkE4();     // 109 // type of unkE4/ret-val is unknown
         virtual void SetUnkE4(void*); // 10A // type of unkE4/arg is unknown
         virtual void* GetUnkE8();     // 10B // type of unkE8/ret-val is unknown
         virtual void SetUnkE8(void*); // 10C // type of unkE8/arg is unknown
         virtual void Unk_10D(UInt32, UInt32, UInt32, UInt32); // 10D
                                                               // function to kill an actor?
                                                               //  - calls perk entry point: add leveled list on death
                                                               //  - checks INI settings like fDyingTimer
                                                               //  - has code to force the player into bleedout camera
         virtual bool Unk_10E(TESForm*, UInt32 unk);    // 10E
         virtual UInt8 Unk_10F(UInt32, UInt32, UInt32); // 10F
         virtual void Unk_110();       // 110 // no-op for Actor
         virtual SInt32  GetCurrentShoutVariation(); // 111 // returns -1 if no process manager
         virtual void    SetLastRiddenHorseHandle(UInt32 horseRefHandle); // 112 // no-op for Actor
         virtual UInt32* GetLastRiddenHorseHandle(UInt32* outHandle); // 113 // no-op for Actor; sets the outHandle to *g_invalidRefHandle and returns outHandle
         virtual void Unk_114(void);   // 114
         virtual void Unk_115(NiPoint3* out); // 115 // adds this->pos to out, but only under certain conditions
         virtual void Unk_116(UInt32); // 116 // no-op for Actor
         virtual void Unk_117();       // 117 // calls stuff related to actor values
         virtual void* Unk_118();      // 118 // returns a name from the skeleton node name list, building the list if needed. returned name is at 0x012E31B0
         virtual void Unk_119(UInt32, UInt32, UInt32); // 119 // works with the actor hkCharacterProxy
         virtual void Unk_11A();       // 11A // works with the actor hkCharacterProxy
         virtual void Unk_11B(UInt32); // 11B
         virtual bool Unk_11C(UInt32, UInt32, UInt32); // 11C
         virtual bool Unk_11D(UInt32); // 11D
         virtual void Unk_11E();       // 11E // related to actor pitch
         virtual void Unk_11F();       // 11F // related to skeleton nodes
         virtual void Unk_120(UInt32, UInt32, UInt32, UInt32, UInt32, UInt32); // 120 // related to inventory
         virtual void Unk_121(UInt32); // 121
         virtual void Unk_122(UInt32); // 122 // related to dialogue and the skeleton // checks several INI settings; we should see which ones
         virtual void Unk_123(UInt32); // 123 // related to facial animations/expressions
         virtual ActorMover* ResetUnkCC(); // 124 // creates unkCC (first destroying it if it already exists) and returns the new instance
         virtual void DestroyUnkCC();      // 125 // destroys unkCC
         virtual bool Unk_126(UInt32, UInt32); // 126 // related to menus and to the actor's "ghost" and "is running" flags
         virtual float IncerceptActorValueChange(UInt32 avIndex, float avChangingBy); // 127 // Called whenever an AV is about to change; returning a value overrides by how much the AV will change. Return (avChangingBy) verbatim to do nothing.

         class SpellArray { // sizeof == 0x0C // actually just a tArray with special functionality
            public:
               SpellItem* Get(UInt32 idx) {
                  if (idx >= spellCount)
                     return NULL;
                  if (allocatedCount & kLocalAlloc)
                     return (idx == 0) ? singleSpell : NULL;
                  else
                     return spells ? spells[idx] : NULL;
               }
               UInt32 Length(void) { return spellCount; }
            private:
               enum {
                  kLocalAlloc = 0x80000000,
               };
               //
               UInt32 allocatedCount; // 00 - upper bit is set when single-element optimization is used
               union {
                  SpellItem	** spells;     // 04
                  SpellItem	* singleSpell; // 04 - used when kLocalAlloc is set
               };
               UInt32 spellCount; // 08
         };
         //
         enum Flags1 : UInt32 {
            kFlags_AIEnabled        = 0x00000002,
            kFlags_Flag1_00000200   = 0x00000200,
            kFlags_Flag1_00004000   = 0x00004000,
            kFlags_Flag1_00008000   = 0x00008000,
            kFlags_Flag1_00040000   = 0x00040000, // related to the Waterbreathing magic effect. also related to the actor being in cell water, or in lava; checked by a condition meant for the latter
            kFlags_Flag1_01000000   = 0x01000000,
            kFlags_IsPlayerTeammate = 0x04000000,
            kFlags_IsGuard          = 0x40000000,
         };
         enum Flags2 : UInt32 {
            kFlags_Flag2_00000002         = 0x00000002,
            kFlags_NoBleedoutRecovery     = 0x00000020,
            kFlags_CanDoFavor             = 0x00000080,
            kFlags_Flag2_00000100         = 0x00000100,
            kFlags_AllowBleedoutDialogue  = 0x00000200,
            kFlags_Flag2_00000800         = 0x00000800, // "is in combat with player" ?
            kFlags_IsTrespassing          = 0x00001000,
            kFlags_IsInKillMove           = 0x00004000,
            kFlags_AttackMeOnSight        = 0x00008000,
            kFlags_IsCommanded            = 0x00010000,
            kFlags_IsEssential            = 0x00040000,
            kFlags_IsProtected            = 0x00080000,
            kFlags_DontShowOnStealthMeter = 0x04000000,
            kFlags_Flag2_01000000         = 0x10000000,
         };

         MagicTarget       magicTarget;     // 054 // MagicTarget::unk08 is flags; flag 02 means the MagicTarget has invisibility
         ActorValueOwner   actorValueOwner; // 060
         ActorState        actorState;      // 064
         BSTEventSink<void*> transformDeltaEvent;      // 070 .?AV?$BSTEventSink@VBSTransformDeltaEvent@@@@
         BSTEventSink<void*> characterMoveFinishEvent; // 074 .?AV?$BSTEventSink@VbhkCharacterMoveFinishEvent@@@@
         IPostAnimationChannelUpdateFunctor	unk078;   // 078 IPostAnimationChannelUpdateFunctor
         UInt32	flags1; // 07C
         float    unk080; // 080 // related to headtracking, and derived from the "update seconds" min and max when the Actor is constructed
         UInt32	unk084; // 084
         ActorProcessManager* processManager; // 088
         UInt32   refHandleDialogueTarget; // 08C // reference handle of this actor's dialogue target
         UInt32   refHandleCombatTarget; // 090 // reference handle of this actor's combat target
         UInt32   refHandleKiller; // 094 // reference handle of this actor's killer; important during killmoves
         UInt32   unk098; // 098
         float    unk09C; // 09C
         UInt32   unk0A0; // 0A0
         UInt32   unk0A4; // 0A4
         UInt32   unk0A8; // 0A8
         UInt32   unk0AC; // 0AC
         UInt32   unk0B0; // 0B0
         NiPoint3 unk0B4; // 0B4
         UInt32   unk0C0; // 0C0
         UInt32   unk0C4; // 0C4
         BGSLocation* editorLocation; // 0C8
         ActorMover*    unk0CC; // 0CC
         MovementControllerNPC* unk0D0; // 0D0 // This uses the same class even for the player. // Papyrus Actor.SetPlayerControls(bool) operates on this.
         UInt32   unk0D4; // D4
         Struct007AF7A0* unk0D8; // D8 // Unk_D3 accesses this pointer // AI information; "Confidence" AV affects data in here
         UInt32   unk0DC[(0x0FC - 0x0DC) >> 2]; // 0D4
         SpellArray addedSpells;                // 0FC
         ActorMagicCaster*	unk108[4]; // 108
         TESForm* equippedItems[4]; // 118 // equipped items or spells; see 0x006ED753
         TESForm* equippedShout; // 128 // not validated by the game after being loaded from the savegame; if the player were to equip a shout, upgrade the mod it came from, and the upgrade removed the shout and put something else in that form ID, you'd run into problems
         UInt32	unk12C; // 12C
         TESRace* race;   // 130
         float 	unk134; // 134
         UInt32	flags2; // 138

         //
         // The stuff below describes the internals for ActorValues and how they're stored 
         // on actors. It's probably not stuff you should tamper with directly; use the 
         // actorValueOwner field as an interface for this data instead.
         //
         struct ActorValueState { // sizeof == 0xC
            //
            // Constructor at 0x00474D80. Labeled in my disassembler as Struct00474D80.
            //
            float modifiers[3]; // 00 // indices are from the ActorValueModifier enum
            //
            MEMBER_FN_PREFIX(ActorValueState);
            DEFINE_MEMBER_FN(AreAllZero, bool,  0x006F20C0);
            DEFINE_MEMBER_FN(GetSum,     float, 0x006F2090); // returns the net offset, i.e. buffs + nerfs
            DEFINE_MEMBER_FN(Load,       void,  0x006F2140, BGSLoadFormBuffer*);
            DEFINE_MEMBER_FN(Reset,      void,  0x0058C4F0); // sets all floats to zero
            DEFINE_MEMBER_FN(Save,       void,  0x006F20F0, BGSSaveFormBuffer*); // writes (int)(3), and then writes its three floats; in theory, you can write a smaller int "n" and store just the first "n" floats (with the rest being treated as zero); however, "n" must not exceed 3
            //
            // === SAVEDATA LAYOUT ============================================================================================
            // UInt32         count       Number of floats written to the savegame. Must not exceed 3. Must be at least 1.
            // float[count]   modifiers   The floats. If (count) is less than 3, then the remaining floats will be set to zero.
         };
         struct Struct006F2190 { // sizeof == 0x10
            //
            // Holds a map of actor value indices to floats, and another map of actor 
            // value indices to ActorValueState objects. Note that it will never hold 
            // state objects for health, magicka, stamina, or voicepoints, because 
            // those are held elsewhere on the Actor.
            //
            // ActorValues only exist in here if they are being tracked or modified in 
            // some way. If an ActorValueState object's floats all become zero, it is 
            // destroyed and removed from the list here.
            //
            template <typename T> struct _Map {
               StringCache::Ref keys;
               T* values;
               //
               T* operator[](UInt8 key) const {
                  const UInt8* data = this->keys.data;
                  UInt8  bl = data[0];
                  size_t i  = 0;
                  if (bl < key + 1)
                     while (bl && bl < key + 1)
                        bl = data[++i];
                  {
                     RE::simple_lock_rev_guard scopedLock((RE::SimpleLockReversed)0x01B39110); // same lock as in vanilla
                     if (bl == key + 1)
                        return &this->values[i];
                     return nullptr;
                  }
               };
            };
            //
            _Map<float>           baseValues; // 00
            _Map<ActorValueState> modifiers;  // 08
            //
            /*//
            StringCache::Ref unk00; // 00 // the const char* is actually a byte array matching actor value indices to indices in unk04; this->unk04[index of AV index in unk00] == float
            float* unk04 = nullptr; // 04 // array of base actor values
            StringCache::Ref unk08; // 08 // map actor value indices to state list indices
            ActorValueState* unk0C = nullptr; // 0C // array
            //*/
            //
            MEMBER_FN_PREFIX(Struct006F2190);
            DEFINE_MEMBER_FN(DestroyFirstList,     void,             0x006F21C0); // sets unk04 to nullptr and empties out unk00
            DEFINE_MEMBER_FN(DestroyLists,         void,             0x006F2230);
            DEFINE_MEMBER_FN(Destructor,           void,             0x006F22B0); // calls DestroyLists and then frees StringCache::Refs.
            DEFINE_MEMBER_FN(GetStateObjectFor,    ActorValueState*, 0x006F22D0, UInt32 actorValueIndex);
            DEFINE_MEMBER_FN(InsertIntoSecondList, ActorValueState*, 0x006F2340, UInt32 actorValueIndex);
            DEFINE_MEMBER_FN(Load,                 void,             0x006F3450, BGSLoadFormBuffer*);
            DEFINE_MEMBER_FN(RemoveFromSecondList, void,             0x006F2520, UInt32 actorValueIndex);
            DEFINE_MEMBER_FN(Save,                 void,             0x006F2720, BGSSaveFormBuffer*);
            DEFINE_MEMBER_FN(GetBaseValueFor,      bool,             0x006F2870, UInt32 actorValueIndex, float* out); // gets a float from unk04 based on the avIndex's position in unk00; returns false if not found
            DEFINE_MEMBER_FN(SetBaseValueFor,      void,             0x006F28D0, UInt32 actorValueIndex, float value);
            DEFINE_MEMBER_FN(Subroutine006F2A80,   void,             0x006F2A80, UInt32 actorValueIndex);
            //
            /*//
            ActorValueState* Pseudocode_GetStateObjectFor(UInt32 actorValueIndex) const {
               const char* data = this->unk08.data;
               char   bl = data[0];
               size_t i  = 0;
               if (bl < actorValueIndex + 1) {
                  while (bl && bl < actorValueIndex + 1) {
                     bl = data[++i];
                  }
               }
               {
                  // uses a scoped lock stored at (SimpleLock*)0x01B39110
                  //
                  if (bl == actorValueIndex + 1) {
                     auto list = this->unk0C;
                     return &this->unk0C[i];
                  }
                  return nullptr;
               }
            };
            bool Pseudocode_GetBaseValueFor(UInt32 actorValueIndex, float* out) const {
               //
               // Note: uses a scoped lock stored at (SimpleLock*)0x01B39110.
               //
               const char* data = this->unk00.data;
               char cl = data[0];
               if (cl < actorValueIndex + 1) {
                  size_t i = 0;
                  do {
                     cl = data[++i];
                  } while (cl < actorValueIndex + 1);
               }
               auto floatList = this->unk04;
               if (floatList && cl == actorValueIndex + 1) {
                  *out = floatList[cl];
                  return true;
               }
               *out = 0.0F;
               return false;
            }
            //*/
         };
         //
         Struct006F2190 unk13C; // 13C
         BGSDialogueBranch* unk14C; // 14C // found the code that loads its form ID from a save file
         ActorValueState avStateHealth;      // 150
         ActorValueState avStateMagicka;     // 15C
         ActorValueState avStateStamina;     // 168
         ActorValueState avStateVoicePoints; // 174
         float  unk180; // 180 // see code circa 0x006EC46E
         UInt32 unk184; // 184
         UInt32 unk188[(0x194 - 0x188) >> 2];
         UInt8  unk194; // 194 // loaded from the savedata as a single byte; see Actor::Unk_0F
         UInt8  unk195; // 195
         UInt8  unk196; // 196
         UInt8  unk197; // 197
         UInt32 unk198; // 198 // bitmask

         MEMBER_FN_PREFIX(Actor);
         DEFINE_MEMBER_FN(AddSpell,              bool,    0x006EC120, SpellItem* spell);
         DEFINE_MEMBER_FN(CanFlyHere,            bool,    0x006A6F80);
         DEFINE_MEMBER_FN(ClearExtraArrows,      void,    0x006A9F20);
         DEFINE_MEMBER_FN(CreateActorValueStateObj, ActorValueState*, 0x006DE8D0, UInt32 avIndex); // don't call this; use GetActorValueStateObj
         DEFINE_MEMBER_FN(Decapitate,            void,    0x006AA560); // refuses if the actor is already decapped, but not if they're alive
         DEFINE_MEMBER_FN(Dismount,              void,    0x006C2EA0);
         DEFINE_MEMBER_FN(EnableAI,              void,    0x006AA690, bool); // argument determines whether we enable (true) or disable (false)
         DEFINE_MEMBER_FN(ExecuteDoNothingPackage, void,        0x006AC890);
         DEFINE_MEMBER_FN(GetCurrentPackage,       TESPackage*, 0x006A9AD0); // TODO: Rename this here and in OllyDbg; it's NOT what Papyrus uses! It's used internally.
         DEFINE_MEMBER_FN(GetActorValueStateObj,   ActorValueState*, 0x006DE850, UInt32 avIndex);
         DEFINE_MEMBER_FN(GetActorValuePercentage, float,       0x006A8240);
         DEFINE_MEMBER_FN(GetActorValueRegenDelay, float,       0x006DEB40, UInt32 avIndex); // only valid for health/magicka/stamina
         DEFINE_MEMBER_FN(GetAimYaw,             float,   0x006C3420); // actor look direction? // Similar to GetHeading(0), but it accounts for the "AimHeadingCurrent" animation graph variable
         DEFINE_MEMBER_FN(GetBribeAmount,        SInt32,  0x006AF030);
         DEFINE_MEMBER_FN(GetCombatState,        UInt32,  0x006E10F0); // same as Papyrus: 0, 1, 2 == not in combat; in combat; searching
         DEFINE_MEMBER_FN(GetComputedHeight,     float,   0x006AB410);
         DEFINE_MEMBER_FN(GetComputedHeightMult, float,   0x004D5230); // ActorBase height mult * Race height mult
         DEFINE_MEMBER_FN(GetCrimeFaction,       TESFaction*, 0x006AED30);
         DEFINE_MEMBER_FN(GetDetected,           SInt32,  0x006AE830, Actor* canWeSeeThisActor, UInt32 oftenIs3_mustNotExceed5); // GetDetected condition uses 3 for the enum and checks if the result > 0
         DEFINE_MEMBER_FN(GetEquippedWeapon,     TESObjectWEAP*,  0x006E0D20, bool whichHand);
         DEFINE_MEMBER_FN(GetGoldAmount,         SInt32,  0x006A8190);
         DEFINE_MEMBER_FN(GetHealthPercentage,   float,   0x006A8320); // just a convenience wrapper for GetActorValuePercentage
         DEFINE_MEMBER_FN(GetLevel,              UInt16,  0x006A7320);
         DEFINE_MEMBER_FN(GetLightLevel,         double,  0x006AE980); // places result on the FPU stack
         DEFINE_MEMBER_FN(GetThreatLevel,        float,   0x006E0DE0, float UnkModifier_LessOrEqualZeroUsesSomeDefault); // the "threat ratio" between two actors is one actor's threat level divided by the other's
         DEFINE_MEMBER_FN(GetSoulSize,           UInt32,  0x006E8BC0); // returns ExtraSoul::SoulSize enum
         DEFINE_MEMBER_FN(HasBeenEaten,          bool,    0x006A8B50);
         DEFINE_MEMBER_FN(HasPerk,               bool,    0x006AA190, BGSPerk* perk);
         DEFINE_MEMBER_FN(HasSpell,              bool,    0x006EAF10, SpellItem* perk);
         DEFINE_MEMBER_FN(InterruptCast,         void,    0x006E8F60, UInt32 unk_usuallyZero);
         DEFINE_MEMBER_FN(IsAlerted,             bool,    0x006A87C0); // Calls this->processManager->TESV_006F4770();
         DEFINE_MEMBER_FN(IsArrestingTarget,     bool,    0x006B4650);
         DEFINE_MEMBER_FN(IsBlocking,            bool,    0x006BBE60);
         DEFINE_MEMBER_FN(IsBribed,              bool,    0x006AA490);
         DEFINE_MEMBER_FN(IsEssentialOrProtected, bool,    0x006AAD20, bool alsoCheckProtected); // if the argument is false, then only checks for the "essential" flag
         DEFINE_MEMBER_FN(IsGhost,               bool,    0x006A8CA0);
         DEFINE_MEMBER_FN(IsHorse,               bool,    0x006B0EC0); // checks whether the actor has the Horse Keyword default object
         DEFINE_MEMBER_FN(IsHostileToActor,      bool,    0x006D4360, Actor* actor);
         DEFINE_MEMBER_FN(IsInKillMove,          bool,    0x006E16F0);
         DEFINE_MEMBER_FN(IsIntimidated,         bool,    0x006AA3E0);
         DEFINE_MEMBER_FN(IsRunning,             bool,    0x006AB210);
         DEFINE_MEMBER_FN(IsSneaking,            bool,    0x004D9290);
         DEFINE_MEMBER_FN(Kill,                  void,    0x006AC3A0, UInt32, float, UInt32, UInt32);
         DEFINE_MEMBER_FN(ModifyYaw,             void,    0x006A9A50, float); // Sets yaw to GetHeading(0) + Arg1. Uses Actor::SetYaw.
         DEFINE_MEMBER_FN(OnKillmoveDone,        void,    0x006E3CC0);
         DEFINE_MEMBER_FN(OnKillmoveStart,       void,    0x006E3C20, Actor* killer); // for paired anims, call for both actors; killer should use nullptr as argument
         DEFINE_MEMBER_FN(QueueNiNodeUpdate,     void,    0x00730EE0, bool updateWeight);
         DEFINE_MEMBER_FN(ResetAI,               void,    0x0051EA30, UInt32 unk1, UInt32 unk2);
         DEFINE_MEMBER_FN(ResetHealthAndLimbs,   void,    0x006B1CA0);
         DEFINE_MEMBER_FN(SetAlert,              void,    0x006A87A0, bool makeAlert);
         DEFINE_MEMBER_FN(SetCrimeFaction,       void,    0x006CDA10, TESFaction*);
         DEFINE_MEMBER_FN(SetDoingFavor,         void,    0x006C2870, bool);
         DEFINE_MEMBER_FN(SetFlag2_00000800,     void,    0x006CE270, bool);
         DEFINE_MEMBER_FN(SetHasBeenEaten,       void,    0x006A8AF0, bool);
         DEFINE_MEMBER_FN(SetIntimidated,        void,    0x006AA440, bool);
         DEFINE_MEMBER_FN(SetNoBleedoutRecovery, void,    0x006E13C0, bool);
         DEFINE_MEMBER_FN(SetNotShowOnStealthMeter, void, 0x006A7030, bool); // this just modifies the flag directly
         DEFINE_MEMBER_FN(SetPlayerControls,     void,    0x008DC790, bool);
         DEFINE_MEMBER_FN(SetPlayerResistingArrest, void, 0x006AEF40, bool isResisting, TESFaction*); // literally doesn't matter what actor you call this on; hell, you can call it on nullptr
         DEFINE_MEMBER_FN(SetRace,               void,    0x006AF590, TESRace*, bool isPlayer);
         DEFINE_MEMBER_FN(SetYaw,                void,    0x006A8910, float); // Honors actor-specific settings, like the race "immobile" flag. Uses underlying SetYaw method on TESObjectREFR.
         DEFINE_MEMBER_FN(TrapSoul,              bool,    0x006EC950, Actor* target); // (this) traps the soul of (target)
         DEFINE_MEMBER_FN(UpdateArmorAbility,    void,    0x006E8650, TESForm*, BaseExtraList* extraData);
         DEFINE_MEMBER_FN(UpdateWeaponAbility,   void,    0x006ED980, TESForm*, BaseExtraList* extraData, bool bLeftHand);
         DEFINE_MEMBER_FN(WillIntimidateSucceed, bool,    0x006AF180);
         //
         DEFINE_MEMBER_FN(Subroutine006A75F0, void, 0x006A75F0);
         DEFINE_MEMBER_FN(Subroutine006B6BA0, void, 0x006B6BA0, SInt32); // register with the garbage collector?
         DEFINE_MEMBER_FN(Subroutine006E05F0, void, 0x006E05F0, UInt32 avIndex, ActorValueState*, UInt32 unk);
         DEFINE_MEMBER_FN(Subroutine006DFEC0, void, 0x006DFEC0, UInt32 avIndex, float mod); // apparently RestoreActorValue, but the caller has to do a little bit of work themselves, too
         //
         bool IsAIEnabled();
         TESForm* GetEquippedObject(bool abLeftHand);
         void SetHeadTracking(bool abEnable);
         void StopCombatAlarm();
         void UpdateSkinColor();
         void UpdateHairColor();

         class FactionVisitor {
            public:
               virtual bool Accept(TESFaction * faction, SInt8 rank) = 0;
         };

         // Can contain duplicate entries with different rankings
         bool VisitFactions(FactionVisitor & visitor);

         SInt32 GetProcessManagerUnk9B() {
            auto eax = this->processManager;
            if (!eax)
               return -1;
            return eax->unk9B;
         };
         inline bool IsEssential() {
            return !!(this->flags2 & kFlags_IsEssential);
         };
         inline bool IsGuard() {
            return !!(this->flags1 & kFlags_IsGuard);
         };
         inline bool IsSwimming() {
            return (this->actorState.flags04 & 0x400 & 0x3FFF) == 0x400;
         };
   };

   STATIC_ASSERT(offsetof(Actor, magicTarget) == 0x54);
   STATIC_ASSERT(offsetof(Actor, actorValueOwner) == 0x60);
   STATIC_ASSERT(offsetof(Actor, actorState) == 0x64);
   STATIC_ASSERT(offsetof(Actor, transformDeltaEvent) == 0x70);
   STATIC_ASSERT(offsetof(Actor, addedSpells) == 0xFC);
   STATIC_ASSERT(sizeof(Actor) == 0x19C);

   class Character : public Actor {
   };

   // ----- resources -----

   class UnknownClass_VTBL_01122D98 {
      public:
         virtual ~UnknownClass_VTBL_01122D98();

         virtual bool  Unk_01(UInt32, UInt32); // Base class: this->unk24.TESV_00B726A0(arg1, arg2); return true;
         virtual void* Unk_02(UInt32);
         virtual void* Unk_03(UInt32);
         virtual void  Unk_04(UInt32);
         virtual void* Unk_05();
         virtual void  Unk_06(); // Base class: no-op.
         virtual void  Unk_07(UInt32);
         virtual void  Unk_08(UInt32);
         virtual bool  Unk_09(void); // Base class: no-op; returns true.

         BSIntrusiveRefCounted refCount; // 04 // exact type assumed, but I know it's a refcount
         UInt32 unk08 = 0x80000000;
         UnkFormArray unk0C;
         UnkFormArray unk18;
         UInt32 unk24[(0x84 - 0x24) / 4];
         UInt32 unk84 = 0;
         void*  unk88; // initialized to 0x01B9E33C; pointed-to object has VTBL but no RTTI
   };
   class IMovementMessageInterface {};
   class IMovementMotionDrivenControl {
      public:
         ~IMovementMotionDrivenControl();

         virtual void Unk_01(); // checks actor race flag 0x80; passes an object (conditional on that) as argument to MovementControllerNPC::TESV_00B71AB0
         virtual void Unk_02(); // checks actor race flag 0x80; does other stuff, and then calls ((MovementControllerNPC*)this)->unk94->Unk_04();
         virtual void Unk_03(void);
         virtual void Unk_04();
         virtual void Unk_05(void);
         virtual void Unk_06(void);
         virtual void Unk_07(void);
         virtual void Unk_08(void);

         // All its virtual methods are the exact same subroutine -- same pointer. Presumably, subclasses or even specific 
         // instances override them. We can see that overriding in the member on MovementControllerNPC; check the debugger 
         // for VTBL:MovementControllerNPC::unk90. Comments describe the methods' effects on that class.
   };
   class IMovementSelectIdle {
      public:
         ~IMovementSelectIdle();

         virtual void Unk_01(void);
         virtual void Unk_02(void);
         virtual void Unk_03(void);
         virtual void Unk_04();
         virtual void Unk_05(void);

         // All five virtual methods are the exact same subroutine -- same pointer. Presumably, subclasses or even specific 
         // instances override them. We can see that overriding in the member on MovementControllerNPC; check the debugger 
         // for VTBL:MovementControllerNPC::unk94.
   };
   class IMovementDirectControl {};
   class IMovementPlannerDirectControl {};
   class IAnimationSetCallbackFunctor {};
   class MovementControllerNPC : public UnknownClass_VTBL_01122D98 { // sizeof == 0xF0
      public:
         virtual ~MovementControllerNPC();

         virtual void Unk_06();     // overrides base
         virtual bool Unk_09(void); // overrides base... with another no-op that returns true. wot m8?

         virtual void Unk_0A(UInt32); // contains the string "IMovementSetSprinting" so I think we can guess at what it's related to
         virtual void Unk_0B(void); // contains the string "IMovementSetSprinting" so I think we can guess at what it's related to
         virtual void Unk_0C(); // Papyrus Actor.SetPlayerControls(false) uses this, unless I got it mixed up
         virtual void Unk_0D(); // Papyrus Actor.SetPlayerControls(true)  uses this, unless I got it mixed up
         virtual bool Unk_0E(); // returns true if unkE9 is false
         virtual bool Unk_0F(); // returns unkE9
         virtual void Unk_10(); // if (this->TESV_00B70D30(*(0x01B38108))) { this->unkE4 = 6; }
         virtual void Unk_11(UInt8); // if (arg1 != this->unkE8) { this->unkE8 = arg1; if (this->unk90.Unk_06()) { this->unk90.Unk_04(); } }
         virtual void Unk_12(UInt32); // aligns stack and does lots of stuff
         virtual void Unk_13(void); // related to PathingStreamLoadGame (creates an instance of?)
         virtual void Unk_14(void);

         //
         // Interfaces:
         IMovementMessageInterface     unk8C;
         IMovementMotionDrivenControl  unk90;
         IMovementSelectIdle           unk94;
         IMovementDirectControl        unk98;
         IMovementPlannerDirectControl unk9C;
         IAnimationSetCallbackFunctor  unkA0;

         UInt32       unkA4 = 0;
         UInt32       unkA8 = 0;
         UnkFormArray unkAC;
         UnkFormArray unkB8;
         UnkFormArray unkC4;
         UInt32       unkD0 = 0;
         UInt32       unkD4 = 0;
         UInt32       unkD8 = 0;
         UInt32       unkDC = 0;
         Actor*       unkE0 = NULL; // actor to which this controller applies?
         UInt32       unkE4 = 2;
         UInt8        unkE8 = 0;
         UInt8        unkE9 = 0; // likely a bool
         UInt8        unkEA = 0;
         UInt8        unkEB = 0;
         UInt8        unkEC = 0;
         UInt8        unkED = 0;
         UInt8        unkEE = 0;
         UInt8        unkEF = 0;
   };
};