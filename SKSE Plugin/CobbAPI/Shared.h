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
extern const UInt32 g_pluginVersion;

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