Scriptname CobbAPITestSuiteExecutorImSpace extends CobbAPITestSuiteIExecutor Hidden

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
   Return "ImageSpace"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[1]
   sResult[0] = "GetAllForms"
   Return sResult
EndFunction

State Method_GetAllForms
   String Function GetMethodReturnType()
      Return "Form[]"
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnArray = CobbAPIImageSpace.GetAllForms()
   EndFunction
EndState
