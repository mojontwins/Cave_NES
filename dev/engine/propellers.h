// Propellers

// Propellers may be added by different means, 
// But anyway you should call this funcion.

// Define PROPELLERS_X and PROPELLERS_Y in definitions.h
// so I know where to look to get the coordinates.

void propellers_add (void) {
#ifdef PROPELLERS_CREATE_FROM_MAP
	rda = gpit - 16;
	gpint = NAMETABLE_A + rdx + (rdy << 5);
#else
	rda = PROPELLERS_X + (PROPELLERS_Y << 4) - 16;
	gpint = GAME_AREA_ADR + (PROPELLERS_X << 1) + (PROPELLERS_Y << 6);
#endif
	
	propellers_add_hi [propellers_idx] = MSB (gpint) | NT_UPD_HORZ;
	propellers_add_lo [propellers_idx ++] = LSB (gpint);
#ifdef PROPELLERS_LIMIT
	rdb = PROPELLERS_LIMIT;
	while (!scr_attr [rda] && rda < 192 && rdb --) { scr_attr [rda] = PROPELLERS_BIT; rda -= 16; }
#else
	while (!scr_attr [rda] && rda < 192) { scr_attr [rda] = PROPELLERS_BIT; rda -= 16; }
#endif	
}

void propellers_do (void) {
	rda = frame_counter & 2;
	gpit = propellers_idx; while (gpit --) {
		*ul ++ = propellers_add_hi [gpit];
		*ul ++ = propellers_add_lo [gpit];
		*ul ++ = 2;
		*ul ++ = PROPELLERS_PATTERN + rda;
		*ul ++ = PROPELLERS_PATTERN + (rda | 1);
	}
}
