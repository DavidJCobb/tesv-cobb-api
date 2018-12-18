Scriptname CobbAPIBatchSpawnComplex Hidden

;
; Move a collection of objects, optionally relative to a target. APIs are designed 
; to mimic SKSE's SpawnerTask in functionality and usage, save for some fundamental 
; differences:
;
; This Papyrus class allows you to group spawns into buckets called "sets." After 
; you execute a batch operation, the "task" object will hang around in memory instead 
; of being automatically deleted, and you will be able to query it to retrieve each 
; set of spawned objects individually, as arrays. Moreover, spawns that fail will not 
; be silently removed; they will produce None entries in those arrays.
;
; When you are done with a spawn operation and have retrieved all needed results, you 
; should call the Delete() function to manually destroy the "task" object.
;

Int Function Create() Global Native

Function AddSpawn            (Int aiHandle, Int aiSet, Form akBase, ObjectReference akTarget, Float[] afPositionOffset, Float[] afRotation, Bool abForcePersist = False, Bool abInitiallyDisabled = False) Global Native
Function AddSpawnAroundPoint (Int aiHandle, Int aiSet, Form akBase, ObjectReference akAnchor, Float[] afTargetPosition, Float[] afTargetRotation, Float[] afPositionOffset, Float[] afRotation, Bool abForcePersist = False, Bool abInitiallyDisabled = False) Global Native

ObjectReference[] Function Run        (Int aiHandle)            Global Native ; Returns an array of all objects that were spawned.
ObjectReference[] Function GetResults (Int aiHandle, Int aiSet) Global Native ; Returns an array of all spawns in a given set.
                  Function Delete (Int aiHandle) Global Native
                  Function Cancel (Int aiHandle) Global Native