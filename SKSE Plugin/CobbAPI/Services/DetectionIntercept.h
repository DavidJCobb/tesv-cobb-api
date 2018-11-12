#pragma once
#include <mutex>
#include <unordered_map>
#include <vector>
#include "Miscellaneous/strings.h"
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
               FormID actor    = 0;
               UInt32 refCount = 0;
               cobb::lowerstring tag;
               bool   enabled  = true; // set to false if you forcibly remove an actor
               UInt8  loadOrderSlot = 0xFF;
               //
               Registration() {};
               Registration(FormID a, UInt8 b, const char* c) : actor(a), loadOrderSlot(b), tag(c), refCount(1) {};
               inline bool is_empty() const {
                  return (!this->actor && !this->refCount) || (this->loadOrderSlot == 0xFF);
               };
            };
         public:
            Feature() {};
            Feature(const Feature&) = delete; // no copying
            //
            RegistrationHandle add(FormID formID, UInt8 loadOrderSlot, const char* tag = nullptr);
            RegistrationHandle add(RE::Actor* actor, TESForm* persistenceForm, const char* tag = nullptr);
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
            std::unordered_map<cobb::lowerstring, std::vector<RegistrationHandle>> byStringTag;
            mutable std::recursive_mutex lock;
            RegistrationHandle           firstEmpty = 0;
            //
            typedef std::lock_guard<decltype(lock)> feature_lock_guard;
            //
            // These functions don't lock:
            //
            bool _has_any_of_actor(FormID formID) const; // returns true if there are any enabled registrations for this actor
            RegistrationHandle _find_enabled_only(FormID formID, UInt8 loadOrderSlot, const char* tag) const; // finds the first enabled registration matching the criteria
            RegistrationHandle _find_next_empty(RegistrationHandle startAt) const;
            void _remove_element(RegistrationHandle index, bool updateAffected = true); // blanks out a registration; optionally calls _update_affected_state_after_removal
            void _shrink_to_fit(); // shears "dead" registrations off of the end of the list
            void _update_affected_state_after_removal(FormID formID); // removes formID from affectedActors if no alive registrations match it
      };
   public:
      bool isActive = true;
      Feature unseenActors;
      Feature unseeingActors;
      //
      void GetActorStatus(RE::Actor*, bool& outUnseen, bool& outUnseeing) const;
      bool IsEmpty() const;
      void OnFormDestroyed(UInt32 formID);
      void Reset();
      bool ShouldCancelDetection(RE::Actor* seeker, RE::Actor* target) const;
      //
      enum { kSaveVersion = 2 };
      bool Save(SKSESerializationInterface* intfc);
      bool Load(SKSESerializationInterface* intfc, UInt32 version);
};