Scriptname CobbAPISimpleSearches Hidden

; These functions search a single cell. Omit the cell to search in the current cell, if our C++-side code 
; can identify the current cell at the time.
;
; If abSearchLoadedArea is True OR akCellToSearch is None, and if the current cell is an exterior, then 
; we will also search all cells in the loaded grid.
;
ObjectReference[] Function GetAllOfBaseFormInCell    (Form akBaseForm, Cell akCellToSearch = None, Bool abSearchLoadedArea = False) Global Native
ObjectReference[] Function GetAllWithLinkedRefInCell (ObjectReference akLinkedRef, Keyword akKeyword = None, Cell akCellToSearch = None, Bool abSearchLoadedArea = False) Global Native