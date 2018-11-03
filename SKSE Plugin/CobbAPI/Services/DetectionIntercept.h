#pragma once
#include <mutex>
#include <unordered_map>
#include <vector>
#include "ReverseEngineered/Forms/Actor.h"
#include "skse/GameReferences.h"

namespace { // unnamed namespaces are file-local
   typedef UInt32 RefHandle;
   typedef SInt32 RegistrationHandle;
   constexpr SInt32 registration_not_found = -1;
   //
   struct Registration {
      RefHandle   actor    = *g_invalidRefHandle;
      UInt32      refCount = 0;
      const char* tag      = nullptr;
      bool        enabled  = true; // set to false if you forcibly remove an actor
      //
      Registration() {};
      Registration(RefHandle a, const char* b) : actor(a), tag(b), refCount(1) {};
      inline bool is_empty() const {
         return this->actor == *g_invalidRefHandle && !this->refCount;
      };
   };
   class Feature {
      public:
         Feature() {};
         Feature(const Feature&) = delete; // no copying
         //
         RegistrationHandle add(RE::Actor* actor);
         RegistrationHandle add(RefHandle handle, const char* tag = nullptr);
         RegistrationHandle add(RE::Actor* actor, const char* tag);
         void remove(RE::Actor* actor, RegistrationHandle);
         void remove(RefHandle handle, RegistrationHandle);
         void remove_all_of(const char* tag);
         void force_remove(RE::Actor* actor);
         void force_remove(RefHandle handle);
         //
         bool contains(RE::Actor* actor) const; // can't be const due to the use of a lock
         void reset(); // just wipes the lists; only use this when switching between playthroughs
      protected:
         std::vector<Registration>    registrations;
         std::vector<RefHandle>       affectedActors;
         std::unordered_map<std::string, std::vector<RegistrationHandle>> byStringTag;
         mutable std::recursive_mutex lock;
         RegistrationHandle           firstEmpty = 0;
         //
         typedef std::lock_guard<decltype(lock)> feature_lock_guard;
         //
         RegistrationHandle _find_enabled_only(RefHandle handle) const; // doesn't lock
         RegistrationHandle _find_next_empty(RegistrationHandle startAt) const; // doesn't lock
   };
};

class DetectionInterceptService {
   public:
      static DetectionInterceptService& GetInstance() {
         static DetectionInterceptService instance;
         return instance;
      };
      DetectionInterceptService() {};
      DetectionInterceptService(const DetectionInterceptService&) = delete; // no copying
   public:
      Feature unseenActors;
      Feature unseeingActors;
      //
      void GetActorStatus(RE::Actor*, bool& outUnseen, bool& outUnseeing) const;
      void Reset();
      bool ShouldCancelDetection(RE::Actor* seeker, RE::Actor* target) const;
};