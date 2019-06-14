#include "LogBSTEventData.h"
#include "ReverseEngineered\Systems\BSTEvent.h"
#include "ReverseEngineered\Forms\TESObjectREFR.h"
#include "ReverseEngineered\Forms\Actor.h"

namespace CobbPatches {
   namespace Exploratory {
      namespace LogBSTEventData {
         class DumpActorLoc : public RE::BSTEventSink<RE::TESActorLocationChangeEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESActorLocationChangeEvent");
                     _MESSAGE(" - unk00 == %08X", ev->unk00);
                     _MESSAGE(" - unk04 == %08X", ev->unk04);
                     _MESSAGE(" - unk08 == %08X", ev->unk08);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpActorLoc& GetInstance() {
                  static DumpActorLoc instance;
                  return instance;
               };
         };
         class DumpBook : public RE::BSTEventSink<RE::TESBookReadEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESBookReadEvent");
                     _MESSAGE(" - book  == %08X", ev->book);
                     _MESSAGE(" - unk04 == %08X", ev->unk04);
                     _MESSAGE(" - unk08 == %04X", ev->unk08);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpBook& GetInstance() {
                  static DumpBook instance;
                  return instance;
               };
         };
         class DumpCombat : public RE::BSTEventSink<RE::TESCombatEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESCombatEvent");
                     _MESSAGE(" - subject     == %08X", ev->subject);
                     _MESSAGE(" - target      == %08X", ev->target);
                     _MESSAGE(" - combatState == %d",   ev->combatState);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpCombat& GetInstance() {
                  static DumpCombat instance;
                  return instance;
               };
         };
         class DumpContainer : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESContainerChangedEvent");
                     _MESSAGE(" - sourceFormID == %08X", ev->sourceFormID);
                     _MESSAGE(" - targetFormID == %08X", ev->targetFormID);
                     _MESSAGE(" - itemFormID   == %08X", ev->itemFormID);
                     _MESSAGE(" - countMoved   == %08X", ev->countMoved);
                     _MESSAGE(" - itemRefID    == %08X", ev->itemRefID);
                     _MESSAGE(" - unk14        == %04X", ev->unk14);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpContainer& GetInstance() {
                  static DumpContainer instance;
                  return instance;
               };
         };
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
         class DumpMagicWard : public RE::BSTEventSink<RE::TESMagicWardHitEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESMagicWardHitEvent");
                     _MESSAGE(" - defender            == %08X", ev->defender);
                     _MESSAGE(" - attacker            == %08X", ev->attacker);
                     _MESSAGE(" - incomingSpellFormID == %08X", ev->incomingSpellFormID);
                     _MESSAGE(" - status              == %08X", ev->status);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpMagicWard& GetInstance() {
                  static DumpMagicWard instance;
                  return instance;
               };
         };
         class DumpPerk : public RE::BSTEventSink<RE::TESPerkEntryRunEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESPerkEntryRunEvent");
                     _MESSAGE(" - perkTarget    == %08X", ev->perkTarget);
                     _MESSAGE(" - perkHaver     == %08X", ev->perkHaver);
                     _MESSAGE(" - perkFormID    == %08X", ev->perkFormID);
                     _MESSAGE(" - fragmentIndex == %04X", ev->fragmentIndex);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpPerk& GetInstance() {
                  static DumpPerk instance;
                  return instance;
               };
         };
         class DumpPlayerBow : public RE::BSTEventSink<RE::TESPlayerBowShotEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESPlayerBowShotEvent");
                     _MESSAGE(" - weaponFormID == %08X", ev->weaponFormID);
                     _MESSAGE(" - ammoFormID   == %08X", ev->ammoFormID);
                     _MESSAGE(" - shotPower    == %f",   ev->shotPower);
                     _MESSAGE(" - isSungazing  == %d",   ev->isSungazing);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpPlayerBow& GetInstance() {
                  static DumpPlayerBow instance;
                  return instance;
               };
         };
         class DumpScene : public RE::BSTEventSink<RE::TESSceneEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESSceneEvent");
                     _MESSAGE(" - sceneFormID == %08X", ev->sceneFormID);
                     _MESSAGE(" - unk04       == %08X", ev->unk04);
                     _MESSAGE(" - unk08       == %08X", ev->unk08);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpScene& GetInstance() {
                  static DumpScene instance;
                  return instance;
               };
         };
         class DumpSceneAct : public RE::BSTEventSink<RE::TESSceneActionEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESSceneActionEvent");
                     _MESSAGE(" - sceneFormID == %08X", ev->sceneFormID);
                     _MESSAGE(" - actionIndex == %08X", ev->actionIndex);
                     _MESSAGE(" - questFormID == %08X", ev->questFormID);
                     _MESSAGE(" - unk0C       == %08X", ev->unk0C);
                     _MESSAGE(" - unk10       == %08X", ev->unk10);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpSceneAct& GetInstance() {
                  static DumpSceneAct instance;
                  return instance;
               };
         };
         class DumpScenePhase : public RE::BSTEventSink<RE::TESScenePhaseEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESScenePhaseEvent");
                     _MESSAGE(" - sceneFormID == %08X", ev->sceneFormID);
                     _MESSAGE(" - unk04       == %08X", ev->unk04);
                     _MESSAGE(" - unk08       == %08X", ev->unk08);
                     _MESSAGE(" - unk0C       == %08X", ev->unk0C);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpScenePhase& GetInstance() {
                  static DumpScenePhase instance;
                  return instance;
               };
         };
         class DumpSell : public RE::BSTEventSink<RE::TESSellEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESSellEvent");
                     _MESSAGE(" - unk00 == %08X", ev->unk00);
                     _MESSAGE(" - unk04 == %08X", ev->unk04);
                     _MESSAGE(" - unk08 == %04X", ev->unk08);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpSell& GetInstance() {
                  static DumpSell instance;
                  return instance;
               };
         };
         class DumpSleepStart : public RE::BSTEventSink<RE::TESSleepStartEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESSleepStartEvent");
                     _MESSAGE(" - unk00 == %08X (%f)", ev->unk00, ev->unk00);
                     _MESSAGE(" - unk04 == %08X (%f)", ev->unk04, ev->unk04);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpSleepStart& GetInstance() {
                  static DumpSleepStart instance;
                  return instance;
               };
         };
         class DumpSleepStop : public RE::BSTEventSink<RE::TESSleepStopEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESSleepStopEvent");
                     _MESSAGE(" - interrupted == %d", ev->interrupted);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpSleepStop& GetInstance() {
                  static DumpSleepStop instance;
                  return instance;
               };
         };
         class DumpSpell : public RE::BSTEventSink<RE::TESSpellCastEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESSpellCastEvent");
                     _MESSAGE(" - caster      == %08X", ev->caster);
                     _MESSAGE(" - spellFormID == %08X", ev->spellFormID);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpSpell& GetInstance() {
                  static DumpSpell instance;
                  return instance;
               };
         };
         class DumpSwitchRace : public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESSwitchRaceCompleteEvent");
                     _MESSAGE(" - subject == %08X [ACHR:%08X]", ev->subject, ev->subject->formID);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpSwitchRace& GetInstance() {
                  static DumpSwitchRace instance;
                  return instance;
               };
         };
         class DumpTopicInfo : public RE::BSTEventSink<RE::TESTopicInfoEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESTopicInfoEvent");
                     _MESSAGE(" - speaker    == %08X", ev->speaker);
                     _MESSAGE(" - unk04      == %08X", ev->unk04);
                     _MESSAGE(" - infoFormID == %08X", ev->infoFormID);
                     _MESSAGE(" - type       == %08X", ev->type);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpTopicInfo& GetInstance() {
                  static DumpTopicInfo instance;
                  return instance;
               };
         };
         class DumpTrackedStat : public RE::BSTEventSink<RE::TESTrackedStatsEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     auto name = ev->statName.data;
                     if (!name)
                        name = "";
                     _MESSAGE("TESTrackedStatsEvent");
                     _MESSAGE(" - name  == %s", name);
                     _MESSAGE(" - value == %d", ev->value);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpTrackedStat& GetInstance() {
                  static DumpTrackedStat instance;
                  return instance;
               };
         };
         class DumpUID : public RE::BSTEventSink<RE::TESUniqueIDChangeEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESUniqueIDChangeEvent");
                     _MESSAGE(" - refrFormID == %08X", ev->refrFormID);
                     _MESSAGE(" - unk04      == %08X", ev->unk04);
                     _MESSAGE(" - baseFormID == %08X", ev->baseFormID);
                     _MESSAGE(" - unk0C      == %04X", ev->unk0C);
                     _MESSAGE(" - unk0E      == %04X", ev->unk0E);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpUID& GetInstance() {
                  static DumpUID instance;
                  return instance;
               };
         };
         class DumpWaitStart : public RE::BSTEventSink<RE::TESWaitStartEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESWaitStartEvent");
                     _MESSAGE(" - unk00 == %08X (%f)", ev->unk00, ev->unk00);
                     _MESSAGE(" - days  == %08X (%f)", ev->days,  ev->days);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpWaitStart& GetInstance() {
                  static DumpWaitStart instance;
                  return instance;
               };
         };
         class DumpWaitStop : public RE::BSTEventSink<RE::TESWaitStopEvent> {
            public:
               virtual EventResult Handle(void* aEv, void* aSource) override {
                  auto ev     = convertEvent(aEv);
                  auto source = convertSource(aSource);
                  if (ev) {
                     _MESSAGE("TESWaitStopEvent");
                     _MESSAGE(" - interrupted == %d", ev->interrupted);
                  }
                  return EventResult::kEvent_Continue;
               };
               //
               static DumpWaitStop& GetInstance() {
                  static DumpWaitStop instance;
                  return instance;
               };
         };
         //
         void Apply() {
            auto holder = RE::BSTEventSourceHolder::GetOrCreate();
            CALL_MEMBER_FN(&holder->actorLocationChange, AddEventSink)(&DumpActorLoc::GetInstance());
            CALL_MEMBER_FN(&holder->bookRead, AddEventSink)(&DumpBook::GetInstance());
            CALL_MEMBER_FN(&holder->combat, AddEventSink)(&DumpCombat::GetInstance());
            CALL_MEMBER_FN(&holder->containerChanged, AddEventSink)(&DumpContainer::GetInstance());
            CALL_MEMBER_FN(&holder->death, AddEventSink)(&DumpDeath::GetInstance());
            CALL_MEMBER_FN(&holder->equip, AddEventSink)(&DumpEquip::GetInstance()); 
            CALL_MEMBER_FN(&holder->hit,   AddEventSink)(&DumpHit::GetInstance());
            CALL_MEMBER_FN(&holder->magicWardHit, AddEventSink)(&DumpMagicWard::GetInstance());
            CALL_MEMBER_FN(&holder->perkEntryRun, AddEventSink)(&DumpPerk::GetInstance());
            CALL_MEMBER_FN(&holder->playerBowShotEvent, AddEventSink)(&DumpPlayerBow::GetInstance());
            CALL_MEMBER_FN(&holder->scene, AddEventSink)(&DumpScene::GetInstance());
            CALL_MEMBER_FN(&holder->sceneAction, AddEventSink)(&DumpSceneAct::GetInstance());
            CALL_MEMBER_FN(&holder->scenePhase, AddEventSink)(&DumpScenePhase::GetInstance());
            CALL_MEMBER_FN(&holder->sell,  AddEventSink)(&DumpSell::GetInstance());
            CALL_MEMBER_FN(&holder->sleepStart, AddEventSink)(&DumpSleepStart::GetInstance());
            CALL_MEMBER_FN(&holder->sleepStop, AddEventSink)(&DumpSleepStop::GetInstance());
            CALL_MEMBER_FN(&holder->spellCast, AddEventSink)(&DumpSpell::GetInstance());
            CALL_MEMBER_FN(&holder->switchRaceComplete, AddEventSink)(&DumpSwitchRace::GetInstance());
            CALL_MEMBER_FN(&holder->topicInfo, AddEventSink)(&DumpTopicInfo::GetInstance());
            CALL_MEMBER_FN(&holder->trackedStats, AddEventSink)(&DumpTrackedStat::GetInstance());
            CALL_MEMBER_FN(&holder->uniqueIDChange, AddEventSink)(&DumpUID::GetInstance());
            CALL_MEMBER_FN(&holder->waitStart, AddEventSink)(&DumpWaitStart::GetInstance());
            CALL_MEMBER_FN(&holder->waitStop, AddEventSink)(&DumpWaitStop::GetInstance());
            _MESSAGE("Registered for some test events; will dump fields when they fire");
         }
      }
   }
}