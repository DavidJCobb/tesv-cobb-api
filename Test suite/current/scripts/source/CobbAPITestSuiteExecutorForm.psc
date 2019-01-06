Scriptname CobbAPITestSuiteExecutorForm extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Form"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[6]
   sResult[0] = "GetBounds"
   sResult[1] = "GetBoundsToHalfwidthOffsets"
   sResult[2] = "GetBoundsToHalfwidths"
   sResult[3] = "GetEditorID"
   sResult[4] = "GetEditorIDs"
   sResult[5] = "IsDefiniteItem"
   Return sResult
EndFunction

State Method_GetBounds
   String Function GetMethodReturnType()
      Return "Int[]"
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
      sTypes[0] = "Form"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIForm.GetBounds(s.pkParameters[0])
   EndFunction
EndState
State Method_GetBoundsToHalfwidthOffsets
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
      sTypes[0] = "Form"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIForm.GetBoundsToHalfwidthOffsets(s.pkParameters[0])
   EndFunction
EndState
State Method_GetBoundsToHalfwidths
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
      sTypes[0] = "Form"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIForm.GetBoundsToHalfwidths(s.pkParameters[0])
   EndFunction
EndState
State Method_GetEditorID
   String Function GetMethodReturnType()
      Return "String"
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
      sTypes[0] = "Form"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.psReturnValue = CobbAPIForm.GetEditorID(s.pkParameters[0])
   EndFunction
EndState
State Method_GetEditorIDs
   String Function GetMethodReturnType()
      Return "String[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akSubjects[0]"
      sNames[1] = "akSubjects[1]"
      sNames[2] = "akSubjects[2]"
      sNames[3] = "akSubjects[3]"
      sNames[4] = "akSubjects[4]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Form"
      sTypes[1] = "Form"
      sTypes[2] = "Form"
      sTypes[3] = "Form"
      sTypes[4] = "Form"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Form[] akSubjects = new Form[5]
      akSubjects[0] = s.pkParameters[0] as Form
      akSubjects[1] = s.pkParameters[1] as Form
      akSubjects[2] = s.pkParameters[2] as Form
      akSubjects[3] = s.pkParameters[3] as Form
      akSubjects[4] = s.pkParameters[4] as Form
      s.psReturnArray = CobbAPIForm.GetEditorIDs(akSubjects)
   EndFunction
EndState
State Method_IsDefiniteItem
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
      sTypes[0] = "Form"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIForm.IsDefiniteItem(s.pkParameters[0])
   EndFunction
EndState
