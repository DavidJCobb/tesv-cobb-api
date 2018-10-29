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
Bool Function ActorCannotSee            (Actor akActor) Global Native
Bool Function ActorCannotBeSeen         (Actor akActor) Global Native
Int  Function PreventActorFromSeeing    (Actor akActor) Global Native
Int  Function PreventActorFromBeingSeen (Actor akActor) Global Native
     Function ReturnActorToSeeing       (Actor akActor, Int aiHandle) Global Native
     Function ReturnActorToBeingSeen    (Actor akActor, Int aiHandle) Global Native
;
; These methods forcibly unflag an actor by disabling its internal registration. 
; Other callers still need to "turn in" their handles.
;
Function ForceActorToSeeing    (Actor akActor) Global Native
Function ForceActorToBeingSeen (Actor akActor) Global Native