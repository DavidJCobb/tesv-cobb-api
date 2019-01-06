Scriptname CobbAPITestSuiteExecutorSimpSch extends CobbAPITestSuiteIExecutor Hidden

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
   Return "SimpleSearches"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[2]
   sResult[0] = "GetAllOfBaseFormInCell"
   sResult[1] = "GetAllWithLinkedRefInCell"
   Return sResult
EndFunction

State Method_GetAllOfBaseFormInCell
   String Function GetMethodReturnType()
      Return "ObjectReference[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akBaseForm"
      sNames[1] = "akCellToSearch"
      sNames[2] = "abSearchLoadedArea"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Form"
      sTypes[1] = "Cell"
      sTypes[2] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      ObjectReference[] kResult = CobbAPISimpleSearches.GetAllOfBaseFormInCell(s.pkParameters[0], s.pkParameters[1] as Cell, s.pbParameters[2])
      Int iLength   = kResult.Length
      Form[] _kReturnArray = Utility.CreateFormArray(iLength)
      Int iIterator = 0
      While iIterator < iLength
         _kReturnArray[iIterator] = kResult[iIterator]
         iIterator += 1
      EndWhile
      s.pkReturnArray = _kReturnArray
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = None
   EndFunction
EndState
State Method_GetAllWithLinkedRefInCell
   String Function GetMethodReturnType()
      Return "ObjectReference[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akLinkedRef"
      sNames[1] = "akKeyword"
      sNames[2] = "akCellToSearch"
      sNames[3] = "abSearchLoadedArea"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Keyword"
      sTypes[2] = "Cell"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      ObjectReference[] kResult = CobbAPISimpleSearches.GetAllWithLinkedRefInCell(s.pkParameters[0] as ObjectReference, s.pkParameters[1] as Keyword, s.pkParameters[2] as Cell, s.pbParameters[3])
      Int iLength   = kResult.Length
      Form[] _kReturnArray = Utility.CreateFormArray(iLength)
      Int iIterator = 0
      While iIterator < iLength
         _kReturnArray[iIterator] = kResult[iIterator]
         iIterator += 1
      EndWhile
      s.pkReturnArray = _kReturnArray
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = None
      s.pbParameters[1] = None
   EndFunction
EndState
