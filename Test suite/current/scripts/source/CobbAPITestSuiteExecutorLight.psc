Scriptname CobbAPITestSuiteExecutorLight extends CobbAPITestSuiteIExecutor Hidden

;/Block/; ; Because subclassing is a little weird and I need to reproduce the functions here, apparently
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
   Return "Light"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[8]
   sResult[0] = "GetDepthBias"
   sResult[1] = "GetFOV"
   sResult[2] = "GetFade"
   sResult[3] = "GetLightsLandscape"
   sResult[4] = "GetLightsWater"
   sResult[5] = "GetRadius"
   sResult[6] = "SetFade"
   sResult[7] = "SetRadius"
   Return sResult
EndFunction

State Method_GetDepthBias
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akLightReference"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPILight.GetDepthBias(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetFOV
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akLightReference"
      sNames[1] = "abRelative"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPILight.GetFOV(s.pkParameters[0] as ObjectReference, s.pbParameters[1])
   EndFunction
EndState
State Method_GetFade
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akLightReference"
      sNames[1] = "abRelative"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPILight.GetFade(s.pkParameters[0] as ObjectReference, s.pbParameters[1])
   EndFunction
EndState
State Method_GetLightsLandscape
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akLightReference"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPILight.GetLightsLandscape(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetLightsWater
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akLightReference"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPILight.GetLightsWater(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetRadius
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akLightReference"
      sNames[1] = "abRelative"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPILight.GetRadius(s.pkParameters[0] as ObjectReference, s.pbParameters[1])
   EndFunction
EndState
State Method_SetFade
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akLightReference"
      sNames[1] = "afFade"
      sNames[2] = "abRelative"
      sNames[3] = "abReserved"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Float"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPILight.SetFade(s.pkParameters[0] as ObjectReference, s.pfParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
EndState
State Method_SetRadius
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akLightReference"
      sNames[1] = "afRadius"
      sNames[2] = "abRelative"
      sNames[3] = "abReserved"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Float"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPILight.SetRadius(s.pkParameters[0] as ObjectReference, s.pfParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
EndState
