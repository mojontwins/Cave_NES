// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

	// Gyrosaws
	// Clockwise is L U R D, place here:
	// ···
	// ·Xv
	// ·<o with SPEED = 1

	// Counter-clockwise is L D R U, place here:
	// ·<o
	// ·X^
	// ··· with SPEED = 0

	// en_r is GYROSAW_STATE.
	// en_mx is GYROSAW_COUNTER.
	// en_my = 1 means "clockwise" GYROSAW_DIRECTION
	
	if (half_life) {
		delta = (_GYROSAW_DIRECTION ? (_en_state >> 1) : (((_en_state + 1) & 3) >> 1)) ? GYROSAW_V : -GYROSAW_V;
		if (_en_state & 1) _en_y += delta; else _en_x += delta;
		_GYROSAW_COUNTER = (_GYROSAW_COUNTER + GYROSAW_V) & 31; if (!_GYROSAW_COUNTER) _en_state = (_en_state + 1) & 3;
	}
	
	spr_id = GYROSAW_CELL_BASE + half_life;
	// enems_spr ();
