Scriptname CobbAPITestSuiteExecutorDetection extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Detection"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[12]
   sResult[0] = "ActorCannotBeSeen"
   sResult[1] = "ActorCannotSee"
   sResult[2] = "ForceActorToBeingSeen"
   sResult[3] = "ForceActorToSeeing"
   sResult[4] = "ForceTagToBeingSeen"
   sResult[5] = "ForceTagToSeeing"
   sResult[6] = "InvalidateTag"
   sResult[7] = "PreventActorFromBeingSeen"
   sResult[8] = "PreventActorFromSeeing"
   sResult[9] = "ReturnActorToBeingSeen"
   sResult[10] = "ReturnActorToSeeing"
   sResult[11] = "ServiceIsAvailable"
   Return sResult
EndFunction

State Method_ActorCannotBeSeen
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akActor"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIDetection.ActorCannotBeSeen(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_ActorCannotSee
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akActor"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIDetection.ActorCannotSee(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_ForceActorToBeingSeen
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akActor"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIDetection.ForceActorToBeingSeen(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_ForceActorToSeeing
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akActor"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIDetection.ForceActorToSeeing(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_ForceTagToBeingSeen
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "asTag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIDetection.ForceTagToBeingSeen(s.psParameters[0])
   EndFunction
EndState
State Method_ForceTagToSeeing
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "asTag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIDetection.ForceTagToSeeing(s.psParameters[0])
   EndFunction
EndState
State Method_InvalidateTag
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "asTag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIDetection.InvalidateTag(s.psParameters[0])
   EndFunction
EndState
State Method_PreventActorFromBeingSeen
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akActor"
      sNames[1] = "akPersistenceForm"
      sNames[2] = "asTag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Actor"
      sTypes[1] = "Form"
      sTypes[2] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIDetection.PreventActorFromBeingSeen(s.pkParameters[0] as Actor, s.pkParameters[1], s.psParameters[2])
   EndFunction
EndState
State Method_PreventActorFromSeeing
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akActor"
      sNames[1] = "akPersistenceForm"
      sNames[2] = "asTag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Actor"
      sTypes[1] = "Form"
      sTypes[2] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIDetection.PreventActorFromSeeing(s.pkParameters[0] as Actor, s.pkParameters[1], s.psParameters[2])
   EndFunction
EndState
State Method_ReturnActorToBeingSeen
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akActor"
      sNames[1] = "asTag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Actor"
      sTypes[1] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIDetection.ReturnActorToBeingSeen(s.pkParameters[0] as Actor, s.psParameters[1])
   EndFunction
EndState
State Method_ReturnActorToSeeing
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akActor"
      sNames[1] = "asTag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Actor"
      sTypes[1] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIDetection.ReturnActorToSeeing(s.pkParameters[0] as Actor, s.psParameters[1])
   EndFunction
EndState
State Method_ServiceIsAvailable
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIDetection.ServiceIsAvailable()
   EndFunction
EndState
