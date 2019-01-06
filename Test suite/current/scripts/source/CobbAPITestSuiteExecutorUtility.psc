Scriptname CobbAPITestSuiteExecutorUtility extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Utility"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[4]
   sResult[0] = "FormExists"
   sResult[1] = "GetBoundsToHalfwidths"
   sResult[2] = "GetBoundsToHalfwidthOffsets"
   sResult[3] = "GetGoodObjectSpawnCoordinates"
   Return sResult
EndFunction

State Method_FormExists
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akPointer"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Form"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIUtility.FormExists(s.pkParameters[0])
   EndFunction
EndState
State Method_GetBoundsToHalfwidths
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 6
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[6]
      sNames[0] = "afBounds[0]"
      sNames[1] = "afBounds[1]"
      sNames[2] = "afBounds[2]"
      sNames[3] = "afBounds[3]"
      sNames[4] = "afBounds[4]"
      sNames[5] = "afBounds[5]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[6]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      sTypes[5] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afBounds = new Float[6]
      afBounds[0] = s.pfParameters[0]
      afBounds[1] = s.pfParameters[1]
      afBounds[2] = s.pfParameters[2]
      afBounds[3] = s.pfParameters[3]
      afBounds[4] = s.pfParameters[4]
      afBounds[5] = s.pfParameters[5]
      s.pfReturnArray = CobbAPIUtility.GetBoundsToHalfwidths(afBounds)
   EndFunction
EndState
State Method_GetBoundsToHalfwidthOffsets
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 6
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[6]
      sNames[0] = "afBounds[0]"
      sNames[1] = "afBounds[1]"
      sNames[2] = "afBounds[2]"
      sNames[3] = "afBounds[3]"
      sNames[4] = "afBounds[4]"
      sNames[5] = "afBounds[5]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[6]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      sTypes[5] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afBounds = new Float[6]
      afBounds[0] = s.pfParameters[0]
      afBounds[1] = s.pfParameters[1]
      afBounds[2] = s.pfParameters[2]
      afBounds[3] = s.pfParameters[3]
      afBounds[4] = s.pfParameters[4]
      afBounds[5] = s.pfParameters[5]
      s.pfReturnArray = CobbAPIUtility.GetBoundsToHalfwidthOffsets(afBounds)
   EndFunction
EndState
State Method_GetGoodObjectSpawnCoordinates
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 10
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[10]
      sNames[0] = "afBounds[0]"
      sNames[1] = "afBounds[1]"
      sNames[2] = "afBounds[2]"
      sNames[3] = "afBounds[3]"
      sNames[4] = "afBounds[4]"
      sNames[5] = "afBounds[5]"
      sNames[6] = "afPlayerCoordinates[0]"
      sNames[7] = "afPlayerCoordinates[1]"
      sNames[8] = "afPlayerCoordinates[2]"
      sNames[9] = "aiFlags"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[10]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      sTypes[5] = "Float"
      sTypes[6] = "Float"
      sTypes[7] = "Float"
      sTypes[8] = "Float"
      sTypes[9] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afBounds = new Float[6]
      afBounds[0] = s.pfParameters[0]
      afBounds[1] = s.pfParameters[1]
      afBounds[2] = s.pfParameters[2]
      afBounds[3] = s.pfParameters[3]
      afBounds[4] = s.pfParameters[4]
      afBounds[5] = s.pfParameters[5]
      Float[] afPlayerCoordinates = new Float[3]
      afPlayerCoordinates[0] = s.pfParameters[6]
      afPlayerCoordinates[1] = s.pfParameters[7]
      afPlayerCoordinates[2] = s.pfParameters[8]
      s.pfReturnArray = CobbAPIUtility.GetGoodObjectSpawnCoordinates(afBounds, afPlayerCoordinates, s.piParameters[9])
   EndFunction
EndState
