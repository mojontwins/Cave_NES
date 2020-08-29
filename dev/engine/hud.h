// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// Hud. Of course, this has to be customized for each game
/*
void p_t1 (unsigned char x, unsigned char y, unsigned char n) {
	gp_addr = NAMETABLE_A + (y << 5) + x;
	*ul ++ = MSB (gp_addr);
	*ul ++ = LSB (gp_addr);
	*ul ++ = DIGIT (n);
}
*/
void hud_update (void) {
	oam_meta_spr (
		136, 15, 4,
		spr_items [ (flags [PLAYER_INV_FLAG] > 2 && flags [PLAYER_INV_FLAG] < 0xff) ? flags [PLAYER_INV_FLAG] : 0 ]
	); 

	if (plife != oplife) {
		p_t2 (5, 2, plife);
		oplife = plife;
	}
}

void hud_draw (void) {
	vram_adr (NAMETABLE_A);
	vram_unrle (hud_rle);
}
