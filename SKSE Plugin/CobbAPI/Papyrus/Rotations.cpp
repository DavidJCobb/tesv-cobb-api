#pragma once
#include "Papyrus/Rotations.h"

#include <cmath>

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "Miscellaneous/math.h"
#include "CobbRotation.h"
#include "Shared.h"

#include "skse/NiTypes.h"

#define ERROR_AND_RETURN_EMPTY_ARRAY6_IF(condition, message, registry, stackId) \
   if (condition) {                         \
      registry->LogError(message, stackId);	\
      VMResultArray<float> dummy;           \
      dummy.resize(6.0, 0);                 \
      return dummy;							\
   }

namespace PapyrusPrefix(Papyrus) {
   namespace Rotations {
      VMResultArray<float> GetRelativeCoordinates(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* base, VMArray<float> afParentPosition, VMArray<float> afParentRotation, VMArray<float> afTargetPosition, VMArray<float> afTargetRotation) {
         ERROR_AND_RETURN_EMPTY_ARRAY6_IF(afParentPosition.Length() != 3, "afParentPosition is not a valid set of coordinates; it has the wrong length.", registry, stackId);
         ERROR_AND_RETURN_EMPTY_ARRAY6_IF(afParentPosition.Length() != 3, "afParentRotation is not a valid set of Euler angles; it has the wrong length.", registry, stackId);
         ERROR_AND_RETURN_EMPTY_ARRAY6_IF(afTargetPosition.Length() != 3, "afTargetPosition is not a valid set of coordinates; it has the wrong length.", registry, stackId);
         ERROR_AND_RETURN_EMPTY_ARRAY6_IF(afTargetPosition.Length() != 3, "afTargetRotation is not a valid set of Euler angles; it has the wrong length.", registry, stackId);
         //
         NiPoint3 parentPos;
         NiPoint3 parentRot;
         NiPoint3 offsetPos;
         NiPoint3 offsetRot;
         afParentPosition.Get(&parentPos.x, 0);
         afParentPosition.Get(&parentPos.y, 1);
         afParentPosition.Get(&parentPos.z, 2);
         afParentRotation.Get(&parentRot.x, 0);
         afParentRotation.Get(&parentRot.y, 1);
         afParentRotation.Get(&parentRot.z, 2);
         afTargetPosition.Get(&offsetPos.x, 0);
         afTargetPosition.Get(&offsetPos.y, 1);
         afTargetPosition.Get(&offsetPos.z, 2);
         afTargetRotation.Get(&offsetRot.x, 0);
         afTargetRotation.Get(&offsetRot.y, 1);
         afTargetRotation.Get(&offsetRot.z, 2);
         //
         Cobb::Coordinates out;
         Cobb::GetRelativeCoordinates(&out, &parentPos, &parentRot, &offsetPos, &offsetRot, false, false);
         out.rot.ConvertToDegrees();
         //
         VMResultArray<float> result;
         result.resize(6);
         result[0] = out.pos.x;
         result[1] = out.pos.y;
         result[2] = out.pos.z;
         result[3] = out.rot.x;
         result[4] = out.rot.y;
         result[5] = out.rot.z;
         return result;
      };
      VMResultArray<float> ApplyRelativeCoordinates(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* base, VMArray<float> afParentPosition, VMArray<float> afParentRotation, VMArray<float> afOffsetPosition, VMArray<float> afOffsetRotation) {
         ERROR_AND_RETURN_EMPTY_ARRAY6_IF(afParentPosition.Length() != 3, "afParentPosition is not a valid set of coordinates; it has the wrong length.", registry, stackId);
         ERROR_AND_RETURN_EMPTY_ARRAY6_IF(afParentPosition.Length() != 3, "afParentRotation is not a valid set of Euler angles; it has the wrong length.", registry, stackId);
         ERROR_AND_RETURN_EMPTY_ARRAY6_IF(afOffsetPosition.Length() != 3, "afOffsetPosition is not a valid set of coordinates; it has the wrong length.", registry, stackId);
         ERROR_AND_RETURN_EMPTY_ARRAY6_IF(afOffsetPosition.Length() != 3, "afOffsetRotation is not a valid set of Euler angles; it has the wrong length.", registry, stackId);
         //
         NiPoint3 parentPos;
         NiPoint3 parentRot;
         NiPoint3 offsetPos;
         NiPoint3 offsetRot;
         afParentPosition.Get(&parentPos.x, 0);
         afParentPosition.Get(&parentPos.y, 1);
         afParentPosition.Get(&parentPos.z, 2);
         afParentRotation.Get(&parentRot.x, 0);
         afParentRotation.Get(&parentRot.y, 1);
         afParentRotation.Get(&parentRot.z, 2);
         afOffsetPosition.Get(&offsetPos.x, 0);
         afOffsetPosition.Get(&offsetPos.y, 1);
         afOffsetPosition.Get(&offsetPos.z, 2);
         afOffsetRotation.Get(&offsetRot.x, 0);
         afOffsetRotation.Get(&offsetRot.y, 1);
         afOffsetRotation.Get(&offsetRot.z, 2);
         //
         Cobb::Coordinates out;
         Cobb::ApplyRelativeCoordinates(&out, &parentPos, &parentRot, &offsetPos, &offsetRot, false, false);
         out.rot.ConvertToDegrees();
         //
         VMResultArray<float> result;
         result.resize(6);
         result[0] = out.pos.x;
         result[1] = out.pos.y;
         result[2] = out.pos.z;
         result[3] = out.rot.x;
         result[4] = out.rot.y;
         result[5] = out.rot.z;
         return result;
      };
      //
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
            fSin[i] = sin(temp * cobb::pi / 180.0);
            fCos[i] = cos(temp * cobb::pi / 180.0);
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
               local[i] += offsets[i];
            }
         } else {
            for (UInt32 i = 0; i < 3; i++) {
               float change = 0;
               afPositionChange.Get(&change, i);
               local[i] += change;
            }
         }
         {  // Convert back to the world reference frame.
            result[0] = (frame[0] * local[0]) + (frame[3] * local[1]) + (frame[6] * local[2]);
            result[1] = (frame[1] * local[0]) + (frame[4] * local[1]) + (frame[7] * local[2]);
            result[2] = (frame[2] * local[0]) + (frame[5] * local[1]) + (frame[8] * local[2]);
         }
         return result;
      };
   }
};

bool PapyrusPrefix(Papyrus)::Rotations::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, VMResultArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<float>>(
         "GetRelativeCoordinates",
         PapyrusPrefixString("Rotations"),
         Rotations::GetRelativeCoordinates,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Rotations"), "GetRelativeCoordinates", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, VMResultArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<float>>(
         "ApplyRelativeCoordinates",
         PapyrusPrefixString("Rotations"),
         Rotations::ApplyRelativeCoordinates,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Rotations"), "ApplyRelativeCoordinates", VMClassRegistry::kFunctionFlag_NoWait);
   //
   registry->RegisterFunction(
      new NativeFunction1 <StaticFunctionTag, VMResultArray<float>, VMArray<float>>(
         "ComputeTransformationMatrix",
         PapyrusPrefixString("Rotations"),
         Rotations::ComputeTransformationMatrix,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Rotations"), "ComputeTransformationMatrix", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2 <StaticFunctionTag, VMResultArray<float>, VMArray<float>, VMArray<float>>(
         "ApplyTransformationMatrix",
         PapyrusPrefixString("Rotations"),
         Rotations::ApplyTransformationMatrix,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Rotations"), "ApplyTransformationMatrix", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction5 <StaticFunctionTag, VMResultArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<float>, SInt32>(
         "ApplySnappedOffsetToPosition",
         PapyrusPrefixString("Rotations"),
         Rotations::ApplySnappedOffsetToPosition,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Rotations"), "ApplySnappedOffsetToPosition", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // DONE.
   //
   return true;
};