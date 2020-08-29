// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// Shiru's

#include "neslib.h"

// Some needed stuff...

#include "definitions.h"

// Main const includes

#include "assets/palettes.h"
#include "assets/precalcs.h"
#include "assets/enems0.h"
#include "assets/map0.h"
#include "assets/enems1.h"
#include "assets/map1.h"
#include "assets/enems2.h"
#include "assets/map2.h"
#include "assets/tiledata.h"
#include "assets/metasprites.h"
#include "assets/behs.h"
#include "assets/levelset.h"
// #include "assets/texts.h"
// #include "assets/script_texts.h"
// #include "assets/talk_scripts.h"
#include "assets/title_rle.h"
#include "assets/ending_rle.h"
#include "assets/hud_rle.h"
#include "assets/lm_precalcs.h"

// Some variables

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name(push,"ZEROPAGE")

#include "ram/zp.h"

#pragma bss-name (push,"BSS")
#pragma data-name(push,"BSS")

#include "ram/bss.h"

// Engine functions

#include "someprotos.h"
#include "engine/printer.h"
#include "engine/screens.h"
#include "engine/general.h"
#ifdef ENABLE_SPRINGS
#include "engine/spring.h"
#endif
#ifdef ENABLE_PROPELLERS
#include "engine/propellers.h"
#endif
#ifdef SCRIPTING_ON
#ifdef ENABLE_EXTERN_CODE
#include "engine/extern.h"
#endif
#include "engine/mscnes.h"
#endif
#ifdef SHOOTING_DRAINS
#include "engine/flower.h"
#endif
#ifdef BREAKABLE_WALLS
#include "engine/breakable.h"
#endif
#ifdef PLAYER_CAN_FIRE
#include "engine/bullets.h"
#endif
#ifndef DISABLE_HOTSPOTS
#include "engine/hotspots.h"
#endif
#ifdef HOTSPOT_TYPE_KEY
#include "engine/bolts.h"
#endif
#ifdef ENABLE_FUMETTOS
#include "engine/fumettos.h"
#endif
#ifdef ENABLE_PUAS
#include "engine/puas.h"
#endif
// CUSTOM {
// #include "engine/hitter.h"
// } END_OF_CUSTOM
#include "engine/hud.h"
#ifdef ENABLE_CONTAINERS
#include "engine/containers.h"
#endif
#include "engine/player.h"
#ifdef ENABLE_COCOS
#include "engine/simplecoco.h"
#endif
#include "engine/enems.h"
#include "engine/mapper11.h"
// #include "engine/talkbox.h"
#include "engine/game.h"

// Functions

void main (void) {

#ifdef CNROM
	m11_change_raw (1);
#else
	m11_handle_reset ();		// If bad checksum, this jumps to PRG 0 / CHR 0 (main menu)
#endif
	
#ifdef MAP_ENABLE_AUTOSHADOWS	
	mapmode_autoshadows = 1;
#endif
	
	bank_bg (0);
	bank_spr (1);
			
	ticks = ppu_system () ? 60 : 50;
	halfticks = ticks >> 1;

#ifdef MAP_USE_ALT_TILE	
	c_alt_tile = MAP_USE_ALT_TILE;
#endif	

	oam_size (0);
	pal_bright (0);

	scroll (0, SCROLL_Y);
	
	// Run the game here.
	while (1) {
		game_title ();

		level = 0;
		plife = LIFE_INI;

		while (1) {
			hud_draw ();
			game_init ();
			game_loop ();

			if (pkilled) {
				game_over ();
				break;
			} else {
				level ++;
				if (level == 3) {
					game_ending ();
					break;
				}
				delay (10);
			}
		}
	}

}
