Scriptname CobbAPICellSearcher Hidden

;
; Search a cell for the ObjectReference that most likely occupies a particular point in 
; space. This Papyrus class looks for all ObjectReferences whose oriented bounding boxes 
; contain the specified coordinates, and it returns the one with the smallest bounding 
; box (by volume). The bounding box data used is "OBND" data precomputed by the Creation 
; Kit. Note that not all references have OBND data.
;
; Bounding boxes are expanded in all directions by a tolerance value, making it possible 
; for paper-thin references (e.g. floors) to match a given search point. This tolerance 
; value can be adjusted at run-time to account for inaccuracies in references' OBND.
;
; Additional criteria can be used to filter the search: references can be excluded by 
; type (and exceptions to this can be specified via FormLists of base forms). Searches 
; can exclude the player-character and any objects that don't have loaded 3D with working 
; collision. Filters can be applied in one of two modes:
;
;    0 = totally ignore non-matching objects as if they don't exist
;    1 = test all objects, but only return a result if that result matches the criteria
;
; The former filter mode is faster, but the latter will generally be more accurate.
;

Int Function Create          (Float x, Float y, Float z) Global Native
Int Function CreateFromArray (Float[] afSearchPoint) Global Native

Function SetCell       (Int aiHandle, Cell akCellToSearch = None) Global Native
Function SetTolerance  (Int aiHandle, Float afTolerance) Global Native

Function AddFormList   (Int aiHandle, FormList akFormList) Global Native
Function SetFilterMode (Int aiHandle, Int aiFilterMode) Global Native

;
; Filter the search by form type. Pass -1 to leave an attribute changed, or else pass a 
; truthy (include in search) or falsy (exclude from search) value to change it.
;
Function SetCriteriaA  (Int aiHandle, int aiIncludeACTI = -1, int aiIncludeCONT = -1, int aiIncludeDOOR = -1, int aiIncludeFURN = -1, int aiIncludeLIGH = -1, int aiIncludeMSTT = -1, int aiIncludeNPC_ = -1, int aiIncludeSTAT = -1, int aiIncludeTREE = -1) Global Native
Function SetCriteriaB  (Int aiHandle, int aiIncludeItem = -1) Global Native

Function SetExcludesPlayer    (Int aiHandle, Bool abExcludesPlayer) Global Native
Function SetRequiresCollision (Int aiHandle, Bool abRequireCollision) Global Native
Function SetSearchLoadedArea  (Int aiHandle, Bool abSearchLoadedArea) Global Native ; Controls whether to search all attached grid cells. Only used if the specified/current cell is an exterior.

                Function Cancel      (Int aiHandle) Global Native
ObjectReference Function Run         (Int aiHandle) Global Native

ObjectReference Function CreateAndRun(Float[] afSearchPoint, Bool[] abSearchCriteria, Float afTolerance = 0.5, Cell akSearchCell = None) Global Native