#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include "pokaylib.h"
#include "config.h"

/*  @info Builds a frame array (or a single frame) for sprite/animated sprite
 *  @param spritesheetPath - the relative path to the spritesheet image file
 *  @param sprite - the sprite we want to make
 *  @param spriteNumber - how much induvidual sprites are stored on the spritesheet
 *  @param spriteWidth - the width of the sprite (must be the same for each frame)
 *  @param spriteHeight - the height of the sprite (must be the same for each frame)
 *  @param spriteOffset - how much sprite to skip (spritesheet-wise) until you get to the first frame of the sprite you want to make */
Sprite MakeSprite(const char *spritesheetPath, u8 spriteNumber, u8 spriteWidth, u8 spriteHeight, u8 spriteOffset)
{
    #ifdef DEBUG
        YELLOW_PRINT;
        TraceLog(LOG_DEBUG, "\033[0;33mMaking sprite @ [%s]", spritesheetPath);
        TraceLog(LOG_DEBUG, "    Sprite number:\t%d", spriteNumber);
        TraceLog(LOG_DEBUG, "    Sprite width:\t%dpx", spriteWidth);
        TraceLog(LOG_DEBUG, "    Sprite height:\t%dpx", spriteHeight);
        TraceLog(LOG_DEBUG, "    Sprite offset:\t%d\033[0m", spriteOffset);
        NO_COLOR;
    #endif

    // TODO: maybe extract 'spriteWidth' and 'spriteHeight' from the spritesheet name (e.g. metang-82x64.png)
    Sprite sprite = {
        .spritesheet = LoadTexture(spritesheetPath),
        .frames = (Rectangle*)malloc(spriteNumber * sizeof(Rectangle)),
        .spriteNumber = spriteNumber
    };

    //int lin = sprite.spritesheet.height / spriteHeight;
    int col = sprite.spritesheet.width / spriteWidth;

    int i = 0;
    int j = 0;
    // TODO: rework the "offset" thing because it only works for x-axis offset, and not a sprite offset per-se
    for (int frameIndex = 0; frameIndex < spriteNumber; frameIndex++) {
        sprite.frames[frameIndex].x = (spriteOffset + i) * spriteWidth;
        sprite.frames[frameIndex].y = j * spriteHeight;
        sprite.frames[frameIndex].width = spriteWidth;
        sprite.frames[frameIndex].height = spriteHeight;

        i++;
        if (i >= col) {
            i = 0;
            j++;
        }
    }

    return sprite;
}

// @info Free allocated memory for the sprite frames and unload the spritesheet
void FreeSprite(Sprite sprite)
{
    #ifdef DEBUG
        YELLOW_PRINT;
        printf("DEBUG: Destroying sprite -> ");
        NO_COLOR;
    #endif
    free(sprite.frames);
    UnloadTexture(sprite.spritesheet);
}

void DrawSprite(Sprite sprite, u8 frameIndex, u8 scalingFactor, Vector2 position)
{
    DrawTexturePro(
        sprite.spritesheet,         // Texture2D texture
        sprite.frames[frameIndex],  // Rectangle source
        (Rectangle) {               // Rectangle dest
            position.x,
            position.y,
            sprite.frames[frameIndex].width * scalingFactor,
            sprite.frames[frameIndex].height * scalingFactor,
        },
        (Vector2) {                 // Vector2 origin
            (sprite.frames[frameIndex].width/2) * scalingFactor,    // centers the sprite on the x wanted
            (sprite.frames[frameIndex].height/2) * scalingFactor    // centers the sprite on the y wanted
        },
        0,                          // float rotation
        WHITE                       // Color tint
    );
}

// Maps -------------------------------------------------------
void MakeMap(Map *map, const char *mapData)
{
    char tilesetPath[64];      // path to the tileset spritesheet

    FILE *mapFile = fopen(mapData, "r");
    if (mapFile == NULL) {
        RED_PRINT;
        TraceLog(LOG_ERROR, "could not open/find mapdata file (%s)", mapData);
        NO_COLOR;
    }

    // Read the first line for map properties
    fscanf(mapFile, "%hhu %hhu %hhu %hhu %hhu %hhu\n",
        &map->width,
        &map->height,
        &map->tilesetSpritesNumber,
        &map->tileWidth,
        &map->tileHeight,
        &map->layersNumber
    );
    fscanf(mapFile, "%s\n", tilesetPath);

    // Dynamic layers/tilesX/tilesY allocation and parsing
    map->tiles = (u8 ***)malloc(map->layersNumber * sizeof(u8 **));
    for (int i = 0; i < map->layersNumber; i++) {                    // layer
        map->tiles[i] = (u8 **)malloc(map->height * sizeof(u8 *));
        for (int j = 0; j < map->height; j++) {                      // x
            map->tiles[i][j] = (u8 *)malloc(map->width * sizeof(u8));
            for (int k = 0; k < map->width; k++) {                   // y
                fscanf(mapFile, "%hhu", &map->tiles[i][j][k]);       // get the tile ID
            }
        }
    }

    map->tileset = MakeSprite(
                    tilesetPath,
                    map->tilesetSpritesNumber,
                    map->tileWidth,
                    map->tileHeight,
                    0
                );

    fclose(mapFile);

    #ifdef DEBUG
        YELLOW_PRINT;
        TraceLog(LOG_DEBUG,"Making map @ [%s]", mapData);
        TraceLog(LOG_DEBUG,"    Map width:\t%d", map->width);
        TraceLog(LOG_DEBUG,"    Map height:\t%d", map->height);
        TraceLog(LOG_DEBUG,"    Map tiles:\t%d", map->width * map->height);
        TraceLog(LOG_DEBUG,"    Map layers:\t%d", map->layersNumber);
        TraceLog(LOG_DEBUG,"    Tile width:\t%dpx", map->tileWidth);
        TraceLog(LOG_DEBUG,"    Tile height:\t%dpx", map->tileHeight);
        TraceLog(LOG_DEBUG,"    Map size:\t%dx%dpx", map->tileWidth * map->width, map->tileHeight * map->height);
        MAG_PRINT;
        for (int i = 0; i < map->layersNumber; i++) {    // layer
            printf("MAP: Layer[%d]\n", i);
            for (int j = 0; j < map->height; j++) {      // x
                for (int k = 0; k < map->width; k++) {   // y
                    printf("%3hhu ", map->tiles[i][j][k]);
                }
                printf("\n");
            }
        }
        NO_COLOR;
    #endif

    return;
}

void DrawMapLayer(Map map, u8 layerIndex, u8 scalingFactor)
{
    // If you get a segfault here, that means the 'map.dat' file is wrong
    // eg. the width/height of the map is set to some numbers,but the
    // actual data parsed (matrix) does not have that size.

    for (int i = 0; i < map.height; i++) {
        for (int j = 0; j < map.width; j++) {
            // printf("[%d][%d][%d]\n", layerIndex, i, j);
            DrawSprite(
                map.tileset,
                map.tiles[layerIndex][i][j],
                scalingFactor,
                (Vector2) {
                    j * map.tileWidth * scalingFactor,
                    i * map.tileHeight * scalingFactor,
            });
        }
    }
}

void FreeMap(Map map)
{
    free(map.tiles);
    FreeSprite(map.tileset);
}
