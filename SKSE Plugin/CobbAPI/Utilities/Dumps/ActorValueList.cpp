#include "ActorValueList.h"
#include "ReverseEngineered/Forms/ActorValueInfo.h"
#include "skse/GameData.h"

void DumpActorValueList() {
   _MESSAGE("DUMPING LIST OF ACTOR VALUES...");
   char flags[33];
   flags[32] = '\0';
   auto list = ActorValueList::GetSingleton();
   for (UInt32 i = 0; i < ActorValueList::kNumActorValues; i++) {
      auto info = (RE::ActorValueInfo*) list->GetActorValue(i);
      if (!info)
         continue;
      {  // prepare to log flags
         auto f = info->actorValueFlags;
         for (UInt32 j = 0; j < 32; j++) {
            flags[31 - j] = '0';
            if (f & (1 << j))
               flags[31 - j] = '1';
         }
      }
      _MESSAGE("Actor value %03d: %24s: flags %s", i, info->name, flags);
      for (UInt32 j = 0; j < info->enumValueCount; j++) {
         auto ev = info->enumValues[j];
         if (!ev)
            continue;
         _MESSAGE(" - Enum value %02d: %s", j, ev);
      }
   }
   _MESSAGE("DUMPED LIST OF ACTOR VALUES.");
};