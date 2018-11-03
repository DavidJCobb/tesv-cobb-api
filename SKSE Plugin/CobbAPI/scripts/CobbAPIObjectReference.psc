Scriptname CobbAPIObjectReference Hidden

;
; Collision Primitives
;
;  - These methods can add and modify collision-primitive data on placed references.
;
;  - You can only add collision primitive boxes to references whose base forms: are Activators; and have 
;    the injected keyword with form ID 0x00C0BB03.
;
;  - I'm still not clear on *when* the game actually generates collision data for these references. They 
;    must currently have 3D loaded, so perhaps the data is generated or updated when the reference moves, 
;    or during the process that generates 3D.
;
;  - The NAVCUT layer is 49.
;
Float[] Function GetBoundsToHalfwidths       (Form akBaseForm) Global Native ; Checks the bounds on a form and returns the halfwidths you'd need for a primitive to take up those bounds; note that primitives are centered and bounds aren't always
Float[] Function GetBoundsToHalfwidthOffsets (Form akBaseForm) Global Native ; Checks the bounds on a form and returns how far off-center they are, i.e. the offset a primitive would need for its halfwidths to align with the bounds
Int     Function MakeCollisionPrimitiveBox (ObjectReference akSubject, Int aiCollisionLayer, Float[] afHalfwidths) Global Native

;
; Coordinates
;
Float[] Function GetCoordinates (ObjectReference akSubject) Global Native
Float[] Function GetPosition    (ObjectReference akSubject) Global Native
Float[] Function GetRotation    (ObjectReference akSubject) Global Native

;
; Furniture
;
;  - Entry points: [front, behind, right, left, up]
;  - Marker types: {0 = Lean, 1 = Sit, 2 = Sleep}
;
; The game engine supports up to 24 furniture markers on a FURN. If a 3D model defines more 
; than this many markers, our "All" getters will only return data for the 0th through 23rd 
; markers.
;
; Note also that a furniture marker can be "enabled" yet still have all of its entry points 
; disabled, rendering it unusable. GetFurnitureEnabledMarkers will recognize it as enabled.
;
Int[]    Function GetFurnitureEnabledMarkers       (ObjectReference akFurnitureReference) Global Native
Int[]    Function GetFurnitureMarkerEntryPoints	   (ObjectReference akFurnitureReference, Int aiMarker) Global Native
Float[]  Function GetFurnitureMarkerCoordinates	   (ObjectReference akFurnitureReference, Int aiMarker, Bool abWorldRelative = False) Global Native
Int      Function GetFurnitureMarkerType           (ObjectReference akFurnitureReference, Int aiMarker) Global Native
Float[]  Function GetFurnitureAllMarkerCoordinates (ObjectReference akFurnitureReference, Bool abWorldRelative) Global Native
Int[]    Function GetFurnitureAllMarkerEntryPoints (ObjectReference akFurnitureReference) Global Native ; Should be a Bool[], but SKSE doesn't seem able to implement those properly.
Int[]    Function GetFurnitureAllMarkerTypes       (ObjectReference akFurnitureReference) Global Native

;
; Internals
;
     Function MarkChanged    (ObjectReference akSubject, Int aiFlags) Global Native
     Function UnmarkChanged  (ObjectReference akSubject, Int aiFlags) Global Native
Int  Function GetChangeFlags (ObjectReference akSubject) Global Native
Bool Function HasExtraData   (ObjectReference akSubject, Int aiExtraDataType) Global Native

;
; Load Doors
;
Bool            Function IsLoadDoor                          (ObjectReference akSubject) Global Native
Bool            Function IsTeleportMarkerInAttachedCell      (ObjectReference akSubject, ObjectReference akDestination = None) Global Native ; If you already have the destination door, pass it in the second argument (and maybe pass None for the first) to speed things up.
ObjectReference Function GetDestinationLoadDoor              (ObjectReference akSubject) Global Native
Float[]         Function GetTeleportMarkerCoordinates        (ObjectReference akSubject) Global Native ; NOTE: Teleport markers are "backwards." Given linked doors A and B, Door A's teleport marker is the one that exists in front of Door B, and vice versa.
                Function MoveTeleportMarkerToEditorLocOffset (ObjectReference akSubject) Global Native ; Take the offset from the (destination) marker's coordinates to the reference's editor location. That offset will be applied to the reference's current location, and the marker will be moved to the resulting coordinates.
                Function ResetTeleportMarker                 (ObjectReference akSubject) Global Native ; Return a teleport marker to its original position, if it's been moved by functionality elsewhere in CobbAPI.dll or CobbPos.dll. We cannot undo moves by other DLLs.

;
; Loaded3D
;
Bool Function HasLocalCollision      (ObjectReference akSubject) Global Native ; returns true if any bhkCollisionObject has the "set local" flag set
Bool Function HasNiControllerManager (ObjectReference akSubject) Global Native
     Function Reload3D               (ObjectReference akSubject) Global Native
     Function Unload3D               (ObjectReference akSubject) Global Native

;
; Miscellaneous
;
Int               Function BaseHasWhichKeyword      (ObjectReference akSubject, Keyword[] akKeywords) Global Native
Int               Function BaseHasWhichKeywordName  (ObjectReference akSubject, String[] asKeywords) Global Native
;ObjectReference[] Function GetActivateChildren     (ObjectReference akSubject) Global Native ; if this is still commented out, then it's been omitted from the DLL because it crashes
ObjectReference[] Function GetActivateParents       (ObjectReference akSubject, Form akOptionalBaseFormFilter = None) Global Native
Int[]             Function GetBounds                (ObjectReference akSubject) Global Native ; Returns the OBND data (pre-generated by the Creation Kit's "Recalc Bounds" function) in this format: [ iMinX, iMinY, iMinZ, iMaxX, iMaxY, iMaxZ ].
ObjectReference[] Function GetEnableStateChildren   (ObjectReference akSubject) Global Native
Float[]           Function GetHeadingOffsetPosition (ObjectReference akSubject, Float afHeading) Global Native ; Given a ref and the player's yaw-angle, where would we have to move the player to get them looking directly at the ref, near the ref, without being trapped inside the ref?
Int               Function GetMotionType            (ObjectReference akSubject) Global Native ; Return values correspond to those used in SetMotionType. 0 means there is no collision or there is no 3D. -1 means that different motion types are in use within the 3D model.
Bool              Function IsMarker                 (ObjectReference akSubject) Global Native
Bool              Function IsCleanlyMovable         (ObjectReference akSubject) Global Native ; If this function returns false, then the reference is movable, but changes to its position will "bleed" across savefile loads.