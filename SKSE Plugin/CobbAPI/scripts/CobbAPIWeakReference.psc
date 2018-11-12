Scriptname CobbAPIWeakReference Hidden

;/

Say you need to be able to refer to an arbitrary object, but you also 
need to be sure that you won't prevent it from being deleted. A typical 
variable simply won't do. Use this instead.

NOV 12 2018: This is a years-old draft, and probably won't make it to production. 
If we want to make this work, it would have to work by caching the reference's 
form ID and catching when the ID is freed (if temporary) or missing (if pre-placed 
in a now-uninstalled mod).

/;

Int             Function Set (ObjectReference akSelf, ObjectReference akTarget) Global Native
ObjectReference Function Get (ObjectReference akSelf, Int aiHandle) Global Native

Function Delete    (ObjectReference akSelf, Int aiHandle) Global Native
Function DeleteAll (ObjectReference akSelf) Global Native

;/

Consider the following example script:

Int _iHandle = -1
Event OnActivate(ObjectReference akActionRef)
   _iHandle = CobbPosWeakReference.Set(Self, akActionRef) ; store a "weak reference" to akActionRef, so that we can remember it without preventing deletion
EndEvent

Event OnCellLoad()
   If _iHandle < 0
      Return
   EndIf
   ObjectReference kTemporary = CobbPosWeakReference.Get(Self, _iHandle)
   If !kTemporary
      CobbPosWeakReference.Delete(Self, _iHandle) ; the weak-reference hangs around even when the target is deleted. you need to drop it manually
     _iHandle = -1
     Return
   EndIf
   kTemporary.MoveTo(Self)
EndEvent

/;