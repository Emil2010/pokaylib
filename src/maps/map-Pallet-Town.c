#include "../../include/pokaylib.h"

#define MAP_WIDTH 13
#define MAP_HEIGHT 10

u8 layer1[] = {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
     13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
     26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
     39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
     54, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
     65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77,
     78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
     91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,
};

u8 *layers[] = {layer1};

Map map_PalletTown = {
    .mapID = MAP_PALLET_TOWN,
// Tileset
    .tileset = {0},
    .tilesetPath = "assets/tilesets/tileset-23.png",
    .tilesetSpritesNumber = 104,
    .tileWidth = 8,
    .tileHeight = 8,
// Map
    .width = MAP_WIDTH,
    .height = MAP_HEIGHT,
    .layersNumber = 1,
.tilesNumber = (MAP_WIDTH * MAP_HEIGHT) - 1,
    .layerData = layers,
};

/* typedef struct {
    MapID mapID;    // unique enum ID
// Tileset
    Sprite tileset;
    const char *tilesetPath;    // path to the tileset spritesheet
    u8 tilessetSpritesNumber;   // tileset's number of sprites
    u8 tileWidth;   // tileset's tile width - TODO : as tilesets are always square, maybe regroup width and height in the same variable
    u8 tileHeight;  // tileset's tile height
// Map
    u8 width;       // map width (tile-wise)
    u8 height;      // map height (tile-wise)
    u8 layersNumber;// how much layers the map have
    u16 tilesNumber; // how much tiles the map have (width * height)
    u8 **layerData; // dynamically allocated layers
    u8 **tiles;     // tiles[layer][tile]
} Map; */
