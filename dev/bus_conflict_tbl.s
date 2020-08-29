	; This ROM pages in PRG0 (back to main menu)
	; 1 = Normal ($EE)
	; 2 = Ending ($6E)

	bus_conflict_tbl:
		.byte $00, $EE, $6E
		