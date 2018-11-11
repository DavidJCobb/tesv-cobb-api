#pragma once
#include <mutex>
#include <unordered_map>
#include <vector>
#include "ReverseEngineered/Forms/Actor.h"
#include "skse/GameReferences.h"

class DetectionInterceptService {
   public:
      typedef UInt32 FormID;
      typedef SInt32 RegistrationHandle;
      static constexpr SInt32 registration_not_found = -1;
      //
   public:
      static DetectionInterceptService& GetInstance() {
         static DetectionInterceptService instance;
         return instance;
      };
      DetectionInterceptService() {};
      DetectionInterceptService(const DetectionInterceptService&) = delete; // no copying
   public:
      class Feature {
         protected:
            struct Registration {
               FormID      actor    = 0;
               UInt32      refCount = 0;
               const char* tag      = nullptr;
               bool        enabled  = true; // set to false if you forcibly remove an actor
               //
               Registration() {};
               Registration(FormID a, const char* b) : actor(a), tag(b), refCount(1) {};
               inline bool is_empty() const {
                  return !this->actor && !this->refCount;
               };
            };
         public:
            Feature() {};
            Feature(const Feature&) = delete; // no copying
            //
            RegistrationHandle add(RE::Actor* actor);
            RegistrationHandle add(FormID formID, const char* tag = nullptr);
            RegistrationHandle add(RE::Actor* actor, const char* tag);
            void remove(RE::Actor* actor, RegistrationHandle);
            void remove(FormID formID, RegistrationHandle);
            void remove_all_of(const char* tag);
            void force_remove(RE::Actor* actor);
            void force_remove(FormID formID);
            //
            bool contains(RE::Actor* actor) const;
            bool empty() const;
            void reset(); // just wipes the lists; only use this when switching between playthroughs
            //
            bool Save(SKSESerializationInterface* intfc);
            bool Load(SKSESerializationInterface* intfc, UInt32 version);
         protected:
            std::vector<Registration> registrations;
            std::vector<FormID>       affectedActors;
            std::unordered_map<std::string, std::vector<RegistrationHandle>> byStringTag;
            mutable std::recursive_mutex lock;
            RegistrationHandle           firstEmpty = 0;
            //
            typedef std::lock_guard<decltype(lock)> feature_lock_guard;
            //
            RegistrationHandle _find_enabled_only(FormID formID) const; // doesn't lock
            RegistrationHandle _find_next_empty(RegistrationHandle startAt) const; // doesn't lock
      };
   public:
      Feature unseenActors;
      Feature unseeingActors;
      //
      void GetActorStatus(RE::Actor*, bool& outUnseen, bool& outUnseeing) const;
      bool IsEmpty() const;
      void OnFormDestroyed(UInt32 formID);
      void Reset();
      bool ShouldCancelDetection(RE::Actor* seeker, RE::Actor* target) const;
      //
      enum { kSaveVersion = 1 };
      bool Save(SKSESerializationInterface* intfc);
      bool Load(SKSESerializationInterface* intfc, UInt32 version);
};