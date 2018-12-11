Scriptname CobbAPIArray Hidden

; NOTE: We don't check for negative lengths, indices, etc., in your arguments. The whole point of these 
; APIs is to run at least as fast as doing things "the vanilla way," so I'm reluctant to waste even the 
; imperceptibly-small amount of speed that two assembly opcodes would waste. Expect things to break if 
; you pass in negative numbers for lengths, counts, start indices, and so on. If your code is running 
; in a context where those values could ever make it to these functions to begin with, it's on you to 
; check for them and normalize them however your use case requires.
;
; To wit, these methods will frequently interpret -100 as 4294967196.
;
; To clarify: you can have negative numbers IN YOUR ARRAYS. Just don't pass the numbers themselves as 
; arguments to these functions.
;
;======================================================================================================

; Returns the number of truthy elements in axArray.
;
Int Function CountTruthyFloat     (Float[]           afArray) Global Native
Int Function CountTruthyForm      (Form[]            akArray) Global Native
Int Function CountTruthyInt       (Int[]             aiArray) Global Native
Int Function CountTruthyReference (ObjectReference[] akArray) Global Native

; These operators are essentially the same as SKSE. SKSE doesn't offer all of the same types, and the 
; types that it already offers are mainly included here for parity's sake with the new APIs.
;
Float[]           Function CreateFloat     (Int aiSize, Float           afFill = 0.0)  Global Native
Form[]            Function CreateForm      (Int aiSize, Form            akFill = None) Global Native
Int[]             Function CreateInt       (Int aiSize, Int             aiFill = 0)    Global Native
ObjectReference[] Function CreateReference (Int aiSize, ObjectReference akFill = None) Global Native

; Returns 1 if axArray contains any truthy elements, or 0 otherwise.
;
Bool Function HasTruthyFloat     (Float[]           afArray) Global Native
Bool Function HasTruthyForm      (Form[]            akArray) Global Native
Bool Function HasTruthyInt       (Int[]             aiArray) Global Native
Bool Function HasTruthyReference (ObjectReference[] akArray) Global Native

; These operators are essentially the same as SKSE. SKSE doesn't offer all of the same types, and the 
; types that it already offers are mainly included here for parity's sake with the new APIs.
;
Float[]           Function ResizeFloat     (Float[]           afArray, Int aiSize, Float           afFill = 0.0)  Global Native
Form[]            Function ResizeForm      (Form[]            akArray, Int aiSize, Form            akFill = None) Global Native
Int[]             Function ResizeInt       (Int[]             aiArray, Int aiSize, Int             aiFill = 0)    Global Native
ObjectReference[] Function ResizeReference (ObjectReference[] akArray, Int aiSize, ObjectReference akFill = None) Global Native

; Returns a "slice" of the specified array. I don't remember whether it was faster than or equal to the 
; vanilla way shown below, but it's not slower, and it's a hell of a lot less hideous.
;
;    Type[] axArray = new Type[aiCount]
;    iTemporary = aiStart
;    axArray[0] = axSource[iTemporary]
;    iTemporary += 1
;    axArray[1] = axSource[iTemporary]
;
; (The intermediate int there was needed to ensure proper compilation for indices like (iSomeVar * 3 + i).)
;
Float[]           Function SliceFloat     (Float[]           afArray, Int aiStart, Int aiCount) Global Native
Form[]            Function SliceForm      (Form[]            akArray, Int aiStart, Int aiCount) Global Native
Int[]             Function SliceInt       (Int[]             aiArray, Int aiStart, Int aiCount) Global Native
ObjectReference[] Function SliceReference (ObjectReference[] akArray, Int aiStart, Int aiCount) Global Native

; These functions directly modify axTarget with the contents of axSource. The first element to be overwritten 
; is axTarget[aiStart]. Writing stops when either array's end is encountered. Benchmarking shows it to be about 
; as fast as the vanilla way, shown below.
;
;    iTemporary = some value
;    axTarget[iTemporary] = axSource[0]
;    iTemporary += 1
;    axTarget[iTemporary] = axSource[1]
;
; (The intermediate int there was needed to ensure proper compilation for indices like (iSomeVar * 3 + i).)
;
; Specify a negative aiCount to copy the entirety of axSource.
;
Function OverwriteFloat     (Float[];/&/;           afTarget, Float[]           afSource, Int aiStart, Int aiSourceStart = 0, Int aiCount = -1) Global Native
Function OverwriteForm      (Form[];/&/;            akTarget, Form[]            akSource, Int aiStart, Int aiSourceStart = 0, Int aiCount = -1) Global Native
Function OverwriteInt       (Int[];/&/;             aiTarget, Int[]             aiSource, Int aiStart, Int aiSourceStart = 0, Int aiCount = -1) Global Native
Function OverwriteReference (ObjectReference[];/&/; akTarget, ObjectReference[] akSource, Int aiStart, Int aiSourceStart = 0, Int aiCount = -1) Global Native

Function OverwriteFloatWithInt (Float[];/&/; afTarget, Int[] aiSource, Int aiStart, Int aiSourceStart = 0, Int aiCount = -1) Global Native
Function OverwriteIntWithFloat (Int[];/&/; aiTarget, Float[] afSource, Int aiStart, Int aiSourceStart = 0, Int aiCount = -1) Global Native