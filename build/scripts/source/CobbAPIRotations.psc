Scriptname CobbAPIRotations Hidden

Float[] Function GetRelativeCoordinates   (Float[] afParentPosition, Float[] afParentRotation, Float[] afTargetPosition, Float[] afTargetRotation) Global Native
Float[] Function ApplyRelativeCoordinates (Float[] afParentPosition, Float[] afParentRotation, Float[] afOffsetPosition, Float[] afOffsetRotation) Global Native

; --------------

Float[] Function ComputeTransformationMatrix (Float[] afRotation) Global Native
Float[] Function ApplyTransformationMatrix   (Float[] afPosition, Float[] afMatrix) Global Native

; --------------

; afBasePosition:           The starting position.
; afSnapPosition:           The position of the snap target, converted to its own reference frame.
; afPositionChange:         The change in position to apply.
; afMatrixTransformToLocal: A matrix that can be used to convert to the snap target's reference frame.
; aiSnapUnits:              Snap interval.

Float[] Function ApplySnappedOffsetToPosition(Float[] afBasePosition, Float[] afSnapPosition, Float[] afPositionChange, Float[] afMatrixTransformToLocal, Int aiSnapUnits) Global Native

;/ The above function is equivalent to:

   Float[] vLocal = new Float[3]
   vLocal[0] = afMatrixTransformToLocal[0]*afBasePosition[0] + afMatrixTransformToLocal[1]*afBasePosition[1] + afMatrixTransformToLocal[2]*afBasePosition[2]
   vLocal[1] = afMatrixTransformToLocal[3]*afBasePosition[0] + afMatrixTransformToLocal[4]*afBasePosition[1] + afMatrixTransformToLocal[5]*afBasePosition[2]
   vLocal[2] = afMatrixTransformToLocal[6]*afBasePosition[0] + afMatrixTransformToLocal[7]*afBasePosition[1] + afMatrixTransformToLocal[8]*afBasePosition[2]
   If aiSnapUnits
      ;
      ; Now snap the thing.
      ;
      ; The offset variable pretty much offsets the grid we're snapping to, to 
      ; match the position of the snap target. We'd normally just use a simple 
      ; modulus (pos local % pos snap), but Papyrus doesn't support modulus on 
      ; non-integers.
      ;
      Float[] fOffsets = new Float[3]
      fOffsets[0] = (afSnapPosition[0] as int) % aiSnapUnits + (afSnapPosition[0] - (afSnapPosition[0] as int))
      fOffsets[1] = (afSnapPosition[1] as int) % aiSnapUnits + (afSnapPosition[1] - (afSnapPosition[1] as int))
      fOffsets[2] = (afSnapPosition[2] as int) % aiSnapUnits + (afSnapPosition[2] - (afSnapPosition[2] as int))
      vLocal[0] = vLocal[0] - fOffsets[0]
      vLocal[1] = vLocal[1] - fOffsets[1]
      vLocal[2] = vLocal[2] - fOffsets[2]
      vLocal[0] = Round(vLocal[0] / aiSnapUnits) * aiSnapUnits + (fPosition[0] * aiSnapUnits)
      vLocal[1] = Round(vLocal[1] / aiSnapUnits) * aiSnapUnits + (fPosition[1] * aiSnapUnits)
      vLocal[2] = Round(vLocal[2] / aiSnapUnits) * aiSnapUnits + (fPosition[2] * aiSnapUnits)
      vLocal[0] = vLocal[0] + fOffsets[0]
      vLocal[1] = vLocal[1] + fOffsets[1]
      vLocal[2] = vLocal[2] + fOffsets[2]
   Else
      vLocal[0] = vLocal[0] + fPosition[0]
      vLocal[1] = vLocal[1] + fPosition[1]
      vLocal[2] = vLocal[2] + fPosition[2]
   EndIf
   ;
   ; Transform back, by multiplying by the transpose.
   ;
   fReturnValue[0] = afMatrixTransformToLocal[0]*vLocal[0] + afMatrixTransformToLocal[3]*vLocal[1] + afMatrixTransformToLocal[6]*vLocal[2]
   fReturnValue[1] = afMatrixTransformToLocal[1]*vLocal[0] + afMatrixTransformToLocal[4]*vLocal[1] + afMatrixTransformToLocal[7]*vLocal[2]
   fReturnValue[2] = afMatrixTransformToLocal[2]*vLocal[0] + afMatrixTransformToLocal[5]*vLocal[1] + afMatrixTransformToLocal[8]*vLocal[2]
/;