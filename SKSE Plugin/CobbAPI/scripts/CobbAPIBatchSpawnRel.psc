Scriptname CobbAPIBatchSpawnRel Hidden

;
; Move a collection of objects, optionally relative to a target. APIs are designed 
; to mimic SKSE's SpawnerTask in functionality and usage.
;
; This Papyrus class differs from SKSE's SpawnerTask in that coordinates specified 
; relative to a target will take the target's orientation into account, i.e. they 
; will use the target's relative axes.
;

Int Function Create() Global Native

Function AddSpawn            (Int aiHandle, Form akBase, ObjectReference akTarget, Float[] afPositionOffset, Float[] afRotation, Int aiCount = 1, Bool abForcePersist = False, Bool abInitiallyDisabled = False) Global Native
Function AddSpawnAroundPoint (Int aiHandle, Form akBase, ObjectReference akAnchor, Float[] afTargetPosition, Float[] afTargetRotation, Float[] afPositionOffset, Float[] afRotation, Int aiCount = 1, Bool abForcePersist = False, Bool abInitiallyDisabled = False) Global Native

ObjectReference[] Function Run    (Int aiHandle) Global Native ; Returns an array of all objects that were spawned.
                  Function Cancel (Int aiHandle) Global Native