#pragma once
#include "skse/GameInput.h"
#include "skse/Utilities.h"

namespace RE {
   class BSInputDevice {
      public:
         virtual ~BSInputDevice();
         //
         virtual UInt32 Unk_01(void);       // 01 // pure
         virtual void   Unk_02(float unk1); // 02 // pure
         virtual bool   Unk_03();           // 03 // pure
         virtual bool   IsEnabled(void);    // 04 // Always 1 for non-gamepad?
         virtual void   Unk_05(void);       // 05 // SKSE thinks this is the destructor, but if so, then what is Unk_00?
         virtual void   Unk_06(bool);       // 06 // setter for unk08?
         //
         // Instances of this class are never created, and it doesn't appear to even have a vtbl. It could 
         // be an abstract class, existing only to be extended by subclasses.
   };
   class BSGamepadDevice : public BSInputDevice {
      UInt32 unk04 = 2;
      UInt32 unk08;
      UInt32 unk0C;
      UInt32 unk10 = 0;
      UInt32 unk14 = 0;
      UInt32 unk18 = 0;
      UInt32 unk1C; // object
      UInt32 unk20; // member of unk1C?
      UInt32 unk24 = 0;
      UInt32 unk28 = -1;
      UInt8  unk29;
      UInt8  unk2A;
      UInt8  unk2B;
      bool   unk2C = false;
      UInt8  unk2D = 0;
      UInt8  unk2E;
      UInt8  unk2F;
   };
   class BSWin32GamepadDevice : public BSGamepadDevice { // sizeof == 0x80; no constructor
      UInt32 unk30[(0x80 - 0x30) / sizeof(UInt32)]; // these members aren't initialized in the superclass's constructor; I *assume* they belong to this class
   };

   class PlayerInputHandler {
      public:
         PlayerInputHandler();
         virtual ~PlayerInputHandler();

         virtual bool Unk_01();
         virtual void Unk_02();
         virtual void Unk_03();
         virtual void Unk_04(void*);

         UInt8 unk04 = true;
         UInt8 unk05;
         UInt8 unk06;
         UInt8 unk07;
   };
   class ActivateHandler : public PlayerInputHandler { // sizeof == 10
      UInt8 unk08 = false;
      UInt8 unk09 = false;
      UInt8 unk0A;
      UInt8 unk0B;
      UInt8 unk0C = false;
      UInt8 unk0D = false;
      UInt8 unk0E = false;
      UInt8 isDisabled = false;
   };
   class AttackBlockHandler : public PlayerInputHandler { // sizeof == 0x28
      UInt8 unk08 = false;
      UInt8 unk09 = false;
      UInt8 unk0A;
      UInt8 unk0B;
      UInt32 unk0C = 0;
      UInt32 unk10 = 0;
      UInt32 unk14 = 0;
      UInt32 unk18 = 0;
      UInt32 unk1C = 0;
      UInt32 unk20 = 0;
      UInt8  unk24 = false;
      UInt8  unk25 = false;
      UInt8  unk26 = false; // these are set to false whenever unk08 is set to false (when you use the Unk_06 method to set it)
      UInt8  unk27 = false; // these are set to false whenever unk08 is set to false (when you use the Unk_06 method to set it)
   };
   class AutoMoveHandler : public PlayerInputHandler { // sizeof == 8
   };
   class JumpHandler : public PlayerInputHandler { // sizeof == 8
   };
   class LookHandler : public PlayerInputHandler { // sizeof == 8
   };
   class MovementHandler : public PlayerInputHandler { // sizeof == 8
      virtual bool  Unk_01(void*);
      virtual void* Unk_02(void*, void* out); // returns Arg1 after modifying Arg2 based on it
      virtual void  Unk_03(void); // no-op
      virtual void  Unk_04(void*);
   };
   class ReadyWeaponHandler : public PlayerInputHandler { // sizeof == 8
   };
   class RunHandler : public PlayerInputHandler { // sizeof == 0xC
      UInt8 unk08 = false;
      UInt8 unk09 = false;
      UInt8 unk0A;
      UInt8 unk0B;
   };
   class ShoutHandler : public PlayerInputHandler { // sizeof == 0x10
      UInt8 unk08 = false;
      UInt8 unk09 = false;
      UInt8 unk0A;
      UInt8 unk0B;
      UInt8 unk0C = false;
      UInt8 unk0D;
      UInt8 unk0E;
      UInt8 unk0F;
   };
   class SneakHandler : public PlayerInputHandler { // sizeof == 8
   };
   class SprintHandler : public PlayerInputHandler { // sizeof == 0x10
      SprintHandler();
      virtual ~SprintHandler();

      virtual bool Unk_01(void*);
      virtual void Unk_02(); // no-op
      virtual void Unk_03(); // no-op

      virtual void Unk_04(void*); // argument instanceof PlayerControls*?
                                  // checks god mode and stamina values if the relevant actor is the player
                                  // checks input? if I nop all of the checks, then we get "sticky sprint"
                                  //  - sprint doesn't deactivate until I stop moving
                                  //  - sprint doesn't deactivate if I release the sprint button (exception listed below)
                                  //  - sprint doesn't deactivate if I move sideways or backwards
                                  //     - sideways movement seems to be at normal speed, not sprinting, and it starts and stops rapidly
                                  //     - backwards movement seems to be sprint speed scaled by backstep speed
                                  //     - we still can't actually initiate sprint while moving in directions other than input-forward
                                  //        - as a result of this, releasing and pressing the sprint key while moving sideways actually -will- exit sprinting
                                  // 
                                  // conclusion: if I had to guess, this is a per-tick handler used to terminate sprinting when needed

      UInt8 unk08 = false;
      UInt8 unk09 = false;
      UInt8 unk0A;
      UInt8 unk0B;
      UInt8 unk0C = false;
      UInt8 unk0D;
      UInt8 unk0E;
      UInt8 unk0F;
   };
   class TogglePOVHandler : public PlayerInputHandler { // sizeof == 0x10
      UInt8 unk08 = false;
      UInt8 unk09 = false;
      UInt8 unk0A;
      UInt8 unk0B;
      UInt8 unk0C = false;
      UInt8 unk0D;
      UInt8 unk0E;
      UInt8 unk0F;
   };
   class ToggleRunHandler : public PlayerInputHandler { // sizeof == 8
   };

   class PlayerControls {
      public:
         virtual ~PlayerControls();
         virtual UInt32	Unk_01();

         BSTEventSink<void*> menuOpenCloseEvent;  // 04
         BSTEventSink<void*> menuModeChangeEvent; // 08
         BSTEventSink<void*> furnitureEvent;      // 0C
         UInt32 unk10;
         float  unk14; // 14
         float  unk18; // 18 // unk14 and unk18 are almost certainly part of a single struct of two floats -- joystick state?
         float  unk1C; // 1C // camera uses this instead of unk14 when the gamepad is unavailable(?) or when animcam is enabled
         float  unk20; // 20 // unk1C and unk20 are almost certainly part of a single struct of two floats -- joystick state?
         UInt32 unk24;
         float  unk28;
         UInt32 unk2C;
         UInt32 unk30;
         float  unk34;
         UInt8  autoRun; // 38
         UInt8  runMode; // 39
         bool   unk3A;   // 3A
         bool   unk3B;   // 3B // checked by third-person camera system
         bool   disablePOVChangeScript;   // 3C
         bool   disablePOVChangeWerewolf; // 3D
         bool   unk3E;   // 3E
         bool   unk3F;   // 3F
         UInt32 unk40;   // remapMode? bools?
         UInt32 unk44;
         UInt32 unk48;
         UInt32 unk4C;   // 4C
         UInt32 unk50;
         UInt32 unk54;
         UInt32 unk58;
         UInt32 unk5C;
         UInt32 unk60;
         UInt32 unk64;
         UInt8  unk68;
         UInt8  unk69;
         UInt8  unk6A;
         UInt8  unk6B;
         UInt32 unk6C[(0x128 - 0x6C) >> 2];	// 6C
         //PlayerInputHandler* inputHandlers[13];	// 128
         MovementHandler*     handlerMovement;     // 128
         LookHandler*         handlerLook;         // 12C
         SprintHandler*       handlerSprint;       // 130
         ReadyWeaponHandler*  handlerReadyWeapon;  // 134
         AutoMoveHandler*     handlerAutoMove;     // 138
         ToggleRunHandler*    handlerToggleRun;    // 13C
         ActivateHandler*     handlerActivate;     // 140
         JumpHandler*         handlerJump;         // 144
         ShoutHandler*        handlerShout;        // 148
         AttackBlockHandler*  handlerAttackBlock;  // 14C
         RunHandler*          handlerRun;          // 150
         SneakHandler*        handlerSneak;        // 154
         TogglePOVHandler*    handlerTogglePOV;    // 158

         // is activation disabled: !handlerActivate || /* byte */ handlerActivate->unk0F != 0

         static PlayerControls* GetSingleton(void) {
            return *((PlayerControls**)0x012E7454);
         };

         // used by Hooks_Event
         PlayerControls* ctor_Hook(void);
         MEMBER_FN_PREFIX(PlayerControls);
         DEFINE_MEMBER_FN(Constructor, PlayerControls*, 0x00774F20);
         DEFINE_MEMBER_FN(ConstructInputHandlers, void, 0x00774010);
         //
         DEFINE_MEMBER_FN(SetActivationDisabled, void, 0x007711E0, bool);
         DEFINE_MEMBER_FN(IsActivationDisabled,  bool, 0x00771200);
         DEFINE_MEMBER_FN(IsTogglePOVDisabled,   bool, 0x00772A20);
         DEFINE_MEMBER_FN(PlayerIsInRunMode,     bool, 0x00771FD0); // always true when using gamepad; for keyboard, true when the player hasn't toggled running off
   };
   static_assert(offsetof(PlayerControls, runMode) == 0x039, "Data layout incorrect for RE::PlayerControls: bad runMode."); // IntelliSense chokes on this; ignore
   static_assert(offsetof(PlayerControls, remapMode) == 0x040, "Data layout incorrect for RE::PlayerControls: bad remapMode."); // IntelliSense chokes on this; ignore
   static_assert(offsetof(PlayerControls, handlerMovement) == 0x128, "Data layout incorrect for RE::PlayerControls: bad input handlers."); // IntelliSense chokes on this; ignore
};