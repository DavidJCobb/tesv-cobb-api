Scriptname CobbAPIFurniture Hidden

Spell   Function GetAssociatedSpell   (Furniture akFurniture) Global Native
Int     Function GetMustExitToTalk    (Furniture akFurniture) Global Native

Int[]   Function GetEnabledMarkers    (Furniture akFurniture) Global Native
Int[]   Function GetMarkerEntryPoints (Furniture akFurniture, Int aiMarker) Global Native
Keyword Function GetMarkerKeyword     (Furniture akFurniture, Int aiMarker) Global Native

;
; Workbench functions
;
Int Function GetWorkbenchSkill (Furniture akFurniture) Global Native
Int Function GetWorkbenchType  (Furniture akFurniture) Global Native
;
; Workbench skills:
;    -1 = NONE
;    16 = Alchemy
;    18 = Alteration
;     8 = Archery
;     9 = Block
;    19 = Conjuration
;    20 = Destruction
;    23 = Enchanting
;    11 = Heavy Armor
;    21 = Illusion
;    12 = Light Armor
;    14 = Lockpicking
;    13 = Pickpocketing
;    22 = Restoration
;    10 = Smithing
;    15 = Sneak
;    17 = Speech
;     7 = Two-Handed
;   255 = NONE
;
; Workbench types:
;    -1 = NONE
;     0 = NONE
;     1 = Create Object
;     2 = Smithing Weapon
;     3 = Enchanting
;     4 = Enchanting Experiment
;     5 = Alchemy
;     6 = Alchemy Experiment
;     7 = Smithing Armor
;