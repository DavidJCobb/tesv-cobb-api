Scriptname CobbAPIUtility Hidden

; "Exists" methods:
;
; In order to understand these methods' utility, we must discuss the script engine 
; using terms introduced in Fallout 4. Every running script instance is what Fallout 
; 4 calls a "ScriptObject," and ScriptObjects "wrap" pointers to game engine objects 
; such as TESForm or TESObjectREFR. Papyrus data, such as variables and properties, 
; exists on the ScriptObject. If you pass the ScriptObject to a native method, the 
; method receives the "wrapped pointer," i.e. the raw TESForm* or TESObjectREFR*.
;
; In rare cases, it's possible for a game engine object to disappear out from under 
; a ScriptObject. This is the cause of Papyrus warnings about calling functions on 
; None objects, with stack traces indicating that the functions were called on NULL. 
; ScriptObjects exist semi-independently of the wrapped pointers and can sometimes 
; try to act on those wrapped pointers even after they've gone null. Strangely, 
; these script objects test as != None, so you have no way to check for this in 
; script.
;
; Bethesda themselves have commented on that happening with cells, and I've observed 
; it with ActiveMagicEffect scripts that call DispelSpell(kMyOriginSpell) during an 
; OnEffectFinish stack (when the effect is already being dispelled).
;
; These methods should allow you to check whether a ScriptObject wraps a valid 
; pointer. You can call them like this:
;
;    CobbAPIUtility.FormExists(Self)
;
; Use the method most suitable for what you're extending.
;
Bool Function ActiveEffectExists (ActiveMagicEffect akPointer) Global Native
Bool Function FormExists         (Form akPointer) Global Native

; These functions take six floats representing a bounding box (i.e. a "min" corner 
; and a "max" corner) and return information about the box.
;
Float[] Function GetBoundsToHalfwidths       (Float[] afBounds) Global Native ; Checks the bounds and returns the halfwidths you'd need for a primitive to take up those bounds; note that primitives are centered and bounds aren't always
Float[] Function GetBoundsToHalfwidthOffsets (Float[] afBounds) Global Native ; Checks the bounds and returns how far off-center they are, i.e. the offset a primitive would need for its halfwidths to align with the bounds

; Given an object with the specified bounds, and the specified player coordinates 
; (position and rotation), what would be a good place to spawn the object such that 
; it is in front of the player and at a comfortable distance? This math takes into 
; account the player's rotation and the bounds size, although especially-off-center 
; bounds may foul up the calculation.
;
; aiFlags:
; 0x00000001 = Object's yaw should match the player's yaw
;
Float[] Function GetGoodObjectSpawnCoordinates (Float[] afBounds, Float[] afPlayerCoordinates, Int aiFlags) Global Native