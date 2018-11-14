#include "Detection.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "ReverseEngineered/Forms/Actor.h"
#include "Services/DetectionIntercept.h"

namespace CobbPapyrus {
   namespace Detection {
      constexpr const char* ce_errorNoPersistForm  = "You must specify a \"persistence form,\" so that the DetectionInterceptService can detect when your mod is uninstalled and free your handle. You should use a form from your mod.";
      constexpr const char* ce_errorServiceOffline = "The DetectionInterceptService is not running.";
      constexpr const char* ce_errorUnseeingNone = "You cannot modify the \"unseeing\" status for a None actor.";
      constexpr const char* ce_errorUnseenNone   = "You cannot modify the \"unseen\" status for a None actor.";
      //
      bool ServiceIsAvailable(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         return DetectionInterceptService::GetInstance().isActive;
      };
      //
      bool ActorCannotSee(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* actor) {
         if (actor == nullptr)
            return false;
         return DetectionInterceptService::GetInstance().unseeingActors.contains(actor);
      }
      bool ActorCannotBeSeen(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* actor) {
         if (actor == nullptr)
            return false;
         return DetectionInterceptService::GetInstance().unseenActors.contains(actor);
      }
      bool PreventActorFromSeeing(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* actor, TESForm* persistenceForm, BSFixedString tag) {
         auto& service = DetectionInterceptService::GetInstance();
         if (!service.isActive) {
            registry->LogError(ce_errorServiceOffline, stackId);
            return -1;
         }
         if (actor == nullptr) {
            registry->LogError(ce_errorUnseeingNone, stackId);
            return -1;
         }
         if (persistenceForm == nullptr) {
            registry->LogError(ce_errorNoPersistForm, stackId);
            return -1;
         }
         if (persistenceForm->formID >= 0xFF000000) {
            registry->LogError(ce_errorNoPersistForm, stackId);
            return -1;
         }
         return service.unseeingActors.add(actor, persistenceForm, tag.data);
      }
      bool PreventActorFromBeingSeen(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* actor, TESForm* persistenceForm, BSFixedString tag) {
         auto& service = DetectionInterceptService::GetInstance();
         if (!service.isActive) {
            registry->LogError(ce_errorServiceOffline, stackId);
            return -1;
         }
         if (actor == nullptr) {
            registry->LogError(ce_errorUnseenNone, stackId);
            return -1;
         }
         if (persistenceForm == nullptr) {
            registry->LogError(ce_errorNoPersistForm, stackId);
            return -1;
         }
         if (persistenceForm->formID >= 0xFF000000) {
            registry->LogError(ce_errorNoPersistForm, stackId);
            return -1;
         }
         return service.unseenActors.add(actor, persistenceForm, tag.data);
      }
      void ReturnActorToSeeing(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* actor, BSFixedString tag) {
         ERROR_AND_RETURN_IF(actor == nullptr, ce_errorUnseeingNone, registry, stackId);
         DetectionInterceptService::GetInstance().unseeingActors.remove(actor, tag.data);
      };
      void ReturnActorToBeingSeen(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* actor, BSFixedString tag) {
         ERROR_AND_RETURN_IF(actor == nullptr, ce_errorUnseenNone, registry, stackId);
         DetectionInterceptService::GetInstance().unseenActors.remove(actor, tag.data);
      };
      void ForceActorToSeeing(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* actor) {
         ERROR_AND_RETURN_IF(actor == nullptr, ce_errorUnseeingNone, registry, stackId);
         DetectionInterceptService::GetInstance().unseeingActors.remove_all_of(actor);
      };
      void ForceActorToBeingSeen(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::Actor* actor) {
         ERROR_AND_RETURN_IF(actor == nullptr, ce_errorUnseenNone, registry, stackId);
         DetectionInterceptService::GetInstance().unseenActors.remove_all_of(actor);
      };
      void ForceTagToSeeing(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString tag) {
         if (!tag.data)
            return;
         DetectionInterceptService::GetInstance().unseeingActors.remove_all_of(tag.data);
      };
      void ForceTagToBeingSeen(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString tag) {
         if (!tag.data)
            return;
         DetectionInterceptService::GetInstance().unseenActors.remove_all_of(tag.data);
      };
      void InvalidateTag(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString tag) {
         if (!tag.data)
            return;
         DetectionInterceptService::GetInstance().unseeingActors.remove_all_of(tag.data);
         DetectionInterceptService::GetInstance().unseenActors.remove_all_of(tag.data);
      };

      bool Register(VMClassRegistry* registry) {
         registry->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>(
            "ServiceIsAvailable",
            "CobbAPIDetection",
            ServiceIsAvailable,
            registry
         ));
         registry->SetFunctionFlags("CobbAPIDetection", "ServiceIsAvailable", VMClassRegistry::kFunctionFlag_NoWait);
         registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, RE::Actor*>(
            "ActorCannotSee",
            "CobbAPIDetection",
            ActorCannotSee,
            registry
         ));
         registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, RE::Actor*>(
            "ActorCannotBeSeen",
            "CobbAPIDetection",
            ActorCannotBeSeen,
            registry
         ));
         registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, RE::Actor*, TESForm*, BSFixedString>(
            "PreventActorFromSeeing",
            "CobbAPIDetection",
            PreventActorFromSeeing,
            registry
         ));
         registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, RE::Actor*, TESForm*, BSFixedString>(
            "PreventActorFromBeingSeen",
            "CobbAPIDetection",
            PreventActorFromBeingSeen,
            registry
         ));
         registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, void, RE::Actor*, BSFixedString>(
            "ReturnActorToSeeing",
            "CobbAPIDetection",
            ReturnActorToSeeing,
            registry
         ));
         registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, void, RE::Actor*, BSFixedString>(
            "ReturnActorToBeingSeen",
            "CobbAPIDetection",
            ReturnActorToBeingSeen,
            registry
         ));
         registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, RE::Actor*>(
            "ForceActorToSeeing",
            "CobbAPIDetection",
            ForceActorToSeeing,
            registry
         ));
         registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, RE::Actor*>(
            "ForceActorToBeingSeen",
            "CobbAPIDetection",
            ForceActorToBeingSeen,
            registry
         ));
         registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, BSFixedString>(
            "ForceTagToSeeing",
            "CobbAPIDetection",
            ForceTagToSeeing,
            registry
         ));
         registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, BSFixedString>(
            "ForceTagToBeingSeen",
            "CobbAPIDetection",
            ForceTagToBeingSeen,
            registry
         ));
         registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, BSFixedString>(
            "InvalidateTag",
            "CobbAPIDetection",
            InvalidateTag,
            registry
         ));
         return true;
      };
   };
};