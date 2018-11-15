Scriptname CobbAPIString Hidden

; ---------------------------------------------------------------------------------------
;    NUMBERS
; ---------------------------------------------------------------------------------------
Int    Function HexToInt32    (String asString) Global Native
Int    Function BinaryToInt32 (String asString) Global Native
String Function ToBinary      (Int aiHex, Int aiDigits = 32) Global Native
String Function ToHex         (Int aiHex, Int aiDigits = 8) Global Native

; ---------------------------------------------------------------------------------------
;    UTILITIES
; ---------------------------------------------------------------------------------------
;
; When Papyrus strings are written to ESP and ESM files, they are written in UTF-8 (even 
; though all other strings use different encodings depending on language). I assume that 
; UTF-8 is also used at run-time.
;
; Not sure what would happen to string constants compiled with the Papyrus Compiler, 
; though. Does that treat all script source files as UTF-8?
;
; Anyway, this file's functions for working with strings generally use UTF-8.
;
Int    Function CPLength (String asString) Global Native ; length in UTF-8 code points
String Function Trim     (String asString) Global Native ; only works on the standard space 0x32 for now

; ---------------------------------------------------------------------------------------
;    COMPARISONS
; ---------------------------------------------------------------------------------------
;
; Sorting as a concept gets extremely complicated once you go international. For example, 
; the possible "alphabetical" sort orders for Chinese characters include: stroke count 
; (which varies between traditional and simplified Chinese); Pinyin alphabetical order; 
; and a UTF-8-defined sort order that (I'm told) most Chinese speakers dislike. The sort 
; orders for Japanese and Korean differ even when they use the same glyphs.
;
; I don't want to define just one "natural sort" function that will always work one way,
; and preclude the possibility of implementing internationalized sort orders down the 
; road. My compromise is to suffix any sort function with the name of the characters it 
; supports.
;
; ---------------------------------------------------------------------------------------
;
; The difference between a "natural" or alphabetical sort, and an ASCIIbetical sort, are 
; as follows:
;
; ASCIIbetical | Alphabetical
; z10          | z1
; z11          | z2
; z12          | z3
; z13          | z4
; z1           | z5
; z2           | z6
; z3           | z7
; z4           | z8
; z5           | z9
; z6           | z10
; z7           | z11
; z8           | z12
; z9           | z13
;
Int Function NaturalCompare_ASCII (String a, String b) Global Native
;
; Functions for sorting arrays come in a few variants: a function that sorts just the 
; string array; and a function that sorts a second array passed beside it. The akStrings 
; array is not modified (a copy is returned), but the axSecond array is modified in-place. 
; One use for this would be sorting an array of EditorIDs consistently with a matching 
; array of FormIDs (or forms themselves).
;
String[] Function NaturalSort_ASCII         (String[] akStrings) Global Native
String[] Function NaturalSortPairInt_ASCII  (String[] akStrings, Int[]  axSecond) Global Native
String[] Function NaturalSortPairForm_ASCII (String[] akStrings, Form[] axSecond) Global Native

; ---------------------------------------------------------------------------------------
;    REGULAR EXPRESSIONS
; ---------------------------------------------------------------------------------------
;
; C++ standard regexes use ECMAScript (JavaScript) style.
;
; Papyrus treats strings case-insensitively and uses the earliest defined string. This has 
; a few implications that make regexes somewhat impractical. Regex character codes remain 
; case-sensitive, but Papyrus doesn't maintain their case. This means that if one mod wants 
; to use "\w" and another wants to use "\W", each mod will end up using the same regex 
; (generally whichever was defined first).
;
; We work around this by requiring you to specify a "case sentinel," which is a string that 
; has the same number of characters as your input string, but consists entirely of the '0' 
; and '1' glyphs. Any character that should be lowercase in the regex should be a '0' in the 
; case string; characters that should be uppercase in the regex should be '1' in the case 
; string.
;
; Also, because all strings are case-insensitive, regexes are always executed case-insensi-
; tively.
;
String[] Function RegexMatch     (String asHaystack, String asRegex, String asCaseSentinel, Bool abWholeStringMustMatch) Global Native
String   Function RegexReplace   (String asHaystack, String asRegex, String asCaseSentinel, String asReplaceWith) Global Native
Bool     Function RegexSearch    (String asHaystack, String asRegex, String asCaseSentinel) Global Native