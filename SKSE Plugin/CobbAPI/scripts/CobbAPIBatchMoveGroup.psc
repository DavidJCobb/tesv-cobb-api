Scriptname CobbAPIBatchMoveGroup Hidden

;
; Move a collection of objects. One object in the collection will be designated 
; the "root;" the others will be moved relative to this root. APIs are designed 
; to mimic SKSE's SpawnerTask in functionality and usage.
;
; A root MUST BE SPECIFIED, or the operation will fail without making any changes.
;
; This Papyrus class differs from CobbPosBatchMove in that coordinates specified 
; relative to a target will take the target's orientation into account, i.e. they 
; will use the target's relative axes.
;

Int Function Create() Global Native

Function AddChild    (Int aiHandle, ObjectReference akChild) Global Native
Function AddChildren (Int aiHandle, ObjectReference[] akChildren) Global Native

Function SetRoot                    (Int aiHandle, ObjectReference akRoot) Global Native
Function SetDestination             (Int aiHandle, Float pX, Float pY, Float pZ, Float rX, Float rY, Float rZ) Global Native
Function SetDestinationPosition     (Int aiHandle, Float pX, Float pY, Float pZ) Global Native
Function SetDestinationRotation     (Int aiHandle, Float rX, Float rY, Float rZ) Global Native
Function SetAlsoMoveTeleportMarkers (Int aiHandle, Bool bWhich) Global Native ; False by default; if set to true and you move load doors, we will also move the matching teleport marker to keep it in the same spot relative to the door
Function SetTargetCell              (Int aiHandle, Cell akCell) Global Native ; Use to move references to a different cell

ObjectReference[] Function Run    (Int aiHandle) Global Native ; Returns an array of all objects that were moved.
                  Function Cancel (Int aiHandle) Global Native