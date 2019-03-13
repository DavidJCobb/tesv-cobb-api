#pragma once
#include <mutex>
#include <set>
#include <vector>
#include "Miscellaneous/strings.h"
#include "ReverseEngineered/Forms/Actor.h"
#include "skse/GameReferences.h"
#include "skse/PluginAPI.h"

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
               FormID actor = 0;
               cobb::lowerstring tag;
               UInt8 loadOrderSlot = 0xFF;
               //
               Registration() {};
               Registration(FormID a, UInt8 b, const char* c) : actor(a), loadOrderSlot(b), tag(c) {};
               //
               inline bool is_empty() const {
                  return (!this->actor) || (this->loadOrderSlot == 0xFF);
               };
               inline bool matches(const FormID id) const {
                  return (this->actor == id);
               };
               inline bool matches(const cobb::lowerstring& tag) const {
                  return (this->tag == tag);
               };
               inline bool matches(const FormID a, const cobb::lowerstring& b) const {
                  return this->matches(a) && this->matches(b);
               };
            };
         public:
            Feature() {};
            Feature(const Feature&) = delete; // no copying
            //
            bool add(FormID formID, UInt8 loadOrderSlot, const char* tag);
            bool add(RE::Actor* actor, TESForm* persistenceForm, const char* tag);
            void remove(RE::Actor* actor, const char* tag);
            void remove(FormID formID, const char* tag);
            void remove_all_of(const char* tag);
            void remove_all_of(RE::Actor* actor);
            void remove_all_of(FormID formID);
            //
            bool contains(RE::Actor* actor) const;
            bool empty() const;
            void reset(); // just wipes the lists; only use this when switching between playthroughs
            //
            bool Save(SKSESerializationInterface* intfc);
            bool Load(SKSESerializationInterface* intfc, UInt32 version);
         protected:
            std::vector<Registration> registrations;
            std::set<FormID>          affectedActors;
            mutable std::recursive_mutex lock;
            //
            typedef std::lock_guard<decltype(lock)> feature_lock_guard;
            //
            // These functions don't lock:
            //
            bool _has_actor(FormID formID) const;
            bool _has_registration(FormID formID, UInt8 loadOrderSlot, const char* tag) const;
            void _rebuild_affected_actors_cache(); // clears affectedActors and then rebuilds it from the registrations
            void _update_affected_state_after_removal(FormID formID); // removes formID from affectedActors if no alive registrations match it
         public:
            bool _contains(FormID formID) const;
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