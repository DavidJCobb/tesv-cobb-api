Scriptname CobbAPITestSuiteExecutorObjRef extends CobbAPITestSuiteIExecutor Hidden

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
   Return "ObjectReference"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[34]
   sResult[0] = "BaseHasWhichKeyword"
   sResult[1] = "BaseHasWhichKeywordName"
   sResult[2] = "GetActivateParents"
   sResult[3] = "GetBounds"
   sResult[4] = "GetChangeFlags"
   sResult[5] = "GetCoordinates"
   sResult[6] = "GetDestinationLoadDoor"
   sResult[7] = "GetEnableStateChildren"
   sResult[8] = "GetFurnitureAllMarkerCoordinates"
   sResult[9] = "GetFurnitureAllMarkerEntryPoints"
   sResult[10] = "GetFurnitureAllMarkerTypes"
   sResult[11] = "GetFurnitureEnabledMarkers"
   sResult[12] = "GetFurnitureMarkerCoordinates"
   sResult[13] = "GetFurnitureMarkerEntryPoints"
   sResult[14] = "GetFurnitureMarkerType"
   sResult[15] = "GetHeadingOffsetPosition"
   sResult[16] = "GetMotionType"
   sResult[17] = "GetPosition"
   sResult[18] = "GetRotation"
   sResult[19] = "GetTeleportMarkerCoordinates"
   sResult[20] = "HasExtraData"
   sResult[21] = "HasLocalCollision"
   sResult[22] = "HasNiControllerManager"
   sResult[23] = "IsCleanlyMovable"
   sResult[24] = "IsLoadDoor"
   sResult[25] = "IsMarker"
   sResult[26] = "IsTeleportMarkerInAttachedCell"
   sResult[27] = "MakeCollisionPrimitiveBox"
   sResult[28] = "MarkChanged"
   sResult[29] = "MoveTeleportMarkerToEditorLocOffset"
   sResult[30] = "Reload3D"
   sResult[31] = "ResetTeleportMarker"
   sResult[32] = "Unload3D"
   sResult[33] = "UnmarkChanged"
   Return sResult
EndFunction

State Method_BaseHasWhichKeyword
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 6
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[6]
      sNames[0] = "akSubject"
      sNames[1] = "akKeywords[0]"
      sNames[2] = "akKeywords[1]"
      sNames[3] = "akKeywords[2]"
      sNames[4] = "akKeywords[3]"
      sNames[5] = "akKeywords[4]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[6]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Keyword"
      sTypes[2] = "Keyword"
      sTypes[3] = "Keyword"
      sTypes[4] = "Keyword"
      sTypes[5] = "Keyword"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Keyword[] akKeywords = new Keyword[5]
      akKeywords[0] = s.pkParameters[1] as Keyword
      akKeywords[1] = s.pkParameters[2] as Keyword
      akKeywords[2] = s.pkParameters[3] as Keyword
      akKeywords[3] = s.pkParameters[4] as Keyword
      akKeywords[4] = s.pkParameters[5] as Keyword
      s.piReturnValue = CobbAPIObjectReference.BaseHasWhichKeyword(s.pkParameters[0] as ObjectReference, akKeywords)
   EndFunction
EndState
State Method_BaseHasWhichKeywordName
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 6
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[6]
      sNames[0] = "akSubject"
      sNames[1] = "asKeywords[0]"
      sNames[2] = "asKeywords[1]"
      sNames[3] = "asKeywords[2]"
      sNames[4] = "asKeywords[3]"
      sNames[5] = "asKeywords[4]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[6]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "String"
      sTypes[2] = "String"
      sTypes[3] = "String"
      sTypes[4] = "String"
      sTypes[5] = "String"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      String[] asKeywords = new String[5]
      asKeywords[0] = s.psParameters[1]
      asKeywords[1] = s.psParameters[2]
      asKeywords[2] = s.psParameters[3]
      asKeywords[3] = s.psParameters[4]
      asKeywords[4] = s.psParameters[5]
      s.piReturnValue = CobbAPIObjectReference.BaseHasWhichKeywordName(s.pkParameters[0] as ObjectReference, asKeywords)
   EndFunction
EndState
State Method_GetActivateParents
   String Function GetMethodReturnType()
      Return "ObjectReference[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "akOptionalBaseFormFilter"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Form"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      ObjectReference[] kResult = CobbAPIObjectReference.GetActivateParents(s.pkParameters[0] as ObjectReference, s.pkParameters[1])
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIObjectReference.GetBounds(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetChangeFlags
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIObjectReference.GetChangeFlags(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetCoordinates
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIObjectReference.GetCoordinates(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetDestinationLoadDoor
   String Function GetMethodReturnType()
      Return "ObjectReference"
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPIObjectReference.GetDestinationLoadDoor(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetEnableStateChildren
   String Function GetMethodReturnType()
      Return "ObjectReference[]"
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      ObjectReference[] kResult = CobbAPIObjectReference.GetEnableStateChildren(s.pkParameters[0] as ObjectReference)
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
State Method_GetFurnitureAllMarkerCoordinates
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akFurnitureReference"
      sNames[1] = "abWorldRelative"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIObjectReference.GetFurnitureAllMarkerCoordinates(s.pkParameters[0] as ObjectReference, s.pbParameters[1])
   EndFunction
EndState
State Method_GetFurnitureAllMarkerEntryPoints
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akFurnitureReference"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIObjectReference.GetFurnitureAllMarkerEntryPoints(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetFurnitureAllMarkerTypes
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akFurnitureReference"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIObjectReference.GetFurnitureAllMarkerTypes(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetFurnitureEnabledMarkers
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 1
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[1]
      sNames[0] = "akFurnitureReference"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[1]
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIObjectReference.GetFurnitureEnabledMarkers(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetFurnitureMarkerCoordinates
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akFurnitureReference"
      sNames[1] = "aiMarker"
      sNames[2] = "abWorldRelative"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Int"
      sTypes[2] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIObjectReference.GetFurnitureMarkerCoordinates(s.pkParameters[0] as ObjectReference, s.piParameters[1], s.pbParameters[2])
   EndFunction
EndState
State Method_GetFurnitureMarkerEntryPoints
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akFurnitureReference"
      sNames[1] = "aiMarker"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPIObjectReference.GetFurnitureMarkerEntryPoints(s.pkParameters[0] as ObjectReference, s.piParameters[1])
   EndFunction
EndState
State Method_GetFurnitureMarkerType
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akFurnitureReference"
      sNames[1] = "aiMarker"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIObjectReference.GetFurnitureMarkerType(s.pkParameters[0] as ObjectReference, s.piParameters[1])
   EndFunction
EndState
State Method_GetHeadingOffsetPosition
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "afHeading"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIObjectReference.GetHeadingOffsetPosition(s.pkParameters[0] as ObjectReference, s.pfParameters[1])
   EndFunction
EndState
State Method_GetMotionType
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPIObjectReference.GetMotionType(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetPosition
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIObjectReference.GetPosition(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetRotation
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIObjectReference.GetRotation(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_GetTeleportMarkerCoordinates
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPIObjectReference.GetTeleportMarkerCoordinates(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_HasExtraData
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiExtraDataType"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIObjectReference.HasExtraData(s.pkParameters[0] as ObjectReference, s.piParameters[1])
   EndFunction
EndState
State Method_HasLocalCollision
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIObjectReference.HasLocalCollision(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_HasNiControllerManager
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIObjectReference.HasNiControllerManager(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_IsCleanlyMovable
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIObjectReference.IsCleanlyMovable(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_IsLoadDoor
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIObjectReference.IsLoadDoor(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_IsMarker
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIObjectReference.IsMarker(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_IsTeleportMarkerInAttachedCell
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "akDestination"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPIObjectReference.IsTeleportMarkerInAttachedCell(s.pkParameters[0] as ObjectReference, s.pkParameters[1] as ObjectReference)
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = None
   EndFunction
EndState
State Method_MakeCollisionPrimitiveBox
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akSubject"
      sNames[1] = "aiCollisionLayer"
      sNames[2] = "afHalfwidths[0]"
      sNames[3] = "afHalfwidths[1]"
      sNames[4] = "afHalfwidths[2]"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Int"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Float[] afHalfwidths = new Float[3]
      afHalfwidths[0] = s.pfParameters[2]
      afHalfwidths[1] = s.pfParameters[3]
      afHalfwidths[2] = s.pfParameters[4]
      s.piReturnValue = CobbAPIObjectReference.MakeCollisionPrimitiveBox(s.pkParameters[0] as ObjectReference, s.piParameters[1], afHalfwidths)
   EndFunction
EndState
State Method_MarkChanged
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiFlags"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIObjectReference.MarkChanged(s.pkParameters[0] as ObjectReference, s.piParameters[1])
   EndFunction
EndState
State Method_MoveTeleportMarkerToEditorLocOffset
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIObjectReference.MoveTeleportMarkerToEditorLocOffset(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_Reload3D
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIObjectReference.Reload3D(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_ResetTeleportMarker
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIObjectReference.ResetTeleportMarker(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_Unload3D
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
      sTypes[0] = "ObjectReference"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIObjectReference.Unload3D(s.pkParameters[0] as ObjectReference)
   EndFunction
EndState
State Method_UnmarkChanged
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiFlags"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "ObjectReference"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPIObjectReference.UnmarkChanged(s.pkParameters[0] as ObjectReference, s.piParameters[1])
   EndFunction
EndState
