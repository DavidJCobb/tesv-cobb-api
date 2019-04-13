Scriptname CobbAPIBatchMoveRel Hidden

;
; Move a collection of objects, optionally relative to a target. APIs are designed 
; to mimic SKSE's SpawnerTask in functionality and usage.
;
; This Papyrus class differs from CobbPosBatchMove in that coordinates specified 
; relative to a target will take the target's orientation into account, i.e. they 
; will use the target's relative axes.
;

Int Function Create() Global Native

Function AddOperation            (Int aiHandle, ObjectReference akSubject, ObjectReference akTarget, Float[] afPositionOffset, Float[] afRotationOffset) Global Native
Function AddOperationAroundPoint (Int aiHandle, ObjectReference akSubject, ObjectReference akAnchor, Float[] afTargetPosition, Float[] afTargetRotation, Float[] afPositionOffset, Float[] afRotation) Global Native

;
; Use these functions to add special/convenience operations to the batch-task. Specify the type of special 
; operation using the aiOperationType argument:
;
; 0 = Normal operation
; 1 = Move akSubject's (destination's) teleport marker, instead of moving the reference itself. (use on load doors)
; 2 = Move akSubject back to its editor location
; 3 = Move akSubject to its editor offset from akTarget (that is, the offest from one editor location to the other)
; 4 = Move akSubject's (destination's) teleport marker to its editor offset from akSubject
;
Function AddOperationSpecial            (Int aiHandle, ObjectReference akSubject, ObjectReference akTarget, Float[] afPositionOffset, Float[] afRotationOffset, Int aiOperationType) Global Native
Function AddOperationSpecialAroundPoint (Int aiHandle, ObjectReference akSubject, ObjectReference akAnchor, Float[] afTargetPosition, Float[] afTargetRotation, Float[] afPositionOffset, Float[] afRotation, Int aiOperationType) Global Native

Function SetAlsoMoveTeleportMarkers (Int aiHandle, Int aiWhich) Global Native ; 0 by default; if set to 1 and you move load doors, we will also move the matching teleport marker to keep it in the same spot relative to the door; if set to 2, we maintain the marker's offset from the door's editor position

ObjectReference[] Function Run    (Int aiHandle) Global Native ; Returns an array of all objects that were moved.
                  Function Cancel (Int aiHandle) Global Native