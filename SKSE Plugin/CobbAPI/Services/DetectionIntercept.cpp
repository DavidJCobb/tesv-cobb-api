#include "DetectionIntercept.h"
#include <algorithm>

namespace {
   typedef DetectionInterceptService::Feature Feature;
   typedef DetectionInterceptService::RegistrationHandle RegistrationHandle;
   typedef DetectionInterceptService::FormID FormID;
};

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
   auto index = this->_find_enabled_only(formID);
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
      if (r.enabled) {
         auto& actors = this->affectedActors;
         actors.erase(std::remove(actors.begin(), actors.end(), formID), actors.end());
      }
      r.actor   = *g_invalidRefHandle;
      r.enabled = false;
      if (this->firstEmpty > index)
         this->firstEmpty = index;
      if (r.tag) {
         try {
            const std::string tag(r.tag);
            auto& list = this->byStringTag.at(tag);
            list.erase(std::remove(list.begin(), list.end(), index), list.end());
            if (!list.size())
               this->byStringTag.erase(tag);
         } catch (std::out_of_range) {};
      }
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
         auto& r = this->registrations[index];
         if (r.enabled) {
            auto& actors = this->affectedActors;
            actors.erase(std::remove(actors.begin(), actors.end(), r.actor), actors.end());
         }
         r.enabled = false;
         if (this->firstEmpty > index)
            this->firstEmpty = index;
      }
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
   for (auto it = l.begin(); it != l.end(); ++it) {
      auto& r = *it;
      if (r.enabled && r.actor == formID)
         r.enabled = false;
   }
   auto& actors = this->affectedActors;
   actors.erase(std::remove(actors.begin(), actors.end(), formID), actors.end());
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
void Feature::reset() {
   feature_lock_guard scopedLock(this->lock);
   this->affectedActors.clear();
   this->registrations.clear();
};
RegistrationHandle Feature::_find_enabled_only(FormID formID) const {
   auto& r = this->registrations;
   for (auto it = r.begin(); it != r.end(); ++it) {
      if (!it->enabled)
         continue;
      if (it->actor == formID)
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
//
//
//
void DetectionInterceptService::GetActorStatus(RE::Actor* subject, bool& outUnseen, bool& outUnseeing) const {
   outUnseen   = this->unseenActors.contains(subject);
   outUnseeing = this->unseeingActors.contains(subject);
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