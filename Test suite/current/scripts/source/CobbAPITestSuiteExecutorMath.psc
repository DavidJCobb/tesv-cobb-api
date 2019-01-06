Scriptname CobbAPITestSuiteExecutorMath extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Math"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[8]
   sResult[0] = "Atan2"
   sResult[1] = "BSwap"
   sResult[2] = "ConvertColorFormat"
   sResult[3] = "GetByte"
   sResult[4] = "JoinColor"
   sResult[5] = "JoinColorComponents"
   sResult[6] = "SetByte"
   sResult[7] = "SplitColor"
   Return sResult
EndFunction

State Method_Atan2
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "y"
      sNames[1] = "x"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Float"
      sTypes[1] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPIMath.Atan2(s.pfParameters[0], s.pfParameters[1])
   EndFunction
EndState
State Method_BSwap
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "aiValue"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIMath.BSwap(s.piParameters[0])
   EndFunction
EndState
State Method_ConvertColorFormat
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "aiColor"
      sNames[1] = "aiInputFormat"
      sNames[2] = "aiOutputFormat"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Int"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIMath.ConvertColorFormat(s.piParameters[0], s.piParameters[1], s.piParameters[2])
   EndFunction
EndState
State Method_GetByte
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "aiValue"
      sNames[1] = "aiWhichByte"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Int"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIMath.GetByte(s.piParameters[0], s.piParameters[1])
   EndFunction
EndState
State Method_JoinColor
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "aiFormat"
      sNames[1] = "aiSplitColor[0]"
      sNames[2] = "aiSplitColor[1]"
      sNames[3] = "aiSplitColor[2]"
      sNames[4] = "aiSplitColor[3]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Int"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      sTypes[3] = "Int"
      sTypes[4] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Int[] aiSplitColor = new Int[4]
      aiSplitColor[0] = s.piParameters[1]
      aiSplitColor[1] = s.piParameters[2]
      aiSplitColor[2] = s.piParameters[3]
      aiSplitColor[3] = s.piParameters[4]
      s.piReturnValue = CobbAPIMath.JoinColor(s.piParameters[0], aiSplitColor)
   EndFunction
EndState
State Method_JoinColorComponents
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "aiFormat"
      sNames[1] = "aiRed"
      sNames[2] = "aiGreen"
      sNames[3] = "aiBlue"
      sNames[4] = "aiAlpha"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Int"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      sTypes[3] = "Int"
      sTypes[4] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIMath.JoinColorComponents(s.piParameters[0], s.piParameters[1], s.piParameters[2], s.piParameters[3], s.piParameters[4])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[4] = 255
   EndFunction
EndState
State Method_SetByte
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "aiValue"
      sNames[1] = "aiWhichByte"
      sNames[2] = "aiSetByteTo"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Int"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIMath.SetByte(s.piParameters[0], s.piParameters[1], s.piParameters[2])
   EndFunction
EndState
State Method_SplitColor
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "aiFormat"
      sNames[1] = "aiColor"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Int"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIMath.SplitColor(s.piParameters[0], s.piParameters[1])
   EndFunction
EndState
