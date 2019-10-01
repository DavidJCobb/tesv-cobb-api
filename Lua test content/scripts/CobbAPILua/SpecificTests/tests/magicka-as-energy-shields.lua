if not CobbAPIUnitTests.enabled.magickaAsEnergyShields then
   return
end
local player = form_by_id(0x14)

--
-- TEST: Route health damage through magicka, such that magicka acts 
-- as energy shields with bleedthrough.
--

local function _hook(actor, avIndex, pendingChange, originalChange)
   if actor ~= player then
      return
   end
   if avIndex == 0x18 then
      if pendingChange < 0 and originalChange < 0 then
         local magicka = actor:getActorValue(0x19)
         if magicka + pendingChange >= 0 then
            actor:damageActorValue(0x19, -pendingChange) -- pendingChange is negative when taking damage
            return 0
         end
         local healthMod = magicka + pendingChange;
         if magicka > 0 then
            actor:damageActorValue(0x19, magicka)
         end
         return healthMod;
      end
      return pendingChange
   end
end
skyrim_hooks.registerForEvent("TESTEnergyShields", SKYRIM_EVENT_INTERCEPT_ACTOR_VALUE_CHANGE, _hook)