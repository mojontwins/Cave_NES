// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// General printing functions

void fade_out (void) { fader = 5; while (fader --) { pal_bright (fader); ppu_wait_nmi (); } }
	
void fade_in (void) { fader = 5; while (fader --) { pal_bright (4 - fader); ppu_wait_nmi (); } }
	
void cls (void) { vram_adr (0x2000); vram_fill (0x00,0x400); }

void reset_attributes (void) {
	gpit = 64; while (gpit --) attr_table [gpit] = 0xff;
}

void show_attributes (void) {
	// CUSTOM {
	vram_adr (NAMETABLE_A + 0x03c8);
	vram_write (attr_table + 8, 56);
	// } END_OF_CUSTOM
	// vram_adr (NAMETABLE_A + 0x03c0);
	// vram_write (attr_table, 64);
}

void close_update_cycle (void) {
	*ul = NT_UPD_EOF; ppu_wait_nmi ();
}

void write_attr (void) {
	rdc = (rdx >> 2) + ((rdy >> 2) << 3);
	rdb = ((rdx >> 1) & 1) + (((rdy >> 1) & 1) << 1);
	rda = attr_table [rdc];
	rda = (rda & bitmasks [rdb]) | (c_ts_pals [rdt] << (rdb << 1));
	attr_table [rdc] = rda;
}

void update_list_write (unsigned char a) {
	*ul ++ = MSB(gp_addr);
	*ul ++ = LSB(gp_addr++);
	*ul ++ = a;
}

void draw_tile (void) {
#ifdef MAP_DONT_PAINT_TILE_0
	if (!rdt) return;
#endif

	write_attr ();
	
	gp_tmap = (unsigned char *) c_ts_tmaps + (rdt << 2);
	gp_addr = ((rdy << 5) + rdx + 0x2000);

#ifdef MAP_ENABLE_AUTOSHADOWS
	if (mapmode_autoshadows && !scr_attr [gpit]) {
		vram_adr (gp_addr++);
		vram_put (*(((scr_attr [gpit - 17] & 8) ? 64 : 0) + gp_tmap));
		gp_tmap ++;
		vram_put (*(((scr_attr [gpit - 16] & 8) ? 64 : 0) + gp_tmap));
		gp_tmap ++;
		gp_addr += 31;
		vram_adr (gp_addr++);
		vram_put (*(((scr_attr [gpit - 1] & 8) ? 64 : 0) + gp_tmap));
		gp_tmap ++;
		vram_put (*gp_tmap);
	} else 
#endif
	{
		vram_adr (gp_addr++);
		vram_put (*gp_tmap++);
		vram_put (*gp_tmap++);
		gp_addr += 31;
		vram_adr (gp_addr++);
		vram_put (*gp_tmap++);
		vram_put (*gp_tmap);	
	}
}

void draw_tile_advance (void) {
	draw_tile (); rdx = (rdx + 2) & 31; if (!rdx) rdy += 2;
}

void update_list_horz_2 () {
	*ul ++ = MSB (gp_addr) | NT_UPD_HORZ;
	*ul ++ = LSB (gp_addr);
	*ul ++ = 2;	
}

void update_list_tile () {
	write_attr ();
	
	gp_addr = 0x23c0 + rdc;
	update_list_write (rda);
	
	// tiles
	gp_tmap = (unsigned char *) c_ts_tmaps + (rdt << 2);
	gp_addr = ((rdy << 5) + rdx + NAMETABLE_A);
	update_list_horz_2 ();
	*ul ++ = *gp_tmap ++;
	*ul ++ = *gp_tmap ++;
	gp_addr += 32;
	update_list_horz_2 ();
	*ul ++ = *gp_tmap ++;
	*ul ++ = *gp_tmap;
}

void set_map_tile (unsigned char x, unsigned char y, unsigned char t) {
	rdx = x + x; rdy = y + y + TOP_ADJUST; rdt = t;
	scr_buff [x + (y << 4)] = rdt; scr_attr [x + (y << 4)] = c_behs [rdt];
	update_list_tile ();
}

void p_t2 (unsigned char x, unsigned char y, unsigned char n) {
	gp_addr = NAMETABLE_A + (y << 5) + x;
	*ul ++ = MSB (gp_addr) | NT_UPD_HORZ;
	*ul ++ = LSB (gp_addr);
	*ul ++ = 2;	
	*ul ++ = DIGIT ((n / 10));
	*ul ++ = DIGIT ((n % 10));
}

void advance_worm (void) {
#ifdef MAP_USE_ALT_TILE
	if (rdt == 0 && (rand8 () & 7) == 1) rdt = c_alt_tile;
#endif

#ifdef CREATE_STUFF_FROM_MAP
	switch (rdt) {
#ifdef PROPELLERS_CREATE_FROM_MAP
		case PROPELLERS_CREATE_FROM_MAP: 
			if (level == 1) // this line is CUSTOM!
			propellers_add ();
			break;
#endif
#ifdef SPRINGS_CREATE_FROM_MAP
		case SPRINGS_CREATE_FROM_MAP:
			springs_add ();
			break;
#endif
	}

	scr_buff [gpit] = rdt; scr_attr [gpit] = c_behs [rdt]; draw_tile_advance (); gpit ++;
}

void draw_scr (void) {
	// Draw current screen
	set_rand (1 + n_pant);

#ifdef MAP_FORMAT_RLE53
	gp_map = (unsigned char *) c_map [n_pant];
	gpit = 0; rdx = 0; rdy = TOP_ADJUST;
	while (gpit < 192) {
		rdt = *gp_map ++;
		rdct = 1 + (rdt >> 5);
		rdt &= 0x1f;
		while (rdct --) advance_worm ();
	}
#endif

#ifdef MAP_FORMAT_RLE44
	gp_map = (unsigned char *) c_map [n_pant];
	gpit = 0; rdx = 0; rdy = TOP_ADJUST;
	while (gpit < 192) {
		rdt = *gp_map ++;
		rdct = 1 + (rdt >> 4);
		rdt &= 0x0f;
		while (rdct --) advance_worm ();
	}
#endif

#ifdef MAP_FORMAT_BYTE_RLE
	gp_map = (unsigned char *) c_map [n_pant];
	gpit = 0; rdx = 0; rdy = TOP_ADJUST;
	while (gpit < 192) {
		rdt = *gp_map ++;
		if (rdt & 128) {
			rdt &= 127;
			rdct = *gp_map ++; while (rdct --) advance_worm ();
		} else advance_worm ();
	}
#endif

#ifdef MAP_FORMAT_PACKED
	// n_pant * 96 = n_pant * 64 + n_pant * 32
	gp_map = (unsigned char *) c_map + (n_pant << 6) + (n_pant << 5);
	gpit = 0; rdx = 0; rdy = TOP_ADJUST;
	while (gpit < 192) {
		rdct = *gp_map ++;
		rdt = rdct >> 4; advance_worm ();
		rdt = rdct & 15; advance_worm ();
	}
#endif

#ifdef MAP_WITH_DECORATIONS
	if (c_decos && c_decos [n_pant]) {
		gp_gen = (unsigned char *) c_decos [n_pant];
	
		while (rdt = *gp_gen ++) {
			if (rdt & 0x80) {
				rdt &= 0x7F;
				rdct = 1;
			} else {
				rdct = *gp_gen ++;
			}
			while (rdct --) {
				gpit = *gp_gen ++;
				rdy = TOP_ADJUST + ((gpit >> 4) << 1); rdx = (gpit & 15) << 1; 
				advance_worm ();
			}
		}
	}
#endif 
}

#ifdef PRINT_PSEUDOASCII
void p_s (unsigned char x, unsigned char y, const unsigned char *s) {
	vram_adr (NAMETABLE_A + (y << 5) + x);
	while (rda = *s ++) {
		if (rda == '/') {
			y ++; vram_adr (NAMETABLE_A + (y << 5) + x);
		} else vram_put (rda - 32);
	}
}
#elif PRINT_CUSTOM
void p_s (unsigned char x, unsigned char y, const unsigned char *s) {
	vram_adr (NAMETABLE_A + (y << 5) + x);
	while ((rda = *s ++) != 0xff) {
		vram_put (rda);
	}
}
#endif

#ifdef PRINT_UL_PRINTER
void p_s_ul (unsigned char x, unsigned char y, const unsigned char *s) {
	gp_gen = (unsigned char *) s; rda = 0; while (*gp_gen ++) rda ++;
	gp_addr = NAMETABLE_A + (y << 5) + x;
	*ul ++ = MSB (gp_addr) | NT_UPD_HORZ;
	*ul ++ = LSB (gp_addr);
	*ul ++ = rda;
	while (rda = *s ++) *ul ++ = (rda - 32);
}
#endif

/*
void un_rle_screen (const unsigned char *rle) {
	rdx = 0; rdy = 0; 
	gpit = 240;

	while (gpit) {
		rdt = (*rle);
		rle ++;
		if (rdt & 128) {
			gpjt = rdt & 127;
			rdt = (*rle);
			rle ++;
			while (gpjt --) {
				draw_tile_advance ();
				gpit --;
			}
		} else {
			draw_tile_advance ();
			gpit --;
		}
	}

	vram_adr (NAMETABLE_A + 0x03c0);
	vram_write ((unsigned char *) rle, 64);
}
*/

// CUSTOM {
	

void palfx_do (void) {
	/*if (ct_flash) {
		if (ct_flash == 4) {
			pal_bg (mypal_light_bg);
			pal_spr (mypal_light_fg);
		}
		ct_flash --;
		if (ct_flash == 0) {
			pal_bg (c_pal_back);
			pal_spr (c_pal_fg);
		}
	} else */ 
	if ((frame_counter & 15) == 0) {
		gpit = pal_cycle [2]; 
		pal_cycle [2] = pal_cycle [1]; 
		pal_cycle [1] = pal_cycle [0]; 
		pal_cycle [0] = gpit; 
		pal_col (5, pal_cycle [0]);
		pal_col (6, pal_cycle [1]);
		pal_col (7, pal_cycle [2]);
	}
}

void palfx_init (void) {
	pal_cycle [0] = c_pal_bg [5];
	pal_cycle [1] = c_pal_bg [6];
	pal_cycle [2] = c_pal_bg [7];
}

// } END_OF_CUSTOM
