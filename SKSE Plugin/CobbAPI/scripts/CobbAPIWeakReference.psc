Scriptname CobbAPIWeakReference Hidden

;/

Say you need to be able to refer to an arbitrary object, but you also 
need to be sure that you won't prevent it from being deleted. A typical 
variable simply won't do. Use this instead.

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