Scriptname CobbAPITestSuiteExecutorActor extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Actor"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[14]
   sResult[0] = "CeaseDetectionEvent"
   sResult[1] = "Decapitate"
   sResult[2] = "GetActorValueModifier"
   sResult[3] = "GetActorValueRegenDelay"
   sResult[4] = "GetBleedoutTimer"
   sResult[5] = "GetDetectionEventAge"
   sResult[6] = "GetDetectionEventPosition"
   sResult[7] = "GetDetectionEventSoundLevel"
   sResult[8] = "GetHasBeenEaten"
   sResult[9] = "GetSoulSize"
   sResult[10] = "HasDetectionEvent"
   sResult[11] = "IsDecapitated"
   sResult[12] = "IsHorse"
   sResult[13] = "SetHasBeenEaten"
   Return sResult
EndFunction

State Method_CeaseDetectionEvent
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIActor.CeaseDetectionEvent(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_Decapitate
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akTarget"
      sNames[1] = "abEvenIfAlive"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Actor"
      sTypes[1] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIActor.Decapitate(s.pkParameters[0] as Actor, s.pbParameters[1])
   EndFunction
EndState
State Method_GetActorValueModifier
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akSubject"
      sNames[1] = "aiActorValue"
      sNames[2] = "aiModifierType"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Actor"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPIActor.GetActorValueModifier(s.pkParameters[0] as Actor, s.piParameters[1], s.piParameters[2])
   EndFunction
EndState
State Method_GetActorValueRegenDelay
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiActorValue"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Actor"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPIActor.GetActorValueRegenDelay(s.pkParameters[0] as Actor, s.piParameters[1])
   EndFunction
EndState
State Method_GetBleedoutTimer
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPIActor.GetBleedoutTimer(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_GetDetectionEventAge
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPIActor.GetDetectionEventAge(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_GetDetectionEventPosition
   String Function GetMethodReturnType()
      Return "Float[]"
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIActor.GetDetectionEventPosition(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_GetDetectionEventSoundLevel
   String Function GetMethodReturnType()
      Return "Int"
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIActor.GetDetectionEventSoundLevel(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_GetHasBeenEaten
   String Function GetMethodReturnType()
      Return "Bool"
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIActor.GetHasBeenEaten(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_GetSoulSize
   String Function GetMethodReturnType()
      Return "Int"
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIActor.GetSoulSize(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_HasDetectionEvent
   String Function GetMethodReturnType()
      Return "Bool"
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIActor.HasDetectionEvent(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_IsDecapitated
   String Function GetMethodReturnType()
      Return "Bool"
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIActor.IsDecapitated(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_IsHorse
   String Function GetMethodReturnType()
      Return "Bool"
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
      sTypes[0] = "Actor"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIActor.IsHorse(s.pkParameters[0] as Actor)
   EndFunction
EndState
State Method_SetHasBeenEaten
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "abEaten"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Actor"
      sTypes[1] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIActor.SetHasBeenEaten(s.pkParameters[0] as Actor, s.pbParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = True
   EndFunction
EndState
