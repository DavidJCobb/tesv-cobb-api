#include "Papyrus/Actor.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/Forms/Actor.h"
#include "ReverseEngineered/Forms/BGSBodyPartData.h"
#include "ReverseEngineered/Systems/Timing.h"

namespace CobbPapyrus {
   namespace Actor {
      namespace ActorValueStuff {
         float GetActorValueModifier(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject, SInt32 avIndex, SInt32 modifier) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check actor value data on a None actor.", registry, stackId);
            ERROR_AND_RETURN_0_IF(modifier < 0 || modifier > 2, "You must specify a valid modifier. Valid values are: 0 = Permanent; 1 = Temporary; 2 = Damage.", registry, stackId);
            return CALL_MEMBER_FN(subject, GetActorValueModifier)((RE::ActorValueModifier)avIndex, modifier);
         };
         float GetActorValueRegenDelay(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject, SInt32 avIndex) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check actor value regen delay on a None actor.", registry, stackId);
            return CALL_MEMBER_FN(subject, GetActorValueRegenDelay)(avIndex);
         };
      };
      namespace DetectionEventStuff {
         void CeaseDetectionEvent(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
            ERROR_AND_RETURN_IF(subject == nullptr, "Cannot modify the detection event for None.", registry, stackId);
            auto ai = subject->processManager;
            if (ai == nullptr)
               return;
            CALL_MEMBER_FN(ai, CeaseDetectionEvent)();
         };
         float GetDetectionEventAge(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check the detection event age for None.", registry, stackId);
            auto ai = subject->processManager;
            if (ai == nullptr)
               return 0.0F;
            auto event = CALL_MEMBER_FN(ai, GetDetectionEvent)();
            if (!event)
               return 0.0F;
            return *RE::g_timeNow - event->timeCreatedAt;
         };
         VMResultArray<float> GetDetectionEventPosition(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
            VMResultArray<float> result;
            if (subject == nullptr) {
               registry->LogError("Cannot check the detection event age for None.", stackId);
               return result;
            }
            auto ai = subject->processManager;
            if (ai == nullptr)
               return result;
            auto event = CALL_MEMBER_FN(ai, GetDetectionEvent)();
            if (!event)
               return result;
            result.resize(3);
            result[0] = event->position.x;
            result[0] = event->position.y;
            result[0] = event->position.z;
            return result;
         };
         SInt32 GetDetectionEventSoundLevel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
            if (subject == nullptr) {
               registry->LogError("Cannot check the detection event sound level for None.", stackId);
               return -1;
            }
            auto ai = subject->processManager;
            if (ai == nullptr)
               return -1;
            auto event = CALL_MEMBER_FN(ai, GetDetectionEvent)();
            if (!event)
               return -1;
            return event->soundLevel;
         };
         bool HasDetectionEvent(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check the detection event age for None.", registry, stackId);
            auto ai = subject->processManager;
            if (ai == nullptr)
               return false;
            return CALL_MEMBER_FN(ai, GetDetectionEvent)() != nullptr;
         };
      };

      void Decapitate(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject, bool evenIfAlive) {
         //
         // Probably doesn't need to be latent; the vanilla Actor::Kill isn't.
         //
         ERROR_AND_RETURN_IF(subject == nullptr, "Cannot decapitate a None actor.", registry, stackId);
         ERROR_AND_RETURN_IF(!evenIfAlive && subject->IsDead(0) == false, "Living actors cannot be decapitated unless you explicitly request it.", registry, stackId);
         CALL_MEMBER_FN(subject, Decapitate)();
      };
      bool IsDecapitated(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check decapitation on a None actor.", registry, stackId);
         return CALL_MEMBER_FN(subject, IsLimbSevered)(RE::BGSBodyPartData::kPartType_Head);
      };
      SInt32 GetSoulSize(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot get soul size for a None actor.", registry, stackId);
         return CALL_MEMBER_FN(subject, GetSoulSize)();
      };
      //
      bool GetHasBeenEaten(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check cannibalism status for a None actor.", registry, stackId);
         return CALL_MEMBER_FN(subject, HasBeenEaten)();
      };
      void SetHasBeenEaten(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject, bool state) {
         ERROR_AND_RETURN_IF(subject == nullptr, "Cannot modify cannibalism status for a None actor.", registry, stackId);
         CALL_MEMBER_FN(subject, SetHasBeenEaten)(state);
      };
      //
      bool IsHorse(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check whether None is a horse.", registry, stackId);
         return CALL_MEMBER_FN(subject, IsHorse)();
      };
      //
      float GetBleedoutTimer(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check the bleedout timer for None.", registry, stackId);
         auto ai = subject->processManager;
         if (ai == nullptr)
            return 0.0F;
         return CALL_MEMBER_FN(ai, GetBleedoutTimer)();
      };
   }
}

bool CobbPapyrus::Actor::Register(VMClassRegistry* registry) {
   {  // Actor Value stuff
      registry->RegisterFunction(
         new NativeFunction3<StaticFunctionTag, float, RE::Actor*, SInt32, SInt32>(
            "GetActorValueModifier",
            PapyrusPrefixString("Actor"),
            Actor::ActorValueStuff::GetActorValueModifier,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction2<StaticFunctionTag, float, RE::Actor*, SInt32>(
            "GetActorValueRegenDelay",
            PapyrusPrefixString("Actor"),
            Actor::ActorValueStuff::GetActorValueRegenDelay,
            registry
         )
      );
   }
   {  // Detection event stuff
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, void, RE::Actor*>(
            "CeaseDetectionEvent",
            PapyrusPrefixString("Actor"),
            Actor::DetectionEventStuff::CeaseDetectionEvent,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, float, RE::Actor*>(
            "GetDetectionEventAge",
            PapyrusPrefixString("Actor"),
            Actor::DetectionEventStuff::GetDetectionEventAge,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, VMResultArray<float>, RE::Actor*>(
            "GetDetectionEventPosition",
            PapyrusPrefixString("Actor"),
            Actor::DetectionEventStuff::GetDetectionEventPosition,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, SInt32, RE::Actor*>(
            "GetDetectionEventSoundLevel",
            PapyrusPrefixString("Actor"),
            Actor::DetectionEventStuff::GetDetectionEventSoundLevel,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, bool, RE::Actor*>(
            "HasDetectionEvent",
            PapyrusPrefixString("Actor"),
            Actor::DetectionEventStuff::HasDetectionEvent,
            registry
         )
      );
   }
   //
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, RE::Actor*, bool>(
         "Decapitate",
         PapyrusPrefixString("Actor"),
         Actor::Decapitate,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, RE::Actor*>(
         "IsDecapitated",
         PapyrusPrefixString("Actor"),
         Actor::IsDecapitated,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Actor"), "IsDecapitated", VMClassRegistry::kFunctionFlag_NoWait);
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, float, RE::Actor*>(
         "GetBleedoutTimer",
         PapyrusPrefixString("Actor"),
         Actor::GetBleedoutTimer,
         registry
      )
   );
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, SInt32, RE::Actor*>(
         "GetSoulSize",
         PapyrusPrefixString("Actor"),
         Actor::GetSoulSize,
         registry
      )
   );
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, RE::Actor*>(
         "GetHasBeenEaten",
         PapyrusPrefixString("Actor"),
         Actor::GetHasBeenEaten,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, RE::Actor*, bool>(
         "SetHasBeenEaten",
         PapyrusPrefixString("Actor"),
         Actor::SetHasBeenEaten,
         registry
      )
   );
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, RE::Actor*>(
         "IsHorse",
         PapyrusPrefixString("Actor"),
         Actor::IsHorse,
         registry
      )
   );
   //
   return true;
};