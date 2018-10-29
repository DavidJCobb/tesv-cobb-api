#pragma once
#include "Papyrus/SKSEDebug.h"
#include <cmath>
#include <cstdlib>

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameData.h"
#include "skse/GameExtraData.h"
#include "skse/GameForms.h"
#include "skse/GameObjects.h"

#include "Miscellaneous/math.h"

#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Forms/BaseForms/base.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Objects/BGSPrimitive.h"
#include "Services/TeleportMarkerService.h"
#include "Services/WeakReferenceService.h"

#include "Patches/PlaceableCollisionPrimitives.h"

namespace PapyrusPrefix(Papyrus) {
   namespace SKSEDebug {
      void EchoMemoryLocationOf(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESForm* subject) {
         ERROR_AND_RETURN_IF(subject == NULL, "Form does not exist.", registry, stackId);
         _MESSAGE("Form 0x%08X is located in memory at: 0x%08X.", subject->formID, (UInt32)subject);
      };
      void EchoMemoryLocationOf(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BGSBaseAlias* subject) {
         _MESSAGE("Alias is located in memory at: 0x%08X.", (UInt32)subject);
      };
      void RunTestOnReference(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject) {
         ERROR_AND_RETURN_IF(subject == NULL, "Reference does not exist.", registry, stackId);
         if (!(subject->flags & 0x800)) { // if the reference is enabled?
            //
            // The below is a simplified version of what Bethesda does under the hood.
            //
            ((RE::TESObjectREFR*)subject)->Unk_6C(0, 0);
            subject->flags |= 0x800; // no bloody way...
            CALL_MEMBER_FN((RE::TESObjectREFR*)subject, Subroutine004E0E30)(); // enable it again, indirectly reloading 3D in the process? this is what MoveRefrToPosition does
         }
      };
      void RunTestOnForm(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESForm* subject) {
         ERROR_AND_RETURN_IF(subject == NULL, "Form does not exist.", registry, stackId);
         if (subject->formType == FormType::kFormType_Light) {
            _MESSAGE("Form 0x%08X is located in memory at: 0x%08X.", subject->formID, (UInt32)subject);
         }
      };
      void AttemptToMoveTeleportMarker(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, VMArray<float> coords) {
         if (subject == NULL) {
            registry->LogError("Cannot get teleport marker coordinates for an unspecified or None reference.", stackId);
            return;
         }
         if (!subject->extraData.HasType(kExtraData_Teleport)) {
            registry->LogError("The specified reference is not a load door.", stackId);
            return;
         }
         RE::ExtraTeleport::TeleportData* data = CALL_MEMBER_FN(((RE::BaseExtraList*)&(subject->extraData)), GetExtraTeleportData)();
         if (!data) {
            registry->LogError("Unable to access teleport data on the specified reference.", stackId);
            return;
         }
         NiPoint3 pos;
         NiPoint3 rot;
         coords.Get(&pos.x, 0);
         coords.Get(&pos.y, 1);
         coords.Get(&pos.z, 2);
         coords.Get(&rot.x, 3);
         coords.Get(&rot.y, 4);
         coords.Get(&rot.z, 5);
         rot.x = cobb::degrees_to_radians(rot.x);
         rot.y = cobb::degrees_to_radians(rot.y);
         rot.z = cobb::degrees_to_radians(rot.z);
         //data->markerPosition = pos;
         //data->markerRotation = rot;

         bool success = TeleportMarkerService::GetInstance().MoveMarker(subject, pos, rot);
         if (!success) {
            _MESSAGE("TeleportMarkerService MoveMarker operation failed!");
            registry->LogError("TeleportMarkerService MoveMarker operation failed!", stackId);
            return;
         }
      };
      void DumpWeakReferenceInfo(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         #ifdef _DEBUG
         WeakReferenceService::GetInstance().DebugLogAllData();
         #endif
      };
      //
      VMResultArray<float> ArraySliceFloat(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<float> arr, SInt32 start, SInt32 count) {
         VMResultArray<float> result;
         result.resize(count);
         UInt32 length = arr.Length();
         for (UInt32 i = 0; i < count; i++) {
            if (start + i >= length)
               break;
            arr.Get(&(result[i]), start + i);
         };
         return result;
      };
      VMResultArray<TESObjectREFR*> ArrayWriteReference(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<TESObjectREFR*> target, VMArray<TESObjectREFR*> source, SInt32 start) {
         VMResultArray<TESObjectREFR*> result;
         UInt32 length = target.Length();
         UInt32 count = source.Length();
         result.resize(length);
         for (UInt32 i = 0; i < length; i++) {
            if (i >= start && i - start < count)
               source.Get(&(result[i]), i - start);
            else
               target.Get(&(result[i]), i);
         }
         return result;
      };
      void ArrayWriteReferenceNOCOPY(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<TESObjectREFR*> target, VMArray<TESObjectREFR*> source, SInt32 start) {
         UInt32 length = target.Length();
         UInt32 count = source.Length();
         for (UInt32 i = start; i < count; i++) {
            if (i >= length)
               break;
            TESObjectREFR* item;
            source.Get(&item, i - start);
            target.Set(&item, i);
         }
      };
      SInt32 Round(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, float value) {
         return round(value);
      };
      VMResultArray<float> ComputeTransformationMatrix(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<float> afRotation
      ) {
         VMResultArray<float> result;
         result.resize(9);
         if (afRotation.Length() < 3) {
            registry->LogError("afRotation must have three or more elements (extras will be ignored).", stackId);
            return result;
         };
         float fCos[3];
         float fSin[3];
         for (UInt8 i = 0; i < 3; i++) {
            float temp;
            afRotation.Get(&temp, i);
            fSin[i] = sin(temp * cobb::pi / 180.0F);
            fCos[i] = cos(temp * cobb::pi / 180.0F);
         }
         /* PAPYRUS:
            _fEditingSnapTransformToLocal[0] = fCosY * fCosZ
            _fEditingSnapTransformToLocal[1] = fCosY * fSinZ
            _fEditingSnapTransformToLocal[2] = -fSinY
            _fEditingSnapTransformToLocal[3] = fSinX * fSinY * fCosZ - fCosX * fSinZ
            _fEditingSnapTransformToLocal[4] = fSinX * fSinY * fSinZ + fCosX * fCosZ
            _fEditingSnapTransformToLocal[5] = fSinX * fCosY
            _fEditingSnapTransformToLocal[6] = fCosX * fSinY * fCosZ + fSinX * fSinZ
            _fEditingSnapTransformToLocal[7] = fCosX * fSinY * fSinZ - fSinX * fCosZ
            _fEditingSnapTransformToLocal[8] = fCosX * fCosY
         */
         //
         // We actually need the transpose of the above, perhaps because we're in a left-handed rotation 
         // system; hence the weird index order below.
         //
         result[0] = fCos[1] * fCos[2];
         result[3] = fCos[1] * fSin[2];
         result[6] = -fSin[1];
         result[1] = fSin[0] * fSin[1] * fCos[2] - fCos[0] * fSin[2];
         result[4] = fSin[0] * fSin[1] * fSin[2] + fCos[0] * fCos[2];
         result[7] = fSin[0] * fCos[1];
         result[2] = fCos[0] * fSin[1] * fCos[2] + fSin[0] * fSin[2];
         result[5] = fCos[0] * fSin[1] * fSin[2] - fSin[0] * fCos[2];
         result[8] = fCos[0] * fCos[1];
         return result;
      };
      VMResultArray<float> ApplyTransformationMatrix(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<float> afPosition,
         VMArray<float> afMatrix
      ) {
         VMResultArray<float> result;
         result.resize(3);
         if (afPosition.Length() < 3) {
            registry->LogError("afPosition must have three or more elements (extras will be ignored).", stackId);
            return result;
         };
         if (afMatrix.Length() < 9) {
            registry->LogError("afMatrix must have nine or more elements (extras will be ignored).", stackId);
            return result;
         };
         float position[3];
         float matrix[9];
         for (UInt8 i = 0; i < 3; i++)
            afPosition.Get(&position[i], i);
         for (UInt8 i = 0; i < 9; i++)
            afMatrix.Get(&matrix[i], i);
         //
         result[0] = (matrix[0] * position[0]) + (matrix[1] * position[1]) + (matrix[2] * position[2]);
         result[1] = (matrix[3] * position[0]) + (matrix[4] * position[1]) + (matrix[5] * position[2]);
         result[2] = (matrix[6] * position[0]) + (matrix[7] * position[1]) + (matrix[8] * position[2]);
         /* PAPYRUS:
            _fEditingSnapPositionLocal[0] = _fEditingSnapTransformToLocal[0]*_fEditingSnapPosition[0] + _fEditingSnapTransformToLocal[1]*_fEditingSnapPosition[1] + _fEditingSnapTransformToLocal[2]*_fEditingSnapPosition[2]
            _fEditingSnapPositionLocal[1] = _fEditingSnapTransformToLocal[3]*_fEditingSnapPosition[0] + _fEditingSnapTransformToLocal[4]*_fEditingSnapPosition[1] + _fEditingSnapTransformToLocal[5]*_fEditingSnapPosition[2]
            _fEditingSnapPositionLocal[2] = _fEditingSnapTransformToLocal[6]*_fEditingSnapPosition[0] + _fEditingSnapTransformToLocal[7]*_fEditingSnapPosition[1] + _fEditingSnapTransformToLocal[8]*_fEditingSnapPosition[2]
         */
         return result;
      };
      VMResultArray<float> ApplySnappedOffsetToPosition(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<float> afBasePosition,
         VMArray<float> afSnapPosition,
         VMArray<float> afPositionChange,
         VMArray<float> matrix,
         SInt32 snap
      ) {
         VMResultArray<float> result;
         result.resize(3);
         if (afBasePosition.Length() < 3) {
            registry->LogError("afBasePosition must have three or more elements (extras will be ignored).", stackId);
            return result;
         };
         if (afSnapPosition.Length() < 3) {
            registry->LogError("afSnapPosition must have three or more elements (extras will be ignored).", stackId);
            return result;
         };
         if (afPositionChange.Length() < 3) {
            registry->LogError("afPositionChange must have three or more elements (extras will be ignored).", stackId);
            return result;
         };
         if (matrix.Length() < 9) {
            registry->LogError("afMatrixTransformToLocal must have nine or more elements (extras will be ignored).", stackId);
            return result;
         };
         if (snap < 0)
            snap = -snap;
         //
         float frame[9];
         float local[3];
         float offsets[3];
         for (UInt8 i = 0; i < 9;i++)
            matrix.Get(&frame[i], i);
         {  // Convert (base) to the reference frame established by (matrix).
            float base[3];
            for (UInt8 i = 0; i < 3; i++)
               afBasePosition.Get(&base[i], i);
            local[0] = (frame[0] * base[0]) + (frame[1] * base[1]) + (frame[2] * base[2]);
            local[1] = (frame[3] * base[0]) + (frame[4] * base[1]) + (frame[5] * base[2]);
            local[2] = (frame[6] * base[0]) + (frame[7] * base[1]) + (frame[8] * base[2]);
         }
         if (snap != 0) {  // Carry out initial computations.
            for (UInt32 i = 0; i < 3; i++) {
               {  // Get offsets.
                  // Papyrus: fOffsets[0] = (_fEditingSnapPositionLocal[i] as int) % _iCurrentPositionSnap + (_fEditingSnapPositionLocal[i] - (_fEditingSnapPositionLocal[i] as int))
                  float snapPosDec = 0;
                  afSnapPosition.Get(&snapPosDec, i);
                  offsets[i] = ((SInt32)snapPosDec % snap) + (snapPosDec - (SInt32)snapPosDec);
               }
               local[i] -= offsets[i];
               {  // Snap.
                  // Papyrus: vLocal[i] = Round(vLocal[i] / _iCurrentPositionSnap) * _iCurrentPositionSnap + (fPosition[i] * _iCurrentPositionSnap)
                  float change = 0;
                  afPositionChange.Get(&change, i);
                  local[i] = (round(local[i] / snap) * snap) + (change * snap);
               }
_MESSAGE("fOffset[%d] == %f", i, offsets[i]);
               local[i] += offsets[i];
            }
         } else {
            for (UInt32 i = 0; i < 3; i++) {
               float change = 0;
               afPositionChange.Get(&change, i);
               local[i] += change;
            }
         }
         _MESSAGE("vLocal[0] == %f", local[0]);
         _MESSAGE("vLocal[1] == %f", local[1]);
         _MESSAGE("vLocal[2] == %f", local[2]);
         {  // Convert back to the world reference frame.
            result[0] = (frame[0] * local[0]) + (frame[3] * local[1]) + (frame[6] * local[2]);
            result[1] = (frame[1] * local[0]) + (frame[4] * local[1]) + (frame[7] * local[2]);
            result[2] = (frame[2] * local[0]) + (frame[5] * local[1]) + (frame[8] * local[2]);
         }
         return result;
      };
      SInt32 AddNavcutByBounds(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, TESObjectREFR* subject, TESForm* boundsSurrogate) {
         ERROR_AND_RETURN_0_IF(subject == NULL, "Reference does not exist.", registry, stackId);
         ERROR_AND_RETURN_0_IF(boundsSurrogate == NULL, "Bounds form does not exist.", registry, stackId);
         {  // Validate base form.
            auto baseForm = subject->baseForm;
            ERROR_AND_RETURN_0_IF(baseForm == NULL, "Reference has no identifiable base form.", registry, stackId);
            ERROR_AND_RETURN_0_IF(baseForm->formID != g_PlaceableCollisionPrimitiveFormID, "Reference base form ID must be 0x00C0BB00.", registry, stackId);
         }
         RE::TESBoundObject* boundObject = (RE::TESBoundObject*) DYNAMIC_CAST(boundsSurrogate, TESForm, TESBoundObject);
         ERROR_AND_RETURN_0_IF(boundObject == NULL, "Bounds form is a bad type.", registry, stackId);
         //
         subject->handleRefObject.IncRef();
         SInt32 result = 0;
         if (subject->extraData.HasType(kExtraData_Primitive) == false) {
            if (subject->extraData.HasType(kExtraData_CollisionData) == false) {
               auto extra_p = RE::ExtraPrimitive::Create();
               auto extra_c = RE::ExtraCollisionData::Create();
               extra_c->data->collisionLayer = 49; // NAVCUT
               {  // Fill primitive data.
                  //    There are some complications here stemming from the fact that primitives are centered 
                  //    on the reference origin, whereas bounds may not be. For now, we just use the minimum 
                  //    bound: for asymmetrical bounds, we won't take up any extra space, though there is the 
                  //    risk of not taking up as much space as we ought to.
                  //
                  //    We can refine this in the future -- for example, add an Interface script allowing 
                  //    sphere primitives to be specified, in which case only the X-value of our halfwidth 
                  //    NiPoint3 will be used (as a radius).
                  //
                  NiPoint3 sizes;
                  sizes.x = min(abs(boundObject->boundsMax.x), abs(boundObject->boundsMin.x));
                  sizes.y = min(abs(boundObject->boundsMax.y), abs(boundObject->boundsMin.y));
                  sizes.z = min(abs(boundObject->boundsMax.z), abs(boundObject->boundsMin.z));
                  //
                  RE::BGSPrimitiveBox* box = RE::BGSPrimitiveBox::Create(&sizes);
                  extra_p->primitive = (BGSPrimitive*) box;
               }
               subject->extraData.Add(kExtraData_Primitive, extra_p);
               subject->extraData.Add(kExtraData_CollisionData, extra_c);
               result = 1;
            }
         }
         subject->handleRefObject.DecRef();
         return result;
      };
   }
}

bool PapyrusPrefix(Papyrus)::SKSEDebug::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, TESForm*>(
         "EchoMemoryLocationOf",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::EchoMemoryLocationOf,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, BGSBaseAlias*>(
         "EchoMemoryLocationOfAlias",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::EchoMemoryLocationOf,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, TESForm*>(
         "RunTestOnForm",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::RunTestOnForm,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, void, TESObjectREFR*>(
         "RunTestOnReference",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::RunTestOnReference,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, void, TESObjectREFR*, VMArray<float>>(
         "AttemptToMoveTeleportMarker",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::AttemptToMoveTeleportMarker,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, void>(
         "DumpWeakReferenceInfo",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::DumpWeakReferenceInfo,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, SInt32, TESObjectREFR*, TESForm*>(
         "AddNavcutByBounds",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::AddNavcutByBounds,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction3 <StaticFunctionTag, VMResultArray<float>, VMArray<float>, SInt32, SInt32>(
         "ArraySliceFloat",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::ArraySliceFloat,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("SKSEDebug"), "ArraySliceFloat", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction3 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, SInt32>(
         "ArrayWriteReference",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::ArrayWriteReference,
         registry
         )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("SKSEDebug"), "ArrayWriteReference", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction3 <StaticFunctionTag, void, VMArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, SInt32>(
         "ArrayWriteReferenceNOCOPY",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::ArrayWriteReferenceNOCOPY,
         registry
         )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("SKSEDebug"), "ArrayWriteReferenceNOCOPY", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, SInt32, float>(
         "Round",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::Round,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("SKSEDebug"), "Round", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<float>, VMArray<float>>(
         "ComputeTransformationMatrix",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::ComputeTransformationMatrix,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("SKSEDebug"), "ComputeTransformationMatrix", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, VMResultArray<float>, VMArray<float>, VMArray<float>>(
         "ApplyTransformationMatrix",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::ApplyTransformationMatrix,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("SKSEDebug"), "ApplyTransformationMatrix", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction5 <StaticFunctionTag, VMResultArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, SInt32>(
         "ApplySnappedOffsetToPosition",
         PapyrusPrefixString("SKSEDebug"),
         SKSEDebug::ApplySnappedOffsetToPosition,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("SKSEDebug"), "ApplySnappedOffsetToPosition", VMClassRegistry::kFunctionFlag_NoWait);
   return true;
};