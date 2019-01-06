Scriptname CobbAPITestSuiteExecutorRotations extends CobbAPITestSuiteIExecutor Hidden

;/Block/; ; Because subclassing is broken and I need to reproduce the functions here, apparently
   CobbAPITestSuiteExecutorState Function GetExecutorState()
      Return Quest.GetQuest("CobbAPITestSuiteExecutorState") as CobbAPITestSuiteExecutorState
   EndFunction
   Bool Function MethodIsMemberFunction()
      Return False
   EndFunction
   String Function GetMethodReturnType()
      Return ""
   EndFunction
   Int Function GetMethodParameterCount()
      Return 0
   EndFunction
   String[] Function GetMethodParameterNames()
      Return new String[1]
   EndFunction
   String[] Function GetMethodParameterTypes()
      Return new String[1]
   EndFunction
   Function SetMethodDefaultParameters()
   EndFunction
   Function CallMethod()
   EndFunction
;/EndBlock/;

String Function GetClassName()
   Return "Rotations"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[5]
   sResult[0] = "ApplyRelativeCoordinates"
   sResult[1] = "ApplySnappedOffsetToPosition"
   sResult[2] = "ApplyTransformationMatrix"
   sResult[3] = "ComputeTransformationMatrix"
   sResult[4] = "GetRelativeCoordinates"
   Return sResult
EndFunction

State Method_ApplyRelativeCoordinates
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 12
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "afParentPosition[0]"
      sNames[1] = "afParentPosition[1]"
      sNames[2] = "afParentPosition[2]"
      sNames[3] = "afParentRotation[0]"
      sNames[4] = "afParentRotation[1]"
      sNames[5] = "afParentRotation[2]"
      sNames[6] = "afOffsetPosition[0]"
      sNames[7] = "afOffsetPosition[1]"
      sNames[8] = "afOffsetPosition[2]"
      sNames[9] = "afOffsetRotation[0]"
      sNames[10] = "afOffsetRotation[1]"
      sNames[11] = "afOffsetRotation[2]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      sTypes[5] = "Float"
      sTypes[6] = "Float"
      sTypes[7] = "Float"
      sTypes[8] = "Float"
      sTypes[9] = "Float"
      sTypes[10] = "Float"
      sTypes[11] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afParentPosition = new Float[3]
      afParentPosition[0] = s.pfParameters[0]
      afParentPosition[1] = s.pfParameters[1]
      afParentPosition[2] = s.pfParameters[2]
      Float[] afParentRotation = new Float[3]
      afParentRotation[0] = s.pfParameters[3]
      afParentRotation[1] = s.pfParameters[4]
      afParentRotation[2] = s.pfParameters[5]
      Float[] afOffsetPosition = new Float[3]
      afOffsetPosition[0] = s.pfParameters[6]
      afOffsetPosition[1] = s.pfParameters[7]
      afOffsetPosition[2] = s.pfParameters[8]
      Float[] afOffsetRotation = new Float[3]
      afOffsetRotation[0] = s.pfParameters[9]
      afOffsetRotation[1] = s.pfParameters[10]
      afOffsetRotation[2] = s.pfParameters[11]
      s.pfReturnArray = CobbAPIRotations.ApplyRelativeCoordinates(afParentPosition, afParentRotation, afOffsetPosition, afOffsetRotation)
   EndFunction
EndState
State Method_ApplySnappedOffsetToPosition
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 19
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "afBasePosition[0]"
      sNames[1] = "afBasePosition[1]"
      sNames[2] = "afBasePosition[2]"
      sNames[3] = "afSnapPosition[0]"
      sNames[4] = "afSnapPosition[1]"
      sNames[5] = "afSnapPosition[2]"
      sNames[6] = "afPositionChange[0]"
      sNames[7] = "afPositionChange[1]"
      sNames[8] = "afPositionChange[2]"
      sNames[9] = "afMatrixTransformToLocal[0]"
      sNames[10] = "afMatrixTransformToLocal[1]"
      sNames[11] = "afMatrixTransformToLocal[2]"
      sNames[12] = "afMatrixTransformToLocal[3]"
      sNames[13] = "afMatrixTransformToLocal[4]"
      sNames[14] = "afMatrixTransformToLocal[5]"
      sNames[15] = "afMatrixTransformToLocal[6]"
      sNames[16] = "afMatrixTransformToLocal[7]"
      sNames[17] = "afMatrixTransformToLocal[8]"
      sNames[18] = "aiSnapUnits"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      sTypes[5] = "Float"
      sTypes[6] = "Float"
      sTypes[7] = "Float"
      sTypes[8] = "Float"
      sTypes[9] = "Float"
      sTypes[10] = "Float"
      sTypes[11] = "Float"
      sTypes[12] = "Float"
      sTypes[13] = "Float"
      sTypes[14] = "Float"
      sTypes[15] = "Float"
      sTypes[16] = "Float"
      sTypes[17] = "Float"
      sTypes[18] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afBasePosition = new Float[3]
      afBasePosition[0] = s.pfParameters[0]
      afBasePosition[1] = s.pfParameters[1]
      afBasePosition[2] = s.pfParameters[2]
      Float[] afSnapPosition = new Float[3]
      afSnapPosition[0] = s.pfParameters[3]
      afSnapPosition[1] = s.pfParameters[4]
      afSnapPosition[2] = s.pfParameters[5]
      Float[] afPositionChange = new Float[3]
      afPositionChange[0] = s.pfParameters[6]
      afPositionChange[1] = s.pfParameters[7]
      afPositionChange[2] = s.pfParameters[8]
      Float[] afMatrixTransformToLocal = new Float[9]
      afMatrixTransformToLocal[0] = s.pfParameters[9]
      afMatrixTransformToLocal[1] = s.pfParameters[10]
      afMatrixTransformToLocal[2] = s.pfParameters[11]
      afMatrixTransformToLocal[3] = s.pfParameters[12]
      afMatrixTransformToLocal[4] = s.pfParameters[13]
      afMatrixTransformToLocal[5] = s.pfParameters[14]
      afMatrixTransformToLocal[6] = s.pfParameters[15]
      afMatrixTransformToLocal[7] = s.pfParameters[16]
      afMatrixTransformToLocal[8] = s.pfParameters[17]
      s.pfReturnArray = CobbAPIRotations.ApplySnappedOffsetToPosition(afBasePosition, afSnapPosition, afPositionChange, afMatrixTransformToLocal, s.piParameters[18])
   EndFunction
EndState
State Method_ApplyTransformationMatrix
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 12
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "afPosition[0]"
      sNames[1] = "afPosition[1]"
      sNames[2] = "afPosition[2]"
      sNames[3] = "afMatrix[0]"
      sNames[4] = "afMatrix[1]"
      sNames[5] = "afMatrix[2]"
      sNames[6] = "afMatrix[3]"
      sNames[7] = "afMatrix[4]"
      sNames[8] = "afMatrix[5]"
      sNames[9] = "afMatrix[6]"
      sNames[10] = "afMatrix[7]"
      sNames[11] = "afMatrix[8]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      sTypes[5] = "Float"
      sTypes[6] = "Float"
      sTypes[7] = "Float"
      sTypes[8] = "Float"
      sTypes[9] = "Float"
      sTypes[10] = "Float"
      sTypes[11] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afPosition = new Float[3]
      afPosition[0] = s.pfParameters[0]
      afPosition[1] = s.pfParameters[1]
      afPosition[2] = s.pfParameters[2]
      Float[] afMatrix = new Float[9]
      afMatrix[0] = s.pfParameters[3]
      afMatrix[1] = s.pfParameters[4]
      afMatrix[2] = s.pfParameters[5]
      afMatrix[3] = s.pfParameters[6]
      afMatrix[4] = s.pfParameters[7]
      afMatrix[5] = s.pfParameters[8]
      afMatrix[6] = s.pfParameters[9]
      afMatrix[7] = s.pfParameters[10]
      afMatrix[8] = s.pfParameters[11]
      s.pfReturnArray = CobbAPIRotations.ApplyTransformationMatrix(afPosition, afMatrix)
   EndFunction
EndState
State Method_ComputeTransformationMatrix
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "afRotation[0]"
      sNames[1] = "afRotation[1]"
      sNames[2] = "afRotation[2]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afRotation = new Float[3]
      afRotation[0] = s.pfParameters[0]
      afRotation[1] = s.pfParameters[1]
      afRotation[2] = s.pfParameters[2]
      s.pfReturnArray = CobbAPIRotations.ComputeTransformationMatrix(afRotation)
   EndFunction
EndState
State Method_GetRelativeCoordinates
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 12
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "afParentPosition[0]"
      sNames[1] = "afParentPosition[1]"
      sNames[2] = "afParentPosition[2]"
      sNames[3] = "afParentRotation[0]"
      sNames[4] = "afParentRotation[1]"
      sNames[5] = "afParentRotation[2]"
      sNames[6] = "afTargetPosition[0]"
      sNames[7] = "afTargetPosition[1]"
      sNames[8] = "afTargetPosition[2]"
      sNames[9] = "afTargetRotation[0]"
      sNames[10] = "afTargetRotation[1]"
      sNames[11] = "afTargetRotation[2]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      sTypes[5] = "Float"
      sTypes[6] = "Float"
      sTypes[7] = "Float"
      sTypes[8] = "Float"
      sTypes[9] = "Float"
      sTypes[10] = "Float"
      sTypes[11] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afParentPosition = new Float[3]
      afParentPosition[0] = s.pfParameters[0]
      afParentPosition[1] = s.pfParameters[1]
      afParentPosition[2] = s.pfParameters[2]
      Float[] afParentRotation = new Float[3]
      afParentRotation[0] = s.pfParameters[3]
      afParentRotation[1] = s.pfParameters[4]
      afParentRotation[2] = s.pfParameters[5]
      Float[] afTargetPosition = new Float[3]
      afTargetPosition[0] = s.pfParameters[6]
      afTargetPosition[1] = s.pfParameters[7]
      afTargetPosition[2] = s.pfParameters[8]
      Float[] afTargetRotation = new Float[3]
      afTargetRotation[0] = s.pfParameters[9]
      afTargetRotation[1] = s.pfParameters[10]
      afTargetRotation[2] = s.pfParameters[11]
      s.pfReturnArray = CobbAPIRotations.GetRelativeCoordinates(afParentPosition, afParentRotation, afTargetPosition, afTargetRotation)
   EndFunction
EndState
