#pragma once

#include "skse/GameBSExtraData.h"
#include "skse/GameObjects.h"
#include "skse/GameRTTI.h"
#include "skse/NiObjects.h"
#include "skse/NiTypes.h"
#include "skse/PluginAPI.h"
#include "Services/PapyrusFunctorStorage.h"
#include "skse/Utilities.h"

extern PluginHandle g_pluginHandle;

extern SKSEMessagingInterface*     g_ISKSEMessaging;
extern SKSESerializationInterface* g_ISKSESerialization;
extern SKSETaskInterface*          g_ISKSETask;

namespace cobb {
   std::string stringf(const char* format, ...);
};

#define SERIALIZATION_ASSERT(condition, error, ...) \
   if (!(condition)) { \
      _MESSAGE("%s: " error, __FUNCTION__, __VA_ARGS__); \
      return false; \
   }

#define DEBUG_ONLY_MESSAGE(...) 
#ifdef _DEBUG
#undef DEBUG_ONLY_MESSAGE
#define DEBUG_ONLY_MESSAGE(...) _MESSAGE(__VA_ARGS__);
#endif

#define DEFINE_SUBROUTINE_EXTERN(retType, name, address, ...) extern retType(*name##)(__VA_ARGS__);
#define DEFINE_SUBROUTINE(retType, name, address, ...) retType(*name##)(__VA_ARGS__) = (retType(*)(__VA_ARGS__))address;
// Creates a function pointer to an assembly subroutine.
// Usage:
//  
// DEFINE_SUBROUTINE(void, UnknownSubroutineDEADBEEF, 0xDEADBEEF, UInt32, UInt32);
// UnknownSubroutineDEADBEEF(0, 1);
//
// Don't forget to prefix this with "static" if used in Shared.h and outside of a class. Note also 
// that other compilers will complain if you use this to define subroutines without arguments.

inline __declspec(naked) UInt32 __fastcall bswap(UInt32) { // no idea if this will actually work; the goal is to let us use bswap(myvar) to compile as just bswap relevant-register;
   __asm {
      bswap eax;
   };
};

//
// Stuff we've reverse-engineered follows.
//

/* // TODO: remove Extended prefix and move to RE namespace and ReverseEngineered folder

struct ExtendedActorCharacterProxy;
struct ExtendedActor {
   public:
      MEMBER_FN_PREFIX(ExtendedActor);
      DEFINE_MEMBER_FN(GetCharacterProxy, ExtendedActorCharacterProxy*, 0x006A96D0);
      DEFINE_MEMBER_FN(GetAIProcessUnk9B, UInt32, 0x006AE350); // Returns this->processManager->unk9B, or 0xFFFFFFFF if there is no AI process manager.
      DEFINE_MEMBER_FN(Subroutine006BD6C0, void, 0x006BD6C0, void*);
};
struct ExtendedActorCharacterProxy { // Actor->processManager->middleProcess->unk15C? // hkCharacterProxy?
   public:
      struct UnknownData190 {
         public:
            MEMBER_FN_PREFIX(UnknownData190);
            DEFINE_MEMBER_FN(Subroutine00D42B80, UInt32, 0x00D42B80);
      };
      struct UnknownData298 {
         public:
            virtual void Unk01(void); // ???
            virtual void Unk02(void); // ???
            virtual void Unk03(void); // ???
            virtual void Unk04(void); // ???
            virtual void Unk05(void); // ???
            virtual void Unk06(void); // ???
            virtual void Unk07(void); // ???
            virtual void Unk08(void); // ???
            virtual void Unk09(void); // ???
            virtual void Unk0A(void); // ???
            virtual void Unk0B(void); // ???
            virtual void Unk0C(void); // ???
            virtual void Unk0D(void); // ???
            virtual void Unk0E(void); // ???
            virtual void Unk0F(void); // ???
            virtual void Unk11(void); // ???
            virtual void Unk12(void); // ???
            virtual void Unk13(void); // ???
            virtual void Unk14(void); // ???
            virtual void Unk15(void); // ???
            virtual void Unk16(void); // ???
            virtual void Unk17(void); // ???
            virtual void Unk18(void); // ???
            virtual void Unk19(void); // ???
            virtual void Unk1A(void); // ???
            virtual void Unk1B(void); // ???
            virtual void Unk1C(void); // ???
            virtual void Unk1D(void); // ???
            virtual void Unk1E(void); // ???
            virtual void Unk1F(void); // ???
            virtual void Unk21(void); // ???
            virtual void Unk22(void); // ???
            virtual void Unk23(void); // ???
            virtual void Unk24(void); // ???
            virtual void* Unk25();    // Address unknown. No arguments. I know it returns something at least 0x364 bytes long, but I don't know what.
      };
      struct UnknownData2A0 {
         public:
            struct UnknownData10 {
               public:
                  float unk000[4]; // TODO: If we find any virtual methods, we need to realign these members to make room for the vtbl pointer!
                  UInt8 pad010[0xE0];
                  float unk0F0[4]; // quaternion? x-modifier of some kind? local axis?!
                  float unk0A0[4]; // quaternion? y-modifier of some kind? local axis?!
                  float unk0B0[4]; // quaternion? z-modifier of some kind? local axis?!
                  UInt8 pad0C0[0x70];
                  float unk120[4]; // quaternion?
                  UInt8 pad130[0x30];
                  float unk160[4]; // quaternion?
            };
            UInt32          pad00[3];
            UnknownData10*  unk10;
            //
            DEFINE_MEMBER_FN_LONG(UnknownData2A0, GetUnk10, UnknownData10*, 0x00D60090);
      };
      //
      UInt8           pad004[0x90 - 4]; // 004
      float           unk090[4];
      UInt8           pad0A0[0xF0];
      UnknownData190* unk190;
      UInt8           pad194[0x24];
      UInt32          unk1B8;
      UInt8           pad1BC[0xDC];
      UnknownData298* unk298;
      UInt32          pad29C;
      UnknownData2A0* unk2A0;
      //
      MEMBER_FN_PREFIX(ExtendedActorCharacterProxy);
      DEFINE_MEMBER_FN(Subroutine004D4950, void, 0x004D4950, float[4]); // Argument is address of four floats. Analysis strongly indicates it's a NiQuaternion, but this is used for POSITION, so... :\
      //
      virtual void Unk01(void); // ???
      virtual void Unk02(void); // ???
      virtual void Unk03(void); // At 0x00D64FE0 // Returns void.
      virtual void Unk04(void); // At 0x00D64D70 // Returns void. Args appear to be a NiQuaternion* and two bools (passed as int32s). First bool enables something about local axes? Second bool enables quaternion-related math on the input?
      virtual void Unk05(void); // ???
      virtual void Unk06(void); // ???
      virtual void Unk07(void); // ???
      virtual void Unk08(void); // ???
      virtual void Unk09(void); // ???
      virtual void Unk0A(void); // ???
      virtual void Unk0B(void); // ???
      virtual void Unk0C(void); // ???
      virtual void Unk0D(void); // ???
      virtual void Unk0E(void); // ???
      virtual void Unk0F(void); // At 0x00D64CA0. // No arguments. Returns this->unk298->Unk25()->unk360, if Unk25 returns an existent result; NULL otherwise.
};
static_assert(offsetof(ExtendedActorCharacterProxy, unk090) == 0x0090, "Data layout incorrect for ExtendedActorCharacterProxy: bad unk090."); // IntelliSense chokes on this; ignore the squiggly red lines.
static_assert(offsetof(ExtendedActorCharacterProxy, unk190) == 0x0190, "Data layout incorrect for ExtendedActorCharacterProxy: bad unk190."); // IntelliSense chokes on this; ignore the squiggly red lines.
static_assert(offsetof(ExtendedActorCharacterProxy, unk1B8) == 0x01B8, "Data layout incorrect for ExtendedActorCharacterProxy: bad unk1B8."); // IntelliSense chokes on this; ignore the squiggly red lines.
static_assert(offsetof(ExtendedActorCharacterProxy, unk2A0) == 0x02A0, "Data layout incorrect for ExtendedActorCharacterProxy: bad unk2A0."); // IntelliSense chokes on this; ignore the squiggly red lines.

struct ExtendedPlayerCharacter {
   public:
      DEFINE_MEMBER_FN_LONG(ExtendedPlayerCharacter, Subroutine00746180, void, 0x00746180, TESObjectREFR*);
};
*/