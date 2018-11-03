#include "ActorValueInfo.h"

namespace RE {
   DEFINE_SUBROUTINE(bool, ComputeDynamicBaseActorValue, 0x005AD640, ActorValueOwner* subject, UInt32 avIndex, float* out); // calls the appropriate OverrideAVBase; returns true if a variable value is computed

   DEFINE_SUBROUTINE(ActorValueInfo*, DefineActorValueInfo,    0x005AD710, void*, UInt32 index, char* name, UInt32, UInt32 flags, void* subroutine, UInt32);
   DEFINE_SUBROUTINE(void,            DefineActorValueEnum,    0x005AD080, ActorValueInfo*, ...); // defines enum values (char*) for an enum actor value
   DEFINE_SUBROUTINE(ActorValueInfo*, GetActorValueByIndex,    0x005AD350, UInt32 index);
   DEFINE_SUBROUTINE(UInt32,          IndexOfActorValueByName, 0x005AD5F0, const char* name);

   extern ActorValueChangeHandler* const g_actorValueChangeHandlers = (ActorValueChangeHandler*) 0x01B39C98;
};