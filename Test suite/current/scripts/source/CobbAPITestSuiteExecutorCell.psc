Scriptname CobbAPITestSuiteExecutorCell extends CobbAPITestSuiteIExecutor Hidden

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
   Return "Cell"
EndFunction
String[] Function GetClassMethods()
   String[] sResult = new String[54]
   sResult[0] = "ForceUniqueLighting"
   sResult[1] = "GetAcousticSpace"
   sResult[2] = "GetAmbientColor"
   sResult[3] = "GetChanges"
   sResult[4] = "GetDirectionalAmbientColors"
   sResult[5] = "GetDirectionalColor"
   sResult[6] = "GetDirectionalFade"
   sResult[7] = "GetDirectionalRotation"
   sResult[8] = "GetEncounterZone"
   sResult[9] = "GetExteriorCellCoordinates"
   sResult[10] = "GetExteriorUnitCoordinates"
   sResult[11] = "GetFogColors"
   sResult[12] = "GetFogDistances"
   sResult[13] = "GetFogMax"
   sResult[14] = "GetFogPow"
   sResult[15] = "GetImageSpace"
   sResult[16] = "GetLightFadeDistances"
   sResult[17] = "GetLightingTemplate"
   sResult[18] = "GetLightingTemplateUsageDefaultFlag"
   sResult[19] = "GetLightingTemplateUsageDefaultFlags"
   sResult[20] = "GetLightingTemplateUsageFlag"
   sResult[21] = "GetLightingTemplateUsageFlags"
   sResult[22] = "GetMusicType"
   sResult[23] = "GetNorthRotation"
   sResult[24] = "GetParentWorldspace"
   sResult[25] = "IsFieldChanged"
   sResult[26] = "IsLightingTemplateUsageFlagChanged"
   sResult[27] = "IsPublic"
   sResult[28] = "ResetFields"
   sResult[29] = "ResetTemplateUsageFlags"
   sResult[30] = "SetAcousticSpace"
   sResult[31] = "SetAmbientColor"
   sResult[32] = "SetDirectionalAmbientColors"
   sResult[33] = "SetDirectionalColor"
   sResult[34] = "SetDirectionalFade"
   sResult[35] = "SetDirectionalRotation"
   sResult[36] = "SetFog"
   sResult[37] = "SetFogColors"
   sResult[38] = "SetFogDistanceClip"
   sResult[39] = "SetFogDistanceFar"
   sResult[40] = "SetFogDistanceNear"
   sResult[41] = "SetFogDistances"
   sResult[42] = "SetFogFarColor"
   sResult[43] = "SetFogMax"
   sResult[44] = "SetFogNearColor"
   sResult[45] = "SetFogPow"
   sResult[46] = "SetImageSpace"
   sResult[47] = "SetLightFadeDistances"
   sResult[48] = "SetLightingTemplate"
   sResult[49] = "SetLightingTemplateUsageFlags"
   sResult[50] = "SetMusicType"
   sResult[51] = "StopPersistingChangedTemplateUsageFlags"
   sResult[52] = "StopPersistingChangesTo"
   sResult[53] = "UsesSkyLighting"
   Return sResult
EndFunction

State Method_ForceUniqueLighting
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.ForceUniqueLighting(s.pkParameters[0] as Cell, s.pbParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = True
   EndFunction
EndState
State Method_GetAcousticSpace
   String Function GetMethodReturnType()
      Return "Form"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPICell.GetAcousticSpace(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetAmbientColor
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPICell.GetAmbientColor(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetChanges
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPICell.GetChanges(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_GetDirectionalAmbientColors
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPICell.GetDirectionalAmbientColors(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetDirectionalColor
   String Function GetMethodReturnType()
      Return "Int"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPICell.GetDirectionalColor(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetDirectionalFade
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPICell.GetDirectionalFade(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetDirectionalRotation
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPICell.GetDirectionalRotation(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetEncounterZone
   String Function GetMethodReturnType()
      Return "EncounterZone"
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPICell.GetEncounterZone(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_GetExteriorCellCoordinates
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPICell.GetExteriorCellCoordinates(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_GetExteriorUnitCoordinates
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPICell.GetExteriorUnitCoordinates(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_GetFogColors
   String Function GetMethodReturnType()
      Return "Int[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnArray = CobbAPICell.GetFogColors(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetFogDistances
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPICell.GetFogDistances(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetFogMax
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPICell.GetFogMax(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetFogPow
   String Function GetMethodReturnType()
      Return "Float"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPICell.GetFogPow(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetImageSpace
   String Function GetMethodReturnType()
      Return "Form"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPICell.GetImageSpace(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetLightFadeDistances
   String Function GetMethodReturnType()
      Return "Float[]"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnArray = CobbAPICell.GetLightFadeDistances(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetLightingTemplate
   String Function GetMethodReturnType()
      Return "Form"
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPICell.GetLightingTemplate(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_GetLightingTemplateUsageDefaultFlag
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiFlag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPICell.GetLightingTemplateUsageDefaultFlag(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
EndState
State Method_GetLightingTemplateUsageDefaultFlags
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPICell.GetLightingTemplateUsageDefaultFlags(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_GetLightingTemplateUsageFlag
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiFlag"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPICell.GetLightingTemplateUsageFlag(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
EndState
State Method_GetLightingTemplateUsageFlags
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.piReturnValue = CobbAPICell.GetLightingTemplateUsageFlags(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_GetMusicType
   String Function GetMethodReturnType()
      Return "MusicType"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiSource"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPICell.GetMusicType(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[1] = 1
   EndFunction
EndState
State Method_GetNorthRotation
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pfReturnValue = CobbAPICell.GetNorthRotation(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_GetParentWorldspace
   String Function GetMethodReturnType()
      Return "WorldSpace"
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pkReturnValue = CobbAPICell.GetParentWorldspace(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_IsFieldChanged
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiMask"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPICell.IsFieldChanged(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
EndState
State Method_IsLightingTemplateUsageFlagChanged
   String Function GetMethodReturnType()
      Return "Bool"
   EndFunction
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiMask"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPICell.IsLightingTemplateUsageFlagChanged(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
EndState
State Method_IsPublic
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPICell.IsPublic(s.pkParameters[0] as Cell)
   EndFunction
EndState
State Method_ResetFields
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiProperties"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.ResetFields(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
EndState
State Method_ResetTemplateUsageFlags
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiFlagsMask"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.ResetTemplateUsageFlags(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
EndState
State Method_SetAcousticSpace
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akSubject"
      sNames[1] = "akAcousticSpace"
      sNames[2] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Cell"
      sTypes[1] = "Form"
      sTypes[2] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetAcousticSpace(s.pkParameters[0] as Cell, s.pkParameters[1], s.pbParameters[2])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetAmbientColor
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akSubject"
      sNames[1] = "aiColorCode"
      sNames[2] = "abForce"
      sNames[3] = "abFixOther"
      sNames[4] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      sTypes[4] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetAmbientColor(s.pkParameters[0] as Cell, s.piParameters[1], s.pbParameters[2], s.pbParameters[3], s.pbParameters[4])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[3] = True
   EndFunction
EndState
State Method_SetDirectionalAmbientColors
   Int Function GetMethodParameterCount()
      Return 10
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[10]
      sNames[0] = "akSubject"
      sNames[1] = "aiColorCodes[0]"
      sNames[2] = "aiColorCodes[1]"
      sNames[3] = "aiColorCodes[2]"
      sNames[4] = "aiColorCodes[3]"
      sNames[5] = "aiColorCodes[4]"
      sNames[6] = "aiColorCodes[5]"
      sNames[7] = "abForce"
      sNames[8] = "abFixOther"
      sNames[9] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[10]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      sTypes[3] = "Int"
      sTypes[4] = "Int"
      sTypes[5] = "Int"
      sTypes[6] = "Int"
      sTypes[7] = "Bool"
      sTypes[8] = "Bool"
      sTypes[9] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Int[] aiColorCodes = new Int[6]
      aiColorCodes[0] = s.piParameters[1]
      aiColorCodes[1] = s.piParameters[2]
      aiColorCodes[2] = s.piParameters[3]
      aiColorCodes[3] = s.piParameters[4]
      aiColorCodes[4] = s.piParameters[5]
      aiColorCodes[5] = s.piParameters[6]
      CobbAPICell.SetDirectionalAmbientColors(s.pkParameters[0] as Cell, aiColorCodes, s.pbParameters[7], s.pbParameters[8], s.pbParameters[9])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[7] = True
      s.pbParameters[8] = True
   EndFunction
EndState
State Method_SetDirectionalColor
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akSubject"
      sNames[1] = "aiColorCode"
      sNames[2] = "abForce"
      sNames[3] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetDirectionalColor(s.pkParameters[0] as Cell, s.piParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetDirectionalFade
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akSubject"
      sNames[1] = "afFade"
      sNames[2] = "abForce"
      sNames[3] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Cell"
      sTypes[1] = "Float"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetDirectionalFade(s.pkParameters[0] as Cell, s.pfParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetDirectionalRotation
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akSubject"
      sNames[1] = "aiDegrees[0]"
      sNames[2] = "aiDegrees[1]"
      sNames[3] = "abForce"
      sNames[4] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      sTypes[3] = "Bool"
      sTypes[4] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      Int[] aiDegrees = new Int[2]
      aiDegrees[0] = s.piParameters[1]
      aiDegrees[1] = s.piParameters[2]
      CobbAPICell.SetDirectionalRotation(s.pkParameters[0] as Cell, aiDegrees, s.pbParameters[3], s.pbParameters[4])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[3] = True
      s.pbParameters[3] = True
   EndFunction
EndState
State Method_SetFog
   Int Function GetMethodParameterCount()
      Return 10
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[10]
      sNames[0] = "akSubject"
      sNames[1] = "aiColorCodeNear"
      sNames[2] = "aiColorCodeFar"
      sNames[3] = "afDistanceNear"
      sNames[4] = "afDistanceFar"
      sNames[5] = "afDistanceClip"
      sNames[6] = "afPow"
      sNames[7] = "afMax"
      sNames[8] = "abForce"
      sNames[9] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[10]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      sTypes[3] = "Float"
      sTypes[4] = "Float"
      sTypes[5] = "Float"
      sTypes[6] = "Float"
      sTypes[7] = "Float"
      sTypes[8] = "Bool"
      sTypes[9] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFog(s.pkParameters[0] as Cell, s.piParameters[1], s.piParameters[2], s.pfParameters[3], s.pfParameters[4], s.pfParameters[5], s.pfParameters[6], s.pfParameters[7], s.pbParameters[8], s.pbParameters[9])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[8] = True
      s.pbParameters[8] = True
   EndFunction
EndState
State Method_SetFogColors
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akSubject"
      sNames[1] = "aiColorCodeNear"
      sNames[2] = "aiColorCodeFar"
      sNames[3] = "abForce"
      sNames[4] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Int"
      sTypes[3] = "Bool"
      sTypes[4] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogColors(s.pkParameters[0] as Cell, s.piParameters[1], s.piParameters[2], s.pbParameters[3], s.pbParameters[4])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[3] = True
      s.pbParameters[3] = True
   EndFunction
EndState
State Method_SetFogDistanceClip
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akSubject"
      sNames[1] = "afDistance"
      sNames[2] = "abForce"
      sNames[3] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Cell"
      sTypes[1] = "Float"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogDistanceClip(s.pkParameters[0] as Cell, s.pfParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetFogDistanceFar
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akSubject"
      sNames[1] = "afDistance"
      sNames[2] = "abForce"
      sNames[3] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Cell"
      sTypes[1] = "Float"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogDistanceFar(s.pkParameters[0] as Cell, s.pfParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetFogDistanceNear
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akSubject"
      sNames[1] = "afDistance"
      sNames[2] = "abForce"
      sNames[3] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Cell"
      sTypes[1] = "Float"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogDistanceNear(s.pkParameters[0] as Cell, s.pfParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetFogDistances
   Int Function GetMethodParameterCount()
      Return 6
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[6]
      sNames[0] = "akSubject"
      sNames[1] = "afDistanceNear"
      sNames[2] = "afDistanceFar"
      sNames[3] = "afDistanceClip"
      sNames[4] = "abForce"
      sNames[5] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[6]
      sTypes[0] = "Cell"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Float"
      sTypes[4] = "Bool"
      sTypes[5] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogDistances(s.pkParameters[0] as Cell, s.pfParameters[1], s.pfParameters[2], s.pfParameters[3], s.pbParameters[4], s.pbParameters[5])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[4] = True
      s.pbParameters[4] = True
   EndFunction
EndState
State Method_SetFogFarColor
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akSubject"
      sNames[1] = "aiColorCode"
      sNames[2] = "abForce"
      sNames[3] = "abFixOther"
      sNames[4] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      sTypes[4] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogFarColor(s.pkParameters[0] as Cell, s.piParameters[1], s.pbParameters[2], s.pbParameters[3], s.pbParameters[4])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[3] = True
   EndFunction
EndState
State Method_SetFogMax
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akSubject"
      sNames[1] = "afValue"
      sNames[2] = "abForce"
      sNames[3] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Cell"
      sTypes[1] = "Float"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogMax(s.pkParameters[0] as Cell, s.pfParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetFogNearColor
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akSubject"
      sNames[1] = "aiColorCode"
      sNames[2] = "abForce"
      sNames[3] = "abFixOther"
      sNames[4] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      sTypes[4] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogNearColor(s.pkParameters[0] as Cell, s.piParameters[1], s.pbParameters[2], s.pbParameters[3], s.pbParameters[4])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[3] = True
   EndFunction
EndState
State Method_SetFogPow
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akSubject"
      sNames[1] = "afValue"
      sNames[2] = "abForce"
      sNames[3] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Cell"
      sTypes[1] = "Float"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetFogPow(s.pkParameters[0] as Cell, s.pfParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetImageSpace
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akSubject"
      sNames[1] = "akImageSpace"
      sNames[2] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Cell"
      sTypes[1] = "Form"
      sTypes[2] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetImageSpace(s.pkParameters[0] as Cell, s.pkParameters[1], s.pbParameters[2])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetLightFadeDistances
   Int Function GetMethodParameterCount()
      Return 5
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[5]
      sNames[0] = "akSubject"
      sNames[1] = "afDistanceBegin"
      sNames[2] = "afDistanceEnd"
      sNames[3] = "abForce"
      sNames[4] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[5]
      sTypes[0] = "Cell"
      sTypes[1] = "Float"
      sTypes[2] = "Float"
      sTypes[3] = "Bool"
      sTypes[4] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetLightFadeDistances(s.pkParameters[0] as Cell, s.pfParameters[1], s.pfParameters[2], s.pbParameters[3], s.pbParameters[4])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[3] = True
      s.pbParameters[3] = True
   EndFunction
EndState
State Method_SetLightingTemplate
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akSubject"
      sNames[1] = "akLightingTemplate"
      sNames[2] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Cell"
      sTypes[1] = "Form"
      sTypes[2] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetLightingTemplate(s.pkParameters[0] as Cell, s.pkParameters[1], s.pbParameters[2])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_SetLightingTemplateUsageFlags
   Int Function GetMethodParameterCount()
      Return 4
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[4]
      sNames[0] = "akSubject"
      sNames[1] = "aiFlagsMask"
      sNames[2] = "abStateToSet"
      sNames[3] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[4]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      sTypes[2] = "Bool"
      sTypes[3] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetLightingTemplateUsageFlags(s.pkParameters[0] as Cell, s.piParameters[1], s.pbParameters[2], s.pbParameters[3])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[3] = True
   EndFunction
EndState
State Method_SetMusicType
   Int Function GetMethodParameterCount()
      Return 3
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[3]
      sNames[0] = "akSubject"
      sNames[1] = "akMusicType"
      sNames[2] = "abPersist"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[3]
      sTypes[0] = "Cell"
      sTypes[1] = "MusicType"
      sTypes[2] = "Bool"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.SetMusicType(s.pkParameters[0] as Cell, s.pkParameters[1] as MusicType, s.pbParameters[2])
   EndFunction
   Function SetMethodDefaultParameters()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbParameters[2] = True
   EndFunction
EndState
State Method_StopPersistingChangedTemplateUsageFlags
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiFlagsMask"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.StopPersistingChangedTemplateUsageFlags(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
EndState
State Method_StopPersistingChangesTo
   Int Function GetMethodParameterCount()
      Return 2
   EndFunction
   String[] Function GetMethodParameterNames()
      String[] sNames = new String[2]
      sNames[0] = "akSubject"
      sNames[1] = "aiProperties"
      Return sNames
   EndFunction
   String[] Function GetMethodParameterTypes()
      String[] sTypes = new String[2]
      sTypes[0] = "Cell"
      sTypes[1] = "Int"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      CobbAPICell.StopPersistingChangesTo(s.pkParameters[0] as Cell, s.piParameters[1])
   EndFunction
EndState
State Method_UsesSkyLighting
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
      sTypes[0] = "Cell"
      Return sTypes
   EndFunction
   Function CallMethod()
      CobbAPITestSuiteExecutorState s = GetExecutorState()
      s.pbReturnValue = CobbAPICell.UsesSkyLighting(s.pkParameters[0] as Cell)
   EndFunction
EndState
