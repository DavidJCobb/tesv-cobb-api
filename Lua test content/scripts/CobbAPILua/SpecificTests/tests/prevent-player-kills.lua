if not CobbAPIUnitTests.enabled.preventPlayerKills then
   return
end
local player = form_by_id(0x14)

--
-- TEST: Prevent the player from killing anything.
--

local function _hook(victim, killer, willStillKill)
   --
   -- (actor) is the actor that is about to die. (willStillKill) 
   -- is true if the Lua listeners that have run before yours 
   -- have not prevented the actor's death.
   --
   -- Return (nil) or (willStillKill) to make no changes to the 
   -- actor's fate. Return (false) to prevent them from dying, 
   -- or (true) if you explicitly want them to die (overruling 
   -- previous listeners). Note that listeners that run after 
   -- yours can overrule your decision.
   --
   -- Note that the actor's health may be below zero when this 
   -- function runs. If you want them to instantly regen health 
   -- or something, then you may want to raise their health to 
   -- zero (but be mindful of other hooks that may have edited 
   -- it already!).
   --
   --logmessage("Intercepted slaying of " .. tostring(victim) .. " by " .. tostring(killer) .. ".")
   if victim == player then
      return
   end
   if killer ~= player then
      return
   end
   victim:restoreActorValue(0x18, 999999) -- restore to full health
   return false -- prevent death
end
skyrim_hooks.registerForEvent("PreventPlayerKills", SKYRIM_EVENT_INTERCEPT_ACTOR_KILL, _hook)