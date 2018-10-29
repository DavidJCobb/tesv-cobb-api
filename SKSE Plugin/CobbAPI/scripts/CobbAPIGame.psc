Scriptname CobbAPIGame Hidden

;
; If abDoFullProcessing is True, then this works exactly like TCL (except that it only runs if we aren't 
; already in the state you want); however, the "full processing" in question can cause major performance 
; hitches and abnormal actor behavior (this is an issue with the base game). If abDoFullProcessing is False, 
; then we skip that processing and just modify the player's collision state; this works just fine and 
; doesn't seem to cause performance and behavior issues, but I don't know what the consequences of skipping 
; full processing might be.
;
Function SetPlayerCollisionEnabled (Bool abEnabled, Bool abDoFullProcessing = True) Global Native

Bool Function IsPlayerCollisionEnabled() Global Native

; The player's last-known outdoor position and worldspace are updated whenever they pass through a loading 
; screen that takes them from an exterior cell to an interior cell. If the player is in an outdoor space, 
; then we just return their coordinates; otherwise, if they're in an interior and have never triggered an 
; update to their last-known position, then the position getter returns an empty array.
;
Float[]    Function GetPlayerLastKnownOutdoorPosition   () Global Native
WorldSpace Function GetPlayerLastKnownOutdoorWorldspace () Global Native

Form Function GetFormFromFile(Int aiFormID, String asFileName) Global Native ; Doesn't spam the log