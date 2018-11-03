Scriptname CobbAPICell Hidden

;
; Setters in this script should be managed properly between playthroughs. 
; Setters that offer an abPersist boolean argument will persist in the 
; savegame if that argument is set to True. Persistence is accomplished 
; via the SKSE co-save.
;
; Note that if you wish to revert a cell's properties to their defaults, 
; you should use the methods named "ResetSOMETHING". These methods will 
; ensure that changes to the cell's lighting no longer persist in the 
; savegame. By contrast, getting the defaults and setting them by hand 
; will persist whatever the defaults were at that moment, such that those 
; particular values override future changes to the load order (which is 
; bad). So, to be clear, revert changes to cells using the methods whose 
; names start with the word "Reset."
;

;
; Utilities
;
Int[] Function FromColorCode (Int aiCode) Global Native
Int   Function ToColorCode   (Int aiRed, Int aiGreen, Int aiBlue) Global Native ; Color codes are just 0xAABBGGRR, typically with a zero alpha.

String        Function GetEditorID      (Cell akSubject) Global Native
EncounterZone Function GetEncounterZone (Cell akSubject) Global Native
Int[]         Function GetExteriorCellCoordinates (Cell akSubject) Global Native ; only valid for exteriors; coordinates measured in cells e.g. (-1, 2)
Float[]       Function GetExteriorUnitCoordinates (Cell akSubject) Global Native ; only valid for exteriors; coordinates measured in units e.g. (-4096, 8192)

;
; Lighting template details.
;
; Even if a cell has a lighting template, it still has values for fog colors, 
; distances, and so on. However, the cell's "template usage flags" determine 
; whether it uses the cell-side values or the template-side values. As such, 
; setting a cell's fog color (even with Bethesda's functions) will have no 
; visible effect if the cell uses the template-side fog color.
;
; Usage flags:
;
;    1 = Ambient color
;    2 = Directional color
;    4 = Fog color
;    8 = Fog near distance
;   16 = Fog far distance
;   32 = Directional rotation
;   64 = Directional fade
;  128 = Fog clip distance
;  256 = Fog power
;  512 = Fog max
; 1024 = Light fade distances
;
     Function ForceUniqueLighting                  (Cell akSubject, Bool abPersist = True) Global Native ; equivalent to SetLightingTemplateUsageFlags(akSubject, 0xFFFFFFFF, False, abPersist)
Form Function GetLightingTemplate                  (Cell akSubject) Global Native
Bool Function GetLightingTemplateUsageFlag         (Cell akSubject, Int aiFlag) Global Native ; checks a flag
Int  Function GetLightingTemplateUsageFlags        (Cell akSubject)             Global Native ; returns the whole bitmask
Bool Function GetLightingTemplateUsageDefaultFlag  (Cell akSubject, Int aiFlag) Global Native ; checks flags on the cell's original data (i.e. before any run-time changes)
Int  Function GetLightingTemplateUsageDefaultFlags (Cell akSubject)             Global Native ; checks flags on the cell's original data (i.e. before any run-time changes)
     Function ResetTemplateUsageFlags              (Cell akSubject, Int aiFlagsMask) Global Native
     Function SetLightingTemplateUsageFlags        (Cell akSubject, Int aiFlagsMask, Bool abStateToSet, Bool abPersist = True) Global Native ; modifies the specified flags
     Function StopPersistingChangedTemplateUsageFlags (Cell akSubject, Int aiFlagsMask) Global Native ; stops persisting changes to the specified flags in the savegame, without actually resetting the cell's in-memory data

;
; The aiSource parameter determines where we retrieve each property from. 
; If the parameter is zero, a method will always return a cell's current 
; local data; otherwise, the parameter serves as a bitmask, indicating 
; which data to check for first (in order of highest to lowest bits).
;
;    1 = Prefer cell's default data (if changes were made)
;    2 = Prefer lighting template (if used for this property)
;    4 = Prefer lighting template (always, unless no template is used)
;
; As such, a value of 7 will always return values from the lighting temp-
; late if there is one, or otherwise the cell's default (ESP/ESM-specified) 
; data (regardless of whether the cell has been modified at run-time). The 
; default value for this argument, 2, will make these methods return what-
; ever value the cell is actually using.
;
Form       Function GetAcousticSpace            (Cell akSubject, Int aiSource = 2) Global Native ; can be used on any cell, but probably only valid for interiors
Int        Function GetAmbientColor             (Cell akSubject, Int aiSource = 2) Global Native ; only valid for interiors
Int[]      Function GetDirectionalAmbientColors (Cell akSubject, Int aiSource = 2) Global Native ; only valid for interiors
Int        Function GetDirectionalColor         (Cell akSubject, Int aiSource = 2) Global Native ; only valid for interiors
Float      Function GetDirectionalFade          (Cell akSubject, Int aiSource = 2) Global Native ; only valid for interiors
Int[]      Function GetDirectionalRotation      (Cell akSubject, Int aiSource = 2) Global Native ; only valid for interiors ; returns [XY, Z]
Float      Function GetFogClipDistance          (Cell akSubject, Int aiSource = 2) Global Native ; only valid for non-skylit interiors
Int[]      Function GetFogColors                (Cell akSubject, Int aiSource = 2) Global Native ; only valid for non-skylit interiors; returns near and far fog RGB colors
Float[]    Function GetFogDistances             (Cell akSubject, Int aiSource = 2) Global Native ; only valid for non-skylit interiors; returns near and far fog planes
Float      Function GetFogMax                   (Cell akSubject, Int aiSource = 2) Global Native ; only valid for non-skylit interiors
Float      Function GetFogPower                 (Cell akSubject, Int aiSource = 2) Global Native ; only valid for non-skylit interiors
Form       Function GetImageSpace               (Cell akSubject, Int aiSource = 2) Global Native ; can be used on any cell, but probably only valid for interiors
Float[]    Function GetLightFadeDistances       (Cell akSubject, Int aiSource = 2) Global Native ; only valid for interiors
MusicType  Function GetMusicType                (Cell akSubject, Int aiSource = 2) Global Native ; can be used on any cell, but probably only valid for interiors
Float      Function GetNorthRotation            (Cell akSubject) Global Native
Worldspace Function GetParentWorldspace         (Cell akSubject) Global Native
Bool       Function IsPublic                    (Cell akSubject) Global Native
Bool       Function UsesSkyLighting             (Cell akSubject) Global Native ; always returns true for exteriors, without even checking the flag

;
; Setters:
;
Function SetAcousticSpace (Cell akSubject, Form akAcousticSpace, Bool abPersist = True) Global Native
Function SetImageSpace    (Cell akSubject, Form akImageSpace, Bool abPersist = True) Global Native
Function SetMusicType     (Cell akSubject, MusicType akMusicType, Bool abPersist = True) Global Native

;
; Setters for lighting data.
;
; "Directional ambient colors" are a group of six colors representing +X, -X, +Y, -Y, +Z, and -Z.
;
; The abForce parameter, if set to true, will force the cell to use your color even if it is using 
; a lighting template. It's equivalent to an appropriate call to SetLightingTemplageUsageFlags.
;
; The setters for individual fog colors also have an abFixOther parameter; if both the abForce and 
; abFixOther parameters are true, then the cell copies the opposite fog color from its lighting 
; template. This option exists because a cell can inherit both fog colors from its lighting temp-
; late, or neither color; it cannot inherit just one. Using abForce and setting just one fog color 
; will produce a visible change in the other color unless the other color is copied from the light-
; ing template to the cell, and abFixOther controls whether that copy operation is performed.
;
; Directional ambient colors and the single ambient color have a similar relationship.
;
Function SetFogColors    (Cell akSubject, Int aiColorCodeNear, Int aiColorCodeFar, Bool abForce = True,  Bool abPersist = True) Global Native
Function SetFogFarColor  (Cell akSubject, Int aiColorCode, Bool abForce = True, Bool abFixOther = False, Bool abPersist = True) Global Native
Function SetFogNearColor (Cell akSubject, Int aiColorCode, Bool abForce = True, Bool abFixOther = False, Bool abPersist = True) Global Native
;
Function SetAmbientColor             (Cell akSubject, Int   aiColorCode,  Bool abForce = True, Bool abFixOther = False, Bool abPersist = True) Global Native
Function SetDirectionalAmbientColors (Cell akSubject, Int[] aiColorCodes, Bool abForce = True, Bool abFixOther = False, Bool abPersist = True) Global Native
Function SetDirectionalColor         (Cell akSubject, Int   aiColorCode,  Bool abForce = True, Bool abPersist = True) Global Native
;
Function SetDirectionalFade     (Cell akSubject, Float afFade, Bool abForce = True, Bool abPersist = True) Global Native
Function SetDirectionalRotation (Cell akSubject, Int[] aiDegrees, Bool abForce = True, Bool abPersist = True) Global Native ; aiDegrees == [XY, Z]
;
Function SetFogDistances    (Cell akSubject, Float afDistanceNear, Float afDistanceFar, Float afDistanceClip, Bool abForce = True, Bool abPersist = True) Global Native
Function SetFogDistanceClip (Cell akSubject, Float afDistance, Bool abForce = True, Bool abPersist = True) Global Native
Function SetFogDistanceFar  (Cell akSubject, Float afDistance, Bool abForce = True, Bool abPersist = True) Global Native
Function SetFogDistanceNear (Cell akSubject, Float afDistance, Bool abForce = True, Bool abPersist = True) Global Native
;
Function SetFogMax (Cell akSubject, Float afValue, Bool abForce = True, Bool abPersist = True) Global Native
Function SetFogPow (Cell akSubject, Float afValue, Bool abForce = True, Bool abPersist = True) Global Native
;
; Combined setter for all fog parameters:
;
Function SetFog (Cell akSubject, Int aiColorCodeNear, Int aiColorCodeFar, Float afDistanceNear, Float afDistanceFar, Float afDistanceClip, Float afPow, Float afMax, Bool abForce = True, Bool abPersist = True) Global Native
;
Function SetLightFadeDistances (Cell akSubject, Float afDistanceBegin, Float afDistanceEnd, Bool abForce = True, Bool abPersist = True) Global Native

;
; Reset Method
;
; The aiProperties parameter is a bitmask indicating which properties to reset. 
; Resetting a property ensures that changes will no longer persist in the save 
; file. Note that resetting a property doesn't reset the corresponding template 
; usage flags; you need a separate call for that.
;
; Bits:
;
;    0x00000001 = Ambient color
;    0x00000002 = Directional color
;    0x00000004 = Fog color, near
;    0x00000008 = Fog color, far
;    0x00000010 = Fog distance, near
;    0x00000020 = Fog distance, far
;    0x00000040 = Fog distance, clip
;    0x00000080 = Fog power
;    0x00000100 = Fog max
;    0x00000200 = Directional rotation, XY
;    0x00000400 = Directional rotation, Z
;    0x00000800 = Directional fade
;    0x00001000 = Directional ambient color, X+
;    0x00002000 = Directional ambient color, X-
;    0x00004000 = Directional ambient color, Y+
;    0x00008000 = Directional ambient color, Y-
;    0x00010000 = Directional ambient color, Z+
;    0x00020000 = Directional ambient color, Z-
;    0x00040000 = Light fade, begin
;    0x00080000 = Light fade, end
;    ...        = Unused
;    0x20000000 = AcousticSpace
;    0x40000000 = ImageSpace
;    0x80000000 = MusicType
;
Function ResetFields (Cell akSubject, Int aiProperties) Global Native

;
; The following stops persisting changes to the cell without resetting the cell's 
; in-memory properties. Same arguments as above.
;
Function StopPersistingChangesTo (Cell akSubject, Int aiProperties) Global Native