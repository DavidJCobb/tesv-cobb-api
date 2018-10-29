Scriptname CobbAPIActor Hidden

Float Function GetActorValueRegenDelay (Actor akSubject, Int aiActorValue) Global Native ; Only meaningful for health/magicka/stamina; returns zero otherwise.

Float Function GetBleedoutTimer (Actor akSubject) Global Native ; returns 0.0 if the actor is not in bleedout

     Function Decapitate    (Actor akTarget, Bool abEvenIfAlive = False) Global Native ; This function doesn't kill the actor; it just dismembers them.
Bool Function IsDecapitated (Actor akSubject) Global Native

; Get or set whether the actor has been eaten by a werewolf or a cannibal (the same flag is used for both).
;
Bool Function GetHasBeenEaten (Actor akSubject) Global Native
     Function SetHasBeenEaten (Actor akSubject, Bool abEaten = True) Global Native ; Does nothing if the actor is still alive.

Int Function GetSoulSize (Actor akSubject) Global Native ; Returns enum: 0 = empty, 1 = petty, and so on up to grand.

Bool Function IsHorse (Actor akSubject) Global Native ; Checks whether the actor has the HRSK default object (KYWD:ActorTypeHorse).

; Actors generate "detection events" whenever they make a sound audible to NPCs. 
; Detection events have a sound level and a position, and expire after a short 
; delay (configurable as GMST:fDetectionEventExpireTime). An actor can only have 
; one ongoing detection event. Detection events are stored within the AI process, 
; so actors with AI disabled cannot produce them.
;
        Function CeaseDetectionEvent         (Actor akSubject) Global Native
Float   Function GetDetectionEventAge        (Actor akSubject) Global Native ; returns zero for no event
Float[] Function GetDetectionEventPosition   (Actor akSubject) Global Native ; returns an empty array if there is no detection event
Int     Function GetDetectionEventSoundLevel (Actor akSubject) Global Native ; returns -1 for no event
Bool    Function HasDetectionEvent           (Actor akSubject) Global Native