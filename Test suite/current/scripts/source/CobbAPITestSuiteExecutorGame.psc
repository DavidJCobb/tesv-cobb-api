Scriptname CobbAPITestSuiteExecutorGame extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Game"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[5]
   sResult[0] = "GetFormFromFile"
   sResult[1] = "GetPlayerLastKnownOutdoorPosition"
   sResult[2] = "GetPlayerLastKnownOutdoorWorldspace"
   sResult[3] = "IsPlayerCollisionEnabled"
   sResult[4] = "SetPlayerCollisionEnabled"
   Return sResult
EndFunction

State Method_GetFormFromFile
   String Function GetMethodReturnType()
      Return "Form"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "aiFormID"
      sNames[1] = "asFileName"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Int"
      sTypes[1] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIGame.GetFormFromFile(s.piParameters[0], s.psParameters[1])
   EndFunction
EndState
State Method_GetPlayerLastKnownOutdoorPosition
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIGame.GetPlayerLastKnownOutdoorPosition()
   EndFunction
EndState
State Method_GetPlayerLastKnownOutdoorWorldspace
   String Function GetMethodReturnType()
      Return "WorldSpace"
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIGame.GetPlayerLastKnownOutdoorWorldspace()
   EndFunction
EndState
State Method_IsPlayerCollisionEnabled
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIGame.IsPlayerCollisionEnabled()
   EndFunction
EndState
State Method_SetPlayerCollisionEnabled
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "abEnabled"
      sNames[1] = "abDoFullProcessing"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Bool"
      sTypes[1] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIGame.SetPlayerCollisionEnabled(s.pbParameters[0], s.pbParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = True
   EndFunction
EndState
