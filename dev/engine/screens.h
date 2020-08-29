// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// Code for fixed screens

void enter_screen (const unsigned char *pal, void (*func) (void)) {
	//scroll (0, 472);
	
	cls ();
	
	rda = 0;
	(*func) ();
	
	pal_bg (pal);
	ppu_on_all ();
	fade_in ();
}

void exit_cleanly (void) {
	//set_vram_update (0);
	oam_hide_rest (0);
	music_stop ();
	fade_out ();
	ppu_off ();	
}

void wait_time_or_input (void) {
	rda = (pad_poll (0) != 0);
	while (game_time) {
		ticker ++; if (ticker == ticks) {
			ticker = 0;
			game_time --;
		}

		if (pad_poll (0)) {
			if (!rda) break;
		} else {
			rda = 0;
		}

		ppu_wait_nmi ();
	}
}

void do_screen (unsigned char seconds) {
	game_time = seconds; ticker = 0;
	wait_time_or_input ();
	exit_cleanly ();
}

void screen_title (void) {
	vram_adr (NAMETABLE_A);
	vram_unrle (title_rle);
#ifdef LANG_ES
	p_s (10, 20, "PULSA START!");
#else
	p_s (10, 20, "PRESS START!");
#endif
	p_s (5, 24, "@ 2017 THE MOJON TWINS");
}

void screen_game_over (void) {
	p_s (11, 14, "GAME OVER!");
}

void screen_game_ending (void) {
	vram_adr (NAMETABLE_A);
	vram_unrle (ending_rle);
#ifdef LANG_ES
	p_s (4, 16, " Y ASI CHERIL CONSIGUIO// ESCOJONCIAR AL MALVADO//MONSTRUO BICHUO DE CAVE!//  BIEN HECHO, TOTETE!!");
#else
	p_s (4, 16, " SO CHERIL FINALLY MADE//IT AND DESTROYED THE EVIL// CAVE MONSTER! WELL DONE// THANK YOU FOR PLAYING!");
#endif				 
}
