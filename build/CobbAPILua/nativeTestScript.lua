local TESV_GENDERS = {
   [-1] = "unknown",
   [0]  = "male",
   [1]  = "female",
}

local function d(...)
   for i = 1, select("#", ...) do
      logmessage(tostring(select(i, ...)))
   end
end
local function df(format, ...)
   local args = {...}
   for i = 1, select("#", ...) do
      if args[i] == nil or type(args[i]) == "boolean" then
         args[i] = tostring(args[i])
      end
   end
   logmessage(string.format(format, table.unpack(args)))
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
      df("formID   == %08X", form:getFormID())
      df("formType == %u",   form:getFormType())
      df("editorID == %08X", form:getEditorID())
      if form.getGender then
         local gender = form:getGender()
         df("gender   == %s", TESV_GENDERS[gender or -1])
         df("isUnique == %s", form:isUnique())
      else
         logmessage("IActorBase methods not present!")
      end
      if form.race then
         local race = form:getRace()
         if race then
            df("race     == %s", race:getName())
            df("isChild? == %s", race:isChild())
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
      df("formID   == %08X", form:getFormID())
      df("formType == %u", form:getFormType())
      df("name     == %s", form:getName())
      if form.getBaseForm then
         local base = form:getBaseForm()
         if base then
            df("baseID   == %08X", base:getFormID())
            df("baseType == %u", base:getFormType())
         else
            logmessage("base     == irretrievable or nullptr")
         end
         local cell = form:getParentCell()
         if cell then
            df("cellID   == %08X", cell:getFormID())
            df("cellType == %u",   cell:getFormType())
         else
            logmessage("cell     == irretrievable or nullptr")
         end
         local x, y, z = form:getPosition()
         df("position == (%s, %s, %s)", x, y, z)
         local a, b, c = form:getRotation()
         df("rotation == (%s, %s, %s)", a, b, c)
         local scale = form:getScale()
         df("scale    == %s", scale)
         if form.getLevel then
            df("level      == %s", form:getLevel())
            df("weapon out == %s", form:isWeaponDrawn())
            df("is dead    == %s", form:isDead())
            df("max carry  == %s", form:getMaxCarryWeight())
         else
            logmessage("IActor member functions not available!")
         end
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
            --logmessage("Hook01 intercepted change on an actor...")
            if actor ~= player then
               --logmessage(" - Not player. Ignoring.")
               return
            end
            if avIndex == 0x18 then
               if pendingChange < 0 and originalChange < 0 then
                  --logmessage(" - It's health damage. Routing through magicka...")
                  local magicka = actor:getActorValue(0x19)
                  if magicka + pendingChange >= 0 then
                     actor:damageActorValue(0x19, -pendingChange) -- pendingChange is negative; negative damage == restore; double-negative damage == damage
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
         local function _hook02(actor, avIndex, pendingChange, originalChange) -- reduce health damage by 20%
            --logmessage("Hook02 intercepted change on an actor...")
            if avIndex ~= 0x18 or actor == player then
               --logmessage(" - Not health, or player. Ignoring.")
               return
            end
            if pendingChange < 0 and originalChange < 0 then
               skyrim_hooks.unregisterForEvent("Reduction25%StackA", SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE);
               skyrim_hooks.unregisterForEvent("Reduction25%StackB", SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE);
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
         skyrim_hooks.registerForEvent("TESTEnergyShields",  SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE, _hook01)
         skyrim_hooks.registerForEvent("Reduction25%StackA", SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE, _hook02) --
         skyrim_hooks.registerForEvent("Reduction25%StackB", SKYRIM_HOOK_INTERCEPT_ACTOR_VALUE_CHANGE, _hook02) -- these should produce a total 50% reduction
         --
         local function _deathHook(victim, killer, willStillKill)
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
            local x, y, z = killer:getPosition()
            local a, b, c = victim:getPosition()
            victim:pushAwayFrom(a, b, c - 50, 5) -- can we stack two pushes to launch them upward as well as away?
            victim:pushAwayFrom(x, y, z, 10)
            victim:restoreActorValue(0x18, 999999) -- restore to full health
            return false -- prevent death
         end
         skyrim_hooks.registerForEvent("PushInsteadOfKill", SKYRIM_HOOK_INTERCEPT_ACTOR_KILL, _deathHook)
      else
         logmessage(" - The event registration API is absent.")
      end
   else
      logmessage("The skyrim_hooks singleton is absent.")
   end
end

return 5