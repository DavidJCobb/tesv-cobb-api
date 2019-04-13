Scriptname CobbAPIBatchSetMotionType Hidden

;
; Set motion type on a batch of objects.
;

Int Function Create() Global Native

Function AddOperation(Int aiHandle, ObjectReference akSubject, Int aiMotionType, Bool abAllowActivate) Global Native
Function SetWhetherMarkChanged(Int aiHandle, Bool abMarkChanged) Global Native

ObjectReference[] Function Run    (Int aiHandle) Global Native ; Returns an array of all objects that were moved.
                  Function Cancel (Int aiHandle) Global Native