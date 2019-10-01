if not CobbAPIUnitTests.enabled.actorValueReductionStack then
   return
end
local player = form_by_id(0x14)

--
-- TEST: A single listener that reduces health damage taken by 20%. 
-- We register it twice in order to verify that it stacks for a total 
-- of 40% reduction.
--
-- The listener unregisters itself, so it only affects the first hit 
-- taken by the player.
--

local function _hook(actor, avIndex, pendingChange, originalChange)
   --logmessage("Hook02 intercepted change on an actor...")
   if avIndex ~= 0x18 or actor == player then
      --logmessage(" - Not health, or player. Ignoring.")
      return
   end
   if pendingChange < 0 and originalChange < 0 then
      skyrim_hooks.unregisterForEvent("Reduction25%StackA", SKYRIM_EVENT_INTERCEPT_ACTOR_VALUE_CHANGE);
      skyrim_hooks.unregisterForEvent("Reduction25%StackB", SKYRIM_EVENT_INTERCEPT_ACTOR_VALUE_CHANGE);
      --logmessage(" - Listener unregistered.")
      local modifier = originalChange * 0.25
      --logmessage(string.format(" - Change is damage: %s (originally %s); will reduce by %s.", -pendingChange, -originalChange, -modifier))
      if pendingChange > modifier then -- they're negative, so flip the comparison
         --logmessage(" - Reducing to zero.")
         return 0
      end
      return pendingChange - modifier
   end
end
skyrim_hooks.registerForEvent("Reduction25%StackA", SKYRIM_EVENT_INTERCEPT_ACTOR_VALUE_CHANGE, _hook)
skyrim_hooks.registerForEvent("Reduction25%StackB", SKYRIM_EVENT_INTERCEPT_ACTOR_VALUE_CHANGE, _hook)