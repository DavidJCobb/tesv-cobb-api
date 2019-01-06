Scriptname CobbAPITestSuiteExecutorFurniture extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Furniture"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[7]
   sResult[0] = "GetAssociatedSpell"
   sResult[1] = "GetEnabledMarkers"
   sResult[2] = "GetMarkerEntryPoints"
   sResult[3] = "GetMarkerKeyword"
   sResult[4] = "GetMustExitToTalk"
   sResult[5] = "GetWorkbenchSkill"
   sResult[6] = "GetWorkbenchType"
   Return sResult
EndFunction

State Method_GetAssociatedSpell
   String Function GetMethodReturnType()
      Return "Spell"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akFurniture"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Furniture"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIFurniture.GetAssociatedSpell(s.pkParameters[0] as Furniture)
   EndFunction
EndState
State Method_GetEnabledMarkers
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akFurniture"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Furniture"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIFurniture.GetEnabledMarkers(s.pkParameters[0] as Furniture)
   EndFunction
EndState
State Method_GetMarkerEntryPoints
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akFurniture"
      sNames[1] = "aiMarker"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Furniture"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIFurniture.GetMarkerEntryPoints(s.pkParameters[0] as Furniture, s.piParameters[1])
   EndFunction
EndState
State Method_GetMarkerKeyword
   String Function GetMethodReturnType()
      Return "Keyword"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akFurniture"
      sNames[1] = "aiMarker"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Furniture"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIFurniture.GetMarkerKeyword(s.pkParameters[0] as Furniture, s.piParameters[1])
   EndFunction
EndState
State Method_GetMustExitToTalk
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akFurniture"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Furniture"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIFurniture.GetMustExitToTalk(s.pkParameters[0] as Furniture)
   EndFunction
EndState
State Method_GetWorkbenchSkill
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akFurniture"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Furniture"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIFurniture.GetWorkbenchSkill(s.pkParameters[0] as Furniture)
   EndFunction
EndState
State Method_GetWorkbenchType
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akFurniture"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Furniture"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIFurniture.GetWorkbenchType(s.pkParameters[0] as Furniture)
   EndFunction
EndState
