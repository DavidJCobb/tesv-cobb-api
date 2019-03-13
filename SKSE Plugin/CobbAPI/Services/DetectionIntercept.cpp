#include "DetectionIntercept.h"
#include "skse/Serialization.h"
#include "ReverseEngineered/Systems/ChangeForms.h"
#include "shared.h"
#include <algorithm>

namespace {
   typedef DetectionInterceptService::Feature Feature;
   typedef DetectionInterceptService::RegistrationHandle RegistrationHandle;
   typedef DetectionInterceptService::FormID FormID;
};
//
//
//
bool Feature::add(RE::Actor* actor, TESForm* persistenceForm, const char* tag) {
   if (!actor)
      return registration_not_found;
   return this->add(actor->formID, persistenceForm->formID >> 0x18, tag);
};
bool Feature::add(FormID formID, UInt8 loadOrderSlot, const char* tag) {
   if (formID == 0)
      return false;
   if (!tag || !strlen(tag))
      return false;
   //
   feature_lock_guard scopedLock(this->lock);
   //
   if (this->_has_registration(formID, loadOrderSlot, tag))
      return true;
   //
   this->affectedActors.insert(formID);
   this->registrations.emplace_back(formID, loadOrderSlot, tag);
   return true;
};
void Feature::remove(RE::Actor* actor, const char* tag) {
   if (!actor)
      return;
   this->remove(actor->formID, tag);
};
void Feature::remove(FormID formID, const char* tag) {
   feature_lock_guard scopedLock(this->lock);
   //
   const cobb::lowerstring ls_tag = tag; // so that we aren't continually making and destroying cobb::lowerstrings in the lambda
   auto& list = this->registrations;
   list.erase(
      std::remove_if(
         list.begin(),
         list.end(),
         [formID, ls_tag](const Registration& reg){
            return reg.matches(formID, ls_tag);
         } 
      ),
      list.end()
   );
   this->_update_affected_state_after_removal(formID);
};
void Feature::remove_all_of(const char* tag) {
   feature_lock_guard scopedLock(this->lock);
   //
   const cobb::lowerstring ls_tag = tag; // so that we aren't continually making and destroying cobb::lowerstrings in the lambda
   auto& list = this->registrations;
   list.erase(
      std::remove_if(
         list.begin(),
         list.end(),
         [ls_tag](const Registration& reg){
            return reg.matches(ls_tag);
         } 
      ),
      list.end()
   );
   this->_rebuild_affected_actors_cache();
};
void Feature::remove_all_of(RE::Actor* actor) {
   if (!actor)
      return;
   this->remove_all_of(actor->formID);
};
void Feature::remove_all_of(FormID formID) {
   feature_lock_guard scopedLock(this->lock);
   //
   auto& list = this->registrations;
   list.erase(
      std::remove_if(
         list.begin(),
         list.end(),
         [formID](const Registration& reg){
            return reg.matches(formID);
         } 
      ),
      list.end()
   );
   this->_update_affected_state_after_removal(formID);
};
bool Feature::contains(RE::Actor* actor) const {
   if (!actor)
      return false;
   feature_lock_guard scopedLock(this->lock);
   return this->_contains(actor->formID);
};
bool Feature::empty() const {
   return this->registrations.empty();
};
void Feature::reset() {
   feature_lock_guard scopedLock(this->lock);
   this->affectedActors.clear();
   this->registrations.clear();
};
//
bool Feature::_contains(FormID formID) const {
   return this->affectedActors.find(formID) != this->affectedActors.end();
}
bool Feature::_has_actor(FormID formID) const {
   auto& r = this->registrations;
   for (auto it = r.begin(); it != r.end(); ++it)
      if (it->matches(formID))
         return true;
   return false;
}
bool Feature::_has_registration(FormID formID, UInt8 loadOrderSlot, const char* tag) const {
   const cobb::lowerstring ls_tag = tag; // so we're not continually creating and destroying cobb::lowerstrings as part of a cast
   auto& r = this->registrations;
   for (auto it = r.begin(); it != r.end(); ++it)
      if (it->loadOrderSlot == loadOrderSlot && it->matches(formID, ls_tag))
         return true;
   return false;
}
void Feature::_rebuild_affected_actors_cache() {
   this->affectedActors.clear();
   auto& list = this->registrations;
   for (auto it = list.begin(); it != list.end(); ++it)
      if (it->actor)
         this->affectedActors.insert(it->actor);
}
void Feature::_update_affected_state_after_removal(FormID formID) {
   if (!this->_has_actor(formID))
      //
      // We just killed the last registration for an actor. Remove that actor from the 
      // "affected actors" list.
      //
      this->affectedActors.erase(formID);
};
//
//
//
void DetectionInterceptService::GetActorStatus(RE::Actor* subject, bool& outUnseen, bool& outUnseeing) const {
   outUnseen   = this->unseenActors.contains(subject);
   outUnseeing = this->unseeingActors.contains(subject);
};
bool DetectionInterceptService::IsEmpty() const {
   return this->unseeingActors.empty() && this->unseenActors.empty();
};
void DetectionInterceptService::OnFormDestroyed(UInt32 formID) {
   this->unseeingActors.remove_all_of(formID);
   this->unseenActors.remove_all_of(formID);
};
__declspec(noinline) void DetectionInterceptService::Reset() {
   this->unseeingActors.reset();
   this->unseenActors.reset();
}
bool DetectionInterceptService::ShouldCancelDetection(RE::Actor* seeker, RE::Actor* target) const {
   /*// Functions that lock:
   if (this->unseeingActors.contains(seeker))
      return true;
   if (this->unseenActors.contains(target))
      return true;
   //*/
   //*// Functions that don't lock:
   if (seeker && this->unseeingActors._contains(seeker->formID))
      return true;
   if (target && this->unseenActors._contains(target->formID))
      return true;
   //*/
   return false;
};




bool Feature::Save(SKSESerializationInterface* intfc) {
   using namespace Serialization;
   feature_lock_guard scopedLock(this->lock);
   //
   UInt32 size = this->registrations.size();
   SERIALIZATION_ASSERT(WriteData(intfc, &size), "Failed to write the registration count.");
   UInt32 i = 0;
   for (size_t i = 0; i < size; i++) {
      auto& entry = this->registrations[i];
      SERIALIZATION_ASSERT(WriteData(intfc, &entry.actor), "Failed to write registration %d's actor formID.", i);
      SERIALIZATION_ASSERT(WriteData(intfc, &entry.loadOrderSlot), "Failed to write registration %d's load order slot.", i);
      if (entry.tag.size()) {
         UInt32 length = entry.tag.size();
         SERIALIZATION_ASSERT(WriteData(intfc, &length), "Failed to write registration %d's tag length.", i);
         SERIALIZATION_ASSERT(intfc->WriteRecordData(entry.tag.data(), length), "Failed to write registration %d's tag.", i);
      } else {
         UInt32 length = 0;
         SERIALIZATION_ASSERT(WriteData(intfc, &length), "Failed to write registration %d's tag length.", i);
      }
   }
   return true;
};
bool Feature::Load(SKSESerializationInterface* intfc, UInt32 version) {
   using namespace Serialization;
   feature_lock_guard scopedLock(this->lock);
   //
   UInt32 count = 0;
   SERIALIZATION_ASSERT(ReadData(intfc, &count), "Failed to read the registration count.");
   for (UInt32 i = 0; i < count; i++) {
      Registration reg;
      SERIALIZATION_ASSERT(ReadData(intfc, &reg.actor), "Failed to read registration %d's actor formID.", i);
      SERIALIZATION_ASSERT(ReadData(intfc, &reg.loadOrderSlot), "Failed to read registration %d's load order slot.", i);
      {
         UInt32 formID = (UInt32)reg.loadOrderSlot << 0x18;
         UInt32 fixedID;
         if (!intfc->ResolveFormId(formID, &fixedID)) {
            reg.loadOrderSlot = 0xFF;
         } else {
            reg.loadOrderSlot = fixedID >> 0x18;
         }
      }
      UInt32 length = 0;
      SERIALIZATION_ASSERT(ReadData(intfc, &length), "Failed to read registration %d's tag length.", i);
      if (length) {
         auto buffer = (char*) malloc(length);
         SERIALIZATION_ASSERT(intfc->ReadRecordData(buffer, length), "Failed to read registration %d's tag.", i);
         reg.tag.assign(buffer, length);
         delete buffer;
      }
      reg.tag.shrink_to_fit();
      {  // Handle load order changes.
         UInt32 fixedFormId;
         if (!intfc->ResolveFormId(reg.actor, &fixedFormId))
            fixedFormId = 0;
         reg.actor = fixedFormId;
      }
      if (reg.actor && reg.loadOrderSlot != 0xFF) {
         this->registrations.push_back(reg);
         if (reg.actor)
            this->affectedActors.insert(reg.actor);
      }
   }
   return true;
};
bool DetectionInterceptService::Save(SKSESerializationInterface* intfc) {
   using namespace Serialization;
   //
   SERIALIZATION_ASSERT(this->unseeingActors.Save(intfc), "Failed to write the unseeing-actors list.");
   SERIALIZATION_ASSERT(this->unseenActors.Save(intfc), "Failed to write the unseen-actors list.");
   return true;
};
bool DetectionInterceptService::Load(SKSESerializationInterface* intfc, UInt32 version) {
   using namespace Serialization;
   //
   SERIALIZATION_ASSERT(this->unseeingActors.Load(intfc, version), "Failed to read the unseeing-actors list.");
   SERIALIZATION_ASSERT(this->unseenActors.Load(intfc, version), "Failed to read the unseen-actors list.");
   return true;
};