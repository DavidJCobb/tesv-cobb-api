if logmessage and nativeTable then
   logmessage("Found the native vars! Logging native table contents...")
   for k, v in pairs(nativeTable) do
      logmessage(string.format("nativeTable[%s] == %s", k, v))
   end
   logmessage("Done logging!")
end

return 5