#pragma once
#include "Papyrus/CompoundOperations/MoveGroup.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameReferences.h"

#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "Services/TeleportMarkerService.h"

#include "Miscellaneous/math.h"
#include "Shared.h" // for access to our object storage
#include "CobbRotation.h"
#include "Papyrus/Rotations.h"

#define ERROR_AND_RETURN_IF_WRONG_STACK(verb) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchMoveGroup") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return; \
   }
#define ERROR_AND_RETURN_VALUE_IF_WRONG_STACK(verb, retval) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("BatchMoveGroup") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return retval; \
   }

namespace CobbPapyrus {
   namespace BatchMoveGroup {
      //
      // Functor methods:
      //
      bool BatchMoveGroupFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         //
         SERIALIZATION_ASSERT(WriteData(intfc, &_rootRefrHandle), "Failed to write the root object's handle.");
         SERIALIZATION_ASSERT(WriteData(intfc, &_destinationPos), "Failed to write the destination position.");
         SERIALIZATION_ASSERT(WriteData(intfc, &_destinationRot), "Failed to write the destination rotation.");
         //
         UInt32 operationCount = this->operations.size();
         SERIALIZATION_ASSERT(WriteData(intfc, &operationCount), "Failed to write the operation count.");
         //
         for (UInt32 i = 0; i < operationCount; i++)
            SERIALIZATION_ASSERT(WriteData(intfc, &(this->operations[i])), "Failed to write operation %d.", i);
         //
         SERIALIZATION_ASSERT(WriteData(intfc, &(this->alsoMoveTeleportMarkers)), "Failed to write the also-move-teleport-markers flag.");
         SERIALIZATION_ASSERT(WriteData(intfc, &(this->targetCell->formID)), "Failed to write the target cell form ID.");
         //
         return true;
      };
      bool BatchMoveGroupFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         {
            UInt32 rootRefrHandle = NULL;
            SERIALIZATION_ASSERT(ReadData(intfc, &rootRefrHandle), "Failed to read the root object's handle.");
            this->_rootRefrHandle = rootRefrHandle;
         }
         {
            NiPoint3 pos;
            NiPoint3 rot;
            SERIALIZATION_ASSERT(ReadData(intfc, &pos), "Failed to read the destination position.");
            SERIALIZATION_ASSERT(ReadData(intfc, &rot), "Failed to read the destination rotation.");
            this->_destinationPos = pos;
            this->_destinationRot = rot;
         }
         {
            UInt32 operationCount;
            SERIALIZATION_ASSERT(ReadData(intfc, &operationCount), "Failed to read the operation count.");
            //
            this->operations.resize(operationCount);
            for (UInt32 i = 0; i < operationCount; i++) {
               OperationData e;
               SERIALIZATION_ASSERT(ReadData(intfc, &e), "Failed to read operation %d.", i);
               this->operations.push_back(e);
            }
         }
         //
         if (version >= 2) {
            SERIALIZATION_ASSERT(ReadData(intfc, &(this->alsoMoveTeleportMarkers)), "Failed to read the also-move-teleport-markers flag.");
         }
         if (version >= 3) {
            UInt32 cellID = 0;
            SERIALIZATION_ASSERT(ReadData(intfc, &cellID), "Failed to read the target cell form ID.");
            if (cellID)
               this->targetCell = DYNAMIC_CAST(LookupFormByID(cellID), TESForm, TESObjectCELL);
         }
         //
         return true;
      };
      void BatchMoveGroupFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         UInt32 nullHandle = *g_invalidRefHandle;
         VMResultArray<TESObjectREFR*> allMoved;
         //
         RE::refr_ptr root;
         NiPoint3 originalRootPos;
         NiPoint3 originalRootRot;
         {
            root.set_from_handle(&_rootRefrHandle);
            if (root == nullptr) {
               registry->LogError("Unable to complete " PapyrusPrefixString("BatchMoveGroup") " operation; no root was ever specified.", this->_stackId);
               PackValue(&resultValue, &allMoved, registry);
               return;
            }
            originalRootPos = root->pos;
            originalRootRot = root->rot;
            {
               RE::TESObjectCELL* parentCell = (RE::TESObjectCELL*) this->targetCell;
               RE::TESWorldSpace* worldspace = nullptr;
               if (parentCell == nullptr) {
                  parentCell = root->parentCell;
                  worldspace = CALL_MEMBER_FN(root, GetWorldspace)();
               } else {
                  worldspace = parentCell->parentWorld;
               }
               NiPoint3 destinationRotAsRadians;
               destinationRotAsRadians.x = cobb::degrees_to_radians(this->_destinationRot.x);
               destinationRotAsRadians.y = cobb::degrees_to_radians(this->_destinationRot.y);
               destinationRotAsRadians.z = cobb::degrees_to_radians(this->_destinationRot.z);
               //
               root->MoveTo(&nullHandle, parentCell, worldspace, &(this->_destinationPos), &destinationRotAsRadians);
               //
               if (this->alsoMoveTeleportMarkers) {
                  RE::refr_ptr destination;
                  root->GetDestinationDoor(destination);
                  if (destination)
                     TeleportMarkerService::GetInstance().MoveMarkerRelativeTo(destination.get_base(), originalRootPos, originalRootRot, this->_destinationPos, destinationRotAsRadians);
               }
            }
            allMoved.push_back((::TESObjectREFR*) root.get_base());
         }
         //
         for (size_t i = 0; i < this->operations.size(); i++) {
            OperationData& e = this->operations[i];
            RE::refr_ptr subject;
            //
            // Look up the reference.
            //
            subject.set_from_handle(&e.subjectRefrHandle);
            if (subject == nullptr || subject == root)
               continue;
            //
            // Position the subject.
            //
            RE::TESObjectCELL* parentCell = (RE::TESObjectCELL*) this->targetCell;
            RE::TESWorldSpace* worldspace = nullptr;
            if (parentCell == nullptr) {
               parentCell = subject->parentCell;
               worldspace = CALL_MEMBER_FN(subject, GetWorldspace)();
            } else {
               worldspace = parentCell->parentWorld;
            }
            //
            NiPoint3 finalPos(subject->pos);
            NiPoint3 finalRot(subject->rot);
            NiPoint3 originalPos(finalPos);
            NiPoint3 originalRot(finalRot);
            {
               //
               // Derive coordinates relative to the root's original location, and then apply them 
               // as an offset from the new location.
               //
               Cobb::Coordinates rel;
               Cobb::Coordinates abs;
               Cobb::GetRelativeCoordinates  (rel, originalRootPos,       originalRootRot,       subject->pos, subject->rot,      true,  true);
               Cobb::ApplyRelativeCoordinates(abs, this->_destinationPos, this->_destinationRot, rel.pos,      (NiPoint3)rel.rot, false, rel.rot.isRadians);
               abs.rot.ConvertToRadians();
               finalPos = abs.pos;
               finalRot.x = abs.rot.x;
               finalRot.y = abs.rot.y;
               finalRot.z = abs.rot.z;
            }
            //
            //MoveRefrToPosition(subject, &nullHandle, parentCell, worldspace, &finalPos, &finalRot);
            subject->MoveTo(&nullHandle, parentCell, worldspace, &finalPos, &finalRot);
            if (this->alsoMoveTeleportMarkers) {
               RE::refr_ptr destination;
               subject->GetDestinationDoor(destination);
               if (destination)
                  TeleportMarkerService::GetInstance().MoveMarkerRelativeTo(destination.get_base(), originalPos, originalRot, finalPos, finalRot);
            }
            //
            allMoved.push_back((::TESObjectREFR*) subject.get_base());
         }
         //
         PackValue(&resultValue, &allMoved, registry);
      };
      void BatchMoveGroupFunctor::AddChild(TESObjectREFR* subject) {
         UInt32 subjectRefrHandle = GetOrCreateRefrHandle(subject);
         if (subjectRefrHandle == *g_invalidRefHandle)
            return;
         //
         OperationData t = { subjectRefrHandle };
         this->operations.push_back(t);
      };
      void BatchMoveGroupFunctor::AddChildren(VMArray<TESObjectREFR*> subject) {
         UInt32 length = subject.Length();
         for (UInt32 i = 0; i < length; i++) {
            TESObjectREFR* value;
            subject.Get(&value, i);
            UInt32 subjectRefrHandle = GetOrCreateRefrHandle(value);
            if (subjectRefrHandle != *g_invalidRefHandle) {
               OperationData entry = { subjectRefrHandle };
               this->operations.push_back(entry);
            }
         }
      };
      void BatchMoveGroupFunctor::SetRoot(TESObjectREFR* subject) {
         UInt32 subjectRefrHandle = GetOrCreateRefrHandle(subject);
         if (subjectRefrHandle == *g_invalidRefHandle)
            return;
         //
         _rootRefrHandle = subjectRefrHandle;
      };
      void BatchMoveGroupFunctor::SetDestinationPosition(float x, float y, float z) {
         _destinationPos.x = x;
         _destinationPos.y = y;
         _destinationPos.z = z;
      };
      void BatchMoveGroupFunctor::SetDestinationRotation(float x, float y, float z) {
         _destinationRot.x = x;
         _destinationRot.y = y;
         _destinationRot.z = z;
      };
      void BatchMoveGroupFunctor::SetAlsoMoveTeleportMarkers(bool which) {
         this->alsoMoveTeleportMarkers = which;
      };
      void BatchMoveGroupFunctor::SetTargetCell(TESObjectCELL* target) {
         this->targetCell = target;
      };
      //
      // Papyrus APIs:
      //
      SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         BatchMoveGroupFunctor* func = new BatchMoveGroupFunctor(stackId);
         UInt32 handle = StorableObjectStorageInstance().StoreObject(func, stackId);
         return handle;
      }
      void AddChild(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         TESObjectREFR* subject
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(subject == NULL, "Subject parameter must not be none.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         func->AddChild(subject);
      }
      void AddChildren(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         VMArray<TESObjectREFR*> subject
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         func->AddChildren(subject);
      }
      void SetDestination(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         float pX, float pY, float pZ, float rX, float rY, float rZ
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         func->SetDestinationPosition(pX, pY, pZ);
         func->SetDestinationRotation(rX, rY, rZ);
      }
      void SetDestinationPosition(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         float pX, float pY, float pZ
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         func->SetDestinationPosition(pX, pY, pZ);
      }
      void SetDestinationRotation(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         float rX, float rY, float rZ
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         func->SetDestinationRotation(rX, rY, rZ);
      }
      void SetRoot(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         TESObjectREFR* subject
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         ERROR_AND_RETURN_IF(subject == NULL, "Subject parameter must not be none.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         func->SetRoot(subject);
      }
      void SetAlsoMoveTeleportMarkers(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* tag,
         SInt32 handle, bool which
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->SetAlsoMoveTeleportMarkers(which);
      };
      void SetTargetCell(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* tag,
         SInt32 handle, TESObjectCELL* cell
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->SetTargetCell(cell);
      };
      bool Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_0_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_VALUE_IF_WRONG_STACK("executed", false);
         func = StorableObjectStorageInstance().TakeObject<BatchMoveGroupFunctor>(handle); // remove it from storage
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("BatchMoveGroup") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled and its handle has been freed.", stackId);
            delete func;
            return false;
         }
      }
      void Cancel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         BatchMoveGroupFunctor* func = StorableObjectStorageInstance().AccessObject<BatchMoveGroupFunctor>(handle);
         ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("canceled");
         func = StorableObjectStorageInstance().TakeObject<BatchMoveGroupFunctor>(handle); // remove it from storage
         delete func;
      }
   }
};

bool CobbPapyrus::BatchMoveGroup::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<BatchMoveGroupFunctor>();
   //
   // PAPYRUS APIS
   //
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, SInt32>(
         "Create",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::Create,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "Create", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, TESObjectREFR*>(
         "AddChild",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::AddChild,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "AddChild", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, VMArray<TESObjectREFR*>>(
         "AddChildren",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::AddChildren,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "AddChildren", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, TESObjectREFR*>(
         "SetRoot",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::SetRoot,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "SetRoot", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction7<StaticFunctionTag, void, SInt32, float, float, float, float, float, float>(
         "SetDestination",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::SetDestination,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "SetDestination", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, SInt32, float, float, float>(
         "SetDestinationPosition",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::SetDestinationPosition,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "SetDestinationPosition", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, SInt32, float, float, float>(
         "SetDestinationRotation",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::SetDestinationRotation,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "SetDestinationRotation", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, bool>(
         "SetAlsoMoveTeleportMarkers",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::SetAlsoMoveTeleportMarkers,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "SetAlsoMoveTeleportMarkers", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, TESObjectCELL*>(
         "SetTargetCell",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::SetTargetCell,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "SetTargetCell", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, SInt32>(
         "Cancel",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::Cancel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("BatchMoveGroup"), "Cancel", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32>(
         "Run",
         PapyrusPrefixString("BatchMoveGroup"),
         BatchMoveGroup::Run,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};