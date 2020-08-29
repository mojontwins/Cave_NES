// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// talkbox.h

// Routines to show simple text boxes.

unsigned char *get_text_pointer (void) {
	// 19 = 16 + 2 + 1
	return (unsigned char *) (texts0 + (rdb << 4) + (rdb << 1) + rdb);
}

void custom_tapestry (void) {
	rdx = 4; rdy = 12; gpjt = 0; rdct = 0; ul = update_list;
	gpit = 48; while (gpit --) {

		rdt = tap ? scr_buff [66 + gpjt ++] : box_buff [gpjt ++]; 
		update_list_tile ();
		rdct = (rdct + 1) & 3; 

		if (!rdct) {
			close_update_cycle (); ul = update_list;
		}

		rdx += 2; if (rdx == 28) { rdx = 4; rdy += 2; gpjt += 4; }
	} ppu_wait_nmi ();
}

void p_s_upd_show_full (unsigned char x, unsigned char y, const unsigned char *s) {
	ul = update_list;
	gp_addr = NAMETABLE_A + (y << 5) + x;
	*ul ++ = MSB (gp_addr) | NT_UPD_HORZ;
	*ul ++ = LSB (gp_addr);
	*ul ++ = 19;
	rdb = 19; while (rdb --) { *ul ++ = *s ++ - 32; }
	close_update_cycle ();
}

void text_split (void) {
	// Make a hole
	oam_hide_rest (0);
	tap = 0; custom_tapestry ();

	// Here: Write texts 
	for (gpit = 0; gpit < 4; gpit ++) {
		if (texts [gpit]) p_s_upd_show_full (7, 14 + gpit, texts [gpit]);
	}

	update_list [0] = NT_UPD_EOF;
	rda = (pad_poll (0) != 0); while (1) {
		if (pad_poll (0)) { if (!rda) break; } else rda = 0;
		ppu_wait_nmi ();
	}

	// Cover hole
	oam_hide_rest (0);
	tap = 1; custom_tapestry ();
}

void clear_texts (void) {
	gpit = 4; while (gpit --) texts [gpit] = 0;
}

void run_script (void) {
	// Runs script #rda
	gp_gen = (unsigned char *) talk_scripts [rda];
	while ((rdc = *gp_gen ++) != 0xfe) {
		// rdc is the character talking. Do whatever you need
		// show name?
		for (gpit = 0; gpit < 4; gpit ++) {
			rdb = *gp_gen ++; texts [gpit] = rdb ? get_text_pointer () : 0;
		}
		text_split ();
	}
}
