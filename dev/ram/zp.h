// MT MK2 NES v0.8b - advanced scripting
// Copyleft 2017 by The Mojon Twins

// ZP globals

unsigned char oam_index;
unsigned char half_life, frame_counter;
unsigned char pad0, pad;
unsigned char pad_this_frame;

// Main globals

unsigned char gpit, gpjt, gpkt, bi;
unsigned char rda, rdb, rdc, rdd, rdi;
unsigned char rdx, rdy, rdt, rdct;
unsigned char fr_ct;
signed char rds;
unsigned int gpint;
signed int gpsint;
signed char delta;

unsigned char *gp_gen, *gp_aux, *gp_map, *gp_tmap;
unsigned int gp_addr;

// Level stuff which has to be fast

const unsigned char * const *spr_enems;
const unsigned char * const *spr_player;
const unsigned char *c_ts_pals;
const unsigned char *c_ts_tmaps;
const unsigned char *c_behs;

// Update list

unsigned char *ul;

// Collision

unsigned char cx1, cy1, cx2, cy2, at1, at2;
unsigned char caux;

// Enems

unsigned char _en_t, _en_x, _en_y;
unsigned char _en_x1, _en_x2, _en_y1, _en_y2;
signed char _en_mx, _en_my;
unsigned char _en_state, _en_ct;

unsigned char en_hl [N_ENEMS];
unsigned char en_life [N_ENEMS];
unsigned char en_s [N_ENEMS];
unsigned char en_facing [N_ENEMS];
unsigned char en_dying [N_ENEMS]; 
unsigned char en_washit [N_ENEMS];

#ifdef ENABLE_TYPE_7
	unsigned char en_fishing [N_ENEMS];
	unsigned char en_v [N_ENEMS];
#endif

#ifdef TYPE7_WITH_GENERATOR
	unsigned char en_gen_washit [N_ENEMS];
	unsigned char en_gen_dying [N_ENEMS];
	unsigned char en_gen_life [N_ENEMS];
#endif

#ifdef ENEMIES_NEED_FP
	#if defined (ENABLE_FANTY) || defined (ENABLE_ESPECTROS)
	unsigned char fanty_timer [N_ENEMS];
	signed int enf_x [N_ENEMS];
	signed char enf_vx [N_ENEMS];
	#endif
	signed int enf_y [N_ENEMS];
	signed char enf_vy [N_ENEMS];
#endif

// Player

unsigned char prx, pry;
signed int px, py;
signed char pvx;
signed int pvy;

#ifdef PLAYER_JUMPS
	unsigned char pj, pctj, pthrust, pjb;
	unsigned char pjustjumped;
#endif

unsigned char pfiring;
unsigned char phit, pflickering;
unsigned char pfacing, pgotten, ppossee, pregotten;
unsigned char pfixct;
unsigned char psprid; 
signed int pvylast;
signed char pgtmx, pgtmy;
unsigned char pcharacter;

#ifdef PLAYER_SPINS
	unsigned char pspin;
#endif	

#ifdef PLAYER_GENITAL
	unsigned char pfacingh, pfacingv, pfacinglast;
#endif

#ifdef EVIL_TILE_MULTI
	signed int pcx, pcy;
#endif

#ifdef PLAYER_SAFE
	unsigned char safe_prx, safe_pry;
	unsigned char safe_n_pant;
	unsigned char safe_pinv;
#endif

#ifdef SHOOTING_DRAINS
	unsigned char pgauge;
#endif

unsigned char pwashit;
unsigned char pstatespradder;

#ifdef PLAYER_HIDES
	unsigned char phidden;
#endif

#ifdef ENABLE_HOLES
	unsigned char pholed;
#endif

// Hotspots

unsigned char hrx, hry, hrt;
unsigned char itcelloffset;

// Flower

//unsigned char flwx, flwy;

// Sprite shuffling

unsigned char sstart;

// Enemies collide with background

#ifdef LINEAR_COLLIDES
unsigned char en_collx, en_colly;
#endif

#ifdef PLAYER_PUSH_BOXES
unsigned char pbx1, pby1;
#endif

#ifdef ENABLE_COCOS
	signed int _coco_x, _coco_y;
	unsigned char coco_it;
#endif

#ifdef SCRIPTING_ON
	unsigned char sc_x, sc_y, sc_n, sc_c;
	const unsigned char *next_script;
	const unsigned char *script;
	unsigned char script_result, sc_terminado, sc_continuar;
	unsigned char commands_executed;
	#ifdef ENABLE_FIRE_ZONE
	unsigned char fzx1, fzy1, fzx2, fzy2, f_zone_ac;
	#endif
	const unsigned char * const *script_pool;
#endif
