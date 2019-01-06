Scriptname CobbAPITestSuiteExecutor extends Quest Hidden

;/Block/; ; Boilerplate
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
   ;
   ;/Block/; ; Parameter setters
      Function SetParameterAlias(Int aiIndex, Alias aParameter)
         _aParameters[aiIndex] = aParameter
      EndFunction
      Function SetParameterBool(Int aiIndex, Bool aParameter)
         _bParameters[aiIndex] = aParameter
      EndFunction
      Function SetParameterFloat(Int aiIndex, Float aParameter)
         _fParameters[aiIndex] = aParameter
      EndFunction
      Function SetParameterForm(Int aiIndex, Form aParameter)
         _kParameters[aiIndex] = aParameter
      EndFunction
      Function SetParameterInt(Int aiIndex, Int aParameter)
         _iParameters[aiIndex] = aParameter
      EndFunction
      Function SetParameterString(Int aiIndex, String aParameter)
         _sParameters[aiIndex] = aParameter
      EndFunction
   ;/EndBlock/;
   ;/Block/; ; Parameter getters
      Alias Function GetParameterAlias(Int aiIndex)
         Return _aParameters[aiIndex]
      EndFunction
      Bool Function GetParameterBool(Int aiIndex)
         Return _bParameters[aiIndex]
      EndFunction
      Float Function GetParameterFloat(Int aiIndex)
         Return _fParameters[aiIndex]
      EndFunction
      Form Function GetParameterForm(Int aiIndex)
         Return _kParameters[aiIndex]
      EndFunction
      Int Function GetParameterInt(Int aiIndex)
         Return _iParameters[aiIndex]
      EndFunction
      String Function GetParameterString(Int aiIndex)
         Return _sParameters[aiIndex]
      EndFunction
   ;/EndBlock/;
   Function ResetParameters()
      _aParameters = new Alias[10]
      _bParameters = new Bool[10]
      _fParameters = new Float[10]
      _kParameters = new Form[10]
      _iParameters = new Int[10]
      _sParameters = new String[10]
      ;
      SetMethodDefaultParameters()
   EndFunction
   Function SetMethodDefaultParameters()
   EndFunction
   Function ResetContext()
      _aContext = None
      _eContext = None
      _kContext = None
   EndFunction
   Function CallMethod()
   EndFunction
   
   Alias             _aContext
   ActiveMagicEffect _eContext
   Form              _kContext
   
   Alias[]  _aParameters
   Bool[]   _bParameters
   Float[]  _fParameters
   Form[]   _kParameters
   Int[]    _iParameters
   String[] _sParameters

   Bool     _bReturnValue
   Float    _fReturnValue
   Float[]  _fReturnArray
   Form     _kReturnValue
   Form[]   _kReturnArray
   Int      _iReturnValue
   Int[]    _iReturnArray
   String   _sReturnValue
   String[] _sReturnArray

   ;/Block/; ; Context getters
      Alias Function GetContextAlias()
         Return _aContext
      EndFunction
      ActiveMagicEffect Function GetContextActiveMagicEffect()
         Return _eContext
      EndFunction
      Form Function GetContextForm()
         Return _kContext
      EndFunction
   ;/EndBlock/;
   ;/Block/; ; Return value getters
      Bool Function GetReturnValueBool()
         Return _bReturnValue
      EndFunction
      Float Function GetReturnValueFloat()
         Return _fReturnValue
      EndFunction
      Form Function GetReturnValueForm()
         Return _kReturnValue
      EndFunction
      Int Function GetReturnValueInt()
         Return _iReturnValue
      EndFunction
      String Function GetReturnValueString()
         Return _sReturnValue
      EndFunction
      Float[] Function GetReturnArrayFloat()
         Return _fReturnArray
      EndFunction
      Form[] Function GetReturnArrayForm()
         Return _kReturnArray
      EndFunction
      Int[] Function GetReturnArrayInt()
         Return _iReturnArray
      EndFunction
      String[] Function GetReturnArrayString()
         Return _sReturnArray
      EndFunction
   ;/EndBlock/;
;/EndBlock/;
;/Block/; ; Helpers
   Form[] Function RefArrayToFormArray(ObjectReference[] aArray)
      Int iLength   = aArray.Length
      Int iIterator = 0
      Form[] kOut = Utility.CreateFormArray(iLength)
      While iIterator < iLength
         kOut[iIterator] = aArray[iIterator]
         iIterator += 1
      EndWhile
      Return kOut
   EndFunction
;/EndBlock/;

;/Block/; ; Actor APIs
   State Class_Actor
      String[] Function GetClassMethods()
         String[] sResult = new String[13]
         sResult[0] = "GetActorValueRegenDelay"
         sResult[1] = "GetBleedoutTimer"
         sResult[2] = "Decapitate"
         sResult[3] = "IsDecapitated"
         sResult[4] = "GetHasBeenEaten"
         sResult[5] = "SetHasBeenEaten"
         sResult[6] = "GetSoulSize"
         sResult[7] = "IsHorse"
         sResult[8] = "CeaseDetectionEvent"
         sResult[9] = "GetDetectionEventAge"
         sResult[10] = "GetDetectionEventPosition"
         sResult[11] = "GetDetectionEventSoundLevel"
         sResult[12] = "HasDetectionEvent"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Actor_GetActorValueRegenDelay
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
         _fReturnValue = CobbAPIActor.GetActorValueRegenDelay(_kParameters[0] as Actor, _iParameters[1])
      EndFunction
   EndState
   State Method_Actor_GetBleedoutTimer
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
         _fReturnValue = CobbAPIActor.GetBleedoutTimer(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_Decapitate
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
         CobbAPIActor.Decapitate(_kParameters[0] as Actor, _bParameters[1])
      EndFunction
   EndState
   State Method_Actor_IsDecapitated
      String Function GetMethodReturnType()
         Return "Bool"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "akTarget"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Actor"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIActor.IsDecapitated(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_GetHasBeenEaten
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
         _bReturnValue = CobbAPIActor.GetHasBeenEaten(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_SetHasBeenEaten
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
         CobbAPIActor.SetHasBeenEaten(_kParameters[0] as Actor, _bParameters[1])
      EndFunction
      ;
      Function ResetParameters()
         _aParameters = new Alias[10]
         _bParameters = new Bool[10]
         _fParameters = new Float[10]
         _kParameters = new Form[10]
         _iParameters = new Int[10]
         _sParameters = new String[10]
         ;
         _bParameters[1] = True
      EndFunction
   EndState
   State Method_Actor_GetSoulSize
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
         _iReturnValue = CobbAPIActor.GetSoulSize(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_IsHorse
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
         _bReturnValue = CobbAPIActor.IsHorse(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_CeaseDetectionEvent
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
         CobbAPIActor.CeaseDetectionEvent(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_GetDetectionEventAge
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
         _fReturnValue = CobbAPIActor.GetDetectionEventAge(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_GetDetectionEventPosition
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
         _fReturnArray = CobbAPIActor.GetDetectionEventPosition(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_GetDetectionEventSoundLevel
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
         _fReturnValue = CobbAPIActor.GetDetectionEventSoundLevel(_kParameters[0] as Actor)
      EndFunction
   EndState
   State Method_Actor_HasDetectionEvent
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
         _bReturnValue = CobbAPIActor.HasDetectionEvent(_kParameters[0] as Actor)
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Alias APIs
   State Class_Alias
      String[] Function GetClassMethods()
         String[] sResult = new String[4]
         sResult[0] = "CreatesReference"
         sResult[1] = "GetRefAliasCreateAtAliasID"
         sResult[2] = "GetRefAliasFillType"
         sResult[3] = "IsPlayerAlias"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Alias_CreatesReference
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
         sTypes[0] = "Alias"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIAlias.CreatesReference(_aParameters[0] as ReferenceAlias)
      EndFunction
   EndState
   State Method_Alias_GetRefAliasCreateAtAliasID
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
         sTypes[0] = "Alias"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIAlias.GetRefAliasCreateAtAliasID(_aParameters[0] as ReferenceAlias)
      EndFunction
   EndState
   State Method_Alias_GetRefAliasFillType
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
         sTypes[0] = "Alias"
         Return sTypes
      EndFunction
      Function CallMethod()
         _iReturnValue = CobbAPIAlias.GetRefAliasFillType(_aParameters[0] as ReferenceAlias)
      EndFunction
   EndState
   State Method_Alias_IsPlayerAlias
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
         sTypes[0] = "Alias"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIAlias.IsPlayerAlias(_aParameters[0] as ReferenceAlias)
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Cell APIs
   State Class_Cell
      String[] Function GetClassMethods()
         String[] sResult = new String[36]
         sResult[0] = "ForceUniqueLighting"
         sResult[1] = "FromColorCode"
         sResult[2] = "GetAcousticSpace"
         sResult[3] = "GetEditorID"
         sResult[4] = "GetEncounterZone"
         sResult[5] = "GetExteriorCellCoordinates"
         sResult[6] = "GetExteriorUnitCoordinates"
         sResult[7] = "GetFogClipDistance"
         sResult[8] = "GetFogColors"
         sResult[9] = "GetFogDistances"
         sResult[10] = "GetFogMax"
         sResult[11] = "GetFogPower"
         sResult[12] = "GetImageSpace"
         sResult[13] = "GetLightingTemplate"
         sResult[14] = "GetLightingTemplateUsageFlag"
         sResult[15] = "GetLightingTemplateUsageFlags"
         sResult[16] = "GetMusicType"
         sResult[17] = "GetNorthRotation"
         sResult[18] = "GetParentWorldspace"
         sResult[19] = "IsPublic"
         sResult[20] = "SetAcousticSpace"
         sResult[21] = "SetFog"
         sResult[22] = "SetFogColors"
         sResult[23] = "SetFogDistanceClip"
         sResult[24] = "SetFogDistanceFar"
         sResult[25] = "SetFogDistanceNear"
         sResult[26] = "SetFogDistances"
         sResult[27] = "SetFogFarColor"
         sResult[28] = "SetFogMax"
         sResult[29] = "SetFogNearColor"
         sResult[30] = "SetFogPow"
         sResult[31] = "SetImageSpace"
         sResult[32] = "SetLightingTemplateUsageFlags"
         sResult[33] = "SetMusicType"
         sResult[34] = "ToColorCode"
         sResult[35] = "UsesSkyLighting"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Cell_ForceUniqueLighting
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[1] = "abReserved"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Cell"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICell.ForceUniqueLighting(_kParameters[0] as Cell, _bParameters[1])
      EndFunction
   EndState
   State Method_Cell_FromColorCode
      String Function GetMethodReturnType()
         Return "Int[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "aiCode"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         _iReturnArray = CobbAPICell.FromColorCode(_iParameters[0])
      EndFunction
   EndState
   State Method_Cell_GetAcousticSpace
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
         _bReturnValue = CobbAPICell.GetAcousticSpace(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetEditorID
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
         sTypes[0] = "Cell"
         Return sTypes
      EndFunction
      Function CallMethod()
         _sReturnValue = CobbAPICell.GetEditorID(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetEncounterZone
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
         _kReturnValue = CobbAPICell.GetEncounterZone(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetExteriorCellCoordinates
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
         _iReturnArray = CobbAPICell.GetExteriorCellCoordinates(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetExteriorUnitCoordinates
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
         _fReturnArray = CobbAPICell.GetExteriorUnitCoordinates(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetFogClipDistance
      String Function GetMethodReturnType()
         Return "Float"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[0] = "abHonorTemplate"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Cell"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnValue = CobbAPICell.GetFogClipDistance(_kParameters[0] as Cell, _bParameters[1])
      EndFunction
   EndState
   State Method_Cell_GetFogColors
      String Function GetMethodReturnType()
         Return "Int[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[0] = "abHonorTemplate"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Cell"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _iReturnArray = CobbAPICell.GetFogColors(_kParameters[0] as Cell, _bParameters[1])
      EndFunction
   EndState
   State Method_Cell_GetFogDistances
      String Function GetMethodReturnType()
         Return "Float[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[0] = "abHonorTemplate"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Cell"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnArray = CobbAPICell.GetFogDistances(_kParameters[0] as Cell, _bParameters[1])
      EndFunction
   EndState
   State Method_Cell_GetFogMax
      String Function GetMethodReturnType()
         Return "Float"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[0] = "abHonorTemplate"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Cell"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnValue = CobbAPICell.GetFogMax(_kParameters[0] as Cell, _bParameters[1])
      EndFunction
   EndState
   State Method_Cell_GetFogPower
      String Function GetMethodReturnType()
         Return "Float"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[0] = "abHonorTemplate"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Cell"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnValue = CobbAPICell.GetFogPower(_kParameters[0] as Cell, _bParameters[1])
      EndFunction
   EndState
   State Method_Cell_GetImageSpace
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
         _kReturnValue = CobbAPICell.GetImageSpace(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetLightingTemplate
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
         _kReturnValue = CobbAPICell.GetLightingTemplate(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetLightingTemplateUsageFlag
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
         _bReturnValue = CobbAPICell.GetLightingTemplateUsageFlag(_kParameters[0] as Cell, _iParameters[1])
      EndFunction
   EndState
   State Method_Cell_GetLightingTemplateUsageFlags
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
         _iReturnValue = CobbAPICell.GetLightingTemplateUsageFlags(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetMusicType
      String Function GetMethodReturnType()
         Return "MusicType"
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
         _kReturnValue = CobbAPICell.GetMusicType(_kParameters[0] as Cell) as Form
      EndFunction
   EndState
   State Method_Cell_GetNorthRotation
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
         _fReturnValue = CobbAPICell.GetNorthRotation(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_GetParentWorldspace
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
         _kReturnValue = CobbAPICell.GetParentWorldspace(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_IsPublic
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
         _bReturnValue = CobbAPICell.IsPublic(_kParameters[0] as Cell)
      EndFunction
   EndState
   State Method_Cell_SetAcousticSpace
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[3]
         sNames[0] = "akSubject"
         sNames[1] = "akAcousticSpace"
         sNames[2] = "abReserved"
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
         CobbAPICell.SetAcousticSpace(_kParameters[0] as Cell, _kParameters[1], _bParameters[2])
      EndFunction
   EndState
   State Method_Cell_SetFog
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
         sNames[9] = "abReserved"
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
         CobbAPICell.SetFog( \
            _kParameters[0] as Cell, \
            _iParameters[1], _iParameters[2], \
            _fParameters[3], _fParameters[4], _fParameters[5], \
            _fParameters[6], _fParameters[7], \
            _bParameters[8], _bParameters[9] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[8] = True
      EndFunction
   EndState
   State Method_Cell_SetFogColors
      Int Function GetMethodParameterCount()
         Return 5
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[5]
         sNames[0] = "akSubject"
         sNames[1] = "aiColorCodeNear"
         sNames[2] = "aiColorCodeFar"
         sNames[3] = "abForce"
         sNames[4] = "abReserved"
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
         CobbAPICell.SetFogColors( \
            _kParameters[0] as Cell, \
            _iParameters[1], _iParameters[2], \
            _bParameters[3], _bParameters[4] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[3] = True
      EndFunction
   EndState
   State Method_Cell_SetFogDistanceClip
      Int Function GetMethodParameterCount()
         Return 4
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[4]
         sNames[0] = "akSubject"
         sNames[1] = "afDistance"
         sNames[2] = "abForce"
         sNames[3] = "abReserved"
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
         CobbAPICell.SetFogDistanceClip( \
            _kParameters[0] as Cell, \
            _fParameters[1], \
            _bParameters[2], _bParameters[3] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[2] = True
      EndFunction
   EndState
   State Method_Cell_SetFogDistanceFar
      Int Function GetMethodParameterCount()
         Return 4
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[4]
         sNames[0] = "akSubject"
         sNames[1] = "afDistance"
         sNames[2] = "abForce"
         sNames[3] = "abReserved"
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
         CobbAPICell.SetFogDistanceFar( \
            _kParameters[0] as Cell, \
            _fParameters[1], \
            _bParameters[2], _bParameters[3] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[2] = True
      EndFunction
   EndState
   State Method_Cell_SetFogDistanceNear
      Int Function GetMethodParameterCount()
         Return 4
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[4]
         sNames[0] = "akSubject"
         sNames[1] = "afDistance"
         sNames[2] = "abForce"
         sNames[3] = "abReserved"
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
         CobbAPICell.SetFogDistanceNear( \
            _kParameters[0] as Cell, \
            _fParameters[1], \
            _bParameters[2], _bParameters[3] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[2] = True
      EndFunction
   EndState
   State Method_Cell_SetFogDistances
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
         sNames[5] = "abReserved"
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
         CobbAPICell.SetFogDistances( \
            _kParameters[0] as Cell, \
            _fParameters[1], _fParameters[2], _fParameters[3], \
            _bParameters[4], _bParameters[5] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[4] = True
      EndFunction
   EndState
   State Method_Cell_SetFogFarColor
      Int Function GetMethodParameterCount()
         Return 5
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[5]
         sNames[0] = "akSubject"
         sNames[1] = "aiColorCode"
         sNames[2] = "abForce"
         sNames[3] = "abFixOther"
         sNames[4] = "abReserved"
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
         CobbAPICell.SetFogFarColor( \
            _kParameters[0] as Cell, \
            _iParameters[1], \
            _bParameters[2], _bParameters[3], _bParameters[4] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[2] = True
      EndFunction
   EndState
   State Method_Cell_SetFogMax
      Int Function GetMethodParameterCount()
         Return 4
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[4]
         sNames[0] = "akSubject"
         sNames[1] = "afValue"
         sNames[2] = "abForce"
         sNames[3] = "abReserved"
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
         CobbAPICell.SetFogMax( \
            _kParameters[0] as Cell, \
            _fParameters[1], \
            _bParameters[2], _bParameters[3] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[2] = True
      EndFunction
   EndState
   State Method_Cell_SetFogNearColor
      Int Function GetMethodParameterCount()
         Return 5
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[5]
         sNames[0] = "akSubject"
         sNames[1] = "aiColorCode"
         sNames[2] = "abForce"
         sNames[3] = "abFixOther"
         sNames[4] = "abReserved"
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
         CobbAPICell.SetFogNearColor( \
            _kParameters[0] as Cell, \
            _iParameters[1], \
            _bParameters[2], _bParameters[3], _bParameters[4] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[2] = True
      EndFunction
   EndState
   State Method_Cell_SetFogPow
      Int Function GetMethodParameterCount()
         Return 4
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[4]
         sNames[0] = "akSubject"
         sNames[1] = "afValue"
         sNames[2] = "abForce"
         sNames[3] = "abReserved"
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
         CobbAPICell.SetFogPow( \
            _kParameters[0] as Cell, \
            _fParameters[1], \
            _bParameters[2], _bParameters[3] \
         )
      EndFunction
      Function SetMethodDefaultParameters()
         _bParameters[2] = True
      EndFunction
   EndState
   State Method_Cell_SetImageSpace
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[3]
         sNames[0] = "akSubject"
         sNames[1] = "akImageSpace"
         sNames[2] = "abReserved"
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
         CobbAPICell.SetImageSpace(_kParameters[0] as Cell, _kParameters[1], _bParameters[2])
      EndFunction
   EndState
   State Method_Cell_SetLightingTemplateUsageFlags
      Int Function GetMethodParameterCount()
         Return 4
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[4]
         sNames[0] = "akSubject"
         sNames[1] = "aiFlagsMask"
         sNames[2] = "abStateToSet"
         sNames[3] = "abReserved"
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
         CobbAPICell.SetLightingTemplateUsageFlags(_kParameters[0] as Cell, _iParameters[1], _bParameters[2], _bParameters[3])
      EndFunction
   EndState
   State Method_Cell_SetMusicType
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[3]
         sNames[0] = "akSubject"
         sNames[1] = "akMusicType"
         sNames[2] = "abReserved"
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
         CobbAPICell.SetMusicType(_kParameters[0] as Cell, _kParameters[1] as MusicType, _bParameters[2])
      EndFunction
   EndState
   State Method_Cell_ToColorCode
      String Function GetMethodReturnType()
         Return "Int"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[3]
         sNames[0] = "aiRed"
         sNames[1] = "aiGreen"
         sNames[2] = "aiBlue"
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
         _iReturnValue = CobbAPICell.ToColorCode(_iParameters[0], _iParameters[1], _iParameters[2])
      EndFunction
   EndState
   State Method_Cell_UsesSkyLighting
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
         _bReturnValue = CobbAPICell.UsesSkyLighting(_kParameters[0] as Cell)
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; CellSearcher APIs
   State Class_CellSearcher
      String[] Function GetClassMethods()
         String[] sResult = new String[13]
         sResult[0] = "Create"
         sResult[1] = "CreateFromArray"
         sResult[2] = "SetCell"
         sResult[3] = "SetTolerance"
         sResult[4] = "AddFormList"
         sResult[5] = "SetFilterMode"
         sResult[6] = "SetCriteriaA"
         sResult[7] = "SetCriteriaB"
         sResult[8] = "SetExcludesPlayer"
         sResult[9] = "SetRequiresCollision"
         sResult[10] = "SetSearchLoadedArea"
         sResult[11] = "Cancel"
         sResult[12] = "Run"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_CellSearcher_Create
      String Function GetMethodReturnType()
         Return "Int"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[3]
         sNames[0] = "x"
         sNames[1] = "y"
         sNames[2] = "z"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[3]
         sTypes[0] = "Float"
         sTypes[1] = "Float"
         sTypes[2] = "Float"
         Return sTypes
      EndFunction
      Function CallMethod()
         _iReturnValue = CobbAPICellSearcher.Create(_fParameters[0], _fParameters[1], _fParameters[2])
      EndFunction
   EndState
   State Method_CellSearcher_CreateFromArray
      String Function GetMethodReturnType()
         Return "Int"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[3]
         sNames[0] = "afSearchPoint[0]"
         sNames[1] = "afSearchPoint[1]"
         sNames[2] = "afSearchPoint[2]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[3]
         sTypes[0] = "Float"
         sTypes[1] = "Float"
         sTypes[2] = "Float"
         Return sTypes
      EndFunction
      Function CallMethod()
         Float[] afSearchPoint = new Float[3]
         afSearchPoint[0] = _fParameters[0]
         afSearchPoint[1] = _fParameters[1]
         afSearchPoint[2] = _fParameters[2]
         _iReturnValue = CobbAPICellSearcher.CreateFromArray(afSearchPoint)
      EndFunction
   EndState
   State Method_CellSearcher_SetCell
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiHandle"
         sNames[1] = "akCellToSearch"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "Cell"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.SetCell(_iParameters[0], _kParameters[1] as Cell)
      EndFunction
   EndState
   State Method_CellSearcher_SetTolerance
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiHandle"
         sNames[1] = "afTolerance"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "Float"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.SetTolerance(_iParameters[0], _fParameters[1])
      EndFunction
   EndState
   State Method_CellSearcher_AddFormList
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiHandle"
         sNames[1] = "akFormList"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "FormList"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.AddFormList(_iParameters[0], _kParameters[1] as FormList)
      EndFunction
   EndState
   State Method_CellSearcher_SetFilterMode
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiHandle"
         sNames[1] = "aiFilterMode"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.SetFilterMode(_iParameters[0], _iParameters[1])
      EndFunction
   EndState
   State Method_CellSearcher_SetCriteriaA
      Int Function GetMethodParameterCount()
         Return 10
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[10]
         sNames[0] = "aiHandle"
         sNames[1] = "aiIncludeACTI"
         sNames[2] = "aiIncludeCONT"
         sNames[3] = "aiIncludeDOOR"
         sNames[4] = "aiIncludeFURN"
         sNames[5] = "aiIncludeLIGH"
         sNames[6] = "aiIncludeMSTT"
         sNames[7] = "aiIncludeNPC_"
         sNames[8] = "aiIncludeSTAT"
         sNames[9] = "aiIncludeTREE"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[10]
         sTypes[0] = "Int"
         sTypes[1] = "Int"
         sTypes[2] = "Int"
         sTypes[3] = "Int"
         sTypes[4] = "Int"
         sTypes[5] = "Int"
         sTypes[6] = "Int"
         sTypes[7] = "Int"
         sTypes[8] = "Int"
         sTypes[9] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.SetCriteriaA(_iParameters[0], _iParameters[1], _iParameters[2], _iParameters[3], _iParameters[4], _iParameters[5], _iParameters[6], _iParameters[7], _iParameters[8], _iParameters[9])
      EndFunction
      ;
      Function ResetParameters()
         _aParameters = new Alias[10]
         _bParameters = new Bool[10]
         _fParameters = new Float[10]
         _kParameters = new Form[10]
         _iParameters = new Int[10]
         _sParameters = new String[10]
         ;
         _iParameters[1] = -1
         _iParameters[2] = -1
         _iParameters[3] = -1
         _iParameters[4] = -1
         _iParameters[5] = -1
         _iParameters[6] = -1
         _iParameters[7] = -1
         _iParameters[8] = -1
         _iParameters[9] = -1
      EndFunction
   EndState
   State Method_CellSearcher_SetCriteriaB
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiHandle"
         sNames[1] = "aiIncludeItem"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.SetCriteriaB(_iParameters[0], _iParameters[1])
      EndFunction
      ;
      Function ResetParameters()
         _aParameters = new Alias[10]
         _bParameters = new Bool[10]
         _fParameters = new Float[10]
         _kParameters = new Form[10]
         _iParameters = new Int[10]
         _sParameters = new String[10]
         ;
         _iParameters[1] = -1
      EndFunction
   EndState
   State Method_CellSearcher_SetExcludesPlayer
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiHandle"
         sNames[1] = "abExcludesPlayer"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.SetExcludesPlayer(_iParameters[0], _bParameters[1])
      EndFunction
   EndState
   State Method_CellSearcher_SetRequiresCollision
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiHandle"
         sNames[1] = "abRequireCollision"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.SetRequiresCollision(_iParameters[0], _bParameters[1])
      EndFunction
   EndState
   State Method_CellSearcher_SetSearchLoadedArea
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiHandle"
         sNames[1] = "abSearchLoadedArea"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.SetSearchLoadedArea(_iParameters[0], _bParameters[1])
      EndFunction
   EndState
   State Method_CellSearcher_Cancel
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "aiHandle"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPICellSearcher.Cancel(_iParameters[0])
      EndFunction
   EndState
   State Method_CellSearcher_Run
      String Function GetMethodReturnType()
         Return "ObjectReference"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "aiHandle"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         _kReturnValue = CobbAPICellSearcher.Run(_iParameters[0])
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Form APIs
   State Class_Form
      String[] Function GetClassMethods()
         String[] sResult = new String[2]
         sResult[0] = "GetBounds"
         sResult[1] = "IsDefiniteItem"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Form_GetBounds
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
         _iReturnArray = CobbAPIForm.GetBounds(_kParameters[0])
      EndFunction
   EndState
   State Method_Form_IsDefiniteItem
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
         _bReturnValue = CobbAPIForm.IsDefiniteItem(_kParameters[0])
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Furniture APIs
   State Class_Furniture
      String[] Function GetClassMethods()
         String[] sResult = new String[7]
         sResult[0] = "GetAssociatedSpell"
         sResult[1] = "GetEnabledMarkers"
         sResult[2] = "GetMarkerEntryPoints"
         sResult[3] = "GetMarkerKeyword"
         sResult[4] = "GetMustExitToTalk"
         sResult[5] = "GetWorkbenchSkill"
         sResult[6] = "GetWorkbenchType"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Furniture_GetAssociatedSpell
      String Function GetMethodReturnType()
         Return "Spell"
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
         sTypes[0] = "Furniture"
         Return sTypes
      EndFunction
      Function CallMethod()
         _kReturnValue = CobbAPIFurniture.GetAssociatedSpell(_kParameters[0] as Furniture)
      EndFunction
   EndState
   State Method_Furniture_GetEnabledMarkers
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
         sTypes[0] = "Furniture"
         Return sTypes
      EndFunction
      Function CallMethod()
         _iReturnArray = CobbAPIFurniture.GetEnabledMarkers(_kParameters[0] as Furniture)
      EndFunction
   EndState
   State Method_Furniture_GetMarkerEntryPoints
      String Function GetMethodReturnType()
         Return "Int[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[1] = "aiMarker"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Furniture"
         sTypes[1] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         _iReturnArray = CobbAPIFurniture.GetMarkerEntryPoints(_kParameters[0] as Furniture, _iParameters[1])
      EndFunction
   EndState
   State Method_Furniture_GetMarkerKeyword
      String Function GetMethodReturnType()
         Return "Keyword"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[1] = "aiMarker"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Furniture"
         sTypes[1] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         _kReturnValue = CobbAPIFurniture.GetMarkerKeyword(_kParameters[0] as Furniture, _iParameters[1])
      EndFunction
   EndState
   State Method_Furniture_GetMustExitToTalk
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
         sTypes[0] = "Furniture"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIFurniture.GetMustExitToTalk(_kParameters[0] as Furniture)
      EndFunction
   EndState
   State Method_Furniture_GetWorkbenchSkill
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
         sTypes[0] = "Furniture"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIFurniture.GetWorkbenchSkill(_kParameters[0] as Furniture)
      EndFunction
   EndState
   State Method_Furniture_GetWorkbenchType
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
         sTypes[0] = "Furniture"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIFurniture.GetWorkbenchType(_kParameters[0] as Furniture)
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Game APIs
   State Class_Game
      String[] Function GetClassMethods()
         String[] sResult = new String[3]
         sResult[0] = "GetFormFromFile"
         sResult[1] = "IsPlayerCollisionEnabled"
         sResult[2] = "SetPlayerCollisionEnabled"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Furniture_GetFormFromFile
      String Function GetMethodReturnType()
         Return "Form"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "aiFormID"
         sNames[1] = "asFileName"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Int"
         sTypes[1] = "String"
         Return sTypes
      EndFunction
      Function CallMethod()
         _kReturnValue = CobbAPIGame.GetFormFromFile(_iParameters[0], _sParameters[1])
      EndFunction
   EndState
   State Method_Furniture_IsPlayerCollisionEnabled
      String Function GetMethodReturnType()
         Return "Bool"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 0
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIGame.IsPlayerCollisionEnabled()
      EndFunction
   EndState
   State Method_Furniture_SetPlayerCollisionEnabled
      String Function GetMethodReturnType()
         Return "Form"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "abEnabled"
         sNames[1] = "abDoFullProcessing"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "Bool"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _kReturnValue = CobbAPIGame.SetPlayerCollisionEnabled(_bParameters[0], _bParameters[1])
      EndFunction
      ;
      Function ResetParameters()
         _aParameters = new Alias[10]
         _bParameters = new Bool[10]
         _fParameters = new Float[10]
         _kParameters = new Form[10]
         _iParameters = new Int[10]
         _sParameters = new String[10]
         ;
         _bParameters[1] = True
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Light APIs
   State Class_Light
      String[] Function GetClassMethods()
         String[] sResult = new String[5]
         sResult[0] = "GetDepthBias"
         sResult[1] = "GetFade"
         sResult[2] = "GetFOV"
         sResult[3] = "GetRadius"
         sResult[4] = "SetFade"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Furniture_GetDepthBias
      String Function GetMethodReturnType()
         Return "Float"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "akLightReference"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "ObjectReference"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnValue = CobbAPILight.GetDepthBias(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetFade
      String Function GetMethodReturnType()
         Return "Float"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akLightReference"
         sNames[1] = "abRelative"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "ObjectReference"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnValue = CobbAPILight.GetFade(_kParameters[0] as ObjectReference, _bParameters[1])
      EndFunction
   EndState
   State Method_Furniture_GetFOV
      String Function GetMethodReturnType()
         Return "Float"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akLightReference"
         sNames[1] = "abRelative"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "ObjectReference"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnValue = CobbAPILight.GetFOV(_kParameters[0] as ObjectReference, _bParameters[1])
      EndFunction
   EndState
   State Method_Furniture_GetRadius
      String Function GetMethodReturnType()
         Return "Float"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akLightReference"
         sNames[1] = "abRelative"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "ObjectReference"
         sTypes[1] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnValue = CobbAPILight.GetRadius(_kParameters[0] as ObjectReference, _bParameters[1])
      EndFunction
   EndState
   State Method_Furniture_SetFade
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akLightReference"
         sNames[1] = "afFade"
         sNames[2] = "abRelative"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "ObjectReference"
         sTypes[1] = "Float"
         sTypes[2] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPILight.SetFade(_kParameters[0] as ObjectReference, _fParameters[1], _bParameters[2])
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Message APIs
   State Class_Message
      String[] Function GetClassMethods()
         String[] sResult = new String[2]
         sResult[0] = "GetButtonLabels"
         sResult[1] = "WillCrashDueToLength"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Message_GetButtonLabels
      String Function GetMethodReturnType()
         Return "String[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "akMessage"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Message"
         Return sTypes
      EndFunction
      Function CallMethod()
         _sReturnArray = CobbAPIMessage.GetButtonLabels(_kParameters[0] as Message)
      EndFunction
   EndState
   State Method_Message_WillCrashDueToLength
      String Function GetMethodReturnType()
         Return "Bool"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "akMessage"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Message"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIMessage.WillCrashDueToLength(_kParameters[0] as Message)
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; ObjectReference APIs
   State Class_ObjectReference
      String[] Function GetClassMethods()
         String[] sResult = new String[36]
         sResult[0] = "BaseHasWhichKeyword"
         sResult[1] = "BaseHasWhichKeywordName"
         sResult[2] = "GetActivateParents"
         sResult[3] = "GetBounds"
         sResult[4] = "GetBoundsToHalfwidthOffsets"
         sResult[5] = "GetBoundsToHalfwidths"
         sResult[6] = "GetChangeFlags"
         sResult[7] = "GetCoordinates"
         sResult[8] = "GetDestinationLoadDoor"
         sResult[9] = "GetEnableStateChildren"
         sResult[10] = "GetFurnitureAllMarkerCoordinates"
         sResult[11] = "GetFurnitureAllMarkerEntryPoints"
         sResult[12] = "GetFurnitureAllMarkerTypes"
         sResult[13] = "GetFurnitureEnabledMarkers"
         sResult[14] = "GetFurnitureMarkerCoordinates"
         sResult[15] = "GetFurnitureMarkerEntryPoints"
         sResult[16] = "GetFurnitureMarkerType"
         sResult[17] = "GetHeadingOffsetPosition"
         sResult[18] = "GetMotionType"
         sResult[19] = "GetPosition"
         sResult[20] = "GetRotation"
         sResult[21] = "GetTeleportMarkerCoordinates"
         sResult[22] = "HasExtraData"
         sResult[23] = "HasLocalCollision"
         sResult[24] = "HasNiControllerManager"
         sResult[25] = "IsCleanlyMovable"
         sResult[26] = "IsLoadDoor"
         sResult[27] = "IsMarker"
         sResult[28] = "IsTeleportMarkerInAttachedCell"
         sResult[29] = "MakeCollisionPrimitiveBox"
         sResult[30] = "MarkChanged"
         sResult[31] = "MoveTeleportMarkerToEditorLocOffset"
         sResult[32] = "Reload3D"
         sResult[33] = "ResetTeleportMarker"
         sResult[34] = "Unload3D"
         sResult[35] = "UnmarkChanged"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Furniture_BaseHasWhichKeyword
      String Function GetMethodReturnType()
         Return "Int"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 10
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[10]
         sNames[0] = "akSubject"
         sNames[1] = "akKeywords[0]"
         sNames[2] = "akKeywords[1]"
         sNames[3] = "akKeywords[2]"
         sNames[4] = "akKeywords[3]"
         sNames[5] = "akKeywords[4]"
         sNames[6] = "akKeywords[5]"
         sNames[7] = "akKeywords[6]"
         sNames[8] = "akKeywords[7]"
         sNames[9] = "akKeywords[8]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[10]
         sTypes[0] = "ObjectReference"
         sTypes[1] = "Keyword"
         sTypes[2] = "Keyword"
         sTypes[3] = "Keyword"
         sTypes[4] = "Keyword"
         sTypes[5] = "Keyword"
         sTypes[6] = "Keyword"
         sTypes[7] = "Keyword"
         sTypes[8] = "Keyword"
         sTypes[9] = "Keyword"
         Return sTypes
      EndFunction
      Function CallMethod()
         Keyword[] akKeywords = new Keyword[9]
         akKeywords[0] = _kParameters[1] as Keyword
         akKeywords[1] = _kParameters[2] as Keyword
         akKeywords[2] = _kParameters[3] as Keyword
         akKeywords[3] = _kParameters[4] as Keyword
         akKeywords[4] = _kParameters[5] as Keyword
         akKeywords[5] = _kParameters[6] as Keyword
         akKeywords[6] = _kParameters[7] as Keyword
         akKeywords[7] = _kParameters[8] as Keyword
         akKeywords[8] = _kParameters[9] as Keyword
         _iReturnValue = CobbAPIObjectReference.BaseHasWhichKeyword(_kParameters[0] as ObjectReference, akKeywords)
      EndFunction
   EndState
   State Method_Furniture_BaseHasWhichKeywordName
      String Function GetMethodReturnType()
         Return "Int"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 10
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[10]
         sNames[0] = "akSubject"
         sNames[1] = "asKeywords[0]"
         sNames[2] = "asKeywords[1]"
         sNames[3] = "asKeywords[2]"
         sNames[4] = "asKeywords[3]"
         sNames[5] = "asKeywords[4]"
         sNames[6] = "asKeywords[5]"
         sNames[7] = "asKeywords[6]"
         sNames[8] = "asKeywords[7]"
         sNames[9] = "asKeywords[8]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[10]
         sTypes[0] = "ObjectReference"
         sTypes[1] = "String"
         sTypes[2] = "String"
         sTypes[3] = "String"
         sTypes[4] = "String"
         sTypes[5] = "String"
         sTypes[6] = "String"
         sTypes[7] = "String"
         sTypes[8] = "String"
         sTypes[9] = "String"
         Return sTypes
      EndFunction
      Function CallMethod()
         String[] asKeywords = new String[9]
         asKeywords[0] = _sParameters[1]
         asKeywords[1] = _sParameters[2]
         asKeywords[2] = _sParameters[3]
         asKeywords[3] = _sParameters[4]
         asKeywords[4] = _sParameters[5]
         asKeywords[5] = _sParameters[6]
         asKeywords[6] = _sParameters[7]
         asKeywords[7] = _sParameters[8]
         asKeywords[8] = _sParameters[9]
         _iReturnValue = CobbAPIObjectReference.BaseHasWhichKeywordName(_kParameters[0] as ObjectReference, asKeywords)
      EndFunction
   EndState
   State Method_Furniture_GetActivateParents
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
         _kReturnArray = RefArrayToFormArray(CobbAPIObjectReference.GetActivateParents(_kParameters[0] as ObjectReference))
      EndFunction
   EndState
   State Method_Furniture_GetBounds
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
         _iReturnArray = CobbAPIObjectReference.GetBounds(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetBoundsToHalfwidthOffsets
      String Function GetMethodReturnType()
         Return "Float[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "akBaseForm"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Form"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnArray = CobbAPIObjectReference.GetBoundsToHalfwidthOffsets(_kParameters[0])
      EndFunction
   EndState
   State Method_Furniture_GetBoundsToHalfwidths
      String Function GetMethodReturnType()
         Return "Float[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "akBaseForm"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Form"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnArray = CobbAPIObjectReference.GetBoundsToHalfwidths(_kParameters[0])
      EndFunction
   EndState
   State Method_Furniture_GetChangeFlags
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
         _iReturnValue = CobbAPIObjectReference.GetChangeFlags(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetCoordinates
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
         _fReturnArray = CobbAPIObjectReference.GetCoordinates(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetDestinationLoadDoor
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
         _kReturnValue = CobbAPIObjectReference.GetDestinationLoadDoor(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetEnableStateChildren
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
         _kReturnArray = RefArrayToFormArray(CobbAPIObjectReference.GetEnableStateChildren(_kParameters[0] as ObjectReference))
      EndFunction
   EndState
   State Method_Furniture_GetFurnitureAllMarkerCoordinates
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
         _fReturnArray = CobbAPIObjectReference.GetFurnitureAllMarkerCoordinates(_kParameters[0] as ObjectReference, _bParameters[1])
      EndFunction
   EndState
   State Method_Furniture_GetFurnitureAllMarkerEntryPoints
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
         _iReturnArray = CobbAPIObjectReference.GetFurnitureAllMarkerEntryPoints(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetFurnitureAllMarkerTypes
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
         _iReturnArray = CobbAPIObjectReference.GetFurnitureAllMarkerTypes(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetFurnitureEnabledMarkers
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
         _iReturnArray = CobbAPIObjectReference.GetFurnitureEnabledMarkers(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetFurnitureMarkerCoordinates
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
         _fReturnArray = CobbAPIObjectReference.GetFurnitureMarkerCoordinates(_kParameters[0] as ObjectReference, _iParameters[1], _bParameters[2])
      EndFunction
   EndState
   State Method_Furniture_GetFurnitureMarkerEntryPoints
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
         _iReturnArray = CobbAPIObjectReference.GetFurnitureMarkerEntryPoints(_kParameters[0] as ObjectReference, _iParameters[1])
      EndFunction
   EndState
   State Method_Furniture_GetFurnitureMarkerType
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
         _iReturnValue = CobbAPIObjectReference.GetFurnitureMarkerType(_kParameters[0] as ObjectReference, _iParameters[1])
      EndFunction
   EndState
   State Method_Furniture_GetHeadingOffsetPosition
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
         _fReturnArray = CobbAPIObjectReference.GetHeadingOffsetPosition(_kParameters[0] as ObjectReference, _fParameters[1])
      EndFunction
   EndState
   State Method_Furniture_GetMotionType
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
         _iReturnValue = CobbAPIObjectReference.GetMotionType(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetPosition
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
         _fReturnArray = CobbAPIObjectReference.GetPosition(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetRotation
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
         _fReturnArray = CobbAPIObjectReference.GetRotation(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_GetTeleportMarkerCoordinates
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
         _fReturnArray = CobbAPIObjectReference.GetTeleportMarkerCoordinates(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_HasExtraData
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
         _bReturnValue = CobbAPIObjectReference.HasExtraData(_kParameters[0] as ObjectReference, _iParameters[1])
      EndFunction
   EndState
   State Method_Furniture_HasLocalCollision
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
         _bReturnValue = CobbAPIObjectReference.HasLocalCollision(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_HasNiControllerManager
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
         _bReturnValue = CobbAPIObjectReference.HasNiControllerManager(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_IsCleanlyMovable
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
         _bReturnValue = CobbAPIObjectReference.IsCleanlyMovable(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_IsLoadDoor
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
         _bReturnValue = CobbAPIObjectReference.IsLoadDoor(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_IsMarker
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
         _bReturnValue = CobbAPIObjectReference.IsMarker(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_IsTeleportMarkerInAttachedCell
      String Function GetMethodReturnType()
         Return "Bool"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 2
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[2]
         sNames[0] = "akSubject"
         sNames[1] = "akDestinationOptional"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[2]
         sTypes[0] = "ObjectReference"
         sTypes[1] = "ObjectReference"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIObjectReference.IsTeleportMarkerInAttachedCell(_kParameters[0] as ObjectReference, _kParameters[1] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_MakeCollisionPrimitiveBox
      String Function GetMethodReturnType()
         Return "Bool"
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
         Float[] afHalfwidths = new Float[3]
         afHalfwidths[0] = _fParameters[2]
         afHalfwidths[1] = _fParameters[3]
         afHalfwidths[2] = _fParameters[4]
         _bReturnValue = CobbAPIObjectReference.MakeCollisionPrimitiveBox(_kParameters[0] as ObjectReference, _iParameters[1], afHalfwidths)
      EndFunction
   EndState
   State Method_Furniture_MarkChanged
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
         _bReturnValue = CobbAPIObjectReference.MarkChanged(_kParameters[0] as ObjectReference, _iParameters[1])
      EndFunction
   EndState
   State Method_Furniture_MoveTeleportMarkerToEditorLocOffset
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
         _bReturnValue = CobbAPIObjectReference.MoveTeleportMarkerToEditorLocOffset(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_Reload3D
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
         _bReturnValue = CobbAPIObjectReference.Reload3D(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_ResetTeleportMarker
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
         _bReturnValue = CobbAPIObjectReference.ResetTeleportMarker(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_Unload3D
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
         _bReturnValue = CobbAPIObjectReference.Unload3D(_kParameters[0] as ObjectReference)
      EndFunction
   EndState
   State Method_Furniture_UnmarkChanged
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
         _bReturnValue = CobbAPIObjectReference.UnmarkChanged(_kParameters[0] as ObjectReference, _iParameters[1])
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; RevealService APIs
   State Class_RevealService
      String[] Function GetClassMethods()
         String[] sResult = new String[8]
         sResult[0] = "GetLightRevealState"
         sResult[1] = "GetMarkerRevealState"
         sResult[2] = "RegisterMarker"
         sResult[3] = "RegisterMarkersInBulk"
         sResult[4] = "SetAllRevealStates"
         sResult[5] = "SetLightModels"
         sResult[6] = "SetLightRevealState"
         sResult[7] = "SetMarkerRevealState"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_RevealService_GetLightRevealState
      String Function GetMethodReturnType()
         Return "Bool"
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIRevealService.GetLightRevealState()
      EndFunction
   EndState
   State Method_RevealService_GetMarkerRevealState
      String Function GetMethodReturnType()
         Return "Bool"
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIRevealService.GetMarkerRevealState()
      EndFunction
   EndState
   State Method_RevealService_RegisterMarker
      Int Function GetMethodParameterCount()
         Return 8
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[8]
         sNames[0] = "akBaseForm"
         sNames[1] = "asRevealPath"
         sNames[2] = "aiBounds[0]"
         sNames[3] = "aiBounds[1]"
         sNames[4] = "aiBounds[2]"
         sNames[5] = "aiBounds[3]"
         sNames[6] = "aiBounds[4]"
         sNames[7] = "aiBounds[5]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[8]
         sTypes[0] = "Form"
         sTypes[1] = "String"
         sTypes[2] = "Int"
         sTypes[3] = "Int"
         sTypes[4] = "Int"
         sTypes[5] = "Int"
         sTypes[6] = "Int"
         sTypes[7] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         Int[] aiBounds = new Int[6]
         aiBounds[0] = _iParameters[2]
         aiBounds[1] = _iParameters[3]
         aiBounds[2] = _iParameters[4]
         aiBounds[3] = _iParameters[5]
         aiBounds[4] = _iParameters[6]
         aiBounds[5] = _iParameters[7]
         CobbAPIRevealService.RegisterMarker(_kParameters[0], _sParameters[1], aiBounds)
      EndFunction
   EndState
   State Method_RevealService_RegisterMarkersInBulk
      Int Function GetMethodParameterCount()
         Return 16
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[16]
         sNames[0] = "akBaseForms[0]"
         sNames[1] = "akBaseForms[1]"
         sNames[2] = "asRevealPaths[0]"
         sNames[3] = "asRevealPaths[1]"
         sNames[4] = "aiBounds[0]"
         sNames[5] = "aiBounds[1]"
         sNames[6] = "aiBounds[2]"
         sNames[7] = "aiBounds[3]"
         sNames[8] = "aiBounds[4]"
         sNames[9] = "aiBounds[5]"
         sNames[10] = "aiBounds[6]"
         sNames[11] = "aiBounds[7]"
         sNames[12] = "aiBounds[8]"
         sNames[13] = "aiBounds[9]"
         sNames[14] = "aiBounds[10]"
         sNames[15] = "aiBounds[11]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[16]
         sTypes[0] = "Form"
         sTypes[1] = "Form"
         sTypes[2] = "String"
         sTypes[3] = "String"
         sTypes[4] = "Int"
         sTypes[5] = "Int"
         sTypes[6] = "Int"
         sTypes[7] = "Int"
         sTypes[8] = "Int"
         sTypes[9] = "Int"
         sTypes[10] = "Int"
         sTypes[11] = "Int"
         sTypes[12] = "Int"
         sTypes[13] = "Int"
         sTypes[14] = "Int"
         sTypes[15] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         Form[]   akBaseForms = new Form[2]
         String[] asRevealPaths = new String[2]
         Int[]    aiBounds = new Int[12]
         akBaseForms[0] = _kParameters[0]
         akBaseForms[1] = _kParameters[1]
         asRevealPaths[0] = _sParameters[2]
         asRevealPaths[1] = _sParameters[3]
         aiBounds[0] = _iParameters[4]
         aiBounds[1] = _iParameters[5]
         aiBounds[2] = _iParameters[6]
         aiBounds[3] = _iParameters[7]
         aiBounds[4] = _iParameters[8]
         aiBounds[5] = _iParameters[9]
         aiBounds[6] = _iParameters[10]
         aiBounds[7] = _iParameters[11]
         aiBounds[8] = _iParameters[12]
         aiBounds[9] = _iParameters[13]
         aiBounds[10] = _iParameters[14]
         aiBounds[11] = _iParameters[15]
         CobbAPIRevealService.RegisterMarkersInBulk(akBaseForms, asRevealPaths, aiBounds)
      EndFunction
   EndState
   State Method_RevealService_SetAllRevealStates
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "abState"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPIRevealService.SetAllRevealStates(_bParameters[0])
      EndFunction
   EndState
   State Method_RevealService_SetLightModels
      Int Function GetMethodParameterCount()
         Return 5
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[5]
         sNames[0] = "akOmni"
         sNames[1] = "akOmniShadow"
         sNames[2] = "akOmniHalf"
         sNames[3] = "akSpot"
         sNames[4] = "akSpotShadow"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[5]
         sTypes[0] = "Activator"
         sTypes[1] = "Activator"
         sTypes[2] = "Activator"
         sTypes[3] = "Activator"
         sTypes[4] = "Activator"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPIRevealService.SetLightModels( \
            _kParameters[0] as Activator, \
            _kParameters[1] as Activator, \
            _kParameters[2] as Activator, \
            _kParameters[3] as Activator, \
            _kParameters[4] as Activator \
         )
      EndFunction
   EndState
   State Method_RevealService_SetLightRevealState
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "abState"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPIRevealService.SetLightRevealState(_bParameters[0])
      EndFunction
   EndState
   State Method_RevealService_SetMarkerRevealState
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "abState"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         CobbAPIRevealService.SetMarkerRevealState(_bParameters[0])
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Rotations APIs
   State Class_Rotations
      String[] Function GetClassMethods()
         String[] sResult = new String[5]
         sResult[0] = "ApplyRelativeCoordinates"
         sResult[1] = "ApplySnappedOffsetToPosition"
         sResult[2] = "ApplyTransformationMatrix"
         sResult[3] = "ComputeTransformationMatrix"
         sResult[4] = "GetRelativeCoordinates"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Rotations_ApplyRelativeCoordinates
      String Function GetMethodReturnType()
         Return "Float[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 12
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[12]
         sNames[0] = "afParentPosition[0]"
         sNames[1] = "afParentPosition[1]"
         sNames[2] = "afParentPosition[2]"
         sNames[3] = "afParentRotation[0]"
         sNames[4] = "afParentRotation[1]"
         sNames[5] = "afParentRotation[2]"
         sNames[6] = "afOffsetPosition[0]"
         sNames[7] = "afOffsetPosition[1]"
         sNames[8] = "afOffsetPosition[2]"
         sNames[9] = "afOffsetRotation[0]"
         sNames[10] = "afOffsetRotation[1]"
         sNames[11] = "afOffsetRotation[2]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[12]
         sTypes[0] = "Float"
         sTypes[1] = "Float"
         sTypes[2] = "Float"
         sTypes[3] = "Float"
         sTypes[4] = "Float"
         sTypes[5] = "Float"
         sTypes[6] = "Float"
         sTypes[7] = "Float"
         sTypes[8] = "Float"
         sTypes[9] = "Float"
         sTypes[10] = "Float"
         sTypes[11] = "Float"
         Return sTypes
      EndFunction
      Function CallMethod()
         Float[] afParentPosition = new Float[3]
         Float[] afParentRotation = new Float[3]
         Float[] afOffsetPosition = new Float[3]
         Float[] afOffsetRotation = new Float[3]
         afParentPosition[0] = _fParameters[0]
         afParentPosition[1] = _fParameters[1]
         afParentPosition[2] = _fParameters[2]
         afParentRotation[0] = _fParameters[3]
         afParentRotation[1] = _fParameters[4]
         afParentRotation[2] = _fParameters[5]
         afOffsetPosition[0] = _fParameters[6]
         afOffsetPosition[1] = _fParameters[7]
         afOffsetPosition[2] = _fParameters[8]
         afOffsetRotation[0] = _fParameters[9]
         afOffsetRotation[1] = _fParameters[10]
         afOffsetRotation[2] = _fParameters[11]
         _fReturnArray = CobbAPIRotations.ApplyRelativeCoordinates(afParentPosition, afParentRotation, afOffsetPosition, afOffsetRotation)
      EndFunction
   EndState
   State Method_Rotations_ApplySnappedOffsetToPosition
      String Function GetMethodReturnType()
         Return "Float[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 19
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[19]
         sNames[0] = "afParentPosition[0]"
         sNames[1] = "afParentPosition[1]"
         sNames[2] = "afParentPosition[2]"
         sNames[3] = "afSnapPosition[0]"
         sNames[4] = "afSnapPosition[1]"
         sNames[5] = "afSnapPosition[2]"
         sNames[6] = "afPositionChange[0]"
         sNames[7] = "afPositionChange[1]"
         sNames[8] = "afPositionChange[2]"
         sNames[9] = "afMatrixTransformToLocal[0]"
         sNames[10] = "afMatrixTransformToLocal[1]"
         sNames[11] = "afMatrixTransformToLocal[2]"
         sNames[12] = "afMatrixTransformToLocal[3]"
         sNames[13] = "afMatrixTransformToLocal[4]"
         sNames[14] = "afMatrixTransformToLocal[5]"
         sNames[15] = "afMatrixTransformToLocal[6]"
         sNames[16] = "afMatrixTransformToLocal[7]"
         sNames[17] = "afMatrixTransformToLocal[8]"
         sNames[18] = "aiSnapUnits"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[19]
         sTypes[0] = "Float"
         sTypes[1] = "Float"
         sTypes[2] = "Float"
         sTypes[3] = "Float"
         sTypes[4] = "Float"
         sTypes[5] = "Float"
         sTypes[6] = "Float"
         sTypes[7] = "Float"
         sTypes[8] = "Float"
         sTypes[9] = "Float"
         sTypes[10] = "Float"
         sTypes[11] = "Float"
         sTypes[12] = "Float"
         sTypes[13] = "Float"
         sTypes[14] = "Float"
         sTypes[15] = "Float"
         sTypes[16] = "Float"
         sTypes[17] = "Float"
         sTypes[18] = "Int"
         Return sTypes
      EndFunction
      Function CallMethod()
         Float[] afParentPosition = new Float[3]
         Float[] afSnapPosition = new Float[3]
         Float[] afPositionChange = new Float[3]
         Float[] afMatrixTransformToLocal = new Float[9]
         afParentPosition[0] = _fParameters[0]
         afParentPosition[1] = _fParameters[1]
         afParentPosition[2] = _fParameters[2]
         afSnapPosition[0] = _fParameters[3]
         afSnapPosition[1] = _fParameters[4]
         afSnapPosition[2] = _fParameters[5]
         afPositionChange[0] = _fParameters[6]
         afPositionChange[1] = _fParameters[7]
         afPositionChange[2] = _fParameters[8]
         afMatrixTransformToLocal[0] = _fParameters[9]
         afMatrixTransformToLocal[1] = _fParameters[10]
         afMatrixTransformToLocal[2] = _fParameters[11]
         afMatrixTransformToLocal[3] = _fParameters[12]
         afMatrixTransformToLocal[4] = _fParameters[13]
         afMatrixTransformToLocal[5] = _fParameters[14]
         afMatrixTransformToLocal[6] = _fParameters[15]
         afMatrixTransformToLocal[7] = _fParameters[16]
         afMatrixTransformToLocal[8] = _fParameters[17]
         _fReturnArray = CobbAPIRotations.ApplySnappedOffsetToPosition(afParentPosition, afSnapPosition, afPositionChange, afMatrixTransformToLocal, _iParameters[18])
      EndFunction
   EndState
   State Method_Rotations_ApplyTransformationMatrix
      String Function GetMethodReturnType()
         Return "Float[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 12
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[12]
         sNames[0] = "afPosition[0]"
         sNames[1] = "afPosition[1]"
         sNames[2] = "afPosition[2]"
         sNames[3] = "afMatrix[0]"
         sNames[4] = "afMatrix[1]"
         sNames[5] = "afMatrix[2]"
         sNames[6] = "afMatrix[0]"
         sNames[7] = "afMatrix[1]"
         sNames[8] = "afMatrix[2]"
         sNames[9] = "afMatrix[0]"
         sNames[10] = "afMatrix[1]"
         sNames[11] = "afMatrix[2]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[12]
         sTypes[0] = "Float"
         sTypes[1] = "Float"
         sTypes[2] = "Float"
         sTypes[3] = "Float"
         sTypes[4] = "Float"
         sTypes[5] = "Float"
         sTypes[6] = "Float"
         sTypes[7] = "Float"
         sTypes[8] = "Float"
         sTypes[9] = "Float"
         sTypes[10] = "Float"
         sTypes[11] = "Float"
         Return sTypes
      EndFunction
      Function CallMethod()
         Float[] afPosition = new Float[3]
         Float[] afMatrix = new Float[9]
         afPosition[0] = _fParameters[0]
         afPosition[1] = _fParameters[1]
         afPosition[2] = _fParameters[2]
         afMatrix[0] = _fParameters[3]
         afMatrix[1] = _fParameters[4]
         afMatrix[2] = _fParameters[5]
         afMatrix[3] = _fParameters[6]
         afMatrix[4] = _fParameters[7]
         afMatrix[5] = _fParameters[8]
         afMatrix[6] = _fParameters[9]
         afMatrix[7] = _fParameters[10]
         afMatrix[8] = _fParameters[11]
         _fReturnArray = CobbAPIRotations.ApplyTransformationMatrix(afPosition, afMatrix)
      EndFunction
   EndState
   State Method_Rotations_ComputeTransformationMatrix
      String Function GetMethodReturnType()
         Return "Float[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[3]
         sNames[0] = "afRotation[0]"
         sNames[1] = "afRotation[1]"
         sNames[2] = "afRotation[2]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[3]
         sTypes[0] = "Float"
         sTypes[1] = "Float"
         sTypes[2] = "Float"
         Return sTypes
      EndFunction
      Function CallMethod()
         Float[] afRotation = new Float[3]
         afRotation[0] = _fParameters[0]
         afRotation[1] = _fParameters[1]
         afRotation[2] = _fParameters[2]
         _fReturnArray = CobbAPIRotations.ComputeTransformationMatrix(afRotation)
      EndFunction
   EndState
   State Method_Rotations_GetRelativeCoordinates
      String Function GetMethodReturnType()
         Return "Float[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 12
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[12]
         sNames[0] = "afParentPosition[0]"
         sNames[1] = "afParentPosition[1]"
         sNames[2] = "afParentPosition[2]"
         sNames[3] = "afParentRotation[0]"
         sNames[4] = "afParentRotation[1]"
         sNames[5] = "afParentRotation[2]"
         sNames[6] = "afTargetPosition[0]"
         sNames[7] = "afTargetPosition[1]"
         sNames[8] = "afTargetPosition[2]"
         sNames[9] = "afTargetRotation[0]"
         sNames[10] = "afTargetRotation[1]"
         sNames[11] = "afTargetRotation[2]"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[12]
         sTypes[0] = "Float"
         sTypes[1] = "Float"
         sTypes[2] = "Float"
         sTypes[3] = "Float"
         sTypes[4] = "Float"
         sTypes[5] = "Float"
         sTypes[6] = "Float"
         sTypes[7] = "Float"
         sTypes[8] = "Float"
         sTypes[9] = "Float"
         sTypes[10] = "Float"
         sTypes[11] = "Float"
         Return sTypes
      EndFunction
      Function CallMethod()
         Float[] afParentPosition = new Float[3]
         Float[] afParentRotation = new Float[3]
         Float[] afTargetPosition = new Float[3]
         Float[] afTargetRotation = new Float[3]
         afParentPosition[0] = _fParameters[0]
         afParentPosition[1] = _fParameters[1]
         afParentPosition[2] = _fParameters[2]
         afParentRotation[0] = _fParameters[3]
         afParentRotation[1] = _fParameters[4]
         afParentRotation[2] = _fParameters[5]
         afTargetPosition[0] = _fParameters[6]
         afTargetPosition[1] = _fParameters[7]
         afTargetPosition[2] = _fParameters[8]
         afTargetRotation[0] = _fParameters[9]
         afTargetRotation[1] = _fParameters[10]
         afTargetRotation[2] = _fParameters[11]
         _fReturnArray = CobbAPIRotations.GetRelativeCoordinates(afParentPosition, afParentRotation, afTargetPosition, afTargetRotation)
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Shout APIs
   State Class_Shout
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
   EndState
   ;
   State Method_Shout_GetAllShoutsUsingSpell
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
         Shout[] kResult = CobbAPIShout.GetAllShoutsUsingSpell(_kParameters[0] as Spell)
         Int iLength   = kResult.Length
         _kReturnArray = Utility.CreateFormArray(iLength)
         Int iIterator = 0
         While iIterator < iLength
            _kReturnArray[0] = kResult[0]
            iIterator += 1
         EndWhile
      EndFunction
   EndState
   State Method_Shout_GetAllShoutsUsingWord
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
         Shout[] kResult = CobbAPIShout.GetAllShoutsUsingWord(_kParameters[0] as WordOfPower)
         Int iLength   = kResult.Length
         _kReturnArray = Utility.CreateFormArray(iLength)
         Int iIterator = 0
         While iIterator < iLength
            _kReturnArray[0] = kResult[0]
            iIterator += 1
         EndWhile
      EndFunction
   EndState
   State Method_Shout_GetAnyShoutUsingSpell
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
         _kReturnValue = CobbAPIShout.GetAnyShoutUsingSpell(_kParameters[0] as Spell)
      EndFunction
   EndState
   State Method_Shout_GetAnyShoutUsingWord
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
         _kReturnValue = CobbAPIShout.GetAnyShoutUsingWord(_kParameters[0] as WordOfPower)
      EndFunction
   EndState
   State Method_Shout_GetShoutMenuDisplayObject
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
         _kReturnValue = CobbAPIShout.GetShoutMenuDisplayObject(_kParameters[0] as Shout)
      EndFunction
   EndState
   State Method_Shout_GetShoutRecoveryTimes
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
         _fReturnArray = CobbAPIShout.GetShoutRecoveryTimes(_kParameters[0] as Shout)
      EndFunction
   EndState
   State Method_Shout_GetShoutSpells
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
         Spell[] kResult = CobbAPIShout.GetShoutSpells(_kParameters[0] as Shout)
         Int iLength   = kResult.Length
         _kReturnArray = Utility.CreateFormArray(iLength)
         Int iIterator = 0
         While iIterator < iLength
            _kReturnArray[0] = kResult[0]
            iIterator += 1
         EndWhile
      EndFunction
   EndState
   State Method_Shout_GetShoutWords
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
         WordOfPower[] kResult = CobbAPIShout.GetShoutWords(_kParameters[0] as Shout)
         Int iLength   = kResult.Length
         _kReturnArray = Utility.CreateFormArray(iLength)
         Int iIterator = 0
         While iIterator < iLength
            _kReturnArray[0] = kResult[0]
            iIterator += 1
         EndWhile
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; SimpleSearches APIs
   State Class_SimpleSearches
      String[] Function GetClassMethods()
         String[] sResult = new String[2]
         sResult[0] = "GetAllOfBaseFormInCell"
         sResult[1] = "GetAllWithLinkedRefInCell"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_SimpleSearches_GetAllOfBaseFormInCell
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
         _kReturnArray = RefArrayToFormArray(CobbAPISimpleSearches.GetAllOfBaseFormInCell(_kParameters[0], _kParameters[1] as Cell, _bParameters[2]))
      EndFunction
   EndState
   State Method_SimpleSearches_GetAllWithLinkedRefInCell
      String Function GetMethodReturnType()
         Return "ObjectReference[]"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 3
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[3]
         sNames[0] = "akLinkedRef"
         sNames[1] = "akKeyword"
         sNames[2] = "akCellToSearch"
         sNames[3] = "abSearchLoadedArea"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[3]
         sTypes[0] = "ObjectReference"
         sTypes[1] = "Keyword"
         sTypes[2] = "Cell"
         sTypes[3] = "Bool"
         Return sTypes
      EndFunction
      Function CallMethod()
         ObjectReference[] kResult = CobbAPISimpleSearches.GetAllWithLinkedRefInCell(_kParameters[0] as ObjectReference, _kParameters[1] as Keyword, _kParameters[2] as Cell, _bParameters[3])
         _kReturnArray = RefArrayToFormArray(kResult)
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; String APIs
   State Class_String
      String[] Function GetClassMethods()
         String[] sResult = new String[2]
         sResult[0] = "HexToUInt32"
         sResult[1] = "ToHex"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_String_HexToUInt32
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
         _iReturnValue = CobbAPIString.HexToUInt32(_sParameters[0])
      EndFunction
   EndState
   State Method_String_ToHex
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
         _sReturnValue = CobbAPIString.ToHex(_iParameters[0], _iParameters[1])
      EndFunction
      ;
      Function ResetParameters()
         _aParameters = new Alias[10]
         _bParameters = new Bool[10]
         _fParameters = new Float[10]
         _kParameters = new Form[10]
         _iParameters = new Int[10]
         _sParameters = new String[10]
         ;
         _iParameters[1] = 8
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Utility APIs
   State Class_Utility
      String[] Function GetClassMethods()
         String[] sResult = new String[2]
         sResult[0] = "ActiveEffectExists"
         sResult[1] = "FormExists"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Utility_ActiveEffectExists
      String Function GetMethodReturnType()
         Return "Bool"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "akPointer"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "ActiveMagicEffect"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIUtility.ActiveEffectExists(None) ;; CAN'T IMPLEMENT; NO SCRIPT SUPPORT FOR ACTIVEMAGICEFFECT[] VARIABLES
      EndFunction
   EndState
   State Method_Utility_FormExists
      String Function GetMethodReturnType()
         Return "Bool"
      EndFunction
      Int Function GetMethodParameterCount()
         Return 1
      EndFunction
      String[] Function GetMethodParameterNames()
         String[] sNames = new String[1]
         sNames[0] = "akPointer"
         Return sNames
      EndFunction
      String[] Function GetMethodParameterTypes()
         String[] sTypes = new String[1]
         sTypes[0] = "Form"
         Return sTypes
      EndFunction
      Function CallMethod()
         _bReturnValue = CobbAPIUtility.FormExists(_kParameters[0])
      EndFunction
   EndState
;/EndBlock/;
;/Block/; ; Worldspace APIs
   State Class_Worldspace
      String[] Function GetClassMethods()
         String[] sResult = new String[2]
         sResult[0] = "GetParentWorldspace"
         sResult[1] = "GetRootWorldspaceOrSelf"
         sResult[2] = "GetWaterLevel"
         Return sResult
      EndFunction
   EndState
   ;
   State Method_Worldspace_GetParentWorldspace
      String Function GetMethodReturnType()
         Return "Worldspace"
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
         sTypes[0] = "Worldspace"
         Return sTypes
      EndFunction
      Function CallMethod()
         _kReturnValue = CobbAPIWorldspace.GetParentWorldspace(_kParameters[0] as Worldspace)
      EndFunction
   EndState
   State Method_Worldspace_GetRootWorldspaceOrSelf
      String Function GetMethodReturnType()
         Return "Worldspace"
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
         sTypes[0] = "Worldspace"
         Return sTypes
      EndFunction
      Function CallMethod()
         _kReturnValue = CobbAPIWorldspace.GetRootWorldspaceOrSelf(_kParameters[0] as Worldspace)
      EndFunction
   EndState
   State Method_Worldspace_GetWaterLevel
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
         sTypes[0] = "Worldspace"
         Return sTypes
      EndFunction
      Function CallMethod()
         _fReturnValue = CobbAPIWorldspace.GetWaterLevel(_kParameters[0] as Worldspace)
      EndFunction
   EndState
;/EndBlock/;

