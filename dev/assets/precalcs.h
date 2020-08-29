// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// If you end up not using those in your game, just whipe them for 20 extra bytes (WOW!)
const unsigned char bitmasks [] = { 0xfc, 0xf3, 0xcf, 0x3f };
#ifdef SHOOTING_DRAINS
	const unsigned char jitter [] = { 0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,1 };
#endif

#ifdef PLAYER_CAN_FIRE
	// RIGHT LEFT DOWN UP
	const signed char en_dx [] = {1, -1, 0, 0};
	const signed char en_dy [] = {0, 0, 1, -1};
	const signed char boffsx [] = {12, -4, 0, 0};
	const signed char boffsy [] = {3, 3, 12, -4};
#endif

#ifdef ENABLE_COCO_STRAIGHT
	// LEFT UP RIGHT DOWN
	const signed char coco_vx_precalc [] = { -COCO_V, 0, COCO_V, 0 };
	const signed char coco_vy_precalc [] = { 0, -COCO_V, 0, COCO_V };
#endif

#ifdef ENABLE_MONOCOCO
	const unsigned char monococo_state_times [] = {
		MONOCOCO_BASE_TIME_HIDDEN, MONOCOCO_BASE_TIME_APPEARING, MONOCOCO_BASE_TIME_ONBOARD, MONOCOCO_BASE_TIME_APPEARING
	};
#endif 

// My walk cycle has 6 cells, so...
/*
const unsigned char mod6 [] = {
	0, 1, 2, 3, 4, 5, 
	0, 1, 2, 3, 4, 5, 
	0, 1, 2, 3, 4, 5, 
	0, 1, 2, 3, 4, 5, 
	0, 1, 2, 3, 4, 5, 
	0, 1
};
*/

/*
const unsigned char box_buff [] = {
	17,18,18,18,18,18,18,18,18,18,18,19, 99, 99, 99, 99,
	20,21,21,21,21,21,21,21,21,21,21,22, 99, 99, 99, 99,
	20,21,21,21,21,21,21,21,21,21,21,22, 99, 99, 99, 99,
	23,24,24,24,24,24,24,24,24,24,24,25
};
*/

const signed char jitter_big [] = {
	-1, -1, -1, -4, 4, -3, 0, -4, -2, 3, -3, 2, 1, -1, 0, -2
};

#if defined (ENABLE_PRECALC_FANTY) || defined (ENABLE_PRECALC_HOMING_FANTY)
	#define FANTY_INCS_MAX 16
	const signed char fanty_incs [] = {
		0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1
	};
#endif

#ifdef ENABLE_PRECALC_HOMING_FANTY
	#define FANTY_RETREAT_INCS_MAX 4
	const signed char fanty_retreat_incs [] = {
		1, 0, 0, 0
	};
#endif
