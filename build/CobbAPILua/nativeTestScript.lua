local TESV_GENDERS = {
   [-1] = "unknown",
   [0]  = "male",
   [1]  = "female",
}

if logmessage and nativeTable then
   logmessage("Found the native vars! Logging native table contents...")
   for k, v in pairs(nativeTable) do
      logmessage(string.format("nativeTable[%s] == %s", k, v))
   end
   logmessage("Done logging!")
end
if form_by_id then
   local function _validateForm(form)
      local function _wrap(form)
         return not not form.getFormID
      end
      if not form then
         logmessage("Form wrapper doesn't exist!")
         return
      end
      logmessage("Form wrapper exists.")
      local err, result = pcall(_wrap, form)
      if err then
         logmessage(" - Wrapper throws on access. No metatable?")
         return false
      end
      if not result then
         logmessage(" - Wrapper is missing its member functions!")
         return false
      end
      logmessage("Form wrapper is good!")
      return true
   end
   --
   logmessage("Attempting to run form_by_id...")
   local form = form_by_id(7)
   logmessage("Retrieved the player actor-base.")
   if _validateForm(form) then
      logmessage(string.format("formID   == %08X", form:getFormID()))
      logmessage(string.format("formType == %u", form:getFormType()))
      if form.getGender then
         local gender = form:getGender()
         logmessage(string.format("gender   == %s", TESV_GENDERS[gender or -1]))
      else
         logmessage("IActorBase methods not present!")
      end
      if form.race then
         local race = form:getRace()
         if race then
            logmessage(string.format("race     == %s", race:getName()))
            logmessage(string.format("isChild? == %s", tostring(race:isChild())))
         else
            logmessage("Unable to retrieve the race!")
         end
      end
      --
      local form2 = form_by_id(7)
      if form == form2 then
         logmessage("Two accesses to the same form compare equal.")
         logmessage("Validating second access...")
         _validateForm(form2)
      else
         logmessage("Two accesses to the same form don't compare equal.")
         if not form2 then
            logmessage("Second access returned nil!")
         end
      end
      form2 = nil
      --
      do
         local mt = getmetatable(form)
         if mt then
            logmessage("IForm metatable tests as truthy.")
            if mt.getFormID then
               logmessage(" - metatable.formID is retrievable")
            else
               logmessage(" - metatable.formID is not retrievable")
            end
            mt.foo = 5
            if mt.foo then
               logmessage(" - Property assignment to the metatable is possible.")
            else
               logmessage(" - Property assignment to the metatable is impossible.")
            end
         else
            logmessage("IForm metatable tests as nil or false.")
         end
      end
   end
   form = nil
   --
   form = form_by_id(0x14)
   if form then
      logmessage("Got the player reference.")
      logmessage(string.format("formID   == %08X", form:getFormID()))
      logmessage(string.format("formType == %u", form:getFormType()))
      if form.getBaseForm then
         local base = form:getBaseForm()
         if base then
            logmessage(string.format("baseID   == %08X", base:getFormID()))
            logmessage(string.format("baseType == %u", base:getFormType()))
         else
            logmessage("base     == irretrievable or nullptr")
         end
         local cell = form:getParentCell()
         if cell then
            logmessage(string.format("cellID   == %08X", cell:getFormID()))
            logmessage(string.format("cellType == %u", cell:getFormType()))
         else
            logmessage("cell     == irretrievable or nullptr")
         end
         local x, y, z = form:getPosition()
         logmessage(string.format("position == (%s, %s, %s)", tostring(x), tostring(y), tostring(z)))
         local a, b, c = form:getRotation()
         logmessage(string.format("rotation == (%s, %s, %s)", tostring(a), tostring(b), tostring(c)))
         local scale = form:getScale()
         logmessage(string.format("scale    == %s", tostring(scale)))
      else
         logmessage("IReference member functions not available!")
      end
      form = nil
   end
   --
   logmessage("Done working with the player actor-base.")
   --
   if skyrim_hooks then
      logmessage("The skyrim_hooks singleton exists.")
      if skyrim_hooks.registerForEvent then
         logmessage(" - The event registration API exists.")
         --
         local player = form_by_id(0x14)
         local function _hook01(actor, avIndex, pendingChange, originalChange) -- route health damage through magicka first
            logmessage("Hook01 intercepted change on an actor...")
            if actor ~= player then
               logmessage(" - Not player. Ignoring.")
               return
            end
            if avIndex == 0x18 then
               if pendingChange < 0 and originalChange < 0 then
                  logmessage(" - It's health damage. Routing through magicka...")
                  local magicka = actor:getActorValue(0x19)
                  if magicka + pendingChange >= 0 then
                     actor:damageActorValue(0x19, pendingChange)
                     return 0
                  end
                  local healthMod = magicka + pendingChange;
                  if magicka > 0 then
                     actor:damageActorValue(0x19, -magicka)
                  end
                  return healthMod;
               end
               return pendingChange
            end
         end
         local function _hook02(actor, avIndex, pendingChange, originalChange) -- reduce health damage by 20%
            logmessage("Hook02 intercepted change on an actor...")
            if avIndex ~= 0x18 or actor == player then
               logmessage(" - Not health, or player. Ignoring.")
               return
            end
            if pendingChange < 0 and originalChange < 0 then
               skyrim_hooks.unregisterForEvent("Reduction25%StackA", SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE);
               skyrim_hooks.unregisterForEvent("Reduction25%StackB", SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE);
               local modifier = originalChange * 0.25
               logmessage(string.format(" - Change is damage: %s (originally %s); will reduce by %s.", -pendingChange, -originalChange, -modifier))
               if pendingChange > modifier then -- they're negative, so flip the comparison
                  logmessage(" - Reducing to zero.")
                  return 0
               end
               return pendingChange - modifier
            end
         end
         skyrim_hooks.registerForEvent("TESTEnergyShields",  SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE, _hook01)
         skyrim_hooks.registerForEvent("Reduction25%StackA", SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE, _hook02) --
         skyrim_hooks.registerForEvent("Reduction25%StackB", SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE, _hook02) -- these should produce a total 50% reduction
      else
         logmessage(" - The event registration API is absent.")
      end
   else
      logmessage("The skyrim_hooks singleton is absent.")
   end
end

return 5