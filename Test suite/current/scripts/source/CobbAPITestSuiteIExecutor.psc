Scriptname CobbAPITestSuiteIExecutor extends Quest Hidden

; Override these.
;
String Function GetClassName()
   Return ""
EndFunction
CobbAPITestSuiteExecutorState Function GetExecutorState()
   Return Quest.GetQuest("CobbAPITestSuiteExecutorState") as CobbAPITestSuiteExecutorState
EndFunction
String[] Function GetClassMethods()
   Return new String[1]
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
