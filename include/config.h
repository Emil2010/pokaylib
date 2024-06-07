#pragma once

#define GAMEBOY_SCREEN_WIDTH        160
#define GAMEBOY_SCREEN_HEIGHT       144
#define GBA_SCREEN_WIDTH            240
#define GBA_SCREEN_HEIGHT           160
#define NINTENDO_DS_SCREEN_WIDTH    256
#define NINTENDO_DS_SCREEN_HEIGHT   192
#define NINTENDO_3DS_SCREEN_WIDTH   400
#define NINTENDO_3DS_SCREEN_HEIGHT  240

// Each of these have different tilesets and pokemon sprites
#define GENERATION_RED_BLUE
#define GENERATION_YELLOW
#define GENERATION_GOLD_SILVER
#define GENERATION_CRYSTAL
#define GENERATION_RUBY_SAPPHIRE
#define GENERATION_EMERALD
#define GENERATION_FIRE_RED_LEAF_GREEN
#define GENERATION_DIAMOND_PEARL_PLATINUM
#define GENERATION_HGSS
#define GENERATION_BLACK_WHITE

#ifndef NDEBUG
# define DEBUG
#endif

#define SCALING_FACTOR 3
#define SCREEN_WIDTH NINTENDO_DS_SCREEN_WIDTH
#define SCREEN_HEIGHT NINTENDO_DS_SCREEN_HEIGHT
