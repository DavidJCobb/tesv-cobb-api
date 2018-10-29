Scriptname CobbAPIBatchMove Hidden

;
; Move a collection of objects, optionally relative to a target. APIs are designed 
; to mimic SKSE's SpawnerTask in functionality and usage.
;

Int Function Create() Global Native

Function AddOperation(Int aiHandle, ObjectReference akSubject, ObjectReference akTarget, Float[] afPositionOffset, Float[] afRotation) Global Native

;
; Use this function to add special/convenience operations to the batch-task. Specify the type of special 
; operation using the aiOperationType argument:
;
; 0 = Normal operation
; 1 = Move the reference's teleport marker, instead of moving the reference itself. (use on load doors)
; 2 = Move the reference back to its editor location
;
Function AddOperationSpecial(Int aiHandle, ObjectReference akSubject, ObjectReference akTarget, Float[] afPositionOffset, Float[] afRotation, Int aiOperationType) Global Native

Function SetAlsoMoveTeleportMarkers (Int aiHandle, Int aiWhich) Global Native ; 0 by default; if set to 1 and you move load doors, we will also move the matching teleport marker to keep it in the same spot relative to the door; if set to 2, we maintain the marker's offset from the door's editor position

ObjectReference[] Function Run    (Int aiHandle) Global Native ; Returns an array of all objects that were moved.
                  Function Cancel (Int aiHandle) Global Native