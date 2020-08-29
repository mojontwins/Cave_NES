// MT MK2 NES v0.7d
// Copyleft 2017 by The Mojon Twins

// Main definitions

// Communications

	#define COMM_POOL						((unsigned char*)0x0300)	// Mapper 113 stuff.

	#define COMM_REG						(COMM_POOL[0x0])
	#define COMM_GAME_SELECT				(COMM_POOL[0x1])
	#define COMM_GAME_FLAGS 				(COMM_POOL[0x2])
	#define COMM_CHECKSUM 					(COMM_POOL[0xf])

// Game configuration

	// Single game ROM
	
// Macros

	#define UPDATE_LIST_SIZE				32
	#define MSB(x)							((x)>>8)
	#define LSB(x)							((x)&0xff)
	#define TOP_ADJUST						4
	#define SCROLL_Y 						476
	#define SPRITE_ADJUST					(TOP_ADJUST-2) * 8 - 1 + 8 - (SCROLL_Y - 472)
	#define FIX_BITS						4
	#define FADE_DELAY 						1
	#define GAME_AREA_ADR					NAMETABLE_A + (TOP_ADJUST << 5)

	#define SGNI(n)							((n)<0?-1:1)
	#define SGNC(a,b,c)						((a)<(b)?-(c):(c))
	#define SATURATE_Z(n)					((n)<0?0:(n))
	#define SATURATE(v,max)					((v)>=0?((v)>(max)?(max):(v)):((v)<-(max)?-(max):(v)))
	#define SATURATE_N(v,min)				(v<min?min:v)
	#define ABS(n)							((n)<0?-(n):(n))
	#define DELTA(a,b)						((a)<(b)?(b)-(a):(a)-(b))
	#define CL(x1,y1,x2,y2)					((x1)+4>=(x2)&&(x1)<=(x2)+12&&(y1)+12>=(y2)&&(y1)<=(y2)+12)
	#define SUBSTR_SAT(a,b)					((a)>(b)?(a)-(b):0)
	#define DISABLE_SFX_NEXT_FRAME			*((unsigned char*)0x01bf)=1;
	#define ADD_SIGN2(a,b,v)				((a)==(b))?(0):(((a)>(b))?(v):(-(v)))
	#define ADD_SIGN(a,v) 					((a)?(((a)>0)?(v):(-(v))):(0))

	// Change this if you change where digits are stored in the pattern table
	#define DIGIT(n) 						(16+(n))

// Game definitions

	#define MAP_W							6
	#define MAP_H							5
	#define MAX_PANTS						MAP_W * MAP_H
	#define N_ENEMS							3
	#define BOLTS_MAX 						1
	#define ENEMS_GENERAL_LIFE				1
	#define ENEMS_GENERAL_DYING_FRAMES		16
	#define ENEMS_GENERAL_HIT_FRAMES		16

	#define LIFE_INI						5
	#define LIFE_REFILL						1

// Game engine values

	//#define PERSISTENT_DEATHS
	//#define PERSISTENT_ENEMIES
	//#define ENEMS_SUFFER_WHEN_HITTING_PLAYER
	//#define ENEMS_MIN_KILLABLE			0x12
	#define ENEMS_DIE_ON_EVIL_TILE

// Printer

	// Choose:
	#define PRINT_PSEUDOASCII				// patterns 0-63 == ASCII 32-95
	//#define PRINT_CUSTOM					// Custom 0xff-terminated strings (mkts)
	//#define PRINT_UL_PRINTER				// Used from the TEXT command (Scripting) 
	
// Map types & configurations

	//#define ENABLE_HOLES					// Type 2 = hole in the ground, for top-down view games.

	#define ENABLE_CONVEYORS
	#define CONVEYOR_SPEED					8
	#define CONVEYOR_BIT 					32

// Map format

	#define MAP_FORMAT_RLE53
	//#define MAP_FORMAT_BYTE_RLE
	//#define MAP_FORMAT_PACKED
	//#define MAP_USE_ALT_TILE				16
	//#define MAP_ENABLE_AUTOSHADOWS

	//#define MAP_DONT_PAINT_TILE_0			// If you want to use backdrops, make the painter skip tile 0
	//#define MAP_CHECK_TOP
	//#define MAP_CHECK_BOTTOM

	#define MAP_WITH_DECORATIONS

// Enems format

	//#define ENEMS_FORMAT_UNPACKED			// Good ol' unpacked format.
	#define ENEMS_FORMAT_COMPACTED			// Only active enems stored + index

// Linear enemies

	#define ENABLE_LINEAR
	#define ENABLE_PLATFORMS				
	//#define LINEAR_COLLIDES				// Collides with BG & 8.
	//#define LINEAR_COLLIDE_EVERYTHING		// Collides with BG != 0.

// Type7 (with or without generators) enemies

	//#define ENABLE_TYPE7
	//#define TYPE7_BASE_SPRITE 			24
	//#define TYPE7_WITH_GENERATOR
	#define TYPE7_GENERATOR_LIFE			10
	#define TYPE7_MINION_LIFE 				1					// Actually this value + 1
	#define TYPE7_SPAWN_TIME				100+(rand8()&63)	// Must be of course > TYPE7_DYING_FRAMES!
	#define TYPE7_SMOKE_TIME				80 //40
	#define TYPE7_MINION_DYING_FRAMES		16
	#define TYPE7_MINION_HIT_FRAMES			16
	#define TYPE7_GENERATOR_DYING_FRAMES	16
	#define TYPE7_GENERATOR_HIT_FRAMES		16
	#define TYPE7_FIXED_SPRITE				3
	#define TYPE7_STOP_FRAMES				0x0f 				// 0x1f meghan
	#define TYPE7_STOP_RATE					0x0f 				// 0x7 meghan

// Walker 

	//#define ENABLE_WALKER
	#define WALKER_BASE_SPRITE				26
	#define WALKER_MIN_RUN					4
	#define WALKER_EXPRESSION				(WALKER_MIN_RUN + (rand8 () & 3))
	#define WALKER_HIT_FRAMES				16

// Steady shooters
	//#define ENABLE_STEADY_SHOOTER
	#define STEADY_SHOOTER_CELL				18

// Monococo
	//#define ENABLE_MONOCOCO
	#define MONOCOCO_BASE_TIME_HIDDEN		150
	#define MONOCOCO_BASE_TIME_APPEARING	50
	#define MONOCOCO_BASE_TIME_ONBOARD		50
	#define MONOCOCO_FIRE_COCO_AT			MONOCOCO_BASE_TIME_ONBOARD/2
	#define MONOCOCO_CELL_BASE				8

// Gyrosaws
	//#define ENABLE_GYROSAW
	#define GYROSAW_CELL_BASE				16
	#define GYROSAW_V						1

// Saws
	#define ENABLE_SAW
	#define SAW_CELL_BASE					16
	#define SAW_V_DISPL						4
	#define SAW_EMERGING_STEPS				10
	#define SAW_CELL_OCCLUSION				ssenc_04

// Fanty
	//#define ENABLE_FANTY
	#define FANTY_TIMER_BASE				120
	#define FANTY_A 						1
	#define FANTY_MAXV 						16
	
// Precalc, all integer fanty from Goddess. 
// Substitutes Fanty (0x3x as well)
	#define ENABLE_PRECALC_FANTY
	
// Precalc, all integer homing fanty from Goddess. 
// Substitutes Fanty (0x3x as well)	
	//#define ENABLE_PRECALC_HOMING_FANTY
	#define FANTY_ST_IDLE					0
	#define FANTY_ST_PURSUING				1
	#define FANTY_ST_RETREATING				2
	#define FANTY_SIGHT_DISTANCE			96

// Global for all fanties
	#define FANTY_COLLIDES
	#define FANTY_CELL_BASE 				18

// Catacrock
	//#define ENABLE_CATACROCK
	#define CATACROCK_CELL_BASE 			28
	#define CATACROCK_CROCK_FRAMES			50
	#define CATACROCK_G 					4
	#define CATACROCK_MAXV					64

// Generatonis
	//#define ENABLE_GENERATONIS
	#define GENERATONI_G					4
	#define GENERATONI_MAXV					64

// Espectros
	//#define ENABLE_ESPECTROS
	#define ESPECTRO_CELL_BASE 				8
	#define ESPECTRO_A 						1
	#define ESPECTRO_MAXV 					32
	#define ESPECTRO_REBOUND				48
	#define ESPECTRO_V_RETREATING			8

// Custom fumettos
	//#define ENABLE_FUMETTOS
	#define FUMETTOS_MAX 					3
	#define FUMETTO_BASE_PATTERN 			0xb1

// Saws
	#define ENABLE_SAWS
	// What else?

// Springs
	#define ENABLE_SPRINGS
	#define SPRINGS_MAX 					8
	#define SPRINGS_TILE					44
	#define SPRINGS_SOUND					6
	#define SPRINGS_CREATE_FROM_MAP			28			// Detect this tile # in the map to create
	//#define SPRINGS_X 					flags [15]	// Where to look up coordinates (if you are not creating from map)
	//#define SPRINGS_Y 					flags [14]	// Where to look up coordinates

// Propellers

	#define ENABLE_PROPELLERS
	#define PROPELLERS_MAX					8
	#define PROPELLERS_PATTERN				64			// Animating patterns start here
	#define PROPELLERS_BIT 					64			// Bit to make a tile "propellable"
	#define PROPELLERS_LIMIT				5			// max # of tiles high. Comment for no limit
	#define PROPELLERS_CREATE_FROM_MAP		33			// Detect this tile # in the map to create
	//#define PROPELLERS_X 					flags [15] 	// Use these if you want to create propellers
	//#define PROPELLERS_Y 					flags [14]	// by other means (for example script & EXTERN)

// Lame boss #1
	#define ENABLE_LAME_BOSS_1
	#define LAME_BOSS_1_CELL_BASE			22
	#define LAME_BOSS_HIT_DISPLACEMENT		16

// Auxiliary stuff

	#if defined (ENABLE_STEADY_SHOOTER) || defined (ENABLE_MONOCOCO) || defined (ENABLE_LAME_BOSS_1)
	
	#define ENABLE_COCOS
	#define COCOS_MAX 						3
	#define COCO_PATTERN					1
	#define COCO_PALETTE					1
	#define COCO_V							24

	#if defined (ENABLE_MONOCOCO) || defined (ENABLE_LAME_BOSS_1)
	#define ENABLE_COCO_AIMED
	#define COCO_FAIR_D						64
	#endif

	#if defined (ENABLE_STEADY_SHOOTER)
	#define ENABLE_COCO_STRAIGHT
	#endif

	#endif

	#if defined (ENABLE_FANTY) || defined (ENABLE_GENERATONIS) || defined (ENABLE_ESPECTROS)
	#define ENEMIES_NEED_FP
	#endif

	#if defined (PROPELLERS_CREATE_FROM_MAP) || defined (SPRINGS_CREATE_FROM_MAP)
	#define CREATE_STUFF_FROM_MAP
	#endif

// Game states

	#define GS_GAME_OVER					0

// Evil tiles

	// Enable the ones you need. Use a custom way to switch between them if you activate several:
	#define EVIL_TILE_ON_FLOOR
	//#define EVIL_TILE_ON_CEILING
	//#define EVIL_TILE_MULTI

	#if defined (EVIL_TILE_MULTI) || defined (EVIL_TILE_ON_FLOOR) || defined (EVIL_TILE_ON_CEILING)
	#define ENABLE_EVIL_TILE
	#endif

// Player

	// Enable the ones you need. Use a custom way to switch between them if you activate several:
	//#define PLAYER_GENITAL
	#define PLAYER_JUMPS
	//#define PLAYER_DOUBLE_JUMP
	//#define PLAYER_MONONO
	//#define PLAYER_JETPAC
	//#define PLAYER_HIDES

	//#define PLAYER_8_PIXELS				// Collision box is 8x8 instead of 8x16

	//#define PLAYER_PUSH_BOXES
	//#define PLAYER_FIRE_TO_PUSH

	#if defined (PLAYER_JUMPS) || defined (PLAYER_MONONO) || defined (PLAYER_JETPAC)
	#define PLAYER_GRAVITY
	#endif

	#define PLAYER_INI_X					7
	#define PLAYER_INI_Y					7

	#define PLAYER_VX_MAX					32
	#define PLAYER_AX						3
	#define PLAYER_RX						4	
	#define PLAYER_VX_MIN 					(PLAYER_AX << 2)

	#define PLAYER_VY_FALLING_MAX			64
	#define PLAYER_G						4 
	#define PLAYER_G_JUMPING				1
	#define PLAYER_VY_MIN 					8

	// #define PLAYER_VY_JETPAC_MAX			12		// Jetpac values
	// #define PLATER_AY_JETPAC 			2

	#define PLAYER_VY_JUMP_INITIAL			52
	#define PLAYER_VY_JUMP_RELEASE			24
	#define PLAYER_VY_JUMP_A_STEPS 			16
	#define PLAYER_AY_JUMP					8	
	
	#define PLAYER_VY_FLICK_TOP				64

	#define PLAYER_AY_PROPELLER				2
	#define PLAYER_VY_PROPELLER_MAX			32
	
	// Y Axis, genital
	
	#define PLAYER_VY_MAX					32
	#define PLAYER_AY						6
	#define PLAYER_RY						8
	#define PLAYER_VY_MIN 					8
	
	#define PLAYER_V_INERTIA				4
	#define PLAYER_V_REBOUND				64
	#define PLAYER_V_REBOUND_MULTI			32
	#define PLAYER_VY_BUTT_REBOUND			64
	#define PLAYER_HIT_FRAMES				16
	#define PLAYER_REBOUND_REVERSE_DIRECTION

	//#define PLAYER_REBOUNDS
	#define PLAYER_FLICKERS					120
	#define PLAYER_DIE_AND_RESPAWN

// Ways to kill

	//#define PLAYER_SPINS
	//#define PLAYER_SPINS_KILLS
	#define PLAYER_STEPS_ON_ENEMIES
	//#define PLAYER_STEPS_KILLS
	//#define PLAYER_BUTT
	//#define KILLABLE_CONDITION				(en_s [gpit] == 8 && game_trabajo_basura)	// Define if needed

// Things related to shooting

	//#define PLAYER_CAN_FIRE
	//#define PLAYER_VX_RECOIL				32

	// Ammonition
	#define AMMO_MAX						99
	#define AMMO_REFILL						50

	// Turret

	//#define PLAYER_TURRET
	#define TURRET_FRAMES					16 	

	// Bullets

	#define BULLETS_MAX						4
	#define BULLETS_V 						4

	//#define SHOOTING_DRAINS

	//#define ENABLE_PUAS
	#define PUAS_MAX						2
	#define PUAS_VX							2

	// Breakable walls
	//#define BREAKABLE_WALLS
	#define BREAKABLE_MAX 					8
	#define BREAKABLE_HITS 					0x60
	#define BREAKABLE_FRAMES_DESTROY		16
	#define BREAKABLE_FRAMES_HIT			4

// Hotspots
// Define those to include all related code

	//#define HOTSPOT_TYPE_OBJECT			1
	//#define HOTSPOT_TYPE_KEY 				2
	//#define HOTSPOT_TYPE_REFILL 			3
	//#define HOTSPOT_TYPE_AMMO				4
	//#define HOTSPOTS_MAY_CHANGE				// Copy hotspot type to RAM. It may change ingame
	#define DISABLE_HOTSPOTS

// Expand upon this when needed

	#if defined (PLAYER_CAN_FIRE) || defined (PLAYER_SPINS_KILLS) || defined (PLAYER_STEPS_ON_ENEMIES) || defined (ENEMS_DIE_ON_EVIL_TILE)
	#define ENEMIES_CAN_DIE
	#define SPRITE_EXPLOSION 				ssit_09
	#endif

	#if defined (PLAYER_DIE_AND_RESPAWN) || defined (ENABLE_HOLES)
	#define PLAYER_SAFE
	#endif

// Handy (expand?)
	#if defined (HOTSPOT_TYPE_KEY) || defined (PLAYER_PUSH_BOXES)
	#define PLAYER_PROCESS_BLOCK
	#endif

// Scripting stuff

	#define SCRIPTING_ON
	//#define CLEAR_FLAGS
	#define MAX_FLAGS 						32
	//#define COUNT_KILLED_ON_FLAG 			1
	//#define ONLY_ONE_OBJECT_FLAG 			2
	#define PLAYER_INV_FLAG					0 		// Exports pinv to this flag

	//#define LINE_OF_TEXT_Y				24
	//#define LINE_OF_TEXT_X				1

	#define ENABLE_FIRE_ZONE
	#define ENABLE_EXTERN_CODE
	#define FIRE_SCRIPT_WITH_ANIMATION

	#define ENABLE_CONTAINERS
	#define CONTAINERS_MAX 					5		// As few as you need!
	//#define CONTAINER_ACTION_STOPS_CHECKS			// ifdef, container hit means no fire script ran
	#define CONTAINERS_HAS_GOT_FLAG			30
	#define CONTAINERS_X 					sc_x
	#define CONTAINERS_Y 					(sc_y+1)
	#define CONTAINERS_N 					sc_n

	#define SPRITE_NO 						ssenc_05

	// Some stuff make hotspots managing slightly more complex

	#if defined (ONLY_ONE_OBJECT_FLAG)
	#define HOTSPOTS_LOGIC_MORE_COMPLEX
	#endif

// Animation cells

	#define CELL_FACING_RIGHT 				0
	#define CELL_FACING_LEFT				8
	#define CELL_IDLE						0
	#define CELL_WALK_BASE 					1
	#define CELL_AIRBORNE 					5
	#define CELL_GUAY	 					7
	#define CELL_USE_BASE 					15

	//#define CELL_PHOLED						20
	//#define CELL_FACING_DOWN				10
	//#define CELL_FACING_UP				15
	//#define CELL_DESCENDING				5
	//#define CELL_ASCENDING				6
	//#define CELL_HIT 						7
	//#define CELL_AIRBORNE 				7
	//#define CELL_FIRING					1
	//#define CELL_HIT 						7
	//#define CELL_OBJECT 					8
	//#define CELL_HIT_BASE 				20
	//#define CELL_AIRBORNE 				7
	//#define CELL_JUMP_BASE				5
	//#define CELL_FALLING 					9

// Sound channels

	#define SC_PLAYER						0
	#define SC_ENEMS						1
	#define SC_LEVEL						1

// SFX

	#define SFX_START						0
	#define SFX_JUMP						1			
	#define SFX_GET_ITEM					2
	#define SFX_ENEMY_HIT					3
	#define SFX_EXTRA_1						4
	#define SFX_SHOOT						5
	#define SFX_FART						6
	#define SFX_CHOF						7
	#define SFX_BUTT_FALL					8
	#define SFX_BUTT_HIT					9
	#define SFX_BLOCO						10
	#define SFX_THUNDER						11
	#define SFX_ESTRUJATOR 					11
	#define SFX_FABOLEE 					12
	#define SFX_TU 							13
	#define SFX_TICK						14
	#define SFX_FLUSH						15

// Music

	#define MUSIC_INGAME_INCAVED			0
	#define MUSIC_INGAME_OUTCAVED 			1
	#define MUSIC_INGAME_REFRIT 			2
	#define MUSIC_BOSS						3

// Make code more readable:

	#define MONOCOCO_COUNTER 				en_my
	#define _MONOCOCO_COUNTER				_en_my

	#define GYROSAW_COUNTER 				en_mx
	#define GYROSAW_DIRECTION				en_my
	#define _GYROSAW_DIRECTION				_en_my
	#define _GYROSAW_COUNTER 				_en_mx
	
	#define CATACROCK_COUNTER 				en_mx
	#define CATACROCK_WAIT 					en_my
	#define _CATACROCK_WAIT 				_en_my
	#define _CATACROCK_COUNTER				_en_mx
	#define _CATACROCK_STATE 				_en_state

	#define GENERATONI_COUNTER 				en_x2
	#define GENERATONI_WAIT 				en_y2
	#define GENERATONI_HL 					en_my

	#define STEADY_SHOOTER_DIRECTION		en_my
	#define STEADY_SHOOTER_WAIT				en_mx
	#define _STEADY_SHOOTER_WAIT 			_en_mx
	#define _STEADY_SHOOTER_DIRECTION       _en_my

	#define SAW_ORIENTATION					en_s
	#define SAW_EMERGING_DIRECTION			en_my
	#define SAW_MOVING_DIRECTION 			en_mx
	#define _SAW_EMERGING_DIRECTION			_en_my
	#define _SAW_MOVING_DIRECTION 			_en_mx
	
	#define MAX_STARS 						32

// Custom stuff

	#define PGAUGE_MAX 						7
	#define PGAUGE_TRANSFORM_PENALTY		4
