Scriptname CobbAPIDetection Hidden

; ------------------------------------------------------------------------------
;    OVERVIEW
; ------------------------------------------------------------------------------
;
; The DetectionInterceptService allows you to tamper with the detection of any 
; actor. You can flag an actor as being unable to detect other actors, or flag 
; the actor as undetectable to other actors.
;
; The service works by associating a "string tag" with a flagged actor. In order 
; to flag or unflag an actor, you must pass in a string. This allows multiple 
; mods to flag an actor at the same time, without either mod interfering with 
; the other; the two mods' effects just overlap.
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
; another mod, then CobbAPI will defer to that other mod: the DetectionIntercept-
; Service will not function, and you will not be able to add new registrations.
;
; Use the ServiceIsAvailable method to check whether the service is online.
;
; ------------------------------------------------------------------------------
;    FURTHER NOTES
; ------------------------------------------------------------------------------
;
; A LeveledActor's registration will be invalidated when the actor respawns.
;
Bool Function ServiceIsAvailable () Global Native

Bool Function ActorCannotSee            (Actor akActor) Global Native
Bool Function ActorCannotBeSeen         (Actor akActor) Global Native
Bool Function PreventActorFromSeeing    (Actor akActor, Form akPersistenceForm, String asTag) Global Native
Bool Function PreventActorFromBeingSeen (Actor akActor, Form akPersistenceForm, String asTag) Global Native
     Function ReturnActorToSeeing       (Actor akActor, String asTag) Global Native
     Function ReturnActorToBeingSeen    (Actor akActor, String asTag) Global Native

; These methods forcibly unflag all actors matching the criteria.
;
Function ForceActorToSeeing    (Actor akActor) Global Native
Function ForceActorToBeingSeen (Actor akActor) Global Native
Function ForceTagToSeeing    (String asTag) Global Native
Function ForceTagToBeingSeen (String asTag) Global Native
Function InvalidateTag        (String asTag) Global Native ; works for both seeing and being seen; may be expanded to cover more if we ever add more