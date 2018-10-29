#include "DetectionIntercept.h"
#include <algorithm>

RegistrationHandle Feature::add(RE::Actor* actor) {
   if (!actor)
      return registration_not_found;
   RefHandle handle = *g_invalidRefHandle;
   CreateRefHandleByREFR(&handle, (TESObjectREFR*) actor);
   return this->add(handle);
};
RegistrationHandle Feature::add(RefHandle handle) {
   if (handle == *g_invalidRefHandle)
      return registration_not_found;
   //
   feature_lock_guard scopedLock(this->lock);
   //
   auto index = this->_find_enabled_only(handle);
   if (index != registration_not_found) {
      this->registrations[index].refCount++;
      return index;
   }
   //
   this->affectedActors.push_back(handle);
   if (this->firstEmpty >= this->registrations.size()) {
      index = this->registrations.size();
      this->registrations.emplace_back(handle);
      this->firstEmpty = this->registrations.size();
      return index;
   }
   SInt32 i = this->firstEmpty;
   this->registrations[i] = Registration(handle);
   this->firstEmpty = this->_find_next_empty(i);
   return i;
};
void Feature::remove(RE::Actor* actor, RegistrationHandle index) {
   if (!actor)
      return;
   if (index < 0 || index >= this->registrations.size())
      return;
   RefHandle handle = *g_invalidRefHandle;
   CreateRefHandleByREFR(&handle, (TESObjectREFR*)actor);
   this->remove(handle, index);
};
void Feature::remove(RefHandle handle, RegistrationHandle index) {
   if (index < 0 || index >= this->registrations.size())
      return;
   //
   feature_lock_guard scopedLock(this->lock);
   //
   auto& r = this->registrations[index];
   if (r.actor != handle)
      return;
   r.refCount--;
   if (r.refCount <= 0) {
      if (r.enabled) {
         auto& actors = this->affectedActors;
         actors.erase(std::remove(actors.begin(), actors.end(), handle), actors.end());
      }
      r.actor   = *g_invalidRefHandle;
      r.enabled = false;
      if (this->firstEmpty > index)
         this->firstEmpty = index;
   }
};
void Feature::force_remove(RefHandle handle) {
   feature_lock_guard scopedLock(this->lock);
   //
   auto& l = this->registrations;
   for (auto it = l.begin(); it != l.end(); ++it) {
      auto& r = *it;
      if (r.enabled && r.actor == handle)
         r.enabled = false;
   }
   auto& actors = this->affectedActors;
   actors.erase(std::remove(actors.begin(), actors.end(), handle), actors.end());
};
bool Feature::contains(RE::Actor* actor) const {
   if (!actor)
      return false;
   RefHandle handle = *g_invalidRefHandle;
   CreateRefHandleByREFR(&handle, (TESObjectREFR*)actor);
   //
   feature_lock_guard scopedLock(this->lock);
   //
   auto& actors = this->affectedActors;
   return std::find(actors.begin(), actors.end(), handle) != actors.end();
};
void Feature::reset() {
   feature_lock_guard scopedLock(this->lock);
   this->affectedActors.clear();
   this->registrations.clear();
};
RegistrationHandle Feature::_find_enabled_only(RefHandle handle) const {
   auto& r = this->registrations;
   for (auto it = r.begin(); it != r.end(); ++it) {
      if (!it->enabled)
         continue;
      if (it->actor == handle)
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