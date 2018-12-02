#pragma once
#include "Papyrus/ObjectReference.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h"
#include "skse/GameRTTI.h"

#include "Miscellaneous/math.h"

#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Forms/BaseForms/base.h"
#include "ReverseEngineered/Forms/BaseForms/TESFurniture.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectLIGH.h"
#include "ReverseEngineered/Objects/BGSPrimitive.h"
#include "ReverseEngineered/NetImmerse/controllers.h"
#include "ReverseEngineered/NetImmerse/havok.h"
#include "ReverseEngineered/NetImmerse/NiExtraData.h"

#include "Patches/PlaceableCollisionPrimitives.h"

#include "Services/TeleportMarkerService.h"

#include "Types/bounds.h"

//#include "Services/ExtraDataService.h"
//#include "Papyrus/SharedFunctors/ExtraDataServiceFunctor.h"

#include "CobbRotation.h"

#include "skse/NiNodes.h"
#include "skse/NiRTTI.h"

RE::BSFurnitureMarkerNode* _GetFRN(TESObjectREFR* reference, VMClassRegistry* registry, UInt32 stackId) {
   //
   // Shortcut function to get a BSFurnitureMarkerNode from a reference's loaded 3D. If it fails, it will 
   // log a relevant error to Papyrus and then return NULL; all you have to do is check the result and 
   // execute your fallback (e.g. returning a sentinel value).
   //
   RE::BSFurnitureMarkerNode* FRN = nullptr;
   {
      NiNode* root = reference->GetNiNode();
      if (!root) {
         registry->LogError("Cannot get furniture marker data for this reference; its 3D is not loaded.", stackId);
         return nullptr;
      }
      FRN = (RE::BSFurnitureMarkerNode*)(root->GetExtraData("FRN"));
      //FRN = (RE::BSFurnitureMarkerNode*)(root->GetExtraData(BSFixedString("FRN")));
      if (!FRN) {
         registry->LogError("Cannot get furniture marker data for this reference; there is no furniture data in its 3D model.", stackId);
         return nullptr;
      }
   }
   return FRN;
};

namespace CobbPapyrus {
   namespace ObjectReference {
      namespace CollisionPrimitives {
         VMResultArray<float> GetBoundsToHalfwidthOffsets(
            VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
            TESForm* form
         ) {
            VMResultArray<float> result;
            if (form == nullptr) {
               registry->LogError("Invalid or None form specified.", stackId);
               return result;
            }
            RE::TESBoundObject* bounded = (RE::TESBoundObject*) DYNAMIC_CAST(form, TESForm, TESBoundObject);
            if (bounded == nullptr) {
               registry->LogError("Invalid form specified.", stackId);
               return result;
            }
            auto offset = [](SInt32 low, SInt32 high) { return ((high - low) / 2) + low; };
            result.resize(3);
            result[0] = offset(bounded->boundsMax.x, bounded->boundsMin.x);
            result[1] = offset(bounded->boundsMax.y, bounded->boundsMin.y);
            result[2] = offset(bounded->boundsMax.z, bounded->boundsMin.z);
            return result;
         };
         VMResultArray<float> GetBoundsToHalfwidths(
            VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
            TESForm* form
         ) {
            VMResultArray<float> result;
            if (form == nullptr) {
               registry->LogError("Invalid or None form specified.", stackId);
               return result;
            }
            RE::TESBoundObject* bounded = (RE::TESBoundObject*) DYNAMIC_CAST(form, TESForm, TESBoundObject);
            if (bounded == nullptr) {
               registry->LogError("Invalid form specified.", stackId);
               return result;
            }
            result.resize(3);
            result[0] = abs((SInt32) bounded->boundsMax.x - (SInt32) bounded->boundsMin.x) / 2;
            result[1] = abs((SInt32) bounded->boundsMax.y - (SInt32) bounded->boundsMin.y) / 2;
            result[2] = abs((SInt32) bounded->boundsMax.z - (SInt32) bounded->boundsMin.z) / 2;
            return result;
         };
         SInt32 MakeCollisionPrimitiveBox(
            VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
            TESObjectREFR* subject,
            UInt32 collisionLayer,
            VMArray<float> halfwidths
         ) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Reference does not exist.", registry, stackId);
            {  // Validate base form.
               auto baseForm = subject->baseForm;
               ERROR_AND_RETURN_0_IF(baseForm == nullptr, "Reference has no identifiable base form.", registry, stackId);
               #if COBB_PLACEABLE_NAVCUT_USES_MANAGER == 0
                  ERROR_AND_RETURN_0_IF(baseForm->formID != g_PlaceableCollisionPrimitiveFormID, "Reference base form ID must be 0x00C0BB00.", registry, stackId);
               #else
                  auto acti = DYNAMIC_CAST(baseForm, TESForm, TESObjectACTI);
                  ERROR_AND_RETURN_0_IF(
                     !acti || !CobbPatches::PlaceableCollisionPrimitives::Manager::GetInstance().Matches(acti),
                     "The reference's base form must be an Activator, and that Activator must have a keyword injected to form ID 0x00C0BB03.",
                     registry,
                     stackId
                  );
               #endif
            }
            ERROR_AND_RETURN_0_IF(halfwidths.Length() < 3, "You need to pass an array of three floats to serve as halfwidths.", registry, stackId);
            //
            SInt32 result = 0;
            {  // Destroy existing primitive and collision data.
               auto extraList = (RE::BaseExtraList*) &(subject->extraData);
               CALL_MEMBER_FN(extraList, SetExtraCollisionData)(nullptr);
               extraList->RemoveExtra(kExtraData_Primitive); // calls ExtraPrimitive::Dispose, which cleans up the accompanying BGSPrimitive*
            }
            {  // Add primitive and collision data.
               auto extra_p = RE::ExtraPrimitive::Create();
               auto extra_c = RE::ExtraCollisionData::Create();
               extra_c->data->collisionLayer = collisionLayer; // NAVCUT
               {  // Fill primitive data.
                  NiPoint3 sizes;
                  halfwidths.Get(&sizes.x, 0);
                  halfwidths.Get(&sizes.y, 1);
                  halfwidths.Get(&sizes.z, 2);
                  //
                  RE::BGSPrimitiveBox* box = RE::BGSPrimitiveBox::Create(&sizes);
                  extra_p->primitive = (BGSPrimitive*) box;
               }
               subject->extraData.Add(kExtraData_Primitive, extra_p);
               subject->extraData.Add(kExtraData_CollisionData, extra_c);
               result = 1;
            }
            return result;
         };
      };
      namespace Coordinates {
         VMResultArray<float> GetCoordinates(VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject) {
            VMResultArray<float> result;
            result.resize(6, 0.0);
            if (subject == nullptr) {
               registry->LogError("Cannot get the coordinates of an unspecified or None reference.", stackId);
               return result;
            }
            result[0] = subject->pos.x;
            result[1] = subject->pos.y;
            result[2] = subject->pos.z;
            result[3] = cobb::radians_to_degrees(subject->rot.x);
            result[4] = cobb::radians_to_degrees(subject->rot.y);
            result[5] = cobb::radians_to_degrees(subject->rot.z);
            return result;
         };
         VMResultArray<float> GetPosition(VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject) {
            VMResultArray<float> result;
            result.resize(3, 0.0);
            if (subject == nullptr) {
               registry->LogError("Cannot get the position of an unspecified or None reference.", stackId);
               return result;
            }
            result[0] = subject->pos.x;
            result[1] = subject->pos.y;
            result[2] = subject->pos.z;
            return result;
         };
         VMResultArray<float> GetRotation(VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject) {
            VMResultArray<float> result;
            result.resize(3, 0.0);
            if (subject == nullptr) {
               registry->LogError("Cannot get the rotation of an unspecified or None reference.", stackId);
               return result;
            }
            result[0] = cobb::radians_to_degrees(subject->rot.x);
            result[1] = cobb::radians_to_degrees(subject->rot.y);
            result[2] = cobb::radians_to_degrees(subject->rot.z);
            return result;
         };
      }
      namespace Furniture {
         VMResultArray<SInt32> GetFurnitureEnabledMarkers(VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject) {
            VMResultArray<SInt32> result;
            if (subject == nullptr) {
               registry->LogError("Subject parameter must not be None.", stackId);
               return result;
            }
            RE::TESFurniture* baseForm = (RE::TESFurniture*) DYNAMIC_CAST(subject->baseForm, TESForm, TESFurniture);
            if (baseForm) {
               UInt32 markersAndFlags = baseForm->flags;
               for (SInt32 i = 0; i < 24; i++) {
                  UInt32 bit = markersAndFlags & (1 << i);
                  if (bit != 0)
                     result.push_back(i);
               }
            }
            return result;
         };
         VMResultArray<UInt32> GetFurnitureMarkerEntryPoints(VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject, UInt32 marker) {
            VMResultArray<UInt32> result;
            result.resize(5, false);
            if (subject == nullptr) {
               registry->LogError("Cannot get furniture data for an unspecified or None reference.", stackId);
               return result;
            }
            RE::TESFurniture* baseForm = (RE::TESFurniture*) DYNAMIC_CAST(subject->baseForm, TESForm, TESFurniture);
            if (!baseForm) {
               registry->LogError("Cannot get furniture data for a reference that isn't a Furniture.", stackId);
               return result;
            }
            {
               RE::BSFurnitureMarkerNode* FRN = _GetFRN(subject, registry, stackId);
               if (!FRN)
                  return result; // Failed to get furniture marker data. Relevant error message already written to the Papyrus log.
               //
               if (FRN->markers.count <= marker) {
                  registry->LogError("Cannot get furniture marker data for the desired reference. You asked for the Xth marker, but the object doesn't have that many markers.", stackId);
                  return result;
               }
               //
               // Get the base entry points defined in the model file.
               //
               RE::BSFurnitureMarkerNode::FurnitureMarker target = FRN->markers[marker];
               for (UInt32 i = 0; i < 5; i++) {
                  if (target.directions & (1 << i))
                     result[i] = 1;
               }
            }
            {
               //
               // Account for individual entry points being disabled in the FURN.
               //
               for (UInt32 i = 0; i < baseForm->markers.count; i++) {
                  if (baseForm->markers[i].furnitureMarkerIndex == marker) {
                     for (UInt32 j = 0; j < 5; j++) {
                        if (baseForm->markers[i].disabledEntryPoints & (1 << j))
                           result[j] = 0;
                     }
                     break;
                  }
               }
            }
            return result;
         }
         VMResultArray<float> GetFurnitureMarkerCoordinates(::VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject, UInt32 marker, bool absolute) {
            VMResultArray<float> result;
            result.resize(6, 0.0);
            if (subject == nullptr) {
               registry->LogError("Cannot get furniture data for an unspecified or None reference.", stackId);
               return result;
            }
            {
               RE::TESFurniture* baseForm = (RE::TESFurniture*) DYNAMIC_CAST(subject->baseForm, TESForm, TESFurniture);
               if (!baseForm) {
                  registry->LogError("Cannot get furniture data for a reference that isn't a Furniture.", stackId);
                  return result;
               }
            }
            RE::BSFurnitureMarkerNode* FRN = _GetFRN(subject, registry, stackId);
            if (!FRN)
               return result; // Failed to get furniture marker data. Relevant error message already written to the Papyrus log.
            if (FRN->markers.count <= marker) {
               registry->LogError("Cannot get furniture marker data for the desired reference. You asked for the Xth marker, but the object doesn't have that many markers.", stackId);
               return result;
            }
            //
            // Get the values we want.
            //
            result[0] = FRN->markers[marker].position.x;
            result[1] = FRN->markers[marker].position.y;
            result[2] = FRN->markers[marker].position.z;
            result[5] = cobb::radians_to_degrees(FRN->markers[marker].yaw);
            if (absolute) {
               NiPoint3 position(result[0], result[1], result[2]);
               NiPoint3 rotation(0.0, 0.0, FRN->markers[marker].yaw);
               Cobb::Coordinates applied;
               Cobb::ApplyRelativeCoordinates(applied, subject->pos, subject->rot, position, rotation, true, true);
               applied.rot.ConvertToDegrees();
               result[0] = applied.pos.x;
               result[1] = applied.pos.y;
               result[2] = applied.pos.z;
               result[3] = applied.rot.x;
               result[4] = applied.rot.y;
               result[5] = applied.rot.z;
            }
            return result;
         }
         UInt32 GetFurnitureMarkerType(::VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject, UInt32 marker) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot get furniture data for an unspecified or None reference.", registry, stackId);
            {
               RE::TESFurniture* baseForm = (RE::TESFurniture*) DYNAMIC_CAST(subject->baseForm, TESForm, TESFurniture);
               ERROR_AND_RETURN_0_IF(!baseForm, "Cannot get furniture data for a reference that isn't a Furniture.", registry, stackId);
            }
            RE::BSFurnitureMarkerNode* FRN = _GetFRN(subject, registry, stackId);
            if (!FRN)
               return 0; // Failed to get furniture marker data. Relevant error message already written to the Papyrus log.
            //
            ERROR_AND_RETURN_0_IF(FRN->markers.count <= marker, "Cannot get furniture marker data for the desired reference. You asked for the Xth marker, but the object doesn't have that many markers.", registry, stackId);
            //
            return FRN->markers[marker].type;
         }
         VMResultArray<float> GetFurnitureAllMarkerCoordinates(::VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject, bool absolute) {
            VMResultArray<float> result;
            if (subject == nullptr) {
               registry->LogError("Cannot get furniture data for an unspecified or None reference.", stackId);
               return result;
            }
            {
               RE::TESFurniture* baseForm = (RE::TESFurniture*) DYNAMIC_CAST(subject->baseForm, TESForm, TESFurniture);
               if (!baseForm) {
                  registry->LogError("Cannot get furniture data for a reference that isn't a Furniture.", stackId);
                  return result;
               }
            }
            RE::BSFurnitureMarkerNode* FRN = _GetFRN(subject, registry, stackId);
            if (!FRN)
               return result; // Failed to get furniture marker data. Relevant error message already written to the Papyrus log.
            //
            UInt32 maxMarkers = min(FRN->markers.count, 24); // Engine only supports up to 24 markers, but a NIF file could do weird stuff. Let's slap a sane limit on here.
            result.reserve(maxMarkers * 6);
            for (UInt32 i = 0; i < maxMarkers; i++) {
               RE::BSFurnitureMarkerNode::FurnitureMarker current = FRN->markers[i];
               if (absolute) {
                  NiPoint3 rotation(0.0, 0.0, current.yaw);
                  Cobb::Coordinates applied;
                  Cobb::ApplyRelativeCoordinates(applied, subject->pos, subject->rot, current.position, rotation, true, true);
                  applied.rot.ConvertToDegrees();
                  result.push_back(applied.pos.x);
                  result.push_back(applied.pos.y);
                  result.push_back(applied.pos.z);
                  result.push_back(applied.rot.x);
                  result.push_back(applied.rot.y);
                  result.push_back(applied.rot.z);
               } else {
                  result.push_back(current.position.x);
                  result.push_back(current.position.y);
                  result.push_back(current.position.z);
                  result.push_back(0);
                  result.push_back(0);
                  result.push_back(cobb::radians_to_degrees(current.yaw));
               }
            }
            return result;
         }
         VMResultArray<SInt32> GetFurnitureAllMarkerEntryPoints(::VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject) {
            VMResultArray<SInt32> result;
            if (subject == nullptr) {
               registry->LogError("Cannot get furniture data for an unspecified or None reference.", stackId);
               return result;
            }
            RE::TESFurniture* baseForm = (RE::TESFurniture*) DYNAMIC_CAST(subject->baseForm, TESForm, TESFurniture);
            if (!baseForm) {
               registry->LogError("Cannot get furniture data for a reference that isn't a Furniture.", stackId);
               return result;
            }
            UInt32 maxMarkers = 0;
            {
               RE::BSFurnitureMarkerNode* FRN = _GetFRN(subject, registry, stackId);
               if (!FRN)
                  return result; // Failed to get furniture marker data. Relevant error message already written to the Papyrus log.
               //
               maxMarkers = min(FRN->markers.count, 24); // Engine only supports up to 24 markers, but a NIF file could do weird stuff. Let's slap a sane limit on here.
               result.reserve(maxMarkers * 5);
               for (size_t i = 0; i < maxMarkers; i++) {
                  RE::BSFurnitureMarkerNode::FurnitureMarker current = FRN->markers[i];
                  for (UInt32 j = 0; j < 5; j++) {
                     result.push_back((bool)(current.directions & (1 << j)));
                  }
               }
            }
            if (maxMarkers) {
               //
               // Account for individual entry points being disabled in the FURN.
               //
               for (UInt32 i = 0; i < baseForm->markers.count; i++) {
                  RE::TESFurniture::ModifiedMarker current = baseForm->markers[i];
                  UInt32 m = current.furnitureMarkerIndex * 5;
                  if (m + 4 < result.size()) {
                     for (UInt32 j = 0; j < 5; j++) {
                        if (current.disabledEntryPoints & (1 << j))
                           result[m + j] = false;
                        if ((current.disabledEntryPoints >> 0x10) & (1 << j)) // Sleep markers have bitshifted flags?
                           result[m + j] = false;
                     }
                     //_MESSAGE("GetFurnitureAllMarkerEntryPoints: override %d (marker %d) flags checked without incident. Flags were %d.", i, markers[i].markerIndex, markers[i].disabledEntryPoints);
                  } else {
                     _MESSAGE("GetFurnitureAllMarkerEntryPoints: bounds issue when trying to get FURN changes to markers. Override %d maps to marker index %d, whose full values are out of our array's range (%d/%d).", i, current.furnitureMarkerIndex, (m + 4), result.size());
                     _MESSAGE("   Marker %d is listed with disabled-entry-points bitmask %d.", current.furnitureMarkerIndex, current.disabledEntryPoints);
                  }
               }
            }
            return result;
         }
         VMResultArray<UInt32> GetFurnitureAllMarkerTypes(::VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject) {
            VMResultArray<UInt32> result;
            if (subject == nullptr) {
               registry->LogError("Cannot get furniture data for an unspecified or None reference.", stackId);
               return result;
            }
            {
               RE::TESFurniture* baseForm = (RE::TESFurniture*) DYNAMIC_CAST(subject->baseForm, TESForm, TESFurniture);
               if (!baseForm) {
                  registry->LogError("Cannot get furniture data for a reference that isn't a Furniture.", stackId);
                  return result;
               }
            }
            UInt32 maxMarkers = 0;
            {
               RE::BSFurnitureMarkerNode* FRN = _GetFRN(subject, registry, stackId);
               if (!FRN)
                  return result; // Failed to get furniture marker data. Relevant error message already written to the Papyrus log.
               //
               maxMarkers = min(FRN->markers.count, 24); // Engine only supports up to 24 markers, but a NIF file could do weird stuff. Let's slap a sane limit on here.
               result.reserve(maxMarkers * 5);
               for (size_t i = 0; i < maxMarkers; i++)
                  result.push_back(FRN->markers[i].type);
            }
            return result;
         }
      }
      /*namespace Lights {
         bool SetFade(VMClassRegistry* registry, UInt32 stackId, ::StaticFunctionTag* base, TESObjectREFR* subject, float fade) {
            ERROR_AND_RETURN_IF(subject == NULL, "You must specify a reference.", registry, stackId);
            float relativeFade = fade;
            {  // Fade on the ExtraData is a relative value. We need to convert the argument from absolute to relative.
               RE::TESObjectLIGH* baseLight = (RE::TESObjectLIGH*)DYNAMIC_CAST(subject->baseForm, TESForm, TESObjectLIGH);
               ERROR_AND_RETURN_IF(baseLight == NULL, "Your reference must be a light.", registry, stackId);
               relativeFade -= baseLight->fade;
            }
            ExtraDataServiceFunctor* func =
               ExtraDataServiceFunctor::CreateFunctor(
                  stackId,
                  subject->formID,
                  kExtraData_LightData,
                  offsetof(RE::ExtraLightData, modifiedFade),
                  sizeof(RE::ExtraLightData::modifiedFade),
                  relativeFade
               );
            if (func->Queue()) {
               return true;
            } else {
               registry->LogError("Unable to queue " PapyrusPrefixString("ExtraDataServiceFunctor") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
               delete func;
               return false;
            }
         };
      };*/
      namespace LoadDoors {
         bool IsLoadDoor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check if a non-existent reference is a load door.", registry, stackId);
            return subject->extraData.HasType(kExtraData_Teleport);
         };
         bool IsTeleportMarkerInAttachedCell(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, TESObjectREFR* destination) {
            ERROR_AND_RETURN_0_IF(subject == nullptr && destination == nullptr, "Cannot check if a non-existent reference has a loaded teleport marker.", registry, stackId);
            return ((RE::TESObjectREFR*)subject)->IsTeleportMarkerInAttachedCell(destination);
         };
         TESObjectREFR* GetDestinationLoadDoor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot get the destination door for an unspecified or None reference.", registry, stackId);
            ERROR_AND_RETURN_0_IF(!subject->extraData.HasType(kExtraData_Teleport), "The specified reference is not a load door.", registry, stackId);
            RE::ExtraTeleport::TeleportData* data = CALL_MEMBER_FN(((RE::BaseExtraList*)&(subject->extraData)), GetExtraTeleportData)();
            ERROR_AND_RETURN_0_IF(!data, "Unable to access teleport data on the specified reference.", registry, stackId);
            TESObjectREFR* result = nullptr;
            {
               UInt32 handle = data->refHandle; // copy, to ensure that the original handle isn't changed by the next call
               LookupREFRByHandle(&handle, &result);
            }
            ERROR_AND_RETURN_0_IF(!result, "Unable to locate the specified load door's destination door.", registry, stackId);
            return result;
         };
         VMResultArray<float> GetTeleportMarkerCoordinates(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            VMResultArray<float> result;
            if (subject == nullptr) {
               registry->LogError("Cannot get teleport marker coordinates for an unspecified or None reference.", stackId);
               return result;
            }
            if (!subject->extraData.HasType(kExtraData_Teleport)) {
               registry->LogError("The specified reference is not a load door.", stackId);
               return result;
            }
            RE::ExtraTeleport::TeleportData* data = CALL_MEMBER_FN(((RE::BaseExtraList*)&(subject->extraData)), GetExtraTeleportData)();
            if (!data) {
               registry->LogError("Unable to access teleport data on the specified reference.", stackId);
               return result;
            }
            result.push_back(data->markerPosition.x);
            result.push_back(data->markerPosition.y);
            result.push_back(data->markerPosition.z);
            result.push_back(cobb::radians_to_degrees(data->markerRotation.x));
            result.push_back(cobb::radians_to_degrees(data->markerRotation.y));
            result.push_back(cobb::radians_to_degrees(data->markerRotation.z));
            return result;
         };
         void MoveTeleportMarkerToEditorLocOffset(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            ERROR_AND_RETURN_IF(subject == nullptr, "Cannot act on an unspecified or None reference.", registry, stackId);
            //
            TESObjectREFR* destination = (TESObjectREFR*) ((RE::TESObjectREFR*)subject)->GetDestinationDoor();
            ERROR_AND_RETURN_IF(destination == nullptr, "Couldn't get the destination door of this reference.", registry, stackId);
            //
            bool success = TeleportMarkerService::GetInstance().MoveMarkerToRelativeEditorLocOffset(destination, subject);
            if (!success) {
               registry->LogError("The operation failed for an unknown reason. One possible explanation is that the targeted door has never actually been moved.", stackId);
               return;
            }
         };
         void ResetTeleportMarker(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            ERROR_AND_RETURN_IF(subject == nullptr, "Cannot get the destination door for an unspecified or None reference.", registry, stackId);
            TeleportMarkerService::GetInstance().ResetMarker(subject);
         };
      }
      namespace Loaded3D {
         bool CheckNodeForControllerManagerFunctor::OnSave(SKSESerializationInterface* intfc) {
            using namespace Serialization;
            SERIALIZATION_ASSERT(WriteData(intfc, &(this->referenceFormID)), "Failed to write the target form ID.");
            return true;
         };
         bool CheckNodeForControllerManagerFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
            using namespace Serialization;
            SERIALIZATION_ASSERT(ReadData(intfc, &(this->referenceFormID)), "Failed to read the target form ID.");
            return true;
         };
         bool CheckNodeForControllerManagerFunctor::FindController(NiNode* node) {
            node->IncRef();
            {  // Does this node have a NiControllerManager?
               RE::NiObject* controller = (RE::NiObject*)node->m_controller;
               if (controller && CALL_MEMBER_FN(controller, VerifyType)(NiRTTI_NiControllerManager) != NULL) {
                  node->DecRef();
                  return true;
               }
            }
            {  // Do any of this node's descendants have a NiControllerManager?
               for (SInt16 i = 0; i < node->m_children.m_emptyRunStart; i++) {
                  NiAVObject* child = node->m_children.m_data[i];
                  if (!child)
                     continue;
                  NiNode* childNode = child->GetAsNiNode();
                  if (childNode) {
                     if (FindController(childNode)) {
                        node->DecRef();
                        return true;
                     }
                  }
               }
            }
            node->DecRef();
            return false;
         };
         void CheckNodeForControllerManagerFunctor::Run(VMValue& resultValue) {
            VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
            //
            if (this->referenceFormID == 0) {
               registry->LogError("A " PapyrusPrefixString("CheckNodeForControllerManager") " operation was unable to identify the reference to act on. (Form ID was zero.)", _stackId);
               return;
            }
            TESObjectREFR* target = (TESObjectREFR*)LookupFormByID(this->referenceFormID);
            if (target == nullptr) {
               registry->LogError("A " PapyrusPrefixString("CheckNodeForControllerManager") " operation was unable to identify the reference to act on. (Unable to get form.)", _stackId);
               return;
            }
            UInt32 result = 0;
            NiNode* node = target->GetNiNode();
            if (node == nullptr) {
               registry->LogError("Cannot examine the 3D model of a reference with no loaded 3D.", _stackId);
               return;
            }
            result = (UInt32) CheckNodeForControllerManagerFunctor::FindController(node);
            PackValue(&resultValue, &result, registry);
         };
         //
         bool HasNiControllerManager(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* reference) {
            ERROR_AND_RETURN_0_IF(reference == nullptr, "You must specify a reference.", registry, stackId);
            ERROR_AND_RETURN_0_IF(reference->GetNiNode() == nullptr, "Cannot examine the 3D model of a reference with no loaded 3D.", registry, stackId);
            CheckNodeForControllerManagerFunctor* func = new CheckNodeForControllerManagerFunctor(stackId);
            func->referenceFormID = reference->formID;
            //
            if (func->Queue()) {
               return true;
            } else {
               registry->LogError("Unable to queue " PapyrusPrefixString("CheckNodeForControllerManagerFunctor") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
               delete func;
               return false;
            }
         };
         //
         bool HasLocalCollision(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* reference) {
            ERROR_AND_RETURN_0_IF(reference == nullptr, "You must specify a reference.", registry, stackId);
            auto node = reference->GetNiNode();
            ERROR_AND_RETURN_0_IF(node == nullptr, "Cannot examine the 3D model of a reference with no loaded 3D.", registry, stackId);
            node->IncRef();
            //
            struct _CollStruct {
               bool result = false;
               //
               static void Iterator(RE::bhkCollisionObject* collision, void* parameters) {
                  _CollStruct* params = (_CollStruct*)parameters;
                  if ((collision->flags & 8) != 0)
                     params->result = true;
               };
            };
            _CollStruct parameters;
            IterateOverBhkCollisionObjects(node, &parameters, (BhkCollisionIteratorFunction*)&_CollStruct::Iterator);
            //
            node->DecRef();
            return parameters.result;
         };
         void Reload3D(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* reference) {
            ERROR_AND_RETURN_IF(reference == nullptr, "You must specify a reference.", registry, stackId);
            ((RE::TESObjectREFR*)reference)->Reload3D();
         };
         void Unload3D(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* reference) {
            ERROR_AND_RETURN_IF(reference == nullptr, "You must specify a reference.", registry, stackId);
            ((RE::TESObjectREFR*)reference)->Unk_6C(0, 0); // unloads 3D
         };
      };
      namespace Internals {
         #ifdef  COBB_PAPYRUS_OBJECTREFERENCE_CHANGEFORMS_USE_FUNCTOR
         bool ChangeFlagOperationFunctor::OnSave(SKSESerializationInterface* intfc) {
            using namespace Serialization;
            SERIALIZATION_ASSERT(WriteData(intfc, &(this->operationType)), "Failed to write the operation type.");
            SERIALIZATION_ASSERT(WriteData(intfc, &(this->formID)),        "Failed to write the target form ID.");
            SERIALIZATION_ASSERT(WriteData(intfc, &(this->operationData)), "Failed to write the operation data.");
            return true;
         };
         bool ChangeFlagOperationFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
            using namespace Serialization;
            SERIALIZATION_ASSERT(ReadData(intfc, &(this->operationType)), "Failed to read the operation type.");
            SERIALIZATION_ASSERT(ReadData(intfc, &(this->formID)),        "Failed to read the target form ID.");
            SERIALIZATION_ASSERT(ReadData(intfc, &(this->operationData)), "Failed to read the operation data.");
            return true;
         };
         void ChangeFlagOperationFunctor::Run(VMValue& resultValue) {
            VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
            //
            if (this->formID == 0) {
               registry->LogWarning("A " PapyrusPrefixString("ChangeFlag") " operation was unable to identify the reference to act on. (Form ID was zero.)", _stackId);
               return;
            }
            TESForm* target = LookupFormByID(this->formID);
            if (target == NULL) {
               registry->LogWarning("A " PapyrusPrefixString("ChangeFlag") " operation was unable to identify the reference to act on. (Unable to get form.)", _stackId);
               return;
            }
            UInt32 result = 0;
            bool returnResult = false;
            switch (this->operationType) {
               case kOperationType_Undefined:
                  break;
               case kOperationType_Retrieve:
                  result = ((RE::TESObjectREFR*)target)->GetChangeFlags(); // TODO: We should move this to RE::TESForm
                  returnResult = true;
                  break;
               case kOperationType_SetFlags:
                  if (this->operationData)
                     ((RE::TESForm*)target)->MarkChanged(this->operationData);
                  break;
               case kOperationType_ClearFlags:
                  if (this->operationData)
                     ((RE::TESForm*)target)->UnmarkChanged(this->operationData);
                  break;
            }
            if (returnResult)
               PackValue(&resultValue, &result, registry);
         };
         bool     MarkChanged(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* base, TESObjectREFR* subject, UInt32 flags) {
            ERROR_AND_RETURN_0_IF(subject == NULL, "Cannot modify change-flags for a None reference.", registry, stackId);
            //
            ChangeFlagOperationFunctor* func = new ChangeFlagOperationFunctor(stackId);
            func->formID = subject->formID;
            func->operationType = ChangeFlagOperationFunctor::OperationType::kOperationType_SetFlags;
            func->operationData = flags;
            //
            if (func->Queue()) {
               return true;
            } else {
               registry->LogError("Unable to queue " PapyrusPrefixString("ChangeFlag") " operation (set); too many batch operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
               delete func;
               return false;
            }
         };
         bool     UnmarkChanged(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* base, TESObjectREFR* subject, UInt32 flags) {
            ERROR_AND_RETURN_0_IF(subject == NULL, "Cannot modify change-flags for a None reference.", registry, stackId);
            //
            ChangeFlagOperationFunctor* func = new ChangeFlagOperationFunctor(stackId);
            func->formID = subject->formID;
            func->operationType = ChangeFlagOperationFunctor::OperationType::kOperationType_ClearFlags;
            func->operationData = flags;
            //
            if (func->Queue()) {
               return true;
            } else {
               registry->LogError("Unable to queue " PapyrusPrefixString("ChangeFlag") " operation (clear); too many batch operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
               delete func;
               return false;
            }
         };
         bool     GetChangeFlags(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
            ERROR_AND_RETURN_0_IF(subject == NULL, "You must specify a reference.", registry, stackId);
            //
            ChangeFlagOperationFunctor* func = new ChangeFlagOperationFunctor(stackId);
            func->formID = subject->formID;
            func->operationType = ChangeFlagOperationFunctor::OperationType::kOperationType_Retrieve;
            func->operationData = 0;
            //
            if (func->Queue()) {
               return true;
            } else {
               registry->LogError("Unable to queue " PapyrusPrefixString("ChangeFlag") " operation (get); too many batch operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
               delete func;
               return false;
            }
         };
         #endif
         //
         #ifndef COBB_PAPYRUS_OBJECTREFERENCE_CHANGEFORMS_USE_FUNCTOR
         void MarkChanged(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* base, TESObjectREFR* subject, UInt32 flags) {
            ERROR_AND_RETURN_IF(subject == nullptr, "Cannot modify change-flags for a None reference.", registry, stackId);
            ERROR_AND_RETURN_IF(flags == 0, "Zero is not a valid change-flag.", registry, stackId);
            subject->MarkChanged(flags);
         };
         void UnmarkChanged(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* base, RE::TESObjectREFR* subject, UInt32 flags) {
            ERROR_AND_RETURN_IF(subject == nullptr, "Cannot modify change-flags for a None reference.", registry, stackId);
            ERROR_AND_RETURN_IF(flags == 0, "Zero is not a valid change-flag.", registry, stackId);
            subject->UnmarkChanged(flags);
         };
         UInt32 GetChangeFlags(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectREFR* subject) {
            ERROR_AND_RETURN_0_IF(subject == nullptr, "You must specify a reference.", registry, stackId);
            return subject->GetChangeFlags();
         };
         #endif
         //
         bool HasExtraData(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* reference, UInt32 extraDataType) {
            ERROR_AND_RETURN_0_IF(reference == nullptr, "You must specify a reference.", registry, stackId);
            ERROR_AND_RETURN_0_IF(extraDataType < 1 || extraDataType > 0xB3, "Invalid extraData type. Must be between 1 and 0xB3, inclusive.", registry, stackId);
            //
            RE::BaseExtraList* extra = (RE::BaseExtraList*)&(reference->extraData);
            if (CALL_MEMBER_FN(extra, HasType)(extraDataType))
               return true;
            return false;
         };
      }
      /*VMResultArray<TESObjectREFR*> GetActivateChildren(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
         VMResultArray<TESObjectREFR*> result;
         if (subject == NULL) {
            registry->LogError("Cannot get the activate children of an unspecified or None reference.", stackId);
            return result;
         }
         RE::ExtraActivateRefChildren::Entry* current = CALL_MEMBER_FN((RE::BaseExtraList*)(&subject->extraData), GetExtraActivateRefChildren)();
         if (!current)
            return result;
         #ifdef _DEBUG
         _MESSAGE(PapyrusPrefixString("ObjectReference") ".GetActivateChildren(): extra data's unk08 is at 0x%08X.", (UInt32)current);
         #endif
         for (; current != NULL; current = current->nextEntry) {
            TESObjectREFR* reference = NULL;
            LookupREFRByHandle(&current->refHandle, &reference);
            if (reference != NULL)
               result.push_back(reference);
            #ifdef _DEBUG
            _MESSAGE(PapyrusPrefixString("ObjectReference") ".GetActivateChildren(): found reference handle 0x%08X. Has a reference? %d.", current->refHandle, (UInt32)(bool)reference);
            #endif
         }
         return result;
      };*/
      //
      SInt32 BaseHasWhichKeyword(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         TESObjectREFR* subject,
         VMArray<BGSKeyword*> keywords
      ) {
         TESForm* baseForm;
         if (subject == nullptr || (baseForm = subject->baseForm) == nullptr) {
            registry->LogError("Cannot test keywords on a None reference or a reference with no base form.", stackId);
            return -1;
         }
         BGSKeywordForm* formKeywords = DYNAMIC_CAST(baseForm, TESForm, BGSKeywordForm);
         if (formKeywords) {
            UInt32 kywdCount = formKeywords->numKeywords;
            UInt32 nameCount = keywords.Length();
            for (UInt32 i = 0; i < kywdCount; i++) {
               BGSKeyword* current = formKeywords->keywords[i];
               if (current) {
                  for (UInt32 j = nameCount; j; j--) {
                     BGSKeyword* keyword;
                     keywords.Get(&keyword, j);
                     if (keyword == current)
                        return j;
                  }
               }
            }
         }
         return -1;
      };
      SInt32 BaseHasWhichKeywordName(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         TESObjectREFR* subject,
         VMArray<BSFixedString> keywordNames
      ) {
         TESForm* baseForm;
         if (subject == nullptr || (baseForm = subject->baseForm) == nullptr) {
            registry->LogError("Cannot test keywords on a None reference or a reference with no base form.", stackId);
            return -1;
         }
         //BGSKeywordForm* formKeywords = DYNAMIC_CAST(baseForm, TESForm, BGSKeywordForm);
         BGSKeywordForm* formKeywords = RE::GetKeywordListFor(baseForm);
         if (formKeywords) {
            UInt32 kywdCount = formKeywords->numKeywords;
            UInt32 nameCount = keywordNames.Length();
            for (UInt32 i = 0; i < kywdCount; i++) {
               BGSKeyword* current = formKeywords->keywords[i];
               if (current) {
                  const char* currentName = current->keyword.Get();
                  for (UInt32 j = 0; j < nameCount; j++) {
                     BSFixedString name;
                     keywordNames.Get(&name, j);
                     if (_stricmp(currentName, name.data) == 0) // note: Not sure if BGSKeyword::keyword::Get() is null-terminated; if not, could cause issues
                        return j;
                  }
               }
            }
         }
//else _MESSAGE("Target form has no retrievable keyword list.");
         return -1;
      };
      VMResultArray<TESObjectREFR*> GetActivateParents(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectREFR* subject, TESForm* baseForm = nullptr) {
         VMResultArray<TESObjectREFR*> result;
         if (subject == nullptr) {
            registry->LogError("Cannot get the activate parents of an unspecified or None reference.", stackId);
            return result;
         }
         std::vector<TESObjectREFR*> parents = subject->GetActivateParents(baseForm);
         UInt32 size = parents.size();
         result.resize(size);
         for (UInt32 i = 0; i < size; i++)
            result[i] = parents[i];
         return result;
      };
      VMResultArray<SInt32> GetBounds(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* target) {
         VMResultArray<SInt32> result;
         result.resize(6, 0.0);
         //
         if (target == nullptr) {
            registry->LogError("You must specify a reference.", stackId);
            return result;
         }
         //
         RE::TESBoundObject* baseForm = (RE::TESBoundObject*) DYNAMIC_CAST(target->baseForm, TESForm, TESBoundObject);
         if (baseForm) {
            result[0] = baseForm->boundsMin.x;
            result[1] = baseForm->boundsMin.y;
            result[2] = baseForm->boundsMin.z;
            result[3] = baseForm->boundsMax.x;
            result[4] = baseForm->boundsMax.y;
            result[5] = baseForm->boundsMax.z;
         }
         return result;
      };
      VMResultArray<TESObjectREFR*> GetEnableStateChildren(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* parent) {
         VMResultArray<TESObjectREFR*> result;
         if (parent == nullptr) {
            registry->LogError("Cannot get the enable state children of an unspecified or None reference.", stackId);
            return result;
         }
         //
         RE::ExtraEnableStateChildren* data = (RE::ExtraEnableStateChildren*)(parent->extraData.GetByType(kExtraData_EnableStateChildren));
         if (!data)
            return result;
         {
            TESObjectREFR* current = nullptr;
            LookupREFRByHandle(&data->firstHandle, &current); // this is either a handle or flags; we should probably check that sometime
            if (current)
               result.push_back(current);
         }
         for (RE::ExtraEnableStateChildren::Entry* current = data->nextEntry; current && current != nullptr; current = current->nextEntry) {
            TESObjectREFR* reference = nullptr;
            LookupREFRByHandle(&current->refHandle, &reference);
            if (reference != nullptr)
               result.push_back(reference);
         }
         return result;
      };
      UInt32 GetMotionType(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectREFR* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot get motion type for a None reference.", registry, stackId);
         ERROR_AND_RETURN_0_IF(subject->GetNiNode() == nullptr, "Cannot get motion type for a reference with no 3D.", registry, stackId);
         return subject->GetMotionType();
      };
      VMResultArray<float> GetHeadingOffsetPosition(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, float headingDeg) {
         //
         // Given an ObjectReference and the player's facing direction (yaw angle), compute a position 
         // that we can move the player to such that:
         //
         //  - The player's facing direction does not change.
         //  - The player is facing directly toward the ObjectReference.
         //  - The player is near the ObjectReference.
         //  - The player is not inside the ObjectReference.
         //
         // Code is based on Atronach Crossing's Item.cpp/GetDropCoordinates(...) function.
         //
         VMResultArray<float> result;
         if (subject == nullptr) {
            registry->LogError("Can't calculate a heading-offset position for a None object.", stackId);
            return result;
         }
         result.resize(3);
         //
         float heading = cobb::degrees_to_radians(headingDeg);
         Bounds bounds;
         {
            auto obnd = (RE::TESBoundObject*) DYNAMIC_CAST(subject->baseForm, TESForm, TESBoundObject);
            if (obnd)
               bounds = Bounds(obnd->boundsMin.x, obnd->boundsMin.y, obnd->boundsMin.z, obnd->boundsMax.x, obnd->boundsMax.y, obnd->boundsMax.z);
            else {
               //
               // No bounds. Fall back to the old handling.
               //
               //registry->LogWarning("Can't calculate a heading-offset position for this reference. Its base form has no OBND data.", stackId);
               result[0] = subject->pos.x + sin(heading) * -320;
               result[1] = subject->pos.x + cos(heading) * -320;
               result[2] = 0;
               return result;
            }
         }
         NiPoint3 playerAxis;
         {
            playerAxis.x = sin(heading);
            playerAxis.y = cos(heading);
         }
         float distance = 256;
         {
            float length = bounds.lengthAlongAxis(playerAxis);
            if (length > 224)
               // distance = length * 1.25;
               distance = min(length * 1.5 + 512, length * (1 + (length - 256) / 256));
            //
            // Above formula caps length at roughly 512 units away from the edge of the object.
            // A concern to note: an object's apparent length can vary a lot if the user is 
            // looking at it diagonally (i.e. facing an AABB corner instead of an edge).
         }
         NiPoint3 position = subject->pos + (playerAxis * -distance);
         result[0] = position.x;
         result[1] = position.y;
         result[2] = position.z;
         return result;
      };
      bool IsMarker(::VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* target) {
         //
         // UNFINISHED. Checks if the given activator has its "Is Marker" flag set.
         //
         if (target == nullptr) {
            registry->LogError("Target parameter must not be None.", stackId);
            return false;
         }
         //
         TESForm* baseForm = (TESForm*)(target->baseForm);
         if (!baseForm)
            return false;
         return (baseForm->flags & 0x800000) != 0; // "Is Marker" flag, bit 23
      };
      bool IsCleanlyMovable(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectREFR* subject) {
         ERROR_AND_RETURN_0_IF(subject == nullptr, "Cannot check if a non-existent reference is cleanly movable.", registry, stackId);
         if (CALL_MEMBER_FN(subject, Subroutine004D9CF0)())
            return true;
         return false;
      };
   }
}

bool CobbPapyrus::ObjectReference::Register(VMClassRegistry* registry) {
   #define REGISTER_PAPYRUS_FUNCTION(className, funcName, argCount, returnType, func, ...) \
      registry->RegisterFunction( \
         new NativeFunction##argCount <StaticFunctionTag, returnType, __VA_ARGS__>( \
            funcName, \
            PapyrusPrefixString(className), \
            func, \
            registry \
         ) \
      );
   //
   // COLLISION PRIMITIVE FUNCTIONS
   //
   REGISTER_PAPYRUS_FUNCTION(
      "ObjectReference", "MakeCollisionPrimitiveBox",
      3, SInt32, ObjectReference::CollisionPrimitives::MakeCollisionPrimitiveBox,
      TESObjectREFR*, UInt32, VMArray<float>
   );
   REGISTER_PAPYRUS_FUNCTION(
      "ObjectReference", "GetBoundsToHalfwidths",
      1, VMResultArray<float>, ObjectReference::CollisionPrimitives::GetBoundsToHalfwidths,
      TESForm*
   );
   REGISTER_PAPYRUS_FUNCTION(
      "ObjectReference", "GetBoundsToHalfwidthOffsets",
      1, VMResultArray<float>, ObjectReference::CollisionPrimitives::GetBoundsToHalfwidthOffsets,
      TESForm*
   );
   //
   // COORDINATES FUNCTIONS
   //
   REGISTER_PAPYRUS_FUNCTION(
      "ObjectReference", "GetCoordinates",
      1, VMResultArray<float>, ObjectReference::Coordinates::GetCoordinates,
      TESObjectREFR*
   );
   REGISTER_PAPYRUS_FUNCTION(
      "ObjectReference", "GetPosition",
      1, VMResultArray<float>, ObjectReference::Coordinates::GetPosition,
      TESObjectREFR*
   );
   REGISTER_PAPYRUS_FUNCTION(
      "ObjectReference", "GetRotation",
      1, VMResultArray<float>, ObjectReference::Coordinates::GetRotation,
      TESObjectREFR*
   );
   //
   // FURNITURE FUNCTIONS
   //
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<SInt32>, TESObjectREFR*>(
         "GetFurnitureEnabledMarkers",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Furniture::GetFurnitureEnabledMarkers,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetFurnitureEnabledMarkers", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, VMResultArray<UInt32>, TESObjectREFR*, UInt32>(
         "GetFurnitureMarkerEntryPoints",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Furniture::GetFurnitureMarkerEntryPoints,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetFurnitureMarkerEntryPoints", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction3 <StaticFunctionTag, VMResultArray<float>, TESObjectREFR*, UInt32, bool>(
         "GetFurnitureMarkerCoordinates",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Furniture::GetFurnitureMarkerCoordinates,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetFurnitureMarkerCoordinates", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, UInt32, TESObjectREFR*, UInt32>(
         "GetFurnitureMarkerType",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Furniture::GetFurnitureMarkerType,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetFurnitureMarkerType", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, VMResultArray<float>, TESObjectREFR*, bool>(
         "GetFurnitureAllMarkerCoordinates",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Furniture::GetFurnitureAllMarkerCoordinates,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetFurnitureAllMarkerCoordinates", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<SInt32>, TESObjectREFR*>(
         "GetFurnitureAllMarkerEntryPoints",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Furniture::GetFurnitureAllMarkerEntryPoints,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetFurnitureAllMarkerEntryPoints", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<UInt32>, TESObjectREFR*>(
         "GetFurnitureAllMarkerTypes",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Furniture::GetFurnitureAllMarkerTypes,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetFurnitureAllMarkerTypes", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // INTERNALS FUNCTIONS
   //
   #ifdef COBB_PAPYRUS_OBJECTREFERENCE_CHANGEFORMS_USE_FUNCTOR
   registry->RegisterFunction(
      new LatentNativeFunction2<StaticFunctionTag, void, TESObjectREFR*, UInt32>(
         "MarkChanged",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Internals::MarkChanged,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction2<StaticFunctionTag, void, TESObjectREFR*, UInt32>(
         "UnmarkChanged",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Internals::UnmarkChanged,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, UInt32, TESObjectREFR*>(
         "GetChangeFlags",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Internals::GetChangeFlags,
         registry
      )
   );
   #endif
   #ifndef COBB_PAPYRUS_OBJECTREFERENCE_CHANGEFORMS_USE_FUNCTOR
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, void, TESObjectREFR*, UInt32>(
         "MarkChanged",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Internals::MarkChanged,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, void, RE::TESObjectREFR*, UInt32>(
         "UnmarkChanged",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Internals::UnmarkChanged,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, UInt32, RE::TESObjectREFR*>(
         "GetChangeFlags",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Internals::GetChangeFlags,
         registry
      )
   );
   #endif
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, bool, TESObjectREFR*, UInt32>(
         "HasExtraData",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Internals::HasExtraData,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "HasExtraData", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // LOAD DOOR FUNCTIONS
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>(
         "IsLoadDoor",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::LoadDoors::IsLoadDoor,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "IsLoadDoor", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*>(
         "IsTeleportMarkerInAttachedCell",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::LoadDoors::IsTeleportMarkerInAttachedCell,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, TESObjectREFR*, TESObjectREFR*>(
         "GetDestinationLoadDoor",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::LoadDoors::GetDestinationLoadDoor,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<float>, TESObjectREFR*>(
         "GetTeleportMarkerCoordinates",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::LoadDoors::GetTeleportMarkerCoordinates,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, void, TESObjectREFR*>(
         "MoveTeleportMarkerToEditorLocOffset",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::LoadDoors::MoveTeleportMarkerToEditorLocOffset,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, void, TESObjectREFR*>(
         "ResetTeleportMarker",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::LoadDoors::ResetTeleportMarker,
         registry
      )
   );
   //
   // LOADED3D FUNCTIONS
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>(
         "HasLocalCollision",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Loaded3D::HasLocalCollision,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>(
         "HasNiControllerManager",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Loaded3D::HasNiControllerManager,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, void, TESObjectREFR*>(
         "Reload3D",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Loaded3D::Reload3D,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, void, TESObjectREFR*>(
         "Unload3D",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::Loaded3D::Unload3D,
         registry
      )
   );
   //
   // OTHER FUNCTIONS
   //
   /*registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, TESObjectREFR*>(
         "GetActivateChildren",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::GetActivateChildren,
         registry
      )
   );*/
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, SInt32, TESObjectREFR*, VMArray<BGSKeyword*>>(
         "BaseHasWhichKeyword",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::BaseHasWhichKeyword,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "BaseHasWhichKeyword", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, SInt32, TESObjectREFR*, VMArray<BSFixedString>>(
         "BaseHasWhichKeywordName",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::BaseHasWhichKeywordName,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "BaseHasWhichKeywordName", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, RE::TESObjectREFR*, TESForm*>(
         "GetActivateParents",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::GetActivateParents,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<SInt32>, TESObjectREFR*>(
         "GetBounds",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::GetBounds,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetBounds", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, TESObjectREFR*>(
         "GetEnableStateChildren",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::GetEnableStateChildren,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, VMResultArray<float>, TESObjectREFR*, float>(
         "GetHeadingOffsetPosition",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::GetHeadingOffsetPosition,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "GetHeadingOffsetPosition", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, UInt32, RE::TESObjectREFR*>(
         "GetMotionType",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::GetMotionType,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, bool, TESObjectREFR*>(
         "IsMarker",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::IsMarker,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("ObjectReference"), "IsMarker", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, bool, RE::TESObjectREFR*>(
         "IsCleanlyMovable",
         PapyrusPrefixString("ObjectReference"),
         ObjectReference::IsCleanlyMovable,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
}