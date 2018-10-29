Scriptname CobbAPIEditor Hidden

;
; Functions for interacting with Cobb Positioner itself, in a general sense
;

Function SetDisplayOptionGlobals (GlobalVariable[] akGlobals) Global Native ; give the DLL the display option globals for gizmos, shaders

Function SetGizmos  (Activator[] akForms) Global Native
Function SetShaders (EffectShader[] akForms) Global Native

Function ApplyDisplayOptions () Global Native

; See CobbPosObjectReference.psc
;
Float[] Function GetBoundsToHalfwidths       (Float[] afBounds) Global Native ; Checks the bounds and returns the halfwidths you'd need for a primitive to take up those bounds; note that primitives are centered and bounds aren't always
Float[] Function GetBoundsToHalfwidthOffsets (Float[] afBounds) Global Native ; Checks the bounds and returns how far off-center they are, i.e. the offset a primitive would need for its halfwidths to align with the bounds

;
; aiFlags:
; 0x00000001 = Rotate objects to match player yaw
; 0x00000002 = Rotate objects to match player pitch and roll
;
Float[] Function GetStorageServiceRecallCoordinates (Float[] afBounds, Float[] afPlayerCoordinates, Int aiFlags) Global Native