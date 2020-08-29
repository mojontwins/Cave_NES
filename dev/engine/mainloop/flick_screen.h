		// Change screen
		// Very basic. Extend when needed.

		if (prx == 4 && pvx < 0) {
			n_pant --;
			px = 244 << FIX_BITS;
		} else if (prx == 244 && pvx > 0) {
			n_pant ++;
			px = 8<<FIX_BITS;
		} else if (pry == 0 && pvy < 0 && n_pant >= map_w) {
			n_pant -= map_w;
			py = 192<<FIX_BITS;
#if defined (PLAYER_JUMPS) || defined (PLAYER_MONONO)
			pvy = -PLAYER_VY_FLICK_TOP;
#endif
		} else if (pry >= 192 && pvy > 0) {
			n_pant += map_w;
			py = 0;
		}

		if (on_pant != n_pant && do_game) {
			prx = px >> FIX_BITS;
			pry = py >> FIX_BITS;
			game_prepare_screen ();
			on_pant = n_pant;
		} 
