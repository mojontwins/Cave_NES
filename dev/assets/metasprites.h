// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// Metaspritesets (in nice arrays)

#include "assets/spritedata.h"

// Of course, you have to somewhat customize this for every game and make it fit.

// Rock the Kashbah
const unsigned char * const spr_enems_1 [] = {
	ssena_00_a, ssena_01_a, ssena_00_b, ssena_01_b,		// Lizardy
	ssenc_00, ssenc_01, ssenc_00, ssenc_01, 			// Goldfish phantoma
	ssenb_00, ssenb_01, ssenb_02, ssenb_03,				// Coola. Remeber: different animation. Custom-patch it!
	ssenc_06, ssenc_07, ssenc_06, ssenc_07,				// Moving platform

	// Separate stuff.
	// 16
	// SAW
	ssenc_02, ssenc_03,									// ssenc_04 is the mask overlay
	
	// 18
	// FANTY (bat)
	ssend_00, ssend_01, ssend_02, ssend_01,

	// 22
	// Lame boss
	ssboss_00_a, ssboss_01_a, ssboss_00_b, ssboss_01_b
};

// RIGHT, LEFT x IDLE, W1 W2 W3 W4, JUMP, NIL, GUAY
// U1 U2 U3 U4 U5 U6 GUAY NIL
const unsigned char * const spr_player_0 [] = {
	// 0
	sspla_00_a,											// Idle
	sspla_01_a, sspla_02_a, sspla_03_a, sspla_04_a,		// Walk
	sspla_05_a,											// Airborne
	0, 													// Reserved
	ssplb_06, 											// Guay

	// 8
	sspla_00_b,
	sspla_01_b, sspla_02_b, sspla_03_b, sspla_04_b,
	sspla_05_b,
	0,
	ssplb_06, 											

	// 16: Use animation [+ 0..17]
	ssplb_00, ssplb_01, ssplb_02, 						// Turn around
	ssplb_03, ssplb_04, ssplb_05, 						// Crouch
	ssplb_05, ssplb_04, ssplb_03, ssplb_02,				// Back up
	ssplb_01, ssplb_00, 								// Face camera
	ssplb_06, ssplb_06, ssplb_06, 
	ssplb_06, ssplb_06, ssplb_06, 						// Lock on win pose

	// 34: Single win pose for whatever.
	ssplb_06
};

// Items are: [nothing] object key refill
// The wasted space saves a bit of code
const unsigned char * const spr_items [] = {
//	frame    braga    heart    eng1     eng2     eng3     weight   lever
	ssit_00, ssit_01, ssit_02, ssit_03, ssit_04, ssit_05, ssit_06, ssit_07,
//  key               floppy
	ssit_08, 0      , ssit_0A
};

// FUCK

const unsigned char sprplempty [] = {
	-4, -8, 0, 0, 4, -8, 0, 0, 
	-4, 0, 0, 0, 4, 0, 0, 0, 
	-4, 8, 0, 0, 4, 8, 0, 0, 
	0x80
};
