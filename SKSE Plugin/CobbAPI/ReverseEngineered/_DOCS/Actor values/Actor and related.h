struct AVModifyRequest {
   Actor* target;
   SInt32 avIndex;
   float  avSetTo;
};

// One of these per AV, in a static table:
typedef void (*AVChangeHandler)(Actor* target, SInt32 avIndex, float oldVal, float newVal, UInt32 unk);

bool ActorProcessManager::Struct00730290::TESV_00730080(UInt32 avIndex, float* out) {
   auto eax = *(0x01B39530)[avIndex]; // map actor value indices to indices in the next tArray
   tArray<UnkAVData>& ecx = this->flag00040000ActorValues;
   *out = ecx[eax].unk04;
   return ecx[eax].dirty == false;
}
bool ActorProcessManager::Struct00730290::TESV_00730100(UInt32 avIndex, float* out) {
   auto eax = *(0x01B392A0)[avIndex]; // map actor value indices to indices in the next tArray
   tArray<UnkAVData>& ecx = this->flag00080000ActorValues;
   *out = ecx[eax].unk04;
   return ecx[eax].dirty == false;
}
void ActorProcessManager::Struct00730290::TESV_007300B0(UInt32 avIndex, float valueCurrent) {
   auto eax = *(0x01B39530)[avIndex];
   if (eax >= 0) {
      tArray<UnkAVData>& ecx = this->flag00040000ActorValues;
      ecx[eax].unk04 = valueCurrent;
      ecx[eax].dirty = false;
   }
}
void ActorProcessManager::Struct00730290::TESV_00730150(UInt32 avIndex, float valueMaximum) {
   auto eax = *(0x01B392A0)[avIndex];
   if (eax >= 0) {
      tArray<UnkAVData>& ecx = this->flag00080000ActorValues;
      ecx[eax].unk04 = valueMaximum;
      ecx[eax].dirty = false;
   }
}

float Actor::TESV_006DECB0(UInt32 avIndex) { // called by ActorValueOwner::GetCurrent
   float a = this->actorValueOwner.GetBase(avIndex);
   ActorValueState* eax = this->GetActorValueStateObj(avIndex);
   if (eax) {
      float b = eax->GetSum();
      return a + b;
   }
   return a;
}
float Actor::TESV_006DED10(UInt32 avIndex) { // called by ActorValueOwner::GetMaximum
   float esp08 = this->actorValueOwner.GetBase(avIndex);
   ActorValueState* eax = this->GetActorValueStateObj(avIndex);
   if (eax)
      return eax->unk00 + esp08;
   return esp08;
}

Actor::ActorValueState* Actor::GetActorValueStateObj(UInt32 avIndex) const {
   //
   // Actors always have ActorValueState objects for health, magicka, 
   // stamina, and voicepoints. AVS objects are created as needed for 
   // all other ActorValues.
   //
   switch (avIndex) {
      case 0x18:
         return &this->avStateHealth;
      case 0x19:
         return &this->avStateMagicka;
      case 0x1A:
         return &this->avStateStamina;
      case 0x25:
         return &this->avStateVoicePoints;
   }
   return this->unk13C.GetStateObjectFor(avIndex); // decltype(unk13C) == Actor::Struct006F2190
};

Actor::ActorValueState* Actor::Struct006F2190::GetStateObjectFor(UInt32 avIndex) const {
   const char* data = this->unk08.data;
   char   bl = data[0];
   size_t i  = 0;
   if (bl < actorValueIndex + 1) {
      while (bl && bl < actorValueIndex + 1) {
         bl = data[++i];
      }
   }
   {
      // uses a scoped lock stored at (SimpleLock*)0x01B39110
      //
      if (bl == actorValueIndex + 1) {
         auto list = this->unk0C;
         return &this->unk0C[i];
      }
      return nullptr;
   }
};

void Actor::SetBaseActorValueInternal(SInt32 avIndex, ModifyRequest* data, UInt32 arg3) {
   float esp0C = this->actorValueOwner.GetCurrent(avIndex);
   float esp14 = 0.0F;
   this->unk13C.GetBaseValueFor(data->avIndex, &esp14);
   this->unk13C.SetBaseValueFor(data->avIndex, data->avSetTo);
   esp14 = data->avSetTo - esp14;
   this->TESV_006DF960(avIndex);
   if (esp14 ?? 0.0F)
      this->OnActorValueChanged(avIndex, esp0C, esp14, arg3);
};

void Actor::ModifyBaseActorValueInternal(SInt32 avIndex, float value, UInt32 arg3) {
   float esp08 = this->actorValueOwner.GetCurrent(avIndex);
   float esp10 = Arg2->TESV_006DEC50();
   this->TESV_006DF960(avIndex);
   if (esp14 ?? 0.0F)
      this->OnActorValueChanged(avIndex, esp0C, esp14, arg3);
};

struct UnkAVData {
   ActorValueState* unk00;
   UInt32 unk04; // enum
   float  unk08;
};
void Actor::TESV_006E0760(UInt32 Arg1, UInt32 avIndex, float changeBy, UInt32 Arg4) { // called by ActorValueInfo::Unk_06
   ActorValueState* edi = this->GetActorValueStateObj(avIndex);
   if (!edi) {
      if (Arg3 == 0.0)
         return;
      edi = this->CreateActorValueStateObj(avIndex);
      if (!edi)
         return;
   }
   if (Arg1 == 2) // RestoreAV or DamageAV // at 0x006E07A7
      changeBy = this->IncerceptActorValueChange(avIndex, changeBy);
   float esp24 = edi[Arg1] + changeBy;
   if (Arg1 == 2) {
      auto info = ActorValueList::GetSingleton()->list[avIndex];
      if (info->actorValueFlags & 0x200) {
         if (esp24 < 0.0F)
            esp24 = 0.0F;
      } else
         if (esp24 > 0.0F)
            esp24 = 0.0F;
   }
   if (esp24 != 0.0F) { // at 0x006E0811
      if (TESV_005997B0(esp24, 0.0F, 0.0001F)) // if esp24 is within the specified range
         esp24 = 0.0F;
   }
   UnkAVData esp10;
   esp10.unk00 = edi;
   esp10.unk04 = Arg1;
   esp10.unk08 = esp24;
   auto ebp   = Arg4; // up to this point, ebp had been Arg1
   this->TESV_006E05F0(avIndex, &esp10, Arg4);
   if (avIndex == 0x18) { // health
      if (0.0F > changeBy) // decrease
         this->KillIfNoHealth(Arg4, changeBy); // hm, hold on... pretty sure changeBy was set to esp24 earlier, but I didn't note it...
   }
   if (edi->AreAllZero())
      if (avIndex < 0x18 || (avIndex > 0x1A && avIndex != 0x25))
         //
         // If this actor value has its ActorValueState object 
         // created and destroyed on the fly as necessary:
         //
         this->unk13C.RemoveFromSecondList(avIndex);
};
void Actor::TESV_006E05F0(UInt32 avIndex, UnkAVData* Arg2, UInt32 Arg3) {
   auto  esi = this;
   float esp08 = this->actorValueOwner.GetCurrent(avIndex);
   //
   ActorValueState* edx = Arg2->unk00;
   float* ecx = edx[Arg2->unk04];
   float esp10 = *ecx;
   *ecx = Arg2->unk08;
   //
   float a = Arg2->unk08 - esp10;
   esp10 = a;
   this->TESV_006DF960(avIndex); // operate on ActorProcessManager data: flag the AV as "dirty"
   if (0.0 != esp10)
      this->OnActorValueChanged(avIndex, esp08, esp10, Arg3);
};
void Actor::TESV_006DF960(UInt32 avIndex) {
   auto pm = this->processManager;
   if (!pm)
      return;
   auto eax = pm->unk30;
   if (!eax)
      return;
   auto info = ActorValueList::GetInstance()->unk04[avIndex];
   if (info->actorValueFlags & 0x00040000)
      eax->TESV_007300E0(avIndex); // flag the AV as "dirty"
   if (info->actorValueFlags * 0x00080000)
      eax->TESV_00730130(avIndex); // flag the AV as "dirty"
};

float Actor::IncerceptActorValueChange(UInt32 avIndex, float changedBy) { // virtual 0x127
   //
   // This is called when an ActorValue is about to change. It can 
   // override the amount by which the actor value changes, by 
   // returning a new value. Alternatively, it can return the 
   // (changedBy) value unchanged in order to do nothing.
   //
   float result = changedBy;
   if (0.0 > changedBy) { // actor value is being reduced
      auto info = ActorValueList::GetInstance()->unk04[avIndex];
      if (info->actorValueFlags & 0x20) {
         float esp18;
         if (eax = this->GetActorValueStateObj(avIndex))
            esp18 = eax->unk04;
         else
            esp18 = 0.0F;
         float esp0C;
         if (eax = this->GetActorValueStateObj(avIndex))
            esp0C = eax->unk08;
         else
            esp0C = 0.0F;
         float avMax = this->actorValueOwner.GetMaximum(avIndex);
         esp18 += avMax + esp0C;
         if (-changedBy > esp18)
            result = -esp18;
      }
   }
   if (!this->IsEssentialOrProtected(true)) // is essential?
      return result;
   if (avIndex != 0x18) // if not health
      return result;
   if (0.0 <= changedBy) // AV is increasing
      return result;
   float avCurrent = this->actorValueOwner.GetCurrent(avIndex);
   if (GMST:fEssentialNPCMinimumHealth > avCurrent + changedBy) { // at 0x006DFC28
      if (GMST:fEssentialNPCMinimumHealth < avCurrent) {
         //
         // The NPC is about to fall below the minimum health for 
         // essential NPCs. Return the difference between their 
         // current health and the minimum health, so that they 
         // instead end up exactly at the minimum value.
         //
         float b = avCurrent - GMST:fEssentialNPCMinimumHealth;
         return -b;
      }
      return -0.001F;
   }
   return result;
};
float PlayerCharacter::IncerceptActorValueChange(UInt32 avIndex, float changedBy) { // virtual 0x127
   float result = changedBy; // esp0C
   if (*bDebugImmortalModeEnabled) { // prevent health becoming <= 0
      if (avIndex == 0x18) {
         if (0.0 > changedBy) { // actor value is being reduced
            float a = this->actorValueOwner.GetCurrent(avIndex);
            a += changedBy;
            if (0.0 >= a)
               //
               // This change would bring the health to or below zero. 
               // Override it and stop the change.
               //
               result = 0.0;
         }
      }
   }
   if (*bDebugGodModeEnabled) { // do not allow certain AVs to decrease at all
      auto info = ActorValueList::GetSingleton()->unk04[avIndex];
      if (ecx->actorValueFlags & 0x100000) {
         if (0.0 > result) // actor value is being reduced
            result = 0.0; // override: stop the change
      }
   }
   return Actor::InterceptActorValueChange(avIndex, result); // call super
};

void Actor::OnActorValueChanged(SInt32 avIndex, float oldVal, float newVal, UInt32 arg4) {
   auto funcList = *(AVChangeHandler*)(0x01B38C98); // list of function pointers
   auto ebx = funcList[avIndex];
   //
   float esp10 = this->GetActorValueRegenDelay(avIndex);
   if (this->processManager) { // at 0x006DEBE2
      if (0.0F > Arg2) {
         float a = this->processManager->GetActorValueRegenDelayTimer(avIndex);
         if (esp10 > a)
            this->processManager->SetActorValueRegenDelayTimer(avIndex, esp10);
      }
   }
   //
   if (ebx)
      (ebx)(this, avIndex, oldVal, newVal, arg4);
};