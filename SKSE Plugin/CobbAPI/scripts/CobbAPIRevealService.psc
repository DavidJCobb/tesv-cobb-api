Scriptname CobbAPIRevealService Hidden

;
; Use these functions to show or hide all invisible light emitters. Emitters currently 
; in the loaded area will have 3D reloaded.
;
; SetLightModels must have been called first, to supply suitable 3D models and object 
; bounds (by way of a form with both set). The 3D models in use MUST BE UNIQUE, i.e. 
; no light form should be using the same models at the same path.
;
     Function SetLightModels       (Activator akOmni, Activator akOmniShadow = None, Activator akOmniHalf = None, Activator akSpot = None, Activator akSpotShadow = None) Global Native
     Function SetLightRevealState  (Bool abState) Global Native
Bool Function GetLightRevealState  () Global Native

;
; The reveal service can also be used to "reveal" and "revert" other forms -- a simple 
; model swap. You can register any form to support this functionality, specifying an 
; alternate model and bounds (six ints: min x, y, z; max x, y, z).
;
; As the function names imply, this is intended to produce equivalent functionality to 
; the Creation Kit's "editor markers."
;
     Function SetMarkerRevealState  (Bool abState) Global Native
Bool Function GetMarkerRevealState  () Global Native
     Function SetAllRevealStates    (Bool abState) Global Native
     Function RegisterMarker        (Form akBaseForm, String asRevealPath, Int[] aiBounds) Global Native
     Function RegisterMarkersInBulk (Form[] akBaseForms, String[] asRevealPaths, Int[] aiBounds) Global Native