Scriptname CobbAPIAlias Hidden

Int  Function CreatesReference           (ReferenceAlias akAlias) Global Native ; does this reference alias create a reference?
Int  Function GetRefAliasCreateAtAliasID (ReferenceAlias akAlias) Global Native ; at what alias ID does the reference alias create a reference? (errors, returns 0, if the alias doesn't create a ref)
Int  Function GetRefAliasFillType        (ReferenceAlias akAlias) Global Native ; what's the fill type?
Bool Function IsPlayerAlias              (ReferenceAlias akAlias) Global Native ; the alias fills to the player?