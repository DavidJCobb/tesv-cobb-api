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
      if not form then
         logmessage("Form wrapper doesn't exist!")
         return
      end
      logmessage("Form wrapper exists.")
      if not form.formID then
         logmessage(" - Wrapper is missing its member functions!")
         return false
      end
      logmessage("Form wrapper is good!")
      return true
   end
   
   logmessage("Attempting to run form_by_id...")
   local form = form_by_id(7)
   logmessage("Retrieved the player actor-base.")
   if _validateForm(form) then
      logmessage(string.format("formID   == %08X", form:formID()))
      logmessage(string.format("formType == %u", form:formType()))
      if form.gender then
         local gender = form:gender()
         logmessage(string.format("gender   == %s", TESV_GENDERS[gender or -1]))
      else
         logmessage("IActorBase methods not present!")
      end
      if form.race then
         local race = form:race()
         if race then
            logmessage(string.format("race     == %s", race:name()))
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
            if mt.formID then
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
   logmessage("Done working with the player actor-base.")
end

return 5