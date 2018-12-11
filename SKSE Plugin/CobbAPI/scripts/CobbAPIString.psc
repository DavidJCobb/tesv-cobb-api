Scriptname CobbAPIString Hidden

; ---------------------------------------------------------------------------------------
;    AN OVERVIEW OF HOW COMPUTERS STORE TEXT
; ---------------------------------------------------------------------------------------
;
; In order to avoid ambiguity, I would prefer to document things using the most precise 
; terminology possible, but much of this terminology isn't known to people who don't 
; explicitly do "hard" text programming. I had to do some learning myself in order to 
; offer as much functionality as I wanted to. As such, here's a "brief" primer on how 
; computers store text.
;
; Computers can only work with numeric data, so every symbol that a piece of text may 
; contain must be represented using a number, or "code point." The oldest surviving sys-
; tem for this is ASCII (American Standard Code for Information Interchange), which was 
; introduced in 1963. In this system, every single glyph is represented with a single 
; byte, and so up to 256 glyphs (including a string terminator and other control codes) 
; can be represented. ASCII only defined 128 glyphs, all focused on English text, leav-
; ing the last 128 glyphs undefined.
;
; As such, non-English text could only be represented in alternate encodings. Because 
; virtually all programming is done in English, these encodings are typically supersets 
; of ASCII. For languages that have large numbers of glyphs, such as Chinese, Japanese, 
; Korean, and Vietnamese, these encodings often have to use multiple bytes for some or 
; all glyphs.
;
; In 1991, the Unicode standard was published. It uses multiple bytes for all glyphs in 
; order to maximize its coverage and support as many languages and scripts as possible, 
; but there are alternate encodings (such as UTF-8) that are designed to compress Uni-
; code text whenever possible, with a focus on representing ASCII as compactly as poss-
; ible (given that markup and program code, invisible to a user, will be in ASCII).
;
; Unicode has nuances that are largely invisible to native English-speaking program-
; mers, who won't encounter or have to work with foreign-language text unless they put 
; themselves in situations where they may encounter it. Native English-speaking program-
; mers are used to the assumptions that one glyph will be represented one way using one 
; byte, and all of those assumptions fall apart when using Unicode and UTF-8.
;
; A single glyph can be represented using multiple code point sequences: Unicode offers 
; better ways of representing glyphs that have diacritics, but it also retains older 
; representations for backward compatibility's sake. Moreover, a single code point will 
; be multiple bytes in Unicode; in UTF-8, ASCII code points are single-byte while all 
; other code points are multi-byte (with higher code points requiring more bytes).
;
; The string functions offered by CobbAPI are Unicode-aware, and the documentation will 
; reflect this: string lengths, character comparisons, and more are done on the basis of 
; code points, not glyphs or bytes.
;
; ---------------------------------------------------------------------------------------
;    STRING HANDLING IN SKYRIM'S SCRIPT ENGINE
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
; Papyrus strings are stored case-insensitively according to the C/POSIX locale (which 
; determines what characters are considered "letters" with uppercase and lowercase 
; forms). As such, the following glyphs, and only the following glyphs, are considered 
; interchangeable:
;
;           Uppercase | A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
;          -----------+-----------------------------------------------------
;           Lowercase | a b c d e f g h i j k l m n o p q r s t u v w x y z
;
; The game handles case-insensitivity byte by byte, so an accented letter will be handled 
; differently depending on whether it is a single code point or two (a non-accented 
; letter followed by an accent diacritic).
;
; If you specify two different strings that are identical save for case, the game will 
; use the first variant to load. The catch is that this applies across most or all of the 
; game engine, not just within Papyrus. In an English game, it is impossible to set a 
; Papyrus string variable to "dog" because there is already an in-game Race form named 
; "Dog", and so "Dog" was the first variant of the string to load. Similarly, trying to 
; set a string to "oBjEcTreferenCE" will always yield "ObjectReference" instead.
;

; ---------------------------------------------------------------------------------------
;    NUMBERS
; ---------------------------------------------------------------------------------------
;
; The functions for converting strings to numbers do not allow separators (e.g. decimal 
; points or commas). They search for the first numeric byte, and then keep reading until 
; they encounter any non-numeric byte.
;
Int    Function HexToInt32    (String asString) Global Native
Int    Function BinaryToInt32 (String asString) Global Native
String Function ToBinary      (Int aiBinary, Int aiDigits = 32) Global Native
String Function ToHex         (Int aiHex, Int aiDigits = 8) Global Native

; ---------------------------------------------------------------------------------------
;    UTILITIES
; ---------------------------------------------------------------------------------------
Int    Function CPLength (String asString) Global Native ; length in code points
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
String[] Function NaturalSort_ASCII         (String[] asStrings, Bool abDescending = False) Global Native
String[] Function NaturalSortPairInt_ASCII  (String[] asStrings, Bool abDescending = False, Int[];/&/;  axSecond) Global Native
String[] Function NaturalSortPairForm_ASCII (String[] asStrings, Bool abDescending = False, Form[];/&/; axSecond) Global Native

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
; string. Characters that aren't letters (see explanation at the top of this file) can be 
; represented in the case sentinel with any glyph.
;
; For the purposes of the case sentinel, a "character" is one code point, as opposed to 
; bytes or visible glyphs. (Bytes would require, say, a Chinese regex character to have 
; three case sentinel characters. Visible glyphs would be more intuitive when diacritics 
; enter the picture, but are far harder to code.)
;
; Also, because all strings are case-insensitive, regexes are always executed case-insensi-
; tively.
;
String[] Function RegexMatch     (String asHaystack, String asRegex, String asCaseSentinel, Bool abWholeStringMustMatch) Global Native
String   Function RegexReplace   (String asHaystack, String asRegex, String asCaseSentinel, String asReplaceWith) Global Native
Bool     Function RegexSearch    (String asHaystack, String asRegex, String asCaseSentinel) Global Native