#include "LogBSTEventData.h"
#include "ReverseEngineered\Systems\BSTEvent.h"
#include "ReverseEngineered\Forms\TESObjectREFR.h"

namespace CobbPatches {
   namespace Exploratory {
      namespace LogBSTEventData {
         class DumpDeath : public RE::BSTEventSink<RE::TESDeathEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESDeathEvent");
                     _MESSAGE(" - victim == %08X", ev->victim);
                     _MESSAGE(" - killer == %08X", ev->killer);
                     _MESSAGE(" - isDead == %d",   ev->isDead);
                     //
                     _MESSAGE(" - Is dead? %d", ev->victim->IsDead(0));
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpDeath& GetInstance() {
                  static DumpDeath instance;
                  return instance;
               };
         };
         class DumpEquip : public RE::BSTEventSink<RE::TESEquipEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("DumpEquip");
                     _MESSAGE(" - actor  == %08X", ev->actor);
                     _MESSAGE(" - formID == %08X", ev->formID);
                     _MESSAGE(" - unk08  == %08X", ev->unk08);
                     _MESSAGE(" - unk0C  == %04X", ev->unk0C);
                     _MESSAGE(" - type   == %02X", ev->type);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpEquip& GetInstance() {
                  static DumpEquip instance;
                  return instance;
               };
         };
         class DumpHit : public RE::BSTEventSink<RE::TESHitEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("DumpHit");
                     _MESSAGE(" - target           == %08X", ev->target);
                     _MESSAGE(" - attacker         == %08X", ev->attacker);
                     _MESSAGE(" - sourceFormID     == %08X", ev->sourceFormID);
                     _MESSAGE(" - projectileFormID == %08X", ev->projectileFormID);
                     _MESSAGE(" - flags            == %02X", ev->flags);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpHit& GetInstance() {
                  static DumpHit instance;
                  return instance;
               };
         };
         //
         void Apply() {
            auto holder = RE::BSTEventSourceHolder::GetOrCreate();
            CALL_MEMBER_FN(&holder->death, AddEventSink)(&DumpDeath::GetInstance());
            CALL_MEMBER_FN(&holder->equip, AddEventSink)(&DumpEquip::GetInstance());
            CALL_MEMBER_FN(&holder->hit,   AddEventSink)(&DumpHit::GetInstance());
            _MESSAGE("Registered for some test events; will dump fields when they fire");
         }
      }
   }
}