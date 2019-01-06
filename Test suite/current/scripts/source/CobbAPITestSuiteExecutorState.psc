Scriptname CobbAPITestSuiteExecutorState extends Quest Hidden
   
Alias             Property paContext Auto Hidden
ActiveMagicEffect Property peContext Auto Hidden
Form              Property pkContext Auto Hidden

Alias[]  Property paParameters Auto Hidden
Bool[]   Property pbParameters Auto Hidden
Float[]  Property pfParameters Auto Hidden
Form[]   Property pkParameters Auto Hidden
Int[]    Property piParameters Auto Hidden
String[] Property psParameters Auto Hidden

Bool     Property pbReturnValue Auto Hidden
Float    Property pfReturnValue Auto Hidden
Float[]  Property pfReturnArray Auto Hidden
Form     Property pkReturnValue Auto Hidden
Form[]   Property pkReturnArray Auto Hidden
Int      Property piReturnValue Auto Hidden
Int[]    Property piReturnArray Auto Hidden
String   Property psReturnValue Auto Hidden
String[] Property psReturnArray Auto Hidden

;/Block/; ; Boilerplate
   Function ResetParameters()
      paParameters = new Alias[10]
      pbParameters = new Bool[10]
      pfParameters = new Float[10]
      pkParameters = new Form[10]
      piParameters = new Int[10]
      psParameters = new String[10]
   EndFunction
   Function ResetContext()
      paContext = None
      peContext = None
      pkContext = None
   EndFunction
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
