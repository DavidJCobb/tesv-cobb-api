#pragma once
#include "skse/GameForms.h"
#include "skse/GameReferences.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"

namespace RE {
   class ShaderReferenceEffect; // see ReferenceEffects.h
   class Unknown012E32E8 {
      public:
         static Unknown012E32E8* GetInstance() {
            return *(Unknown012E32E8**)0x012E32E8; // or should it be *(ClassName*)0x012E32E8 ?
         };
         //

         UInt8  unk000;                   // 008
         bool   enableDetection;          // 001 // if true, then AI detection is enabled; see console:ToggleDetection
         bool   unk002;                   // 002 // if true, then AI detection stats should be printed to the screen; see console:ToggleDetectionStats
         UInt8  unk003;                   // 003
         UInt32 unk004;                   // 004
         bool   enableHighProcess;        // 008 // if true, then AI processing for actors in high is on; see console:ToggleHighProcess
         bool   enableLowProcess;         // 009 // if true, then AI processing for actors in low is on; see console:ToggleLowProcess
         bool   enableMiddleHighProcess;  // 00A // if true, then AI processing for actors in middle-high is on; see console:ToggleMiddleHighProcess
         bool   enableMiddleLowProcess;   // 00B // if true, then AI processing for actors in middle-low is on; see console:ToggleMiddleLowProcess
         bool   enableAISchedules;        // 00C // if true, then AI processing for actors' editor schedules is on; see console:ToggleAISchedules
         UInt8  unk00D;                   // 00D
         UInt8  unk00E;                   // 00E
         UInt8  unk00F;                   // 00F
         SInt32 unk010;                   // 010 // loaded in subroutine 0050D190
         UInt32 pad014[(0x028 - 0x014) / sizeof(UInt32)]; // 014
         tArray<UInt32>  unk028; // 028 - ref handles for Actor*s? the TDETECT command loops over this to delete AI/detection state in actors
         tArray<UInt32>  unk034;
         tArray<UInt32>  unk040;
         tArray<UInt32>  unk04C;
         tArray<UInt32>* unk058; // 058 // initialized to &this->unk28 // referenced by opcode at 00529EF2 // examine subroutine 00529EC0 for further information
         tArray<UInt32>* unk05C; // 05C // initialized to &this->unk40
         tArray<UInt32>* unk060; // 060 // initialized to &this->unk4C
         tArray<UInt32>* unk064; // 064 // initialized to &this->unk34
         UInt32 unk068[(0x84 - 0x68) / 4];
         tArray<UInt32> unk084; // 084
         UInt32 unk090 = 0;
         UInt32 unk094 = 0;
         tArray<ShaderReferenceEffect*> activeEffectShaders;      // 098
         SimpleLock                     activeEffectShaderLock;   // 0A4
         UInt32 pad0AC[(0x11C - 0x0AC) / sizeof(UInt32)]; // 0AC
         bool   enableActorAI;        // 11C // if true, then all AI processing is on
         bool   enableActorMovement;  // 11D // if true, then all actor movement processing is on
         bool   enableActorAnimation; // 11E // if true, then all actor animation processing is on
         UInt8  unk11F;               // 11F // modified by opcode at 004A6BDE
         // ...
         //
         MEMBER_FN_PREFIX(Unknown012E32E8);
         DEFINE_MEMBER_FN(ResetAllDetection,         void, 0x00542970, bool* unused);
         DEFINE_MEMBER_FN(Subroutine006A09E0,        void, 0x006A09E0, void* func); // for each of this->unk028, runs func(item); terminates early if func returns false
         DEFINE_MEMBER_FN(StopEffectShader,          void, 0x00754840, TESObjectREFR*, TESEffectShader*);
         DEFINE_MEMBER_FN(Subroutine00754900,        void, 0x00754900, TESObjectREFR*, void*); // stops a "model reference shader"?
         DEFINE_MEMBER_FN(ToggleMovementProcessing,  void, 0x00754000); // toggles the value of unk11D
         DEFINE_MEMBER_FN(ToggleAnimationProcessing, void, 0x00754020); // toggles the value of unk11E
   };
}