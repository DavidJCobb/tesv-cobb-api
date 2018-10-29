Scriptname CobbAPIForm Hidden

Int[] Function GetBounds      (Form akSubject) Global Native ; Returns the OBND data (pre-generated by the Creation Kit's "Recalc Bounds" function) in this format: [ iMinX, iMinY, iMinZ, iMaxX, iMaxY, iMaxZ ].
Bool  Function IsDefiniteItem (Form akSubject) Global Native ; Returns true if the form is a "definite" item, i.e. not a LeveledItem, a Light that isn't carryable, or a COBJ.