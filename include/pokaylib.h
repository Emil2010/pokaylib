#pragma once

#include <stdint.h>
#include <raylib.h>
#include "abilities.h"
#include "moves.h"
#include "maps.h"

// Typedef ----------------------------------------------------
typedef uint8_t u8;     //    256
typedef uint16_t u16;   // 65 535

// Macros -----------------------------------------------------
#define YELLOW_PRINT printf("\033[0;33m")
#define RED_PRINT printf("\033[0;31m")
#define GREEN_PRINT printf("\033[0;32m")
#define BLUE_PRINT printf("\033[0;34m")
#define MAG_PRINT printf("\033[0;35m")
#define CYAN_PRINT printf("\033[0;36m")
#define NO_COLOR printf("\033[0m")

// Enums ------------------------------------------------------
typedef enum {
    NO_TYPE,
    NORMAL,
    FIGHTING,
    FLYING,
    POISON,
    GROUND,
    ROCK,
    BUG,
    GHOST,
    STEEL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    PSYCHIC,
    ICE,
    DRAGON,
    DARK,
    FAIRY
} Type;



// Structs ----------------------------------------------------
typedef struct {
    Texture2D spritesheet;
    Rectangle *frames;
    u8 spriteNumber;
    u8 index;
    u16 *animation; // list of frames to make a sprite animation, as pokemon battle sprites have often 100+ frames of animation yet only have 30ish sprites on the spritesheet
} Sprite;

typedef struct {
    Sprite redBlue;              // gen-1
    Sprite yellow;               // gen-2
    Sprite gold;                 // gen-2
    Sprite silver;               // gen-2
    Sprite crystal;              // gen-2
    Sprite rubySapphire;         // gen-3
    Sprite emerald;              // gen-3
    Sprite fireRedLeafGreen;     // gen-3
    Sprite diamondPearlPlatinum; // gen-4
    Sprite heartGoldSoulSilver;  // gen-4
    Sprite blackWhite;           // gen-5
} GenerationSprite;

typedef struct {
    // General
    const char **name;
    u16 dexNumber;
    Type type1;
    Type type2;
    Ability ability1;
    Ability ability2;
    Ability hiddenAbility;
    GenerationSprite frontSprites;  // battle front-sprites
    GenerationSprite backSprites;   // battle back-sprites
    Sprite owSprites;               // overworld walking sprites
    Sprite menuSprites;             // little icons in the menus
    // Pokedex shit
    float height;                   // height (cm)
    float weight;                   // weight (kg)
    const char *species;            // e.g. Bulbasaur : "Seed Pokemon"
} PkmSpecies;
extern const PkmSpecies pkmSpecies[649];    // all pokemons up to gen-5 Genesect (because we have official sprites)

typedef struct {    // pokemon in your party or ennemies pokemon
    PkmSpecies species;
    char *nickname;
    u8 level;
    Move move1;
    Move move2;
    Move move3;
    Move move4;
} PkmSpecimen;

typedef struct {
    MapID mapID;    // unique enum ID
// Tileset
    Sprite tileset;
    const char *tilesetPath;    // path to the tileset spritesheet
    u8 tilesetSpritesNumber;   // tileset's number of sprites
    u8 tileWidth;   // tileset's tile width - TODO : as tilesets are always square, maybe regroup width and height in the same variable
    u8 tileHeight;  // tileset's tile height
// Map
    u8 width;       // map width (tile-wise)
    u8 height;      // map height (tile-wise)
    u8 layersNumber;// how much layers the map have
    u16 tilesNumber; // how much tiles the map have (width * height)
    u8 ***tiles;     // tiles[layer][tileType]
} Map;

// Player -----------------------------------------------------
typedef struct {
    Vector2 position;
    Sprite owSprites;
    //Sprite battleSprites;
} Player;

// Functions --------------------------------------------------
Sprite MakeSprite(const char *spritesheetPath, u8 framesNumber, u8 spriteWidth, u8 spriteHeight, u8 frameOffset);
void MakeMap(Map *map, const char *mapData);

void DrawSprite(Sprite sprite, u8 frameIndex, u8 scalingFactor, Vector2 position);
void DrawMapLayer(Map map, u8 layerIndex, u8 scalingFactor);

void FreeSprite(Sprite sprite);
void FreeMap(Map map);
