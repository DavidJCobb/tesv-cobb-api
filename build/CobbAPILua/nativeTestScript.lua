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
      local mt = getmetatable(form)
      logmessage("Got the form-wrapper's metatable...")
      if mt then
         if mt.formID then
            logmessage(" - It defines formID.")
         else
            logmessage(" - metatable.formID is not defined!")
            return false
         end
      else
         logmessage(" - It's not defined!")
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
   end
   form = nil
   logmessage("Done working with the player actor-base.")
end

return 5