// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// Game loop & shit

#ifdef SCRIPTING_ON
void game_run_fire_script (void) {
	run_script (2 * MAP_W * MAP_H + 2);		// Press fire in any room.
	run_script (n_pant + n_pant + 1);		// Press fire in current room.
}
#endif

void game_init (void) {
	// This makes my life easier
	gp_gen = scr_attr + 192; gpit = 16; while (gpit --) *gp_gen = 0;

	// Multiple level support (pseudo custom)
	// Some things are hardcoded for some games.

	c_ts_pals = l_ts_pals [level];
	c_ts_tmaps = l_ts_tmaps [level];
	c_behs = l_behs [level];
	n_pant = l_scr_ini [level];
	c_map = l_map [level];
	c_decos = l_decos [level];
	c_enems = l_enems [level];
	c_hotspots = 0; // l_hotspots [level];
	c_locks = 0; // l_locks [level];
	max_hotspots_type_1 = 0; // l_max_hotspots_type_1 [level];
	map_w = l_map_w [level];
	spr_enems = spr_enems_1; // l_spr_enems [level];
	spr_player = spr_player_0; // l_spr_player [level];

	pal_bg (l_pal_bgs [level]);
	pal_spr (palss1 /*l_pal_fgs [level]*/);

	c_pal_bg = l_pal_bgs [level];

#if defined(ENEMIES_CAN_DIE) && defined(PERSISTENT_DEATHS)
	enems_persistent_deaths_init ();
#endif	

#ifdef PERSISTENT_ENEMIES
	persistent_enems_load ();
#endif		

#ifdef HOTSPOT_TYPE_KEY
	bolts_load ();
#endif

	player_init ();

#ifndef DISABLE_HOTSPOTS	
	hotspots_ini ();
#endif	

	// Scripting / flags stuff

#ifdef COUNT_KILLED_ON_FLAG
	flags [COUNT_KILLED_ON_FLAG] = 0;
#endif

#ifdef ONLY_ONE_OBJECT_FLAG
	flags [ONLY_ONE_OBJECT_FLAG] = 0;
#endif

#ifdef PLAYER_INV_FLAG
	flags [PLAYER_INV_FLAG] = 0;
#endif

#ifdef SCRIPTING_ON
	// Script init
	script_pool = level_scripts [level];
#endif

	// Game values (lifes, etc)

	/*plife = l_plife [level]*/; oplife = 0xff;
	pobjs = 0; opobjs = 0xff;
	//pkeys = 0; opkeys = 0xff;
	//pammo = AMMO_MAX; opammo = 0x0ff;

	// The almighty debug line!

	//n_pant = 7; pobjs = 4; pkeys = 5;
}

void game_prepare_screen (void) {
	no_ct = 0;
	update_list [0] = NT_UPD_EOF;
	ppu_wait_nmi ();

	if (first_time) first_time = 0; else fade_out ();
	ppu_off ();

#ifdef ENABLE_SPRINGS
	springs_idx = 0;
#endif

#ifdef ENABLE_PROPELLERS
	propellers_idx = 0;
#endif

	draw_scr ();

#ifdef ENABLE_FUMETTOS
	fumettos_init ();
#endif

#ifdef HOTSPOT_TYPE_KEY
	bolts_update_screen ();
#endif

#ifdef ENABLE_PUAS
	puas_init ();
#endif	

#ifdef PLAYER_CAN_FIRE	
	bullets_ini ();
#endif

#ifdef SHOOTING_DRAINS	
	flower_ini ();
#endif

#ifdef PERSISTENT_ENEMIES
	persistent_update ();
#endif	

#ifdef BREAKABLE_WALLS
	breakable_init ();
#endif

	enems_load ();

#ifndef DISABLE_HOTSPOTS	
	hotspots_load ();
#endif

#ifdef SHOOTING_DRAINS		
	pgauge = 30; bullets_draw_gauge_offline ();
#endif

#ifdef ENABLE_COCOS	
	simplecoco_init ();
#endif

	show_attributes ();

	ppu_on_all ();

	oam_index = 44;
	prx = px >> FIX_BITS; pry = py >> FIX_BITS;
	player_render ();

#ifndef DISABLE_HOTSPOTS	
	hotspots_do ();
#endif	

	enems_do ();

	oam_hide_rest (oam_index);
	ul = update_list; 
	hud_update (); 
	close_update_cycle ();

#ifdef ENABLE_CONTAINERS
	c_idx = 0;
#endif

#ifdef SCRIPTING_ON
	f_zone_ac = 0;
	fzx1 = fzx2 = fzy1 = fzy2 = 0;
	run_script (2 * MAP_W * MAP_H + 1);		// Entering any script
	run_script (n_pant + n_pant);			// Entering this room script
#endif

#ifdef ENABLE_HOLES
	player_register_safe_spot ();
#endif

	// CUSTOM {
	palfx_init ();
	// } END_OF_CUSTOM

	fade_in ();

}

void game_loop (void) {
	half_life = 0;
	first_time = 1;
	on_pant = 99;
	game_res = GS_GAME_OVER;

	// scroll (0, SCROLL_Y);
	// reset_attributes ();
	// gpit = 8; while (gpit --) attr_table [gpit] = 0x55;
	
	ppu_on_all ();

#ifdef PLAYER_FLICKERS
	pflickering = PLAYER_FLICKERS;
#endif	

	music_play (l_music [level]);

	set_vram_update (update_list);
	do_game = 1; ticker = ticks;
	pwashit = 0; 
	pcharacter = 0;

	// Entering Game
#ifdef SCRIPTING_ON
#ifdef CLEAR_FLAGS
	msc_clear_flags ();
#endif
	script_result = 0;
	// Entering game script
	run_script (2 * MAP_W * MAP_H);
#endif

	paused = pad0 = 0; update_list [0] = NT_UPD_EOF;

	while (do_game) {

		//*((unsigned char*)0x2001) = 0x1e;
		ppu_wait_frame ();
		//*((unsigned char*)0x2001) = 0x1f;

		// Press Fire At
#ifdef SCRIPTING_ON
		if (f_zone_ac) {
			if (pry >= fzy1 && pry <= fzy2)
				if (prx >= fzx1 && prx <= fzx2)
					game_run_fire_script ();
		}
#endif

		/*
		if (pkilled 
			|| pobjs == max_hotspots_type_1 
			|| ((pad0 & (PAD_SELECT | PAD_B)) == (PAD_SELECT | PAD_B))
			) do_game = 0;
		*/

		/*
		if (pkilled 
#ifdef SCRIPTING_ON			
			|| script_result == 1
#endif			
		) do_game = 0;
		*/
		// CUSTOM {
		if (pkilled
			|| ((pad0 & (PAD_SELECT|PAD_B)) == (PAD_SELECT|PAD_B))
			|| (level != 2 && script_result == 1)
			|| (level == 2 && n_pant == 29 && en_t [0] == 0 && en_dying [gpit] == 0)
		) do_game = 0;
		// } END_OF_CUSTOM

		#include "engine/mainloop/flick_screen.h"
	
		half_life = 1 - half_life;		// Main flip-flop
		frame_counter ++;				// Increase frame counter
		//if (ticker) ticker --; else ticker = ticks;
	
		// Thanks for this, Nicole & nesdev!
		// https://forums.nesdev.com/viewtopic.php?p=179315#p179315
		pad_this_frame = pad0;
		pad0 = pad_poll (0);			// Read pads here.
		pad_this_frame = (pad_this_frame ^ pad0) & pad0;	
		
		if (pad_this_frame & PAD_START) {
			paused = !paused;
			if (paused) pal_bright (3); else pal_bright (4);
			sfx_play (0, SC_LEVEL);
		}

		if (!paused) {
			ul = update_list;				// Reset pointer to update list
			oam_index = 44;					// Reset OAM index; skip sprite #0 & player

#ifdef ENABLE_PUAS
			puas_do ();
#endif
#ifdef PLAYER_CAN_FIRE
			bullets_do ();
#endif
#ifndef DISABLE_HOTSPOTS		
			hotspots_do ();
#endif		
	
			enems_do ();
#ifdef ENABLE_COCOS		
			simplecoco_do ();
#endif		
#ifdef SHOOTING_DRAINS			
			flower_do ();
#endif
#ifdef BREAKABLE_WALLS
			breakable_do ();
#endif		

			player_move ();	
			if (n_pant == on_pant) player_render ();
			if (pwashit) player_hit ();

#ifdef ENABLE_FUMETTOS
			fumettos_do ();
#endif
#ifdef ENABLE_CONTAINERS
			containers_draw ();
#endif
#ifdef ENABLE_SPRINGS
			if (springs_idx) springs_do ();
#endif
#ifdef ENABLE_PROPELLERS
			if (propellers_idx) propellers_do ();
#endif
			hud_update ();

			oam_hide_rest (oam_index);
			*ul = NT_UPD_EOF;
		}

		// CUSTOM {
		palfx_do ();
		// } END_OF_CUSTOM
	}
	music_stop ();

	set_vram_update (0);
	exit_cleanly ();
}

void game_title (void) {
	enter_screen (/*t_pal_bgs [level]*/ palts1, screen_title);
	pal_spr (/*t_pal_fgs [level]*/ palss1);
	//music_play (MUSIC_TITLE);
	
	half_life = 0;

	/*
	gpit = 0; while (!gpit) {
		// Thanks for this, Nicole & nesdev!
		// https://forums.nesdev.com/viewtopic.php?p=179315#p179315
		pad_this_frame = pad0; 
		pad0 = pad_poll (0);
		pad_this_frame = (pad_this_frame ^ pad0) & pad0;

		if (pad_this_frame & PAD_START) {
			gpit = 1;
		}

		ppu_wait_nmi ();
	}
	while (pad_poll (0));*/

	while (!(pad_poll(0) & PAD_START)) ppu_wait_nmi ();

	music_stop ();
	sfx_play (SFX_START, SC_LEVEL);
	delay (ticks);

	exit_cleanly ();
}

void game_over (void) {
	enter_screen (/*t_pal_bgs [level]*/ palts1, screen_game_over);
	//music_play (MUSIC_EVENT);
	do_screen (10);
}

void game_ending (void) {
	m11_change_raw (2); //bank_bg (1);
	enter_screen (pal_ending, screen_game_ending);
	do_screen (255);
	m11_change_raw (1); //bank_bg (0);
}
