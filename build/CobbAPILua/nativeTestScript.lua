if logmessage and nativeTable then
   logmessage("Found the native vars! Logging native table contents...")
   for k, v in pairs(nativeTable) do
      logmessage(string.format("nativeTable[%s] == %s", k, v))
   end
   logmessage("Done logging!")
end
if form_by_id then
   logmessage("Attempting to run form_by_id...")
   local form = form_by_id(7)
   logmessage("Retrieved the player actor-base.")
   if form then
      local mt = getmetatable(form)
      logmessage("Got the form-wrapper's metatable...")
      if mt then
         if mt.formID then
            logmessage(" - It defines formID.")
         else
            logmessage(" - metatable.formID is not defined!")
         end
      else
         logmessage(" - It's not defined!")
      end
   end
   logmessage(string.format("formID   == %08X", form:formID()))
   logmessage(string.format("formType == %u", form:formType()))
   form = nil
   logmessage("Done working with the player actor-base.")
end

return 5