Scriptname CobbAPITestSuiteExecutorString extends CobbAPITestSuiteIExecutor Hidden

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
   Return "String"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[13]
   sResult[0] = "BinaryToInt32"
   sResult[1] = "CPLength"
   sResult[2] = "HexToInt32"
   sResult[3] = "NaturalCompare_ASCII"
   sResult[4] = "NaturalSortPairForm_ASCII"
   sResult[5] = "NaturalSortPairInt_ASCII"
   sResult[6] = "NaturalSort_ASCII"
   sResult[7] = "RegexMatch"
   sResult[8] = "RegexReplace"
   sResult[9] = "RegexSearch"
   sResult[10] = "ToBinary"
   sResult[11] = "ToHex"
   sResult[12] = "Trim"
   Return sResult
EndFunction

State Method_BinaryToInt32
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "asString"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIString.BinaryToInt32(s.psParameters[0])
   EndFunction
EndState
State Method_CPLength
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "asString"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIString.CPLength(s.psParameters[0])
   EndFunction
EndState
State Method_HexToInt32
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "asString"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIString.HexToInt32(s.psParameters[0])
   EndFunction
EndState
State Method_NaturalCompare_ASCII
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "a"
      sNames[1] = "b"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "String"
      sTypes[1] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIString.NaturalCompare_ASCII(s.psParameters[0], s.psParameters[1])
   EndFunction
EndState
State Method_NaturalSortPairForm_ASCII
   String Function GetMethodReturnType()
      Return "String[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 11
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[11]
      sNames[0] = "asStrings[0]"
      sNames[1] = "asStrings[1]"
      sNames[2] = "asStrings[2]"
      sNames[3] = "asStrings[3]"
      sNames[4] = "asStrings[4]"
      sNames[5] = "axSecond[0]"
      sNames[6] = "axSecond[1]"
      sNames[7] = "axSecond[2]"
      sNames[8] = "axSecond[3]"
      sNames[9] = "axSecond[4]"
      sNames[10] = "abDescending"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[11]
      sTypes[0] = "String"
      sTypes[1] = "String"
      sTypes[2] = "String"
      sTypes[3] = "String"
      sTypes[4] = "String"
      sTypes[5] = "Form"
      sTypes[6] = "Form"
      sTypes[7] = "Form"
      sTypes[8] = "Form"
      sTypes[9] = "Form"
      sTypes[10] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      String[] asStrings = new String[5]
      asStrings[0] = s.psParameters[0]
      asStrings[1] = s.psParameters[1]
      asStrings[2] = s.psParameters[2]
      asStrings[3] = s.psParameters[3]
      asStrings[4] = s.psParameters[4]
      Form[] axSecond = new Form[5]
      axSecond[0] = s.pkParameters[5] as Form
      axSecond[1] = s.pkParameters[6] as Form
      axSecond[2] = s.pkParameters[7] as Form
      axSecond[3] = s.pkParameters[8] as Form
      axSecond[4] = s.pkParameters[9] as Form
      s.psReturnArray = CobbAPIString.NaturalSortPairForm_ASCII(asStrings, axSecond, s.pbParameters[10])
   EndFunction
EndState
State Method_NaturalSortPairInt_ASCII
   String Function GetMethodReturnType()
      Return "String[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 11
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[11]
      sNames[0] = "asStrings[0]"
      sNames[1] = "asStrings[1]"
      sNames[2] = "asStrings[2]"
      sNames[3] = "asStrings[3]"
      sNames[4] = "asStrings[4]"
      sNames[5] = "axSecond[0]"
      sNames[6] = "axSecond[1]"
      sNames[7] = "axSecond[2]"
      sNames[8] = "axSecond[3]"
      sNames[9] = "axSecond[4]"
      sNames[10] = "abDescending"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[11]
      sTypes[0] = "String"
      sTypes[1] = "String"
      sTypes[2] = "String"
      sTypes[3] = "String"
      sTypes[4] = "String"
      sTypes[5] = "Int"
      sTypes[6] = "Int"
      sTypes[7] = "Int"
      sTypes[8] = "Int"
      sTypes[9] = "Int"
      sTypes[10] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      String[] asStrings = new String[5]
      asStrings[0] = s.psParameters[0]
      asStrings[1] = s.psParameters[1]
      asStrings[2] = s.psParameters[2]
      asStrings[3] = s.psParameters[3]
      asStrings[4] = s.psParameters[4]
      Int[] axSecond = new Int[5]
      axSecond[0] = s.piParameters[5]
      axSecond[1] = s.piParameters[6]
      axSecond[2] = s.piParameters[7]
      axSecond[3] = s.piParameters[8]
      axSecond[4] = s.piParameters[9]
      s.psReturnArray = CobbAPIString.NaturalSortPairInt_ASCII(asStrings, axSecond, s.pbParameters[10])
   EndFunction
EndState
State Method_NaturalSort_ASCII
   String Function GetMethodReturnType()
      Return "String[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 6
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[6]
      sNames[0] = "asStrings[0]"
      sNames[1] = "asStrings[1]"
      sNames[2] = "asStrings[2]"
      sNames[3] = "asStrings[3]"
      sNames[4] = "asStrings[4]"
      sNames[5] = "abDescending"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[6]
      sTypes[0] = "String"
      sTypes[1] = "String"
      sTypes[2] = "String"
      sTypes[3] = "String"
      sTypes[4] = "String"
      sTypes[5] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      String[] asStrings = new String[5]
      asStrings[0] = s.psParameters[0]
      asStrings[1] = s.psParameters[1]
      asStrings[2] = s.psParameters[2]
      asStrings[3] = s.psParameters[3]
      asStrings[4] = s.psParameters[4]
      s.psReturnArray = CobbAPIString.NaturalSort_ASCII(asStrings, s.pbParameters[5])
   EndFunction
EndState
State Method_RegexMatch
   String Function GetMethodReturnType()
      Return "String[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "asHaystack"
      sNames[1] = "asRegex"
      sNames[2] = "asCaseSentinel"
      sNames[3] = "abWholeStringMustMatch"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "String"
      sTypes[1] = "String"
      sTypes[2] = "String"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.psReturnArray = CobbAPIString.RegexMatch(s.psParameters[0], s.psParameters[1], s.psParameters[2], s.pbParameters[3])
   EndFunction
EndState
State Method_RegexReplace
   String Function GetMethodReturnType()
      Return "String"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "asHaystack"
      sNames[1] = "asRegex"
      sNames[2] = "asCaseSentinel"
      sNames[3] = "asReplaceWith"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "String"
      sTypes[1] = "String"
      sTypes[2] = "String"
      sTypes[3] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.psReturnValue = CobbAPIString.RegexReplace(s.psParameters[0], s.psParameters[1], s.psParameters[2], s.psParameters[3])
   EndFunction
EndState
State Method_RegexSearch
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "asHaystack"
      sNames[1] = "asRegex"
      sNames[2] = "asCaseSentinel"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "String"
      sTypes[1] = "String"
      sTypes[2] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIString.RegexSearch(s.psParameters[0], s.psParameters[1], s.psParameters[2])
   EndFunction
EndState
State Method_ToBinary
   String Function GetMethodReturnType()
      Return "String"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "aiBinary"
      sNames[1] = "aiDigits"
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
      s.psReturnValue = CobbAPIString.ToBinary(s.piParameters[0], s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 32
   EndFunction
EndState
State Method_ToHex
   String Function GetMethodReturnType()
      Return "String"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "aiHex"
      sNames[1] = "aiDigits"
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
      s.psReturnValue = CobbAPIString.ToHex(s.piParameters[0], s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 8
   EndFunction
EndState
State Method_Trim
   String Function GetMethodReturnType()
      Return "String"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "asString"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.psReturnValue = CobbAPIString.Trim(s.psParameters[0])
   EndFunction
EndState
