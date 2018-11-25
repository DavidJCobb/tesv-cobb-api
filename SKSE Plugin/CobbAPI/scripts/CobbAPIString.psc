Scriptname CobbAPIString Hidden

; ---------------------------------------------------------------------------------------
;    BACKGROUND
; ---------------------------------------------------------------------------------------
;
; When Papyrus strings are written to ESP and ESM files, they are always written in 
; UTF-8, regardless of the language in use by a mod author's system. I assume that UTF-8 
; is also used for Papyrus strings at run-time. (I'm not sure what would happen to string 
; constants in a script, though; does the Papyrus Compiler treat all files as UTF-8?)
;
; These script functions generally use UTF-8.
;
; It's worth noting that non-Papyrus strings are not guaranteed to use UTF-8. In Skyrim 
; Classic, the text encoding used by the rest of the game varies from language to lang-
; uage. Worse, ESP and ESM files don't contain any metadata identifying their language 
; or their encoding. As such, Papyrus functions that pull strings from game data (such 
; as getters for editor IDs and actors' names) will generally return data in an unknown 
; encoding, which may or may not be UTF-8.
;

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
; z1           | z1
; z10          | z2
; z11          | z3
; z12          | z4
; z13          | z5
; z2           | z6
; z3           | z7
; z4           | z8
; z5           | z9
; z6           | z10
; z7           | z11
; z8           | z12
; z9           | z13
;
Int Function NaturalCompare_ASCII (String a, String b) Global Native ; same return values as C++ _stricmp
;
; Functions for sorting arrays come in a few variants: a function that sorts just the 
; string array; and a function that sorts a second array passed beside it. The akStrings 
; array is not modified (a copy is returned), but the axSecond array is modified in-place. 
; One use for this would be sorting an array of EditorIDs consistently with a matching 
; array of FormIDs (or forms themselves).
;
String[] Function NaturalSort_ASCII         (String[] akStrings) Global Native
String[] Function NaturalSortPairInt_ASCII  (String[] akStrings, Int[];/&/;  axSecond) Global Native
String[] Function NaturalSortPairForm_ASCII (String[] akStrings, Form[];/&/; axSecond) Global Native

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