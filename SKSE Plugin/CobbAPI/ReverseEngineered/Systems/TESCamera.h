#pragma once
#include "skse/GameInput.h"
#include "skse/GameReferences.h"
#include "skse/Utilities.h"

namespace RE {
   class TESCameraState;
   class TESCamera {
      public:
         TESCamera();
         virtual ~TESCamera();

         virtual void SetNode(NiNode* node);
         virtual void Update();

         float           rotZ;        // 04
         float           rotX;        // 08
         NiPoint3        pos;         // 0C
         float           zoom;        // 18
         NiNode*         cameraNode;  // 1C - First child is usually NiCamera
         TESCameraState* cameraState; // 20
         UInt8           unk24;       // 24
         UInt8           pad25[3];    // 25

         MEMBER_FN_PREFIX(TESCamera);
         DEFINE_MEMBER_FN(ModifyPosition, void,   0x00653230, float x, float y, float z);
         DEFINE_MEMBER_FN(ModifyRotation, void,   0x00653200, float x, float z);
         DEFINE_MEMBER_FN(ModifyZoom,     void,   0x00653220, float);
         DEFINE_MEMBER_FN(SetCameraNode,  void,   0x00653260, NiNode*);
         DEFINE_MEMBER_FN(SetCameraNode,  bool,   0x006532B0, NiNode**); // returns bool: does the current camera node (after setting) exist?
         DEFINE_MEMBER_FN(SetCameraState, UInt32, 0x006533D0, TESCameraState* cameraState);
   };
   static_assert(offsetof(TESCamera, cameraNode) == 0x1C, "Data layout incorrect for RE::TESCamera: bad cameraNode."); // IntelliSense chokes on this; ignore

   class TESCameraState {
      public:
         TESCameraState();
         virtual ~TESCameraState();

         virtual void OnStateStart(); // pure
         virtual void OnStateEnd();	// pure
         virtual void OnUpdate(TESCameraState* unk1); // called by TESCamera::Update // arg is out-variable; if set, TESCamera::Update will switch to the given state. increment refcount before returning (receiver will decrement refcount; presumably this prevents deletion before the state is in use)
         virtual NiQuaternion* Unk_04(NiQuaternion* out);
         virtual NiPoint3* Unk_05(NiPoint3* out); // for ThirdPersonState, returns unk20
         virtual void Unk_06(void*); // pure // related to saving and loading
         virtual void Unk_07();	// pure
         virtual void Unk_08();	// pure // related to serialization?

         BSIntrusiveRefCounted refCount; // 04
         TESCamera* camera;  // 08
         UInt32     stateId; // 0C
   };
   class HorseCameraState : public TESCameraState {
      public:
         HorseCameraState();
         virtual ~HorseCameraState();

         PlayerInputHandler inputHandler;    // 10
         NiNode*             cameraNode;     // 18
         NiNode*             controllerNode; // 1C
         float               unk20[0x03];    // 20
         UInt32              unk2C[0x07];    // 2C
         float               unk48[0x03];    // 48
         UInt32              unk54[0x11];    // 54
         float               unk98[0x03];    // 98
         UInt32              unkA4[0x04];    // A4
         UInt8               unkB4[0x07];    // B4
         UInt8               padBB;          // BB
         // ...
   };
   class TweenMenuCameraState : public TESCameraState {
      public:
         TweenMenuCameraState();
         virtual ~TweenMenuCameraState();

         UInt32 unk10[0x0B]; // 10
         UInt8  unk3C;       // 3C
         UInt8  pad3D;       // 3D
         UInt16 pad3E;       // 3E
   };
   class VATSCameraState : public TESCameraState {
      public:
         VATSCameraState();
         virtual ~VATSCameraState();

         UInt32 unk10[0x03]; // 10

         MEMBER_FN_PREFIX(VATSCameraState);
         DEFINE_MEMBER_FN(Subroutine00841630, void, 0x00841630, NiPoint3*); // sets this->unk1C to the argument... is this class decoded right?
   };
   class FreeCameraState : public TESCameraState {
      public:
         FreeCameraState();
         virtual ~FreeCameraState();

         PlayerInputHandler inputHandler; // 10
         float              unk18;        // 18
         float              unk1C;        // 1C
         float              unk20;        // 20
         UInt32             unk24[0x04];  // 24
         UInt16             unk34;        // 34
         UInt8              unk36;        // 36
         UInt8              unk37;        // 37
   };
   class AutoVanityState : public TESCameraState {
      public:
         AutoVanityState();
         virtual ~AutoVanityState();
   };
   class FurnitureCameraState : public TESCameraState {
      public:
         FurnitureCameraState();
         virtual ~FurnitureCameraState();

         UInt32 unk10; // 10
         float  unk14; // 14
         float  unk18; // 18
         float  unk1C; // 1C
         UInt32 unk20; // 20
         UInt32 unk24; // 24
         UInt32 unk28; // 28
         UInt8  unk2C; // 2C
         UInt8  unk2D; // 2D
         UInt8  unk2E; // 2E
         UInt8  pad2F; // 2F
   };
   class IronSightsState : public TESCameraState {
      public:
         IronSightsState();
         virtual ~IronSightsState();

         NiNode* cameraNode; // 10
   };
   class FirstPersonState : public TESCameraState {
      public:
         FirstPersonState();
         virtual ~FirstPersonState();

         PlayerInputHandler inputHandler;   // 10
         NiNode*            cameraNode;     // 18
         NiNode*            controllerNode; // 1C
         float              sittingRotation;// 20
         float              unk24;          // 24
         float              unk28;          // 28
         UInt32             unk2C;          // 2C // -|
         UInt32             unk30;          // 30 //  |- these three values are related
         UInt32             unk34;          // 34 // _|
         UInt8              unk38;          // 38 // Setter: PlayerCamera::Subroutine0083BE20(UInt8)
         UInt8              unk39;          // 39
         UInt8              unk3A;          // 3A
         UInt8              unk3B;          // 3B // Setter: PlayerCamera::Subroutine0083BDD0(UInt8) // related to IdleCartPrisonerCIdle and IdleCartPrisonerCExit
         UInt8              unk3C;          // 3C
         UInt8              pad3D;          // 3D
         UInt16             pad3E;          // 3E
   };

   class ThirdPersonState : public TESCameraState {
      public:
         ThirdPersonState();
         virtual ~ThirdPersonState();
         virtual void Unk_09(void);                 // 09 // no-op with one argument; could be overridden by subclasses
         virtual void Unk_0A();                     // 0A // no-op with no arguments; could be overridden by subclasses
         virtual void UpdateMode(bool weaponDrawn); // 0B
         virtual bool Unk_0C();                     // 0C // true if this->animcamEnabled, this->unkB4, or PlayerControls::GetSingleton()->unk3B
         virtual void Unk_0D(void);                 // 0D // updates camera state and player model; among other things, faces the player camera-forward conditionally
         virtual void Unk_0E();                     // 0E
         virtual void Unk_0F(void*);                // 0F // Responsible for look controls. Updates unkAC. Only has an effect when not moving. Argument is a struct of two floats, received from the input system -- likely joystick state (or normalized to such for K&M); maybe angle and magnitude.

         PlayerInputHandler inputHandler;       // 10
                            // UInt32 unk10 == vtbl
                            // bool   unk14 == if false, cannot switch to first-person
                            // UInt8  unk15 == padding? changing at run-time has no effect
                            // UInt8  unk16 == padding? changing at run-time has no effect
                            // UInt8  unk17 == padding? changing at run-time has no effect
         NiNode*            cameraNode;         // 18 // offset from the actor, or actor 3D node to focus on? if nulled out at run-time, camera is relative to actor's position (i.e. between feet)
         NiNode*            controllerNode;     // 1C // not always used?
         NiPoint3           unk20;              // 20 // root position -- that is, actor's loaded-3D world position or gameplay position, preferring the former
         NiQuaternion       unk2C;              // 2C // returned by this->Unk_04
         union {                                // 3C // current camera offset; these values are overwritten when UpdateMode is called
            struct {
               float x;
               float y;
               float z;
            } overShoulderOffset; // actually a NiPoint3, but unions are bratty about member functions
            struct {
               float fOverShoulderPosX;
               float fOverShoulderCombatAddY;
               float fOverShoulderPosZ;
            };
         };
         NiPoint3           unk48;              // 48
         float              unk54;              // 54
         UInt32             unk58;              // 58
         float              unk5C;              // 5C // related to yaw; is added to unkAC during computations in Unk_0E().
         float              unk60;              // 60 // related to yaw; is modified the same as unk5C in Unk_0E().
         float              unk64;              // 64
         float              unk68;              // 68
         float              unk6C;              // 6C
         void*              unk70;              // 70 // examination in memory indicates TESIdleForm*
         void*              unk74;              // 74 // examination in memory indicates TESIdleForm*
         void*              unk78;              // 78 // examination in memory indicates TESIdleForm*
         float              unk7C;              // 7C
         void*              unk80;              // 80
         UInt32             unk84;              // 84
         NiQuaternion       unk88;              // 88
         NiPoint3           unk98;              // 98
         float              unkA4;              // A4
         float              unkA8;              // A8 // unkA8 is a ref handle on PlayerCamera; if we see it used that way, we're looking at PlayerCamera, so scroll down
         float              unkAC;              // AC // This is the difference between the camera yaw and the player yaw. Updated by Unk_0F whenever the target isn't moving. Used to face the player camera-forward (by way of Actor::ModifyYaw) when the player starts moving.
         float              unkB0;              // B0 // related to X-rotation? // unkAC and unkB0 seem to be a pair of related floats
         bool               unkB4;              // B4 // controls whether Unk_0D needs to perform certain operations, including facing the player camera-forward. not a straightforward true/false; it's compared to (somebool && somebool), the operation is performed if not equal, and then it's set to (somebool && somebool)
         UInt8              unkB5;              // B5
         bool               unkB6;              // B6 // if true, then some operation has been queued to happen OnUpdate; the operation involves updating *g_thePlayer's 3D based on their yaw (and also doing other stuff?)
         UInt8              unkB7;              // B7
         bool               animcamEnabled;     // B8
         UInt8              unkB9;              // B9
         UInt8              unkBA;              // BA
         UInt8              unkBB;              // BB // not a bool

         // Much of the structure has been decoded from ThirdPersonState::Unk_08, which appears to be a "reset" 
         // method of some kind; in particular, the NiQuaternions and NiPoints above were positively identified 
         // from code in that function that resets them (via copy) to stock values kept in memory.
   };
   static_assert(offsetof(ThirdPersonState, fOverShoulderPosX) == 0x3C, "Data layout incorrect for RE::ThirdPersonState: bad fOverShoulderPosX.");
   static_assert(offsetof(ThirdPersonState, unk48) == 0x48, "Data layout incorrect for RE::ThirdPersonState: bad unk48.");
   static_assert(offsetof(ThirdPersonState, animcamEnabled) == 0xB8, "Data layout incorrect for RE::ThirdPersonState: bad animcamEnabled.");
   
   class DragonCameraState : public ThirdPersonState { // sizeof == 0xE4
      public:
         DragonCameraState();
         virtual ~DragonCameraState();

         UInt32              unkBC;          // BC // reference handle
         UInt32              unkC0 = 0;      // C0
         UInt8               unkC4 = false;  // C4
         UInt8               unkC5;          // C5
         UInt8               unkC6;          // C6
         UInt8               unkC7;          // C7
         float               unkC8 = 1.0;    // C8
         UInt8               unkCC = false;  // CC
         UInt8               unkCD;          // CD
         UInt8               unkCE;          // CE
         UInt8               unkCF;          // CF
         UInt32              unkD0;          // D0 // initialized to *(0x01B912CC)
         UInt32              unkD4;          // D4 // initialized to *(0x01B912D0)
         UInt32              unkD8;          // D8 // initialized to *(0x01B912CC)
         UInt32              unkDC;          // DC // initialized to *(0x01B912D0)
         UInt32              unkE0;          // E0 // reference handle
   };
   class BleedoutCameraState : public ThirdPersonState {
      public:
         BleedoutCameraState();
         virtual ~BleedoutCameraState();

         PlayerInputHandler inputHandler;   // 10
         NiNode*            cameraNode;     // 18
         NiNode*            controllerNode; // 1C
         float              unk20[0x03];    // 20
         UInt32             unk2C[0x07];    // 2C
         float              unk48[0x03];    // 48
         UInt32             unk54[0x11];    // 54
         float              unk98[0x03];    // 98
         UInt32             unkA4[0x04];    // A4
         UInt8              unkB4[0x07];    // B4
         UInt8              padBB;          // BB
         // ...
   };
   class PlayerCameraTransitionState : public TESCameraState {
      public:
         PlayerCameraTransitionState();
         virtual ~PlayerCameraTransitionState();

         UInt32 unk10[0x05]; // 10
         UInt8  unk24;       // 24
         UInt8  unk25;       // 25
         UInt16 pad26;
   };

   class PlayerCamera : TESCamera {
      PlayerCamera();
      virtual ~PlayerCamera();

      static PlayerCamera* GetInstance() {
         return *((PlayerCamera**)0x012E7288); // == 0x01B399A0; the PlayerCharacter constructor creates it there, directly
      };

      enum {
         kCameraState_FirstPerson = 0,
         kCameraState_AutoVanity,
         kCameraState_VATS,
         kCameraState_Free,
         kCameraState_IronSights,
         kCameraState_Furniture,
         kCameraState_Transition,
         kCameraState_TweenMenu,
         kCameraState_ThirdPersonFurniture, // third-person furniture (i.e. workbenches)
         kCameraState_ThirdPersonGameplay,  // normal and animcam
         kCameraState_Horse,
         kCameraState_Bleedout,
         kCameraState_Dragon,
         kNumCameraStates
      };

      struct UnkA0 { // sizeof == 8; constructor == 0x00C52C60
         void*  unk00;
         UInt32 unk04;
      };

      UInt32 cameraTargetHandle; // 2C // reference handle
      UInt32 unk30; // bitmask?
      UInt32 unk34[(0x68 - 0x34) / sizeof(UInt32)]; // This is (or can be?) a list of something. See 0x006E2700.
      UInt32 unk68;
      TESCameraState* cameraStates[kNumCameraStates];
      UnkA0* unkA0;
      void*  unkA4; // sizeof >= 8
      UInt32 unkA8; // a reference handle // gets cleared whenever we switch/force to first-person camera, via a call to 0x0083C8D0
      float	worldFOV;       // AC
      float	firstPersonFOV; // B0
      NiPoint3 unkB4;       // referenced in 0x0083D4A0
      UInt32 unkC0;
      float  unkC4;
      float  unkC8;
      UInt32 unkCC;
      UInt8  unkD0;
      UInt8  unkD1; // referenced at 0x00782C40, 0x006E0B0F
      UInt8  unkD2;
      UInt8  unkD3; // referenced in member function 0x0083D4A0
      UInt8  unkD4; // D4
      UInt8  unkD5; // D5
      UInt8  padD6[2]; // D6

      MEMBER_FN_PREFIX(PlayerCamera);
      DEFINE_MEMBER_FN(ForceFirstPerson,     bool, 0x0083CE90);               // *(0x012E7288)->TESV_0083CE90();
      DEFINE_MEMBER_FN(ForceThirdPerson,     bool, 0x0083C6B0);               // *(0x012E7288)->TESV_0083C6B0();
      DEFINE_MEMBER_FN(EnterThirdPerson,     void, 0x0083CE50);
      DEFINE_MEMBER_FN(ReferenceIsCameraTarget, bool, 0x006C4610, TESObjectREFR* reference);
      DEFINE_MEMBER_FN(SetCameraTarget,      void, 0x0083D960, Actor*);       // *(0x012E7288)->TESV_0083D960(targetActor);
      DEFINE_MEMBER_FN(SetSittingRotation,   void, 0x0083BE80, float yaw);    // *(0x012E7288)->TESV_0083BE80(float);
      DEFINE_MEMBER_FN(ToggleAnimatorCam,    void, 0x0083BB90);               // *(0x012E7288)->TESV_0083BB90(); // This is the "ANIMCAM" console command.
      DEFINE_MEMBER_FN(UpdateThirdPerson,    void, 0x0083C7E0, bool weaponDrawn);
      //
      DEFINE_MEMBER_FN(GetUnkB4OrEquivalent, void, 0x0083D4A0, NiPoint3* out);
      DEFINE_MEMBER_FN(SetFirstPersonUnk34,  void, 0x0083BE60, UInt32);
      DEFINE_MEMBER_FN(ClearHandleUnkA8,     void, 0x0083C8D0);
   };
}