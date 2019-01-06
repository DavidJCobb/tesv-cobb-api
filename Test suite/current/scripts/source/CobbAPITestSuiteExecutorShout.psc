Scriptname CobbAPITestSuiteExecutorShout extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Shout"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[8]
   sResult[0] = "GetAllShoutsUsingSpell"
   sResult[1] = "GetAllShoutsUsingWord"
   sResult[2] = "GetAnyShoutUsingSpell"
   sResult[3] = "GetAnyShoutUsingWord"
   sResult[4] = "GetShoutMenuDisplayObject"
   sResult[5] = "GetShoutRecoveryTimes"
   sResult[6] = "GetShoutSpells"
   sResult[7] = "GetShoutWords"
   Return sResult
EndFunction

State Method_GetAllShoutsUsingSpell
   String Function GetMethodReturnType()
      Return "Shout[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akSpell"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Spell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Shout[] kResult = CobbAPIShout.GetAllShoutsUsingSpell(s.pkParameters[0] as Spell)
      Int iLength   = kResult.Length
      Form[] _kReturnArray = Utility.CreateFormArray(iLength)
      Int iIterator = 0
      While iIterator < iLength
         _kReturnArray[iIterator] = kResult[iIterator]
         iIterator += 1
      EndWhile
      s.pkReturnArray = _kReturnArray
   EndFunction
EndState
State Method_GetAllShoutsUsingWord
   String Function GetMethodReturnType()
      Return "Shout[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akWord"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "WordOfPower"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Shout[] kResult = CobbAPIShout.GetAllShoutsUsingWord(s.pkParameters[0] as WordOfPower)
      Int iLength   = kResult.Length
      Form[] _kReturnArray = Utility.CreateFormArray(iLength)
      Int iIterator = 0
      While iIterator < iLength
         _kReturnArray[iIterator] = kResult[iIterator]
         iIterator += 1
      EndWhile
      s.pkReturnArray = _kReturnArray
   EndFunction
EndState
State Method_GetAnyShoutUsingSpell
   String Function GetMethodReturnType()
      Return "Shout"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akSpell"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Spell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIShout.GetAnyShoutUsingSpell(s.pkParameters[0] as Spell)
   EndFunction
EndState
State Method_GetAnyShoutUsingWord
   String Function GetMethodReturnType()
      Return "Shout"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akWord"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "WordOfPower"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIShout.GetAnyShoutUsingWord(s.pkParameters[0] as WordOfPower)
   EndFunction
EndState
State Method_GetShoutMenuDisplayObject
   String Function GetMethodReturnType()
      Return "Static"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akShout"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Shout"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIShout.GetShoutMenuDisplayObject(s.pkParameters[0] as Shout)
   EndFunction
EndState
State Method_GetShoutRecoveryTimes
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akShout"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Shout"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIShout.GetShoutRecoveryTimes(s.pkParameters[0] as Shout)
   EndFunction
EndState
State Method_GetShoutSpells
   String Function GetMethodReturnType()
      Return "Spell[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akShout"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Shout"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Spell[] kResult = CobbAPIShout.GetShoutSpells(s.pkParameters[0] as Shout)
      Int iLength   = kResult.Length
      Form[] _kReturnArray = Utility.CreateFormArray(iLength)
      Int iIterator = 0
      While iIterator < iLength
         _kReturnArray[iIterator] = kResult[iIterator]
         iIterator += 1
      EndWhile
      s.pkReturnArray = _kReturnArray
   EndFunction
EndState
State Method_GetShoutWords
   String Function GetMethodReturnType()
      Return "WordOfPower[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akShout"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "Shout"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      WordOfPower[] kResult = CobbAPIShout.GetShoutWords(s.pkParameters[0] as Shout)
      Int iLength   = kResult.Length
      Form[] _kReturnArray = Utility.CreateFormArray(iLength)
      Int iIterator = 0
      While iIterator < iLength
         _kReturnArray[iIterator] = kResult[iIterator]
         iIterator += 1
      EndWhile
      s.pkReturnArray = _kReturnArray
   EndFunction
EndState
