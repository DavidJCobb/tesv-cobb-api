Scriptname CobbAPIDetection Hidden

; The DetectionInterceptService allows you to tamper with the detection of any 
; actor. You can flag an actor as being unable to detect other actors, or flag 
; the actor as undetectable to other actors.
;
; The service works by exchanging "handles." When you flag an actor, you receive 
; an integer "handle." To unflag the actor, you must pass that handle back in. 
; This ensures that if multiple scripts are acting on the same actor, they do 
; not conflict: instead, their effects overlap.
;
; Negative handles are invalid and indicate that the actor could not be added.
;
; In addition to using handles, you can associate an optional string "tag" with 
; any given change. You can also forcibly unflag all handles associated with a 
; given tag. This is useful for uninstallation scripts and last-resort fixups.
;
Bool Function ActorCannotSee            (Actor akActor) Global Native
Bool Function ActorCannotBeSeen         (Actor akActor) Global Native
Int  Function PreventActorFromSeeing    (Actor akActor, String asTag = "") Global Native
Int  Function PreventActorFromBeingSeen (Actor akActor, String asTag = "") Global Native
     Function ReturnActorToSeeing       (Actor akActor, Int aiHandle) Global Native
     Function ReturnActorToBeingSeen    (Actor akActor, Int aiHandle) Global Native

; These methods forcibly unflag an actor by disabling its internal registration. 
; Other callers still need to "turn in" their handles before those handles are 
; freed for use by other scripts.
;
Function ForceActorToSeeing    (Actor akActor) Global Native
Function ForceActorToBeingSeen (Actor akActor) Global Native

Function ReturnTagToSeeing    (String asTag) Global Native
Function ReturnTagToBeingSeen (String asTag) Global Native
Function InvalidateTag        (String asTag) Global Native ; works for both seeing and being seen; may be expanded to cover more if we ever add more