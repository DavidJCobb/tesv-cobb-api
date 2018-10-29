void AVChangeHandler_InventoryWeight(Actor* target, SInt32 avIndex, float oldVal, float changedBy, UInt32 Arg5) {
   // AV 1E
   target->unk13C.TESV_006F2A80(avIndex);
   target->TESV_006A75F0();
   auto ecx = *(0x01B2E38C);
   if (ecx->unk3F0 & 2)
      target->TESV_006CE140();
   if (target != *(Actor**)(0x01310588)) // return if not the player
      return;
   if (0.0 <= changedBy) // inventory weight decreased
      return;
   float carryWeight = (*g_thePlayer)->actorValueOwner.GetCurrent(0x20); // CarryWeight
   if (carryWeight < oldVal)
      return;
   if (carryWeight <= oldVal + changedBy) // shouldn't this be ">" ? I may have decoded it wrong.
      return;
   (*g_thePlayer)->ShowOverencumberedMessage(true);
};

void AVChangeHandler_CarryWeight(Actor* target, SInt32 avIndex, float oldVal, float changedBy, UInt32 Arg5) {
   // AV 20
   target->TESV_006A75F0();
   auto ecx = *(0x01B2E38C);
   if (ecx->unk3F0 & 2)
      target->TESV_006CE140();
   if (target != *(Actor**)(0x01310588)) // return if not the player
      return;
   if (0.0 <= changedBy) // carry capacity decreased
      return;
   float invWeight = (*g_thePlayer)->actorValueOwner.GetCurrent(0x1F); // InventoryWeight
   if (invWeight > oldVal)
      return;
   if (invWeight > oldVal + changedBy) // shouldn't this be "<" ? I may have decoded it wrong.
      return;
   (*g_thePlayer)->ShowOverencumberedMessage(true);
};