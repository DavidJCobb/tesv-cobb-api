Scriptname CobbAPITestSuite extends SKI_ConfigBase Hidden

Bool _bDidShowWarning = False

Float _fSliderRange     = 100.0
Float _fSliderMinimum   = -100.0
Float _fSliderMaximum   = 100.0
Float _fSliderPrecision = 1.0

CobbAPITestSuiteExecutorState Function GetExecutorState()
   Return Quest.GetQuest("CobbAPITestSuiteExecutorState") as CobbAPITestSuiteExecutorState
EndFunction
CobbAPITestSuiteIExecutor[] Function GetExecutorList()
   CobbAPITestSuiteIExecutor[] kArray = new CobbAPITestSuiteIExecutor[21]
   ;
   kArray[ 0] = Quest.GetQuest("CobbAPITestSuiteExecutorAcousticSpace") as CobbAPITestSuiteIExecutor
   kArray[ 1] = Quest.GetQuest("CobbAPITestSuiteExecutorActor") as CobbAPITestSuiteIExecutor
   kArray[ 2] = Quest.GetQuest("CobbAPITestSuiteExecutorCell") as CobbAPITestSuiteIExecutor
   kArray[ 3] = Quest.GetQuest("CobbAPITestSuiteExecutorCellSearcher") as CobbAPITestSuiteIExecutor
   kArray[ 4] = Quest.GetQuest("CobbAPITestSuiteExecutorDetection") as CobbAPITestSuiteIExecutor
   kArray[ 5] = Quest.GetQuest("CobbAPITestSuiteExecutorForm") as CobbAPITestSuiteIExecutor
   kArray[ 6] = Quest.GetQuest("CobbAPITestSuiteExecutorFurniture") as CobbAPITestSuiteIExecutor
   kArray[ 7] = Quest.GetQuest("CobbAPITestSuiteExecutorGame") as CobbAPITestSuiteIExecutor
   kArray[ 8] = Quest.GetQuest("CobbAPITestSuiteExecutorImageSpace") as CobbAPITestSuiteIExecutor
   kArray[ 9] = Quest.GetQuest("CobbAPITestSuiteExecutorLight") as CobbAPITestSuiteIExecutor
   kArray[10] = Quest.GetQuest("CobbAPITestSuiteExecutorLightingTemplate") as CobbAPITestSuiteIExecutor
   kArray[11] = Quest.GetQuest("CobbAPITestSuiteExecutorMath") as CobbAPITestSuiteIExecutor
   kArray[12] = Quest.GetQuest("CobbAPITestSuiteExecutorMessage") as CobbAPITestSuiteIExecutor
   kArray[13] = Quest.GetQuest("CobbAPITestSuiteExecutorObjectReference") as CobbAPITestSuiteIExecutor
   kArray[14] = Quest.GetQuest("CobbAPITestSuiteExecutorRevealService") as CobbAPITestSuiteIExecutor
   kArray[15] = Quest.GetQuest("CobbAPITestSuiteExecutorRotations") as CobbAPITestSuiteIExecutor
   kArray[16] = Quest.GetQuest("CobbAPITestSuiteExecutorShout") as CobbAPITestSuiteIExecutor
   kArray[17] = Quest.GetQuest("CobbAPITestSuiteExecutorSimpleSearches") as CobbAPITestSuiteIExecutor
   kArray[18] = Quest.GetQuest("CobbAPITestSuiteExecutorString") as CobbAPITestSuiteIExecutor
   kArray[19] = Quest.GetQuest("CobbAPITestSuiteExecutorUtility") as CobbAPITestSuiteIExecutor
   kArray[20] = Quest.GetQuest("CobbAPITestSuiteExecutorWorldspace") as CobbAPITestSuiteIExecutor
   ;
   Return kArray
EndFunction
CobbAPITestSuiteIExecutor Function GetExecutor()
   Return _kExecutorsForSession[_iCurrentClass]
EndFunction
Bool Function StringIsAliasType(String asType)
   If asType == "Alias" \
   || asType == "LocationAlias" \
   || asType == "ReferenceAlias"
      Return True
   EndIf
   Return False
EndFunction
Bool Function StringIsAliasArrayType(String asType)
   Int iLength = StringUtil.GetLength(asType)
   If StringUtil.Substring(asType, iLength - 2, 2) != "[]"
      Return False
   EndIf
   Return StringIsAliasType(StringUtil.Substring(asType, 0, iLength - 2))
EndFunction
Bool Function StringIsFormType(String asType)
   If asType == "Form" \
   || asType == "Action" \
   || asType == "Activator" \
   || asType == "Actor" \
   || asType == "ActorBase" \
   || asType == "ActorValueInfo" \
   || asType == "Ammo" \
   || asType == "Apparatus" \
   || asType == "Armor" \
   || asType == "Art" \
   || asType == "AssociationType" \
   || asType == "Book" \
   || asType == "Cell" \
   || asType == "Class" \
   || asType == "ColorForm" \
   || asType == "CombatStyle" \
   || asType == "ConstructibleObject" \
   || asType == "Container" \
   || asType == "DefaultObjectManager" \
   || asType == "Door" \
   || asType == "EffectShader" \
   || asType == "Enchantment" \
   || asType == "EncounterZone" \
   || asType == "EquipSlot" \
   || asType == "Explosion" \
   || asType == "Faction" \
   || asType == "FormList" \
   || asType == "Furniture" \
   || asType == "GlobalVariable" \
   || asType == "Hazard" \
   || asType == "HeadPart" \
   || asType == "Idle" \
   || asType == "ImageSpaceModifier" \
   || asType == "ImpactDataSet" \
   || asType == "Ingredient" \
   || asType == "Key" \
   || asType == "Keyword" \
   || asType == "LeveledActor" \
   || asType == "LeveledItem" \
   || asType == "LeveledSpell" \
   || asType == "Light" \
   || asType == "Location" \
   || asType == "LocationRefType" \
   || asType == "MagicEffect" \
   || asType == "Message" \
   || asType == "MiscObject" \
   || asType == "MusicType" \
   || asType == "ObjectReference" \
   || asType == "Outfit" \
   || asType == "Package" \
   || asType == "Perk" \
   || asType == "Potion" \
   || asType == "Projectile" \
   || asType == "Quest" \
   || asType == "Race" \
   || asType == "Scene" \
   || asType == "Scroll" \
   || asType == "Shout" \
   || asType == "SoulGem" \
   || asType == "Sound" \
   || asType == "SoundCategory" \
   || asType == "SoundDescriptor" \
   || asType == "Spell" \
   || asType == "Static" \
   || asType == "TextureSet" \
   || asType == "Topic" \
   || asType == "TopicInfo" \
   || asType == "TreeObject" \
   || asType == "VisualEffect" \
   || asType == "VoiceType" \
   || asType == "Weapon" \
   || asType == "Weather" \
   || asType == "WordOfPower" \
   || asType == "WorldSpace"
      Return True
   EndIf
   Return False
EndFunction
Bool Function StringIsFormArrayType(String asType)
   Int iLength = StringUtil.GetLength(asType)
   If StringUtil.Substring(asType, iLength - 2, 2) != "[]"
      Return False
   EndIf
   Return StringIsFormType(StringUtil.Substring(asType, 0, iLength - 2))
EndFunction

Event OnConfigInit()
EndEvent
Event OnConfigOpen()
   _kExecutorsForSession = GetExecutorList()
   _bDidShowWarning = False
EndEvent
Event OnConfigClose()
   _fSliderRange = 100.0
   _kExecutorsForSession = new CobbAPITestSuiteIExecutor[1]
EndEvent
Event OnPageReset(String asPage)
   If !_bDidShowWarning
      SetCursorFillMode(TOP_TO_BOTTOM)
      SetCursorPosition(0)
      Int iIterator = 1
      While iIterator < 10
         String sTemporary = iIterator
         If iIterator < 10
            sTemporary = "0" + sTemporary
         EndIf
         AddTextOption("$CobbAPIDemoWarningL" + sTemporary, "")
         iIterator += 1
      EndWhile
      AddEmptyOption()
      AddTextOptionST("OPTExitWarning", "$CobbAPIDemoContinueLabel", "$CobbAPIDemoContinueButton")
      Return
   EndIf
   OnStandardPageReset(asPage)
EndEvent

Int      _iCurrentClass = -1
Int      _iCurrentMethod = -1
String[] _sClassList
String[] _sClassMethods
Int      _iMethodParameterCount = 0
String[] _sMethodParameterTypes
String[] _sMethodParameterNames
CobbAPITestSuiteIExecutor[] _kExecutorsForSession
;
String[] Function GetCustomClassList()
   Int iCount = _kExecutorsForSession.Length
   _sClassList = Utility.CreateStringArray(iCount)
   Int iIterator = 0
   While iIterator < iCount
      _sClassList[iIterator] = _kExecutorsForSession[iIterator].GetClassName()
      iIterator += 1
   EndWhile
   Return _sClassList
EndFunction
String[] Function GetVanillaClassList()
   _sClassList = new String[104]
   _sClassList[0] = "Action"
   _sClassList[1] = "Activator"
   _sClassList[2] = "ActiveMagicEffect"
   _sClassList[3] = "Actor"
   _sClassList[4] = "ActorBase"
   _sClassList[5] = "ActorValueInfo"
   _sClassList[6] = "Alias"
   _sClassList[7] = "Ammo"
   _sClassList[8] = "Apparatus"
   _sClassList[9] = "Armor"
   _sClassList[10] = "ArmorAddon"
   _sClassList[11] = "Art"
   _sClassList[12] = "AssociationType"
   _sClassList[13] = "Book"
   _sClassList[14] = "Cell"
   _sClassList[15] = "Class"
   _sClassList[16] = "ColorComponent"
   _sClassList[17] = "ColorForm"
   _sClassList[18] = "CombatStyle"
   _sClassList[19] = "ConstructibleObject"
   _sClassList[20] = "Container"
   _sClassList[21] = "Debug"
   _sClassList[22] = "DefaultObjectManager"
   _sClassList[23] = "Door"
   _sClassList[24] = "EffectShader"
   _sClassList[25] = "Enchantment"
   _sClassList[26] = "EncounterZone"
   _sClassList[27] = "EquipSlot"
   _sClassList[28] = "Explosion"
   _sClassList[29] = "Faction"
   _sClassList[30] = "Flora"
   _sClassList[31] = "Form"
   _sClassList[32] = "FormList"
   _sClassList[33] = "FormType"
   _sClassList[34] = "Furniture"
   _sClassList[35] = "Game"
   _sClassList[36] = "GameData"
   _sClassList[37] = "GlobalVariable"
   _sClassList[38] = "Hazard"
   _sClassList[39] = "HeadPart"
   _sClassList[40] = "Idle"
   _sClassList[41] = "ImageSpaceModifier"
   _sClassList[42] = "ImpactDataSet"
   _sClassList[43] = "Info Fragments"
   _sClassList[44] = "Ingredient"
   _sClassList[45] = "Input"
   _sClassList[46] = "Key"
   _sClassList[47] = "Keyword"
   _sClassList[48] = "LeveledActor"
   _sClassList[49] = "LeveledItem"
   _sClassList[50] = "LeveledSpell"
   _sClassList[51] = "Light"
   _sClassList[52] = "Location"
   _sClassList[53] = "LocationAlias"
   _sClassList[54] = "LocationRefType"
   _sClassList[55] = "MagicEffect"
   _sClassList[56] = "Math"
   _sClassList[57] = "Message"
   _sClassList[58] = "MiscObject"
   _sClassList[59] = "ModEvent"
   _sClassList[60] = "MusicType"
   _sClassList[61] = "NetImmerse"
   _sClassList[62] = "ObjectReference"
   _sClassList[63] = "Outfit"
   _sClassList[64] = "Package"
   _sClassList[65] = "Package Fragments"
   _sClassList[66] = "Perk"
   _sClassList[67] = "Perk Fragments"
   _sClassList[68] = "Phase Fragments"
   _sClassList[69] = "Potion"
   _sClassList[70] = "Projectile"
   _sClassList[71] = "Quest"
   _sClassList[72] = "Race"
   _sClassList[73] = "ReferenceAlias"
   _sClassList[74] = "SKSE"
   _sClassList[75] = "Scene"
   _sClassList[76] = "Scene Fragments"
   _sClassList[77] = "Scroll"
   _sClassList[78] = "Shout"
   _sClassList[79] = "SoulGem"
   _sClassList[80] = "Sound"
   _sClassList[81] = "SoundCategory"
   _sClassList[82] = "SoundDescriptor"
   _sClassList[83] = "SpawnerTask"
   _sClassList[84] = "Spell"
   _sClassList[85] = "Stage Fragments"
   _sClassList[86] = "Static"
   _sClassList[87] = "StringUtil"
   _sClassList[88] = "TalkingActivator"
   _sClassList[89] = "TextureSet"
   _sClassList[90] = "Timer Fragments"
   _sClassList[91] = "Topic"
   _sClassList[92] = "TopicInfo"
   _sClassList[93] = "TreeObject"
   _sClassList[94] = "UI"
   _sClassList[95] = "UICallback"
   _sClassList[96] = "Utility"
   _sClassList[97] = "VisualEffect"
   _sClassList[98] = "VoiceType"
   _sClassList[99] = "Weapon"
   _sClassList[100] = "Weather"
   _sClassList[101] = "WordOfPower"
   _sClassList[102] = "WorldSpace"
   _sClassList[103] = "WornObject"
   Return _sClassList
EndFunction
;
Function OnStandardPageReset(String asPage)
   SetCursorFillMode(TOP_TO_BOTTOM)
   SetCursorPosition(0)
   If _iCurrentClass >= 0
      String sClass = _sClassList[_iCurrentClass]
      AddMenuOptionST("OPTSelectClass",  "Class", sClass)
      If _iCurrentMethod >= 0
         AddMenuOptionST("OPTSelectMethod", "Method", _sClassMethods[_iCurrentMethod])
         CobbAPITestSuiteIExecutor     kExecutor = GetExecutor()
         CobbAPITestSuiteExecutorState kState    = GetExecutorState()
         If kExecutor.MethodIsMemberFunction()
            If sClass == "ActiveMagicEffect"
               AddTextOptionST("OPTContext", "$CobbAPIDemoThis", "$CobbAPIDemoNotImplemented", OPTION_FLAG_DISABLED)
            ElseIf StringIsAliasType(sClass)
               AddTextOptionST("OPTContext", "$CobbAPIDemoThis", "$CobbAPIDemoNotImplemented", OPTION_FLAG_DISABLED)
            ElseIf StringIsFormType(sClass)
               AddInputOptionST("OPTContext", "$CobbAPIDemoThis", FormatFormArgument(kState.pkContext))
            EndIf
         EndIf
         ;/Block/; ; Render parameters
            Int iIterator = 0
            While iIterator < _iMethodParameterCount
               String sType = _sMethodParameterTypes[iIterator]
               String sName = "$CobbAPIDemoErrOutOfRange" ; in case the next line errors
               sName = _sMethodParameterNames[iIterator]
               String sStateName = "OPTArgument" + iIterator
               ;
               If sType == "Cell"
                  sName = sType + ":" + sName
                  AddInputOptionST(sStateName, sName, FormatCellArgument(kState.pkParameters[iIterator] as Cell))
               ElseIf sType == "Worldspace"
                  sName = sType + ":" + sName
                  AddInputOptionST(sStateName, sName, FormatWorldspaceArgument(kState.pkParameters[iIterator] as Worldspace))
               ElseIf StringIsFormType(sType)
                  sName = sType + ":" + sName
                  AddInputOptionST(sStateName, sName, FormatFormArgument(kState.pkParameters[iIterator]))
               ElseIf sType == "Bool"
                  AddToggleOptionST(sStateName, sName, kState.pbParameters[iIterator])
               ElseIf sType == "Float"
                  AddSliderOptionST(sStateName, sName, kState.pfParameters[iIterator])
               ElseIf sType == "Int"
                  AddSliderOptionST(sStateName, sName, kState.piParameters[iIterator])
               ElseIf sType == "String"
                  AddInputOptionST(sStateName, sName, kState.psParameters[iIterator])
               Else
                  sName = sType + ":" + sName
                  AddTextOptionST(sStateName, sName, "$CobbAPIDemoErrUnknownType", OPTION_FLAG_DISABLED)
               EndIf
               ;
               iIterator += 1
            EndWhile
         ;/EndBlock/;
      Else
         AddMenuOptionST("OPTSelectMethod", "$CobbAPIDemoMethod", "")
      EndIf
   Else
      AddMenuOptionST("OPTSelectClass",  "$CobbAPIDemoClass",  "")
      AddMenuOptionST("OPTSelectMethod", "$CobbAPIDemoMethod", "", OPTION_FLAG_HIDDEN)
   EndIf
   ;/Block/; ; Right column
      SetCursorPosition(1)
      AddTextOptionST("OPTExecuteMethod", "$CobbAPIDemoRunFuncLabel", "$CobbAPIDemoRunFuncButton")
      AddInputOptionST("OPTSetSliderMin", "$CobbAPIDemoSliderMin", "" + _fSliderMinimum)
      AddInputOptionST("OPTSetSliderMax", "$CobbAPIDemoSliderMax", "" + _fSliderMaximum)
      AddInputOptionST("OPTSetSliderPrecision", "$CobbAPIDemoSliderPrec", "" + _fSliderPrecision)
   ;/EndBlock/;
EndFunction
;
Function OnSelectClass()
   String sClass = _sClassList[_iCurrentClass]
   ;
   CobbAPITestSuiteIExecutor kExecutor = GetExecutor()
   CobbAPITestSuiteExecutorState kState = GetExecutorState()
   kExecutor.GoToState("Class_" + sClass)
   kState.ResetContext()
   _sClassMethods = kExecutor.GetClassMethods()
   _iCurrentMethod = -1
   ;
   ForcePageReset()
EndFunction
Function OnSelectMethod()
   String sClass  = _sClassList[_iCurrentClass]
   String sMethod = _sClassMethods[_iCurrentMethod]
   ;
   CobbAPITestSuiteIExecutor kExecutor = GetExecutor()
   CobbAPITestSuiteExecutorState kState = GetExecutorState()
   kExecutor.GoToState("Method_" + sMethod)
   kState.ResetParameters()
   kExecutor.SetMethodDefaultParameters()
   _iMethodParameterCount = kExecutor.GetMethodParameterCount()
   _sMethodParameterNames = kExecutor.GetMethodParameterNames()
   _sMethodParameterTypes = kExecutor.GetMethodParameterTypes()
   ;
   ForcePageReset()
EndFunction
;
;/Block/; ; Generic handlers
   Function StandardHighlight()
      SetInfoText("")
      String sState = GetState()
      If StringUtil.Find(sState, "OPTArgument") == 0
         Int iArgument = StringUtil.Substring(sState, 11) as Int
         String sType = _sMethodParameterTypes[iArgument]
         If sType == "String"
            SetInfoText("$CobbAPIDemoEnterValueText")
         ElseIf sType == "Cell"
            SetInfoText("$CobbAPIDemoEnterValueCell")
         ElseIf sType == "Worldspace"
            SetInfoText("$CobbAPIDemoEnterValueWorld")
         ElseIf StringIsFormType(sType)
            SetInfoText("$CobbAPIDemoEnterValueForm")
         EndIf
         Return
      EndIf
   EndFunction
   Function StandardInputOpen()
      String sState = GetState()
      If StringUtil.Find(sState, "OPTArgument") == 0
         Int iArgument = StringUtil.Substring(sState, 11) as Int
         String sType = _sMethodParameterTypes[iArgument]
         If sType == "String"
            SetInputDialogStartText(GetExecutorState().psParameters[iArgument])
         ElseIf sType == "Cell"
            OnArgumentEditCellStart(iArgument)
         ElseIf sType == "Worldspace"
            OnArgumentEditWorldspaceStart(iArgument)
         ElseIf StringIsFormType(sType)
            OnArgumentEditFormStart(iArgument)
         EndIf
         Return
      EndIf
   EndFunction
   Function StandardInputAccept(String asEntry)
      String sState = GetState()
      If StringUtil.Find(sState, "OPTArgument") == 0
         Int iArgument = StringUtil.Substring(sState, 11) as Int
         String sType = _sMethodParameterTypes[iArgument]
         If sType == "String"
            GetExecutorState().psParameters[iArgument] = asEntry
            SetInputOptionValueST(asEntry)
         ElseIf sType == "Cell"
            OnArgumentEditCellCommit(iArgument, asEntry)
         ElseIf sType == "Worldspace"
            OnArgumentEditWorldspaceCommit(iArgument, asEntry)
         ElseIf StringIsFormType(sType)
            OnArgumentEditFormCommit(iArgument, asEntry)
         EndIf
         Return
      EndIf
   EndFunction
   Function StandardSelect()
      String sState = GetState()
      If StringUtil.Find(sState, "OPTArgument") == 0
         Int iArgument = StringUtil.Substring(sState, 11) as Int
         CobbAPITestSuiteExecutorState kState = GetExecutorState()
         ;
         If _sMethodParameterTypes[iArgument] == "Bool"
            Bool bToggle = !kState.pbParameters[iArgument]
            kState.pbParameters[iArgument] = bToggle
            SetToggleOptionValueST(bToggle)
         EndIf
         Return
      EndIf
   EndFunction
   Function StandardSliderOpen()
      String sState = GetState()
      If StringUtil.Find(sState, "OPTArgument") == 0
         Int iArgument = StringUtil.Substring(sState, 11) as Int
         CobbAPITestSuiteExecutorState kState = GetExecutorState()
         ;
         If _sMethodParameterTypes[iArgument] == "Float"
            SetSliderDialogStartValue(kState.pfParameters[iArgument])
            SetSliderDialogRange(_fSliderMinimum, _fSliderMaximum)
            SetSliderDialogInterval(_fSliderPrecision)
         ElseIf _sMethodParameterTypes[iArgument] == "Int"
            SetSliderDialogStartValue(kState.piParameters[iArgument])
            SetSliderDialogRange(_fSliderMinimum as Int, _fSliderMaximum as Int)
            SetSliderDialogInterval(1)
            If _fSliderPrecision > 1
               SetSliderDialogInterval(_fSliderPrecision as Int)
            EndIf
         EndIf
         Return
      EndIf
   EndFunction
   Function StandardSliderAccept(Float afValue)
      String sState = GetState()
      If StringUtil.Find(sState, "OPTArgument") == 0
         Int iArgument = StringUtil.Substring(sState, 11) as Int
         CobbAPITestSuiteExecutorState kState = GetExecutorState()
         ;
         If _sMethodParameterTypes[iArgument] == "Float"
            kState.pfParameters[iArgument] = afValue
            SetSliderOptionValueST(afValue)
         ElseIf _sMethodParameterTypes[iArgument] == "Int"
            kState.piParameters[iArgument] = afValue as Int
            SetSliderOptionValueST(afValue)
         EndIf
         Return
      EndIf
   EndFunction
;/EndBlock/;
;/Block/; ; Specific options
   State OPTExitWarning
      Event OnSelectST()
         _bDidShowWarning = True
         ForcePageReset()
      EndEvent
   EndState
   State OPTSelectClass
      Event OnMenuOpenST()
         SetMenuDialogOptions(GetCustomClassList())
         SetMenuDialogDefaultIndex(0)
         If _iCurrentClass >= 0
            SetMenuDialogStartIndex(_iCurrentClass)
         Else
            SetMenuDialogStartIndex(0)
         EndIf
      EndEvent
      Event OnMenuAcceptST(Int aiIndex)
         If aiIndex == _iCurrentClass
            Return
         EndIf
         _iCurrentClass = aiIndex
         OnSelectClass()
      EndEvent
   EndState
   State OPTSelectMethod
      Event OnMenuOpenST()
         SetMenuDialogOptions(_sClassMethods)
         SetMenuDialogDefaultIndex(0)
         If _iCurrentMethod >= 0
            SetMenuDialogStartIndex(_iCurrentMethod)
         Else
            SetMenuDialogStartIndex(0)
         EndIf
      EndEvent
      Event OnMenuAcceptST(Int aiIndex)
         If aiIndex == _iCurrentMethod
            Return
         EndIf
         _iCurrentMethod = aiIndex
         OnSelectMethod()
      EndEvent
   EndState
   State OPTExecuteMethod
      Event OnSelectST()
         If _iCurrentClass < 0
            ShowMessage("Select a class and a method.")
            Return
         EndIf
         If _iCurrentMethod < 0
            ShowMessage("Select a method.")
            Return
         EndIf
         String sClass  = _sClassList[_iCurrentClass]
         String sMethod = _sClassMethods[_iCurrentMethod]
         ;
         CobbAPITestSuiteIExecutor     kExecutor = GetExecutor()
         CobbAPITestSuiteExecutorState kState    = GetExecutorState()
         kExecutor.GoToState("Method_" + sMethod)
         kExecutor.CallMethod()
         ;
         String sReturnType = kExecutor.GetMethodReturnType()
         If sReturnType == ""
            ShowMessage("$CobbAPIDemoResultVoid")
         ElseIf sReturnType == "Bool"
            ShowMessage("$CobbAPIDemoResultBool{" + kState.pbReturnValue + "}")
         ElseIf sReturnType == "Float"
            ShowMessage("$CobbAPIDemoResultFloat{" + kState.pfReturnValue + "}")
         ElseIf StringIsFormType(sReturnType)
            ShowMessage("$CobbAPIDemoResultForm{" + kState.pkReturnValue + "}")
         ElseIf sReturnType == "Int"
            ShowMessage("$CobbAPIDemoResultInt{" + kState.piReturnValue + "}")
         ElseIf sReturnType == "String"
            ShowMessage("$CobbAPIDemoResultString{" + kState.psReturnValue + "}")
         ElseIf sReturnType == "Float[]"
            ShowMessage("$CobbAPIDemoResultArrFloat{" + kState.pfReturnArray + "}")
         ElseIf StringIsFormArrayType(sReturnType)
            ShowMessage("$CobbAPIDemoResultArrForm{" + kState.pkReturnArray + "}")
         ElseIf sReturnType == "Int[]"
            ShowMessage("$CobbAPIDemoResultArrInt{" + kState.piReturnArray + "}")
         ElseIf sReturnType == "String[]"
            ShowMessage("$CobbAPIDemoResultArrString{" + kState.psReturnArray + "}")
         Else
            ShowMessage("$CobbAPIDemoResultUnknown{" + sReturnType + "}")
         EndIf
      EndEvent
   EndState
   State OPTSetSliderMin
      Event OnInputOpenST()
         SetInputDialogStartText("" + _fSliderMinimum)
      EndEvent
      Event OnInputAcceptST(String asEntry)
         _fSliderMinimum = asEntry as Float
         SetInputOptionValueST("" + _fSliderMinimum)
      EndEvent
      Event OnHighlightST()
         SetInfoText("$CobbAPIDemoDescSliderMin")
      EndEvent
   EndState
   State OPTSetSliderMax
      Event OnInputOpenST()
         SetInputDialogStartText("" + _fSliderMaximum)
      EndEvent
      Event OnInputAcceptST(String asEntry)
         _fSliderMaximum = asEntry as Float
         SetInputOptionValueST("" + _fSliderMaximum)
      EndEvent
      Event OnHighlightST()
         SetInfoText("$CobbAPIDemoDescSliderMax")
      EndEvent
   EndState
   State OPTSetSliderPrecision
      Event OnInputOpenST()
         SetInputDialogStartText("" + _fSliderPrecision)
      EndEvent
      Event OnInputAcceptST(String asEntry)
         _fSliderPrecision = asEntry as Float
         If _fSliderPrecision <= 0.0
            _fSliderPrecision = 1.0
         EndIf
         SetInputOptionValueST("" + _fSliderPrecision)
      EndEvent
      Event OnHighlightST()
         SetInfoText("$CobbAPIDemoDescSliderPrec")
      EndEvent
   EndState
;/EndBlock/;

Event OnHighlightST()
   StandardHighlight()
EndEvent
Event OnInputOpenST()
   StandardInputOpen()
EndEvent
Event OnInputAcceptST(String asEntry)
   StandardInputAccept(asEntry)
EndEvent
Event OnSelectST()
   StandardSelect()
EndEvent
Event OnSliderOpenST()
   StandardSliderOpen()
EndEvent
Event OnSliderAcceptST(Float afValue)
   StandardSliderAccept(afValue)
EndEvent

String Function FormatFormArgument(Form akForm)
   If !akForm
      Return ""
   EndIf
   If akForm == Game.GetPlayer()
      Return "player"
   EndIf
   If akForm == Game.GetCurrentConsoleRef() as Form
      Return "console"
   EndIf
   If akForm
      Return "0x" + CobbAPIString.ToHex(akForm.GetFormID())
   EndIf
   Return ""
EndFunction
Function OnArgumentEditFormStart(Int aiArgument)
   Form kForm = GetExecutorState().pkParameters[aiArgument]
   SetInputDialogStartText(FormatFormArgument(kForm))
EndFunction
Function OnArgumentEditFormCommit(Int aiArgument, String asEntry)
   String sDisplay = ""
   Form   kForm
   If asEntry == "player"
      kForm = Game.GetPlayer()
      sDisplay = asEntry
   ElseIf asEntry == "console"
      kForm = Game.GetCurrentConsoleRef() as Form
      sDisplay = asEntry
   Else
      Int iFormID = CobbAPIString.HexToInt32(asEntry)
      kForm = Game.GetForm(iFormID)
      If iFormID && kForm
         sDisplay = "0x" + CobbAPIString.ToHex(iFormID)
      EndIf
   EndIf
   GetExecutorState().pkParameters[aiArgument] = kForm
   SetInputOptionValueST(sDisplay)
EndFunction

String Function FormatCellArgument(Cell akCell)
   If !akCell
      Return ""
   EndIf
   If akCell == Game.GetPlayer().GetParentCell()
      Return "here"
   EndIf
   If akCell
      Return "0x" + CobbAPIString.ToHex(akCell.GetFormID())
   EndIf
   Return ""
EndFunction
Function OnArgumentEditCellStart(Int aiArgument)
   Cell kCell = GetExecutorState().pkParameters[aiArgument] as Cell
   SetInputDialogStartText(FormatCellArgument(kCell))
EndFunction
Function OnArgumentEditCellCommit(Int aiArgument, String asEntry)
   String sDisplay = ""
   Cell   kCell
   If asEntry == "here"
      kCell = Game.GetPlayer().GetParentCell()
      sDisplay = asEntry
   Else
      Int iFormID = CobbAPIString.HexToInt32(asEntry)
      kCell = Game.GetForm(iFormID) as Cell
      If iFormID && kCell
         sDisplay = "0x" + CobbAPIString.ToHex(iFormID)
      EndIf
   EndIf
   GetExecutorState().pkParameters[aiArgument] = kCell
   SetInputOptionValueST(sDisplay)
EndFunction

String Function FormatWorldspaceArgument(Worldspace akCell)
   If !akCell
      Return ""
   EndIf
   If akCell == Game.GetPlayer().GetWorldspace()
      Return "here"
   EndIf
   If akCell
      Return "0x" + CobbAPIString.ToHex(akCell.GetFormID())
   EndIf
   Return ""
EndFunction
Function OnArgumentEditWorldspaceStart(Int aiArgument)
   Worldspace kCell = GetExecutorState().pkParameters[aiArgument] as Worldspace
   SetInputDialogStartText(FormatWorldspaceArgument(kCell))
EndFunction
Function OnArgumentEditWorldspaceCommit(Int aiArgument, String asEntry)
   String     sDisplay = ""
   Worldspace kWorldspace
   If asEntry == "here"
      kWorldspace = Game.GetPlayer().GetWorldspace()
      sDisplay = asEntry
   Else
      Int iFormID = CobbAPIString.HexToInt32(asEntry)
      kWorldspace = Game.GetForm(iFormID) as Worldspace
      If iFormID && kWorldspace
         sDisplay = "0x" + CobbAPIString.ToHex(iFormID)
      EndIf
   EndIf
   GetExecutorState().pkParameters[aiArgument] = kWorldspace
   SetInputOptionValueST(sDisplay)
EndFunction