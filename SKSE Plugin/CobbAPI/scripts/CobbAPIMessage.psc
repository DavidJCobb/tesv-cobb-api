Scriptname CobbAPIMessage Hidden

String[] Function GetButtonLabels      (Message akMessage) Global Native
String   Function GetText              (Message akMessage) Global Native
String   Function GetTitle             (Message akMessage) Global Native
Bool     Function WillCrashDueToLength (Message akMessage) Global Native ; checks if the length exceeds 1023 characters