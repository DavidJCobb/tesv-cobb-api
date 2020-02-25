CobbAPIUnitTests = {
   enabled = {
      magickaAsEnergyShields   = false,
      actorValueReductionStack = false,
      launchEnemiesOnDeath     = false,
      preventPlayerKills       = true,
      glowPlayerOnKills        = true,
      swimOnClairvoyance       = true,
      decapPlayerOnCastFear    = true,
   }
}

if not os then
   logmessage("Failed to import the safe portions of the 'os' library.")
elseif os.remove then
   logmessage("The entire 'os' library was imported, including unsafe functions!")
end

function d(...)
   for i = 1, select("#", ...) do
      logmessage(tostring(select(i, ...)))
   end
end
function df(format, ...)
   local args = {...}
   for i = 1, select("#", ...) do
      if args[i] == nil or type(args[i]) == "boolean" then
         args[i] = tostring(args[i])
      end
   end
   logmessage(string.format(format, table.unpack(args)))
end

rng = nil
do
   local COUNT = 12
   local cache = {}
   local index = 0
   --
   local function _reseed()
      math.randomseed(os.clock())
      for i = 1, COUNT do
         cache[i] = math.random()
      end
   end
   _reseed()
   --
   rng = function(u, l)
      if index >= COUNT then
         index = 0
         _reseed()
      end
      index = index + 1
      local out = cache[index]
      if u then
         if l then
            out = out * (u - l) + l
         else
            out = out * u
         end
         out = math.floor(out)
      end
      return out
   end
end