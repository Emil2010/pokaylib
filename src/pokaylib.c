#include <stdio.h>
#include <stdlib.h>

#include "../../raylib-5.x/src/raylib.h"
#include "../../raylib-5.x/src/raymath.h"
#include "../include/pokaylib.h"
#include "../include/config.h"


/*  @info Builds a frame array (or a single frame) for sprite/animated sprite
 *  @param spritesheetPath - the relative path to the spritesheet image file
 *  @param sprite - the sprite we want to make
 *  @param spriteNumber - how much induvidual sprites are stored on the spritesheet
 *  @param spriteWidth - the width of the sprite (must be the same for each frame)
 *  @param spriteHeight - the height of the sprite (must be the same for each frame)
 *  @param spriteOffset - how much sprite to skip (spritesheet-wise) until you get to the first frame of the sprite you want to make */
Sprite MakeSprite(const char *spritesheetPath, u8 spriteNumber, u8 spriteWidth, u8 spriteHeight, u8 spriteOffset)
{
    #if DEBUG
        TraceLog(LOG_DEBUG, "Making sprite @ [%s]", spritesheetPath);
        TraceLog(LOG_DEBUG, "    Sprite number:\t%d", spriteNumber);
        TraceLog(LOG_DEBUG, "    Sprite width:\t%dpx", spriteWidth);
        TraceLog(LOG_DEBUG, "    Sprite height:\t%dpx", spriteHeight);
        TraceLog(LOG_DEBUG, "    Sprite offset:\t%d", spriteOffset);
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
    for (int frameIndex = 0; frameIndex < spriteNumber; ++frameIndex) {
        sprite.frames[frameIndex].x = (spriteOffset + i) * spriteWidth;
        sprite.frames[frameIndex].y = j * spriteHeight;
        sprite.frames[frameIndex].width = spriteWidth;
        sprite.frames[frameIndex].height = spriteHeight;

        ++i;
        if (i >= col) {
            i = 0;
            ++j;
        }
    }

    return sprite;
}


// @info Free allocated memory for the sprite frames and unload the spritesheet
void DestroySprite(Sprite sprite)
{
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
Map MakeMap(Map loadedMap)
{
    Map map;
    map.width = loadedMap.width;
    map.height = loadedMap.height;
    map.tileWidth = loadedMap.tileWidth;
    map.tileHeight = loadedMap.tileHeight;
    map.layersNumber = loadedMap.layersNumber;
    map.tilesNumber = loadedMap.tilesNumber;
    map.tileset = MakeSprite(loadedMap.tilesetPath, loadedMap.tilesetSpritesNumber, loadedMap.tileWidth, loadedMap.tileHeight, 0);

    // Dynamic tiles/layers allocation
    map.tiles = (u8**)malloc(map.layersNumber * sizeof(u8*));
    for (int i=0; i < map.layersNumber; ++i) {
        map.tiles[i] = (u8*)malloc(map.width * map.height * sizeof(u8));
        for (int j=0; j < (map.width * map.height); ++j) {
            map.tiles[i][j] = loadedMap.layerData[i][j];
        }
    }

    #if DEBUG
        TraceLog(LOG_DEBUG, "Making map (ID: %d) @ [%s]", map.mapID, loadedMap.tilesetPath);
        TraceLog(LOG_DEBUG, "    Map width:\t%d", map.width);
        TraceLog(LOG_DEBUG, "    Map height:\t%d", map.height);
        TraceLog(LOG_DEBUG, "    Map layers:\t%d", map.layersNumber);
        TraceLog(LOG_DEBUG, "    Map tiles:\t%d", map.tilesNumber);
        TraceLog(LOG_DEBUG, "    Tile width:\t%dpx", map.tileWidth);
        TraceLog(LOG_DEBUG, "    Tile height:\t%dpx", map.tileHeight);
    #endif

    return map;
}

void DrawMapLayer(Map map, u8 layerIndex, u8 scalingFactor)
{
    u8 i = 0;
    u8 j = 0;
    for (i = 0; i < map.tilesNumber; ++i) {
        DrawSprite(map.tileset,
                   map.tiles[layerIndex][i],
                   scalingFactor,
                   (Vector2) {
                       (i * map.tileWidth * scalingFactor) + 100,
                       (j * map.tileHeight * scalingFactor) + 100,
                   });

        if (i == map.width) {
            i = 0;
            j++;
        }
    }
}

void DestroyMap(Map map)
{
    free(map.tiles);
    DestroySprite(map.tileset);
}
