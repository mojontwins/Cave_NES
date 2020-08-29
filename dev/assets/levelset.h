// MT MK2 NES v0.8b
// Copyleft 2017 by The Mojon Twins

// Levelset
// Coment whichever doesn't apply

const unsigned char * const l_ts_pals [] = { ts0_pals, ts2_pals, ts1_pals };
const unsigned char * const l_ts_tmaps [] = { ts0_tmaps, ts2_tmaps, ts1_tmaps };
const unsigned char * const l_behs [] = { behs0, behs2, behs1 };
const unsigned char l_scr_ini [] = { 0, 1, 24 };
const unsigned char * const * const l_map [] = { map_0, map_2, map_1 };
const unsigned char * const * const l_decos [] = { map_0_decos, map_2_decos, map_1_decos };
const unsigned char * const * const l_enems [] = { enems_0, enems_2, enems_1 } ;
// const unsigned char * const l_hotspots [] = { hotspots_1, hotspots_1, hotspots_1 };
// const unsigned char * const l_locks [] = { map_1_locks, map_1_locks, map_1_locks };
const unsigned char * const l_pal_bgs [] = { palts0, palts2, palts1 };
//const unsigned char * const l_pal_fgs [] = { palss1, palss1, palss1 };
// const unsigned char * const t_pal_bgs [] = { palts0, palts0 };
// const unsigned char * const t_pal_fgs [] = { palss0, palss0 };
// const unsigned char l_max_hotspots_type_1 [] = { MAX_HOTSPOTS_1_TYPE_1, MAX_HOTSPOTS_1_TYPE_1, MAX_HOTSPOTS_1_TYPE_1 };
const unsigned char l_map_w [] = { 5, 5, 6 };
// const unsigned char l_plife [] = { 5, 5, 5 };
const unsigned char player_ini_x [] = { 1, 1, 1 };
const unsigned char player_ini_y [] = { 5, 3, 3 };
// const unsigned char * const * const l_spr_enems [] = { spr_enems_1, spr_enems_1, spr_enems_1 }; 
// const unsigned char * const * const l_spr_player [] = { spr_player_0, spr_player_0, spr_player_0 };
const unsigned char l_music [] = { MUSIC_INGAME_OUTCAVED, MUSIC_INGAME_INCAVED, MUSIC_INGAME_REFRIT };
