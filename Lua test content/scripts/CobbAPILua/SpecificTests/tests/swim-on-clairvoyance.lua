if not CobbAPIUnitTests.enabled.swimOnClairvoyance then
   return
end
local player = form_by_id(0x14)
local clairvoyance = form_by_id(0x21143)

local function _handler(caster, spell)
   logmessage("caught spell-cast-start")
   if caster ~= player then
      return
   end
   logmessage(" - caster is player")
   if spell ~= clairvoyance then
      return
   end
   logmessage(" - forcing caster to swim")
   caster:setSwimming(true) -- hm... the game just undoes this on the next frame, though
end

if clairvoyance then
   logmessage("Setting up Clairvoyance hack: should force air-swimming when it's cast.")
   skyrim_hooks.registerForEvent("SwimOnClairvoyance", SKYRIM_EVENT_SPELL_CAST_START, _handler)
else
   logmessage("Couldn't find the Clairvoyance spell.")
end