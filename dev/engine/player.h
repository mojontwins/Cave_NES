// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// Player movement & stuff

#ifdef PLAYER_SAFE
void player_register_safe_spot (void) {
	safe_prx = prx; safe_pry = pry;
	safe_n_pant = n_pant;
	safe_pinv = pinv;
}

void player_restore_safe_spot (void) {
	prx = safe_prx; px = prx << FIX_BITS;
	pry = safe_pry; py = pry << FIX_BITS;
	n_pant = safe_n_pant;
	pinv = safe_pinv;	
}
#endif

#ifdef ENABLE_HOLES
void player_holed (void) {
	pholed = 48; 
	pvx = ADD_SIGN(pvx, PLAYER_V_INERTIA);
	pvy = ADD_SIGN(pvy, PLAYER_V_INERTIA);
	pad = 0;
}
#endif

void player_reset_movement (void) {
	pvx = pvy = 0;
	pfixct = pfiring = 0;
	phit = 0;
	pvylast = 0;
#ifdef PLAYER_JUMPS	
	pjustjumped = 0;
#endif
#ifdef PLAYER_JETPAC	
	pthrust = 0;
#endif
#ifdef PLAYER_SPINS	
	pspin = 0;
#endif	
#ifdef PLAYER_BUTT
	pbutt = 0;
#endif
#ifdef ENABLE_HOLES
	pholed = 0;
#endif
#ifdef ENABLE_FUMETTOS
	thrustct = 0;
#endif	
}

#ifdef PLAYER_PROCESS_BLOCK
void player_process_block (signed char x, signed char y, unsigned char direction) {
	if (y) y --;
	rdc = x + (y << 4);
	rda = scr_buff [rdc];
	switch (rda) {
#ifdef PLAYER_PUSH_BOXES		
		case 14:
#ifdef PLAYER_FIRE_TO_PUSH
			if (pad & PAD_A) 
#endif		
			{
				// push block
#ifdef PLAYER_GRAVITY
				pbx1 = x;
				if (pad & PAD_LEFT) {
					if (x > 0) if (0 == scr_attr [x - 1 + (y << 4)]) pbx1 = x - 1;
				} else if (pad & PAD_RIGHT) {
					if (x < 15) if (0 == scr_attr [x + 1 + (y << 4)]) pbx1 = x + 1;
				}
				if (pbx1 != x) {
					set_map_tile (x, y, 0); set_map_tile (pbx1, y, 14);
					sfx_play (SFX_BUTT_HIT, SC_LEVEL);
				}
#endif		
#ifdef PLAYER_GENITAL
				pbx1 = x; pby1 = y;
				if (direction) {
					if (pad & PAD_LEFT) {
						if (x > 0) if (0 == scr_attr [x - 1 + (y << 4)]) pbx1 = x - 1;
					} else if (pad & PAD_RIGHT) {
						if (x < 15) if (0 == scr_attr [x + 1 + (y << 4)]) pbx1 = x + 1;
					}
				} else {
					if (pad & PAD_UP) {
						if (y > 0) if (0 == scr_attr [x + ((y - 1) << 4)]) pby1 = y - 1;
					} else if (pad & PAD_DOWN) {
						if (y < 11) if (0 == scr_attr [x + ((y + 1) << 4)]) pby1 = y + 1;
					}
				}
				if (pbx1 != x || pby1 != y) {
					set_map_tile (x, y, 0); set_map_tile (pbx1, pby1, 14);
					sfx_play (SFX_BUTT_HIT, SC_LEVEL);
				}
#endif

#ifdef PLAYER_FIRE_TO_PUSH				
				pfiring = 1;
#endif								
			}
			break;
#endif
#ifdef HOTSPOT_TYPE_KEY			
		case 15:
			// Key hole
			if (pkeys) {
				pkeys --;
				bolts_clear_bolt ();
				set_map_tile (x, y, 0);
				sfx_play (SFX_CHOF, SC_LEVEL);
			}
			break;
#endif				
	}
}
#endif

void player_init (void) {
	// Init player data
	prx = (signed int) (player_ini_x [level] << 4); px = prx << FIX_BITS;
	pry = (signed int) (player_ini_y [level] << 4); py = pry << FIX_BITS;

#ifdef PLAYER_SAFE
	player_register_safe_spot ();
#endif
	
#ifdef PLAYER_GENITAL
	pfacing = pfacingv = CELL_FACING_DOWN;		
	pfacinglast = pfacingh = CELL_FACING_RIGHT;
#else
	pfacing = CELL_FACING_RIGHT;
#endif

	player_reset_movement ();
	pkilled = 0;
}

void player_render (void) {
	if (!pflickering || half_life) {
		oam_meta_spr (prx, pry + SPRITE_ADJUST, 20, spr_player [psprid]);	
	} else {
		oam_meta_spr (prx, pry + SPRITE_ADJUST, 20, sprplempty);
	}

	if (no_ct) {
		no_ct --;
		oam_index = oam_meta_spr (
			prx + 2, pry - 24 + SPRITE_ADJUST,
			oam_index,
			SPRITE_NO
		);
	}
}

void player_hit (void) {
	sfx_play (SFX_ENEMY_HIT, SC_PLAYER);

	close_update_cycle ();

#ifdef PLAYER_DIE_AND_RESPAWN
	player_render ();
	music_pause (1);
	delay (60);
	music_pause (0);
	player_reset_movement ();
	player_restore_safe_spot ();
#endif

#ifdef ENABLE_HOLES
	if (pholed) {
		player_reset_movement ();
		player_restore_safe_spot ();
		pholed = 0;
	}
#endif

#ifdef PLAYER_FLICKERS
	pflickering = PLAYER_FLICKERS;
#endif
	
	if (plife) {
		plife --;
	} else pkilled = 1;

	phit = PLAYER_HIT_FRAMES;

	pwashit = 0;
}

void player_move (void) {
	// Player state
	if (phit) phit --;
	if (pflickering) pflickering --;

	// Refine this with use_ct, guay_ct, etc.
	pad = 0;

#ifdef FIRE_SCRIPT_WITH_ANIMATION
	if (use_ct) {
		psprid = CELL_USE_BASE + use_ct;
		if (fr_ct) fr_ct --; else {
			use_ct ++;
			if (use_ct == 7) {
				if (containers_get) {
					sfx_play (SFX_GET_ITEM, SC_LEVEL);
					if (flags [PLAYER_INV_FLAG] != flags [containers_get]) {
						rda = flags [PLAYER_INV_FLAG];
						flags [PLAYER_INV_FLAG] = flags [containers_get];
						flags [containers_get] = rda;
						commands_executed = 1;
					}
#ifdef CONTAINER_ACTION_STOPS_CHECKS
				} else {
#else
				}
				// CUSTOM { only on screen 27
				if (0 == commands_executed || n_pant >= 26) {
#endif
					game_run_fire_script ();
				}
			}
			if (use_ct == 13 && !commands_executed) {
				use_ct = 0;
				pfacing = CELL_FACING_RIGHT;
				no_ct = ticks;
			}
			if (use_ct == 18) use_ct = 0;
			fr_ct = 6;
		}
	} else if (guay_ct) {
		guay_ct --;
		if (guay_ct == 0) pflickering = ticks;
	} else
#endif 
		pad = pad0;

#ifdef ENABLE_HOLES
	if (pholed) pad = 0;
#endif	

#ifdef ENABLE_EVIL_TILE
	evil_tile_hit = 0;
#endif

#ifdef EVIL_TILE_MULTI
	pcx = px; pcy = py;
#endif

	// ********
	// Vertical
	// ********

#ifdef PLAYER_GRAVITY
	// Gravity
	if (!pgotten
#ifdef PLAYER_JETPAC
		&& !pthrust
#endif
	) {
		if (!pj) pvy += PLAYER_G; else pvy += PLAYER_G_JUMPING;
		if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX;
	}
#endif

#ifdef PLAYER_GENITAL
	// Poll pad
	if (pad & PAD_UP) {
		if (!pfixct) if (pvy > -PLAYER_VY_MAX) {
			pvy -= PLAYER_AY;			
		}
		pfacingv = CELL_FACING_UP;
	} else if (pad & PAD_DOWN) {
		if (!pfixct) if (pvy < PLAYER_VY_MAX) {
			pvy += PLAYER_AY;
		}
		pfacingv = CELL_FACING_DOWN;		
	} else {
#ifdef ENABLE_HOLES
		if (!pholed)
#endif
			if (pvy > 0) {
				pvy -= PLAYER_RY; if (pvy < 0) pvy = 0; 
			} else if (pvy < 0) {
				pvy += PLAYER_RY; if (pvy > 0) pvy = 0;
			}

		pfacingv = 0xff;
	}
#endif

	// Move
	py += pvy;

	// Collision
	//prx = px >> FIX_BITS;
	if (py < 0) { py = pry = 0; }
	else if (py > (192<<FIX_BITS)) pry = 192;
	else pry = py >> FIX_BITS;

	cx1 = prx >> 4;
	cx2 = (prx + 7) >> 4;
	if (pvy + pgtmy) {
		if (pvy + pgtmy < 0) {
#ifdef PLAYER_8_PIXELS
			cy1 = cy2 = (pry + 6) >> 4;
#else
			cy1 = cy2 = pry >> 4;
#endif
			cm_two_points ();
#ifdef PLAYER_GENITAL
			if ((at1 & 12) || (at2 & 12))
#else
			if ((at1 & 8) || (at2 & 8))
#endif
			{
				pgotten = pvy = 0;
#ifdef PLAYER_8_PIXELS
				pry = ((cy1 + 1) << 4) - 6;
#else
				pry = (cy1 + 1) << 4;
#endif
				py = pry << FIX_BITS;
			} else {

#ifdef EVIL_TILE_MULTI
				if ((at1 & 1) || (at2 & 1)) {
					pvy = PLAYER_V_REBOUND_MULTI; evil_tile_hit = 1;
				}
#endif	

#ifdef ENABLE_HOLES
				if (!pholed) {
					cy1 = cy2 = (pry + 14) >> 4;
					cm_two_points ();
					if ((at1 & 2) && (at2 & 2)) player_holed ();
				} 
#endif					
			}
		} else if (pvy + pgtmy > 0)	{
			cy1 = cy2 = (pry + 15) >> 4; 
			cm_two_points (); 
	 		if (((pry - 1) & 15) < 4 && ((at1 & 12) || (at2 & 12))) {
				pgotten = pvy = 0;
				pry = (cy1 - 1) << 4;
				py = pry << FIX_BITS;
			} else {
#ifdef ENABLE_HOLES
				if (!pholed && ((at1 & 2) && (at2 & 2))) player_holed ();
#endif			

#ifdef EVIL_TILE_MULTI
				if ((at1 & 1) || (at2 & 1)) {
					pvy = -PLAYER_V_REBOUND_MULTI; evil_tile_hit = 1;
				}
#endif			
			}
		}
#ifdef PLAYER_PROCESS_BLOCK
#ifdef PLAYER_GENITAL		
		if (at1 == 10) player_process_block (cx1, cy1, 0);
		if (at2 == 10) player_process_block (cx2, cy2, 0);
#endif		
#endif
	}

#ifdef PLAYER_GRAVITY
	// Floor detections: possee

	cy1 = cy2 = (pry + 16) >> 4;
	cm_two_points ();
	ppossee = ((at1 & 12) || (at2 & 12));
#ifdef PLAYER_JUMPS	
	if (ppossee || pgotten) {
		pjustjumped = 0;
#ifdef PLAYER_DOUBLE_JUMP
		njumps = 0;
#endif		
	}
#endif	
#endif	

	// Floor detections: Conveyors
	
#ifdef ENABLE_CONVEYORS
	if (ppossee) {
		if (at1 & CONVEYOR_BIT) { if (at1 & 1) pgtmx = CONVEYOR_SPEED; else pgtmx = -CONVEYOR_SPEED; pgotten = 1; }
		if (at2 & CONVEYOR_BIT) { if (at2 & 1) pgtmx = CONVEYOR_SPEED; else pgtmx = -CONVEYOR_SPEED; pgotten = 1; }
	}
#endif	

	// Floor detections: Propellers
#ifdef ENABLE_PROPELLERS
	if ((at1 & PROPELLERS_BIT) || (at2 & PROPELLERS_BIT)) {
		pvy -= PLAYER_G + PLAYER_AY_PROPELLER;
		if (pvy < -PLAYER_VY_PROPELLER_MAX) pvy = -PLAYER_VY_PROPELLER_MAX;
	}
#endif

	// Floor detections: Evil tile

#ifdef ENABLE_EVIL_TILE
#ifdef EVIL_TILE_ON_FLOOR
	cy1 = cy2 = (pry + 12) >> 4;	// Let it bleed a bit
	cm_two_points ();
	if (at1 == 1 || at2 == 1) evil_tile_hit = 1;
#endif	
#endif

	// Jump!

#ifdef PLAYER_JUMPS
	if (pad & PAD_A) {
		if (!pjb) {
			pjb = 1;
			if (!pj) {
				if (
					(pgotten || ppossee || phit)
#ifdef PLAYER_DOUBLE_JUMP
					|| (njumps < 2)
#endif
				) {
#ifdef PLAYER_DOUBLE_JUMP					
					if (njumps) {
						fumettos_add ();
						pctj = 2;
					} else
#endif					
					pctj = 0; 
#ifdef PLAYER_DOUBLE_JUMP
					njumps ++;
#endif					
					pj = 1; pjustjumped = 1;
					pvy = -PLAYER_VY_JUMP_INITIAL;
					sfx_play (SFX_JUMP, SC_PLAYER);

#ifdef PLAYER_DIE_AND_RESPAWN
					if (ppossee) {
						player_register_safe_spot ();
					}
#endif					
				}
			} 
		}
		if (pj) {
			pctj ++; if (pctj == PLAYER_VY_JUMP_A_STEPS) pj = 0;
		}
	} else {
		pjb = 0;
		if (pj) {
			if (pvy < -PLAYER_VY_JUMP_RELEASE) pvy = -PLAYER_VY_JUMP_RELEASE;
			pj = 0;
		}
	}
#endif

	// Monono!

#ifdef PLAYER_MONONO
	if (!(pad & PAD_A)) { pj = 0; pvylast = -PLAYER_VY_JUMP_INITIAL; } else {
		if (ppossee || pgotten) {
			pvy = SATURATE_N (pvylast - PLAYER_AY_JUMP, -PLAYER_VY_JUMP_MAX);
			pvylast = pvy;
			pj = 1;
			sfx_play (SFX_JUMP, SC_PLAYER);
		}
	}
#endif	

#ifdef PLAYER_JETPAC
	if (pad & PAD_A) {
		pvy = SATURATE_N (pvy - PLATER_AY_JETPAC, -PLAYER_VY_JETPAC_MAX);		
		pthrust = 1;
		if (!(thrustct)) fumettos_add ();
				thrustct ++; if (thrustct == 7) thrustct = 0; // so it stays out of phase
	} else thrustct = pthrust = 0;
#endif

#ifdef PLAYER_SPINS
	if (!pvy < 0) pspin = 0;
	if (pad & PAD_DOWN) {
		if (ppossee && ABS (pvx) > PLAYER_VX_MIN) {
			pspin = 1; sfx_play (SFX_BUTT_FALL, SC_PLAYER);
		}
	}
#endif

	// **********
	// Horizontal
	// **********
	
	// Poll pad
	if (pad & PAD_LEFT) {
		if (!pfixct) if (pvx > -PLAYER_VX_MAX) {
			pvx -= PLAYER_AX;			
		}
#ifdef PLAYER_GENITAL
		pfacinglast = pfacingh = CELL_FACING_LEFT;
#else
		pfacing = CELL_FACING_LEFT;
#endif
	} else if (pad & PAD_RIGHT) {
		if (!pfixct) if (pvx < PLAYER_VX_MAX) {
			pvx += PLAYER_AX;
		}
#ifdef PLAYER_GENITAL		
		pfacinglast = pfacingh = CELL_FACING_RIGHT;		
#else
		pfacing = CELL_FACING_RIGHT;
#endif		
	} else {
#ifdef PLAYER_SPINS
		if (!pspin)
#endif

#ifdef ENABLE_HOLES
		if (!pholed)
#endif			
			if (pvx > 0) {
				pvx -= PLAYER_RX; if (pvx < 0) pvx = 0;
			} else if (pvx < 0) {
				pvx += PLAYER_RX; if (pvx > 0) pvx = 0;
			}
#ifdef PLAYER_GENITAL
		pfacingh = 0xff;
#endif		
	}

	// Move
	px += pvx;		
	if (pgotten) px += pgtmx;
	
	// Collision
	if (px < (4<<FIX_BITS)) prx = 4;
	else if (px > (244<<FIX_BITS)) prx = 244;
	else prx = px >> FIX_BITS;


#ifdef PLAYER_8_PIXELS
	cy1 = (pry + 6) >> 4;
#else
	cy1 = pry >> 4;
#endif
	cy2 = (pry + 15) >> 4;
	if (pvx + pgtmx) {
		if (pvx + pgtmx < 0) {
			cx1 = cx2 = prx >> 4;
			cm_two_points ();
#ifdef PLAYER_GENITAL
			if ((at1 & 12) || (at2 & 12)) {
#else
			if ((at1 & 8) || (at2 & 8)) {
#endif
				pvx = 0;
				prx = (cx1 + 1) << 4;
				px = prx << FIX_BITS;
			} else {

#ifdef ENABLE_HOLES
				if (!pholed && ((at1 & 2) && (at2 & 2))) player_holed ();
#endif

#ifdef EVIL_TILE_MULTI
#ifdef PLAYER_SMALLER_ETCB
				cy1 = (pry + PLAYER_ETCB_UP) >> 4;
				cy2 = (pry + PLAYER_ETCB_DOWN) >> 4;
				cx1 = cx2 = (prx + PLAYER_ETCB_LEFT) >> 4;
				cm_two_points (); 
#endif
				if ((at1 & 1) || (at2 & 1)) {
					pvx = PLAYER_V_REBOUND_MULTI; evil_tile_hit = 1;
				}
#endif
			}
		} else if (pvx + pgtmx > 0)	{
			cx1 = cx2 = (prx + 7) >> 4; 
			cm_two_points (); 
#ifdef PLAYER_GENITAL
			if ((at1 & 12) || (at2 & 12)) {
#else
	 		if ((at1 & 8) || (at2 & 8)) {
#endif
				pvx = 0;
				prx = ((cx1 - 1) << 4) + 8;
				px = prx << FIX_BITS;
			} else {

#ifdef ENABLE_HOLES
				if (!pholed && ((at1 & 2) && (at2 & 2))) player_holed ();
#endif			

#ifdef EVIL_TILE_MULTI
#ifdef PLAYER_SMALLER_ETCB
				cy1 = (pry + PLAYER_ETCB_UP) >> 4;
				cy2 = (pry + PLAYER_ETCB_DOWN) >> 4;
				cx1 = cx2 = (prx + PLAYER_ETCB_RIGHT) >> 4;
				cm_two_points (); 
#endif
				if ((at1 & 1) || (at2 & 1)) {
					pvx = -PLAYER_V_REBOUND_MULTI; evil_tile_hit = 1;
				}
#endif
			}
		}
#ifdef PLAYER_PROCESS_BLOCK
		if (at1 == 10) player_process_block (cx1, cy1, 1);
		if (at2 == 10) player_process_block (cx2, cy2, 1);
#endif
	}

	// Evil tile (continued)

#ifdef ENABLE_EVIL_TILE
	if (evil_tile_hit) {
#ifdef PLAYER_SPINS
		pspin = 0;
#endif
#ifdef PLAYER_JUMPS
		pjustjumped = 0;
#endif

#ifdef EVIL_TILE_ON_FLOOR
		pvy = -PLAYER_V_REBOUND; 
#endif
#ifdef EVIL_TILE_MULTI
		px = pcx; py = pcy;
#endif
		if (
#ifdef PLAYER_FLICKERS
		!pflickering
#else				
		!phit
#endif
		) pwashit = 1; //player_hit ();
	}
#endif	

	// Hidden
#ifdef ENABLE_ESPECTROS
	cx1 = cx2 = (prx + 4) >> 4;
	cy1 = cy2 = (pry + 8) >> 4;
	cm_two_points ();
	phidden = (ABS (pvy) < PLAYER_VY_MIN) && (ABS (pvx) < PLAYER_VX_MIN) && (at1 & 2);
#endif

	// Spinning

#ifdef PLAYER_SPINS
	if (!pvx) pspin = 0;
#endif

	// Can't remember

#ifdef PLAYER_TURRET
	if (pfixct) pfixct --;
#endif

	// PAD B Stuff!
	// In order:
	// 1. containers
	// 2. scripting
	// 3. fire

	// If you are using "FIRE_SCRIPT_WITH_ANIMATION", well...
	// that's incompatible with PLAYER_CAN_FIRE. Unless you work it out.

	// For genital games you *should* change PAD_B to PAD_A

	if (pad & PAD_B) {
#ifdef ENABLE_CONTAINERS
		commands_executed = containers_get = 0;
		if (!pfiring) {
			containers_do ();
#ifdef CONTAINERS_HAS_GOT_FLAG			
			flags [CONTAINERS_HAS_GOT_FLAG] = containers_get;
#endif
			if (containers_get) {
#ifdef FIRE_SCRIPT_WITH_ANIMATION
				player_reset_movement ();
				use_ct = 1;
#else
				rda = flags [PLAYER_INV_FLAG];
				flags [PLAYER_INV_FLAG] = flags [containers_get];
				flags [containers_get] = rda;
#endif
#ifdef CONTAINER_ACTION_STOPS_CHECKS
				pfiring = 1;
#endif
			}
		}
#endif

#ifdef SCRIPTING_ON
		if (!pfiring) {
#ifdef FIRE_SCRIPT_WITH_ANIMATION
			if (ppossee){
				player_reset_movement ();
				use_ct = 1;
			} 
#else
			game_run_fire_script ();
			if (commands_executed) pfiring = 1;
#endif
		}
#endif

#ifdef PLAYER_PUAS
		if (!pfiring) {
			sfx_play (SFX_SHOOT, SC_PLAYER);
			puas_create ();
		}
#endif

#ifdef PLAYER_CAN_FIRE
		if (!pfiring) {
			bullets_fire ();
#ifdef PLAYER_TURRET		
			pfixct = TURRET_FRAMES;
#endif	
		}
#endif
		pfiring = 1;
	} else pfiring = 0;

/*
#ifdef PLAYER_CAN_FIRE
	if (pad & PAD_A) {
		if (!pfiring) bullets_fire ();
		pfiring = 1;
#ifdef PLAYER_TURRET		
		pfixct = TURRET_FRAMES;
#endif		
	} else pfiring = 0;
#endif	
*/

	// Butt

#ifdef PLAYER_BUTT
	if (ppossee || phit) pbutt = 0; else {
		if (pad & PAD_B) {
			if (!pbutt) {
				pbutt = 1; pj = 0;
				sfx_play (SFX_BUTT_FALL, SC_PLAYER);			
			}
		}
	}
#endif	

	// Hitter todo
	/*
	if ((pad & PAD_B) && !hitter_on && pstatespradder) {
		hitter_on = 1; hitter_frame = 0;
		sfx_play (SFX_SHOOT, SC_PLAYER);
	}
	*/
	
	// Solve facing
#ifdef PLAYER_GENITAL	
	if (pfacingv != 0xff) pfacing = pfacingv; else if (pfacingh != 0xff) pfacing = pfacingh;
#endif

	// Sprite cell selection is usually pretty custom...

	/*
	if (pholed) {
		pholed --;
		psprid = CELL_PHOLED + (pholed >> 4);
		if (pholed == 1) player_hit ();
	} else {
		rdx = ABS (pvx); rdy = ABS (pvy);
		if (rdx < PLAYER_VX_MIN && rdy < PLAYER_VY_MIN) {
			psprid = CELL_IDLE;
		} else if (rdx >= rdy) {
			psprid = CELL_WALK_BASE + ((prx >> 4) & 3);
		} else {
			psprid = CELL_WALK_BASE + ((pry >> 4) & 3);
		}

		psprid += pcharacter + pfacing;
	}
	*/
	if (use_ct) {

	} else {
		if ((ppossee || pgotten) && pvy >= 0) {
			if (guay_ct) 
				psprid = CELL_GUAY;
			else if (ABS (pvx) > PLAYER_VX_MIN) {
				psprid = CELL_WALK_BASE + ((prx >> 4) & 3);
			} else psprid = CELL_IDLE;
		} else {
			psprid = CELL_AIRBORNE;
		}

		psprid += pfacing;
	}
}
