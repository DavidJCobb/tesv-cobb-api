Scriptname CobbAPILight Hidden

;
; Some light properties exist on a base form and can be further modified on individual light 
; references; in these cases, the modifications are stored as relative values. For example, 
; if a Light base form has an FOV of 90, and an individual light reference (with that base 
; form) has an FOV of 70, this will be stored as -20. Most getters and setters below support 
; an optional bool parameter that can control whether relative or absolute values are 
; retrieved.
;
; Note that values that only exist on the reference (e.g. depth bias) are not subject to 
; this system; absolute values will always be used.

;
; Getters
;
Float Function GetFOV       (ObjectReference akLightReference, Bool abRelative = False) Global Native
Float Function GetDepthBias (ObjectReference akLightReference) Global Native
Float Function GetRadius    (ObjectReference akLightReference, Bool abRelative = False) Global Native
Float Function GetFade      (ObjectReference akLightReference, Bool abRelative = False) Global Native

Bool Function GetLightsLandscape (ObjectReference akLightReference) Global Native
Bool Function GetLightsWater     (ObjectReference akLightReference) Global Native

;
; Setters
;
; These are experimental, and changes made do not persist.
;
Function SetFade   (ObjectReference akLightReference, Float afFade,   Bool abRelative = False, Bool abReserved = False) Global Native
Function SetRadius (ObjectReference akLightReference, Float afRadius, Bool abRelative = False, Bool abReserved = False) Global Native