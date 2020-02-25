if not CobbAPIUnitTests.enabled.decapPlayerOnCastFear then
   return
end
local player = form_by_id(0x14)
local fear   = form_by_id(0x4DEEA)

local function _handler(caster, spell)
   logmessage("caught spell-cast-start")
   if caster ~= player then
      return
   end
   logmessage(" - caster is player")
   if spell ~= fear then
      return
   end
   local pronoun = "their"
   do
      local base = caster:getBaseForm()
      if base.getGender then
         local g = base:getGender()
         if g == 0 then
            pronoun = "his"
         else
            pronoun = "her"
         end
      end
   end
   caster:decapitate(true)
   show_notification("...but you're the one who lost " .. pronoun .. " head!")
   show_notification("Oh no! You tried to scare everyone else...")
end

if fear then
   logmessage("Setting up Fear hack: should decapitate the player when it's cast.")
   skyrim_hooks.registerForEvent("DecapOnFear", SKYRIM_EVENT_SPELL_CAST_START, _handler)
else
   logmessage("Couldn't find the Fear spell.")
end