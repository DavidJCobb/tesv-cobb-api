Scriptname CobbAPITestSuiteExecutorRevSvc extends CobbAPITestSuiteIExecutor Hidden

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
   Return "RevealService"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[8]
   sResult[0] = "GetLightRevealState"
   sResult[1] = "GetMarkerRevealState"
   sResult[2] = "RegisterMarker"
   sResult[3] = "RegisterMarkersInBulk"
   sResult[4] = "SetAllRevealStates"
   sResult[5] = "SetLightModels"
   sResult[6] = "SetLightRevealState"
   sResult[7] = "SetMarkerRevealState"
   Return sResult
EndFunction

State Method_GetLightRevealState
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIRevealService.GetLightRevealState()
   EndFunction
EndState
State Method_GetMarkerRevealState
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIRevealService.GetMarkerRevealState()
   EndFunction
EndState
State Method_RegisterMarker
   Int Function GetMethodParameterCount()
      Return 8
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akBaseForm"
      sNames[1] = "asRevealPath"
      sNames[2] = "aiBounds[0]"
      sNames[3] = "aiBounds[1]"
      sNames[4] = "aiBounds[2]"
      sNames[5] = "aiBounds[3]"
      sNames[6] = "aiBounds[4]"
      sNames[7] = "aiBounds[5]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Form"
      sTypes[1] = "String"
      sTypes[2] = "Int"
      sTypes[3] = "Int"
      sTypes[4] = "Int"
      sTypes[5] = "Int"
      sTypes[6] = "Int"
      sTypes[7] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Int[] aiBounds = new Int[6]
      aiBounds[0] = s.piParameters[2]
      aiBounds[1] = s.piParameters[3]
      aiBounds[2] = s.piParameters[4]
      aiBounds[3] = s.piParameters[5]
      aiBounds[4] = s.piParameters[6]
      aiBounds[5] = s.piParameters[7]
      CobbAPIRevealService.RegisterMarker(s.pkParameters[0], s.psParameters[1], aiBounds)
   EndFunction
EndState
State Method_RegisterMarkersInBulk
   Int Function GetMethodParameterCount()
      Return 16
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akBaseForms[0]"
      sNames[1] = "akBaseForms[1]"
      sNames[2] = "asRevealPaths[0]"
      sNames[3] = "asRevealPaths[1]"
      sNames[4] = "aiBounds[0]"
      sNames[5] = "aiBounds[1]"
      sNames[6] = "aiBounds[2]"
      sNames[7] = "aiBounds[3]"
      sNames[8] = "aiBounds[4]"
      sNames[9] = "aiBounds[5]"
      sNames[10] = "aiBounds[6]"
      sNames[11] = "aiBounds[7]"
      sNames[12] = "aiBounds[8]"
      sNames[13] = "aiBounds[9]"
      sNames[14] = "aiBounds[10]"
      sNames[15] = "aiBounds[11]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Form"
      sTypes[1] = "Form"
      sTypes[2] = "String"
      sTypes[3] = "String"
      sTypes[4] = "Int"
      sTypes[5] = "Int"
      sTypes[6] = "Int"
      sTypes[7] = "Int"
      sTypes[8] = "Int"
      sTypes[9] = "Int"
      sTypes[10] = "Int"
      sTypes[11] = "Int"
      sTypes[12] = "Int"
      sTypes[13] = "Int"
      sTypes[14] = "Int"
      sTypes[15] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Form[] akBaseForms = new Form[2]
      akBaseForms[0] = s.pkParameters[0] as Form
      akBaseForms[1] = s.pkParameters[1] as Form
      String[] asRevealPaths = new String[2]
      asRevealPaths[0] = s.psParameters[2]
      asRevealPaths[1] = s.psParameters[3]
      Int[] aiBounds = new Int[12]
      aiBounds[0] = s.piParameters[4]
      aiBounds[1] = s.piParameters[5]
      aiBounds[2] = s.piParameters[6]
      aiBounds[3] = s.piParameters[7]
      aiBounds[4] = s.piParameters[8]
      aiBounds[5] = s.piParameters[9]
      aiBounds[6] = s.piParameters[10]
      aiBounds[7] = s.piParameters[11]
      aiBounds[8] = s.piParameters[12]
      aiBounds[9] = s.piParameters[13]
      aiBounds[10] = s.piParameters[14]
      aiBounds[11] = s.piParameters[15]
      CobbAPIRevealService.RegisterMarkersInBulk(akBaseForms, asRevealPaths, aiBounds)
   EndFunction
EndState
State Method_SetAllRevealStates
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "abState"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIRevealService.SetAllRevealStates(s.pbParameters[0])
   EndFunction
EndState
State Method_SetLightModels
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akOmni"
      sNames[1] = "akOmniShadow"
      sNames[2] = "akOmniHalf"
      sNames[3] = "akSpot"
      sNames[4] = "akSpotShadow"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Activator"
      sTypes[1] = "Activator"
      sTypes[2] = "Activator"
      sTypes[3] = "Activator"
      sTypes[4] = "Activator"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIRevealService.SetLightModels(s.pkParameters[0] as Activator, s.pkParameters[1] as Activator, s.pkParameters[2] as Activator, s.pkParameters[3] as Activator, s.pkParameters[4] as Activator)
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = None
      s.pbParameters[1] = None
      s.pbParameters[1] = None
      s.pbParameters[1] = None
   EndFunction
EndState
State Method_SetLightRevealState
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "abState"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIRevealService.SetLightRevealState(s.pbParameters[0])
   EndFunction
EndState
State Method_SetMarkerRevealState
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "abState"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIRevealService.SetMarkerRevealState(s.pbParameters[0])
   EndFunction
EndState
