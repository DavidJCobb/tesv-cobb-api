Scriptname CobbAPITestSuiteExecutorWorldspace extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Worldspace"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[3]
   sResult[0] = "GetParentWorldspace"
   sResult[1] = "GetRootWorldspaceOrSelf"
   sResult[2] = "GetWaterLevel"
   Return sResult
EndFunction

State Method_GetParentWorldspace
   String Function GetMethodReturnType()
      Return "WorldSpace"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akSubject"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "WorldSpace"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIWorldspace.GetParentWorldspace(s.pkParameters[0] as WorldSpace)
   EndFunction
EndState
State Method_GetRootWorldspaceOrSelf
   String Function GetMethodReturnType()
      Return "WorldSpace"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akSubject"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "WorldSpace"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIWorldspace.GetRootWorldspaceOrSelf(s.pkParameters[0] as WorldSpace)
   EndFunction
EndState
State Method_GetWaterLevel
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akSubject"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "WorldSpace"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPIWorldspace.GetWaterLevel(s.pkParameters[0] as WorldSpace)
   EndFunction
EndState
