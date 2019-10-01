if not CobbAPIUnitTests.enabled.glowPlayerOnKills then
   return
end
local player = form_by_id(0x14)

--
-- TEST: When the player kills something, modify an EffectShader 
-- and then (re) apply it to them.
--

local shader = form_by_id(0xFD804)

local function _generateColors()
   local r = rng(0, 255)
   local g = rng(0, 255)
   local b = rng(0, 255)
   if r < 64 and g < 64 and b < 64 then
      local which = rng(0, 3)
      local other = rng(192, 255)
      if which == 0 then
         r = other
      elseif which == 1 then
         g = other
      elseif which == 2 then
         b = other
      end
   end
   return r, g, b
end

local function _onKill(victim, killer, willStillKill)
   if killer ~= player then
      return
   end
   df("Player scored a kill! Setting shader (%s) colors...", shader)
   player:stopEffectShader(shader)
   df(" - Shader stopped. Changing color...")
   local r, g, b = _generateColors()
   df(" = EDGE: rgb(%u, %u, %u)", r, g, b)
   shader:setEdgeColor(r, g, b)
   r, g, b = _generateColors()
   df(" = FILL1 rgb(%u, %u, %u)", r, g, b)
   shader:setFillColorKey1(r, g, b)
   r, g, b = _generateColors()
   df(" = FILL2 rgb(%u, %u, %u)", r, g, b)
   shader:setFillColorKey2(r, g, b)
   r, g, b = _generateColors()
   df(" = FILL3 rgb(%u, %u, %u)", r, g, b)
   shader:setFillColorKey3(r, g, b)
   df(" - Playing...")
   player:playEffectShader(shader)
   df(" - Done.")
end

logmessage("Testing EffectShader APIs...")
if shader then
   local r, g, b, a = shader:getEdgeColor()
   df(" - Shader colors: rgba(%u, %u, %u, %u)", r, g, b, a)
   --
   shader:setFillFullAlphaRatio(1.0)
   shader:setFillPersistentAlphaRatio(0.9)
   df(" - Set shader fill alpha; should now be %f persistent, %f full", shader:getFillPersistentAlphaRatio(), shader:getFillFullAlphaRatio())
   shader:setEdgeFalloff(0.2)
   shader:setMembraneSourceBlendMode(2) -- ONE
   shader:setMembraneDestinationBlendMode(1) -- ZERO
   --
   skyrim_hooks.registerForEvent("GlowPlayerOnKill", SKYRIM_EVENT_INTERCEPT_ACTOR_KILL, _onKill)
else
   logmessage(" - failed to get a shader for our shader tests")
end