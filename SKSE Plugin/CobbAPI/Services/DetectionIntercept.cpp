#include "DetectionIntercept.h"
#include "skse/Serialization.h"
#include <algorithm>

namespace {
   typedef DetectionInterceptService::Feature Feature;
   typedef DetectionInterceptService::RegistrationHandle RegistrationHandle;
   typedef DetectionInterceptService::FormID FormID;
};
//
//
//
RegistrationHandle Feature::add(RE::Actor* actor) {
   if (!actor)
      return registration_not_found;
   return this->add(actor->formID);
};
RegistrationHandle Feature::add(RE::Actor* actor, const char* tag) {
   if (!actor)
      return registration_not_found;
   return this->add(actor->formID, tag);
};
RegistrationHandle Feature::add(FormID formID, const char* tag) {
   if (formID == 0)
      return registration_not_found;
   //
   feature_lock_guard scopedLock(this->lock);
   //
   auto index = this->_find_enabled_only(formID, tag);
   if (index != registration_not_found) {
      this->registrations[index].refCount++;
      return index;
   }
   //
   this->affectedActors.push_back(formID);
   if (this->firstEmpty >= this->registrations.size()) {
      index = this->registrations.size();
      this->registrations.emplace_back(formID, tag);
      if (tag)
         this->byStringTag[tag].push_back(index);
      this->firstEmpty = this->registrations.size();
      return index;
   }
   SInt32 i = this->firstEmpty;
   this->registrations[i] = Registration(formID, tag);
   if (tag) {
      this->byStringTag[tag].push_back(i);
   }
   this->firstEmpty = this->_find_next_empty(i);
   return i;
};
void Feature::remove(RE::Actor* actor, RegistrationHandle index) {
   if (!actor)
      return;
   if (index < 0 || index >= this->registrations.size())
      return;
   this->remove(actor->formID, index);
};
void Feature::remove(FormID formID, RegistrationHandle index) {
   if (index < 0 || index >= this->registrations.size())
      return;
   //
   feature_lock_guard scopedLock(this->lock);
   //
   auto& r = this->registrations[index];
   if (r.actor != formID)
      return;
   r.refCount--;
   if (r.refCount <= 0) {
      this->_remove_element(index, true);
      this->_shrink_to_fit();
   }
};
void Feature::remove_all_of(const char* tag) {
   if (!tag)
      return;
   feature_lock_guard scopedLock(this->lock);
   try {
      auto& indices = this->byStringTag.at(tag);
      auto  size    = indices.size();
      for (size_t i = 0; i < size; i++) {
         const RegistrationHandle index = indices[i];
         if (index < 0)
            continue;
         this->_remove_element(index, true);
      }
      this->_shrink_to_fit();
   } catch (std::out_of_range) {};
};
void Feature::force_remove(RE::Actor* actor) {
   if (!actor)
      return;
   this->force_remove(actor->formID);
};
void Feature::force_remove(FormID formID) {
   feature_lock_guard scopedLock(this->lock);
   //
   auto& l = this->registrations;
   auto  size = l.size();
   for (auto i = 0; i < size; i++) {
      auto& r = l[i];
      if (r.enabled && r.actor == formID)
         this->_remove_element(i, false);
   }
   //
   // Would be more efficient to merge these two operations into the above loop, but 
   // it'd also be less clear and less maintainable.
   //
   this->_shrink_to_fit();
   this->_update_affected_state_after_removal(formID);
};
bool Feature::contains(RE::Actor* actor) const {
   if (!actor)
      return false;
   //
   feature_lock_guard scopedLock(this->lock);
   //
   auto& actors = this->affectedActors;
   return std::find(actors.begin(), actors.end(), actor->formID) != actors.end();
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
bool Feature::_has_any_of_actor(FormID formID) const {
   auto& r = this->registrations;
   for (auto it = r.begin(); it != r.end(); ++it) {
      if (!it->enabled)
         continue;
      if (it->actor == formID && it->refCount)
         return true;
   }
   return false;
};
RegistrationHandle Feature::_find_enabled_only(FormID formID, const char* tag) const {
   auto& r = this->registrations;
   for (auto it = r.begin(); it != r.end(); ++it) {
      if (!it->enabled)
         continue;
      if (it->actor == formID && it->tag == tag && it->refCount)
         return it - r.begin();
   }
   return registration_not_found;
};
RegistrationHandle Feature::_find_next_empty(RegistrationHandle startAt) const {
   size_t size = this->registrations.size();
   size_t i    = startAt;
   for (; i < size; i++)
      if (this->registrations[i].is_empty())
         break;
   return i;
};
void Feature::_remove_element(RegistrationHandle index, bool updateAffected) {
   auto& r = this->registrations[index];
   if (r.enabled) {
      r.enabled = false;
      if (updateAffected)
         this->_update_affected_state_after_removal(r.actor);
   }
   r.actor = 0;
   if (this->firstEmpty > index)
      this->firstEmpty = index;
   if (!r.tag.empty()) {
      try {
         auto& list = this->byStringTag.at(r.tag);
         list.erase(std::remove(list.begin(), list.end(), index), list.end());
         if (!list.size())
            this->byStringTag.erase(r.tag);
      } catch (std::out_of_range) {};
   }
};
void Feature::_shrink_to_fit() {
   auto& list = this->registrations;
   list.erase(
      std::find_if(
         list.rbegin(),
         list.rend(),
         [](const Registration& x) { // find the last element we DON'T want to remove
            return !x.is_empty();
         }
      ).base(),
      list.end()
   );
};
void Feature::_update_affected_state_after_removal(FormID formID) {
   if (!this->_has_any_of_actor(formID)) {
      //
      // We just killed the last registration for an actor. Remove that actor from the 
      // "affected actors" list.
      //
      auto& actors = this->affectedActors;
      actors.erase(std::remove(actors.begin(), actors.end(), formID), actors.end());
   }
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
   this->unseeingActors.force_remove(formID);
   this->unseenActors.force_remove(formID);
};
__declspec(noinline) void DetectionInterceptService::Reset() {
   this->unseeingActors.reset();
   this->unseenActors.reset();
}
bool DetectionInterceptService::ShouldCancelDetection(RE::Actor* seeker, RE::Actor* target) const {
   if (this->unseeingActors.contains(seeker))
      return true;
   if (this->unseenActors.contains(target))
      return true;
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
      SERIALIZATION_ASSERT(WriteData(intfc, &entry.refCount), "Failed to write registration %d's actor refcount.", i);
      SERIALIZATION_ASSERT(WriteData(intfc, &entry.enabled), "Failed to write registration %d's enabled flag.", i);
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
      SERIALIZATION_ASSERT(ReadData(intfc, &reg.refCount), "Failed to read registration %d's actor refcount.", i);
      SERIALIZATION_ASSERT(ReadData(intfc, &reg.enabled), "Failed to read registration %d's enabled flag.", i);
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
         if (!intfc->ResolveFormId(reg.actor, &fixedFormId)) {
            reg.enabled = false;
            fixedFormId = 0;
         }
         reg.actor = fixedFormId;
      }
      this->registrations.push_back(reg);
      if (reg.actor)
         this->affectedActors.push_back(reg.actor);
   }
   this->_shrink_to_fit();
   std::unique(this->affectedActors.begin(), this->affectedActors.end()); // we use this as a cache; we need to update it
   this->affectedActors.shrink_to_fit();
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
   _MESSAGE("Loading unseeing registrations...");
   SERIALIZATION_ASSERT(this->unseeingActors.Load(intfc, version), "Failed to read the unseeing-actors list.");
   _MESSAGE("Loading unseen registrations...");
   SERIALIZATION_ASSERT(this->unseenActors.Load(intfc, version), "Failed to read the unseen-actors list.");
   return true;
};