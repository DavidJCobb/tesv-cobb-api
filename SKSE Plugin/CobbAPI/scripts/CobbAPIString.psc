Scriptname CobbAPIString Hidden

Int      Function Compare        (String a, String b) Global Native
Int      Function FindFirstOf    (String asHaystack, String asNeedle, Int aiOffset = 0) Global Native
Int      Function FindFirstNotOf (String asHaystack, String asNeedle, Int aiOffset = 0) Global Native
Int      Function HexToUInt32    (String asString) Global Native
Int      Function FindLastOf     (String asHaystack, String asNeedle, Int aiOffset = -1) Global Native
Int      Function FindLastNotOf  (String asHaystack, String asNeedle, Int aiOffset = -1) Global Native
;String[] Function RegexMatch     (String asHaystack, String asRegex) Global Native
;String   Function RegexReplace   (String asHaystack, String asRegex, String asReplaceWith) Global Native
;Bool     Function RegexSearch    (String asHaystack, String asRegex) Global Native
String   Function ToHex          (Int aiHex, Int aiDigits = 8) Global Native
String   Function Trim           (String asString) Global Native

;
; These methods treat the underlying string as UTF-8, and should hopefully be suitable for CJKV text.
;
Int Function UTF8Compare (String asString) Global Native
Int Function UTF8Length  (String asString) Global Native

;
; C++ standard regexes use ECMAScript (JavaScript) style.
;
; Papyrus treats strings case-insensitively and uses the earliest defined string. This has 
; a few implications that make regexes somewhat impractical.
;
;  - Regex character codes remain case-sensitive, but Papyrus doesn't maintain their case. 
;    This means that if one mod wants to use "\w" and another wants to use "\W", each mod 
;    will end up using the same regex (generally whichever was defined first).
;
;     - In theory, you could work around this by prefixing your regex with a unique code 
;       that does nothing, e.g.
;
;          CobbAPIString.RegexSearch("abc", "(?:MyRegexNo00001){0}^abc$")  ; returns True
;          CobbAPIString.RegexSearch("abc", "(?:MyRegexNo00002){0}^\\w+$") ; returns True
;          CobbAPIString.RegexSearch("abc", "(?:MyRegexNo00003){0}^\\W+$") ; returns False
;
;       but that's an ugly hack and may possibly impact regex performance.
;
;  - Haystack strings are likewise case-insensitive, so regexes are forced to work case-
;    insensitively.
;