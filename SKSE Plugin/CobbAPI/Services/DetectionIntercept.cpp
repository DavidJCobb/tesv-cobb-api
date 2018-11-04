#include "DetectionIntercept.h"
#include <algorithm>

namespace {
   typedef DetectionInterceptService::Feature Feature;
   typedef DetectionInterceptService::RegistrationHandle RegistrationHandle;
   typedef DetectionInterceptService::RefHandle RefHandle;
};

RegistrationHandle Feature::add(RE::Actor* actor) {
   if (!actor)
      return registration_not_found;
   RefHandle handle = *g_invalidRefHandle;
   CreateRefHandleByREFR(&handle, (TESObjectREFR*) actor);
   return this->add(handle);
};
RegistrationHandle Feature::add(RE::Actor* actor, const char* tag) {
   if (!actor)
      return registration_not_found;
   RefHandle handle = *g_invalidRefHandle;
   CreateRefHandleByREFR(&handle, (TESObjectREFR*) actor);
   return this->add(handle, tag);
};
RegistrationHandle Feature::add(RefHandle handle, const char* tag) {
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
      this->registrations.emplace_back(handle, tag);
      if (tag)
         this->byStringTag[tag].push_back(index);
      this->firstEmpty = this->registrations.size();
      return index;
   }
   SInt32 i = this->firstEmpty;
   this->registrations[i] = Registration(handle, tag);
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
   RefHandle handle = *g_invalidRefHandle;
   CreateRefHandleByREFR(&handle, (TESObjectREFR*)actor);
   this->force_remove(handle);
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