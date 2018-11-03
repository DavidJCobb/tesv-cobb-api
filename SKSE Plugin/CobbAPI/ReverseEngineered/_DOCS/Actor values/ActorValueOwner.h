struct AVModifyRequest {
   Actor* target;
   SInt32 avIndex;
   float  avSetTo;
};

//
// Bethesda's classes use multiple inheritance. To ensure consistency 
// in compiling and clarity in code, however, both the SKSE team and I 
// list multiple-parents as members. In other words, the Actor class 
// is listed as having an ActorValueOwner member, when in reality, it 
// is actually a subclass of ActorValueOwner in addition to being a 
// subclass of TESObjectREFR.
//
// This causes some complications when documenting how the innards of 
// ActorValueOwner actually works: there's no "correct" way to document 
// code in ActorValueOwner that accesses members on Actor. That is to 
// say: when Actor accesses its member at offset 0x13C, it does
//
//    LEA EBX, [ECX + 0x13C] // ebx = &this->unk13C;
//
// and when ActorValueOwner, located at offset 0x60, accesses that same 
// member, it does
//
//    LEA EBX, [ECX + 0xDC]
//
// which has no direct translation into C++. The notation we'll be 
// using for this is:
//
//    ebx = &(Actor!this)->unk13C;
//
// Got it?
//

float ActorValueOwner::GetCurrent(UInt32 avIndex) { // virtual 0x01
   //
   // So from what I can gather:
   //
   //  - If the ActorValue has a certain flag, then we allow the 
   //    ActorProcessManager to override its current value, and 
   //    if there is no override, then we synchronize our value 
   //    into the ActorProcessManager.
   //
   //  - If the ActorProcessManager doesn't override the value, 
   //    then we retrieve the value from our ActorValueState 
   //    objects.
   //
   ActorValueInfo* info = ActorValueList::GetSingleton()->unk04[avIndex];
   ActorProcessManager::Struct00730290* eax = nullptr;
   bool bl = false;
   if (ecx->actorValueFlags & 0x40000) {
      auto pm = (Actor!this)->processManager;
      if (pm && (eax = pm->unk30))
         bl = true;
   }
   float esp0C;
   if (bl && eax->TESV_00730080(avIndex, &esp0C)) // retrives the AV from the ActorProcessManager if the APM-side data isn't flagged "dirty"
      return esp0C;
   //
   // some thread-local-storage stuff...
   //
   float esp1C = (Actor!this)->TESV_006DECB0(avIndex);
   if (bl)
      eax->TESV_007300B0(avIndex, esp1C); // synchronize into ActorProcessManager?
   return esp1C;
};

float ActorValueOwner::GetMaximum(UInt32 avIndex) { // virtual 0x02
   //
   // This works the same way as GetCurrent, but we pluck a different value 
   // out of our ActorValueState objects, and it's a different set of 
   // ActorValues that we allow the ActorProcessManager to tamper with.
   //
   ActorValueInfo* info = ActorValueList::GetSingleton()->unk04[avIndex];
   ActorProcessManager::Struct00730290* eax;
   bool bl = false;
   if (ecx->actorValueFlags & 0x80000) {
      auto eax = (Actor!this)->processManager;
      if (eax && eax->unk30)
         bl = true;
   }
   float esp0C = 0.0F;
   if (bl) {
      eax = (Actor!this)->processManager->unk30;
      if (eax->TESV_00730100(avIndex, &esp0C)) // retrives the AV from the ActorProcessManager if the APM-side data isn't flagged "dirty"
         return esp0C;
   }
   float esp14 = (Actor!this)->TESV_006DED10(avIndex);
   if (bl)
      eax->TESV_00730150(avIndex, esp1C); // synchronize into ActorProcessManager?
   return esp14;
};

float Actor::ActorValueOwner::GetBase(UInt32 avIndex) { // virtual 0x03
   auto esi = avIndex;
   auto edi = this;
   ebx = &(Actor!this)->unk13C;
   float esp0C;
   if (ebx->GetBaseValueFor(avIndex, &esp0C)
      return esp0C;
   if (TESV_005AEA90(avIndex, &esp0C))
      return esp0C;
   TESActorBase* base = (Actor!this)->baseForm;
   float esp14 = base->actorValueOwner.GetCurrent(avIndex);
   ActorValueInfo* info = ActorValueList::GetSIngleton()->unk04[avIndex];
   auto flags = info->actorValueFlags;
   if (!(flags & 0x800) && (Actor!this) != *(Actor*)(0x01310588) && !(flags & 0x80))
      esp0C = esp14;
   else if (ComputeDynamicBaseActorValue((Actor!this) ? this : nullptr, avIndex ,&esp0C)) {
      esp0C = esp14;
   } else {
      if (info->actorValueFlags & 0x40)
         esp0C += esp14;
   }
   if (*(0x01B2E38C)->unk3F0 & 2 == 0)
      ebx->SetBaseValueFor(avIndex, esp0C);
   return esp0C;
};

void ActorValueOwner::SetBase(SInt32 avIndex, float value) { // virtual 0x04
   if (avIndex == -1)
      return;
   AVModifyRequest data;
   data.target  = Actor!this;
   data.avIndex = avIndex;
   data.avSetTo = value;
   (Actor!this)->SetBaseActorValueInternal(avIndex, &data, 0);
};

void ActorValueOwner::ModBase(UInt32 avIndex, float value) { // virtual 0x05
   AVModifyRequest data;
   data.target  = Actor!this;
   data.avIndex = avIndex;
   data.avSetTo = value;
   (Actor!this)->ModifyBaseActorValueInternal(avIndex, &data, 0);
};

void ActorValueOwner::Unk_06(UInt32 Arg1, UInt32 avIndex, float changeBy) { // virtual 0x06
   //
   // Arg1: 2 for RestoreAV or DamageAV; 0 for ForceAV
   //
   (Actor!this)->TESV_006E0760(Arg1, avIndex, changeBy, 0);
};

void ActorValueOwner::SetCurrent(UInt32 avIndex, float value) { // virtual 0x07
   this->SetBase(avIndex, value); // literally why
   //
   // I guess setting the base *would* trigger the change handlers... but that's so, 
   // so misleading. If you were to call this on the Health AV, you'd completely 
   // ruin it.
};

bool ActorValueOwner::Unk_08() {
   return false;
};

