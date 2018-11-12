Scriptname CobbAPIDetection Hidden

; ------------------------------------------------------------------------------
;    OVERVIEW
; ------------------------------------------------------------------------------
;
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
; If multiple registrations apply to the same actor, use the same string tag 
; (see below), and use persistent forms from the same file (see below), then 
; they will share a handle. This is not a conflict or an unintended behavior: 
; handles are ref-counted, so if two scripts request a handle and receive the 
; same one, then that handle must be "freed" twice (once by each script) before 
; it is made available for reuse.
;
; ------------------------------------------------------------------------------
;    STRING TAGS
; ------------------------------------------------------------------------------
;
; In addition to using handles, you can associate an optional string "tag" with 
; any given change. You can also forcibly unflag all handles associated with a 
; given tag. This is useful for uninstallation scripts and last-resort fixups.
;
; ------------------------------------------------------------------------------
;    PERSISTENCE FORMS
; ------------------------------------------------------------------------------
;
; When you obtain a handle, you must also pass in a "persistence form." This is 
; so that if your mod is uninstalled and your script ceases to exist, the handle 
; can be freed instead of hanging around forever. The form you pass in must be 
; from your mod, but there are otherwise no restrictions; CobbAPI only stores 
; its load order slot, so don't worry about modifying or even deleting the form 
; when updating your mod.
;
; In case you're not aware: Skyrim save files include a copy of the load order. 
; When a save file is loaded, the game uses this copy to detect and handle any 
; changes in the load order. SKSE and CobbAPI can detect load order changes in 
; the same way, and can use this to tell when the user disables or uninstalls 
; the mod that the persistence form originally came from.
;
; It is recommended that you use a form added through a Papyrus property. If you 
; use Game.GetFormFromFile to retrieve your form, that will become an obstacle 
; for users that wish to merge your mod. (This is the same reason the detection 
; API requests a form rather than an ES[LPM] filename.)
;
; ------------------------------------------------------------------------------
;    SERVICE AVAILABILITY
; ------------------------------------------------------------------------------
;
; In order to offer this functionality, CobbAPI has to patch a particular part 
; of Skyrim's code in-memory. If the patch site has already been patched by 
; another mod, such as my ESO Death and Resurrection, then CobbAPI will defer 
; to that other mod: the DetectionInterceptService will not function, and you 
; will not be able to add new registrations.
;
; Use the ServiceIsAvailable method to check whether the service is online.
;
Bool Function ServiceIsAvailable () Global Native

Bool Function ActorCannotSee            (Actor akActor) Global Native
Bool Function ActorCannotBeSeen         (Actor akActor) Global Native
Int  Function PreventActorFromSeeing    (Actor akActor, Form akPersistenceForm, String asTag = "") Global Native
Int  Function PreventActorFromBeingSeen (Actor akActor, Form akPersistenceForm, String asTag = "") Global Native
     Function ReturnActorToSeeing       (Actor akActor, Int aiHandle) Global Native
     Function ReturnActorToBeingSeen    (Actor akActor, Int aiHandle) Global Native

; These methods forcibly unflag an actor by disabling its internal registration. 
; Handles affecting the actor must still be turned in so that they can be freed; 
; otherwise, the "zombie" handles will linger in the SKSE co-save (unless the 
; load order slot for those handles disappears).
;
Function ForceActorToSeeing    (Actor akActor) Global Native
Function ForceActorToBeingSeen (Actor akActor) Global Native

; These methods forcibly disable all handles using a given string tag. These 
; handles must still be turned in so that they can be freed; otherwise, the 
; "zombie" handles will linger in the SKSE co-save (unless the load order slot 
; for those handles disappears).
;
Function ReturnTagToSeeing    (String asTag) Global Native
Function ReturnTagToBeingSeen (String asTag) Global Native
Function InvalidateTag        (String asTag) Global Native ; works for both seeing and being seen; may be expanded to cover more if we ever add more