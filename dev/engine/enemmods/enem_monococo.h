// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

	// All hail the monococo!
	
	// Custom version for Sonic Mal
	
	// x1, y1 is where it appears.
	// mx is state. my is counter.


	// Counter & state change
	_MONOCOCO_COUNTER --; if (!_MONOCOCO_COUNTER) {
		_en_state = (_en_state + 1) & 3; _MONOCOCO_COUNTER = monococo_state_times [_en_state] - (rand8 () & 0x15);
	}

	// Shoot
	if (_en_state == 2 && _MONOCOCO_COUNTER == MONOCOCO_FIRE_COCO_AT) {
		rdx = _en_x + 4; rdy = _en_y + 4; simplecoco_aimed_new ();
		sfx_play (SFX_SHOOT, SC_ENEMS);
	}

	// Sprite
	// enems_spr ();
	if (!_en_state || (_en_state != 2 && half_life)) spr_id = 0xff; else spr_id = MONOCOCO_CELL_BASE + (_en_state == 2) + (prx < _en_x ? 2 : 0);

