Scriptname CobbAPIMath Hidden

;
; Recognized color formats:
;
; 0 | 0xAABBGGRR | used in the game engine
; 1 | 0x00RRGGBB | HTML colors, used by SkyUI
;

Float Function Atan2               (Float y, Float x) Global Native
Int   Function BSwap               (Int aiValue) Global Native
Int   Function ConvertColorFormat  (Int aiColor, Int aiInputFormat, Int aiOutputFormat) Global Native
Int   Function GetByte             (Int aiValue, Int aiWhichByte) Global Native
Int   Function JoinColor           (Int aiFormat, Int[] aiSplitColor) Global Native
Int   Function JoinColorComponents (Int aiFormat, Int aiRed, Int aiGreen, Int aiBlue, Int aiAlpha = 255) Global Native
Int   Function SetByte             (Int aiValue, Int aiWhichByte, Int aiSetByteTo) Global Native
Int[] Function SplitColor          (Int aiFormat, Int aiColor) Global Native // splits to R, G, B, A