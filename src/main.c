#include <stdio.h>  // sprintf (debug)

#include <raylib.h>
#include "pokaylib.h"
#include "config.h"

int main(void) {
    #ifdef DEBUG
        SetTraceLogLevel(LOG_ALL);
        TraceLog(LOG_DEBUG, "salut :)");

        char textX[16] = {0};
        char textY[16] = {0};
        // char textFrame[16] = {0};
    #endif
    int screenWidth = SCREEN_WIDTH * SCALING_FACTOR;
    int screenHeight = SCREEN_HEIGHT * SCALING_FACTOR;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "pokaylib");
    SetTargetFPS(60);

    // Initialization -----------------------------------------
    Player player = {
        .position.x = 0.0f,
        .position.y = 0.0f,
        .owSprites = MakeSprite("assets/player_overworld.png", 10, 16, 16, 0) // the path is relative to the outputed file, not to the source file. build.sh outputs in the root directory.
    };

    Sprite metang = MakeSprite("assets/metang-82x64.png", 32, 82, 64, 0);
    // Sprite rayquaza = MakeSprite("assets/rayquaza-110x97.png", 28, 110, 97, 0);
    // Sprite genesect = MakeSprite("assets/genesect-62x70.png", 44, 62, 70, 0);

    int framesSpeed = 8;
    int framesCounter = 0;

    //extern Map mapData[MAP_COUNT];  // array containing each map, accessible with the MapID enum passed as the index
    extern Map map_PalletTown;
    Map currentMap = MakeMap(map_PalletTown);

    // Main game loop -----------------------------------------
    while (!WindowShouldClose())
    {
        // Update ---------------------------------------------
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        framesCounter++;
        if (framesCounter >= (60/framesSpeed)) {
            framesCounter = 0;

            metang.index++;
            if (metang.index >= metang.spriteNumber) metang.index = 0;
            /*
            rayquaza.index++;
            if (rayquaza.index >= rayquaza.spriteNumber) rayquaza.index = 0;

            genesect.index++;
            if (genesect.index >= genesect.spriteNumber) genesect.index = 0;*/
        }

        // Controls -------------------------------------------
        if (IsKeyPressed(KEY_LEFT)) {
            player.position.x -= 16 * SCALING_FACTOR;
            player.owSprites.index = 6;
        } else if (IsKeyPressed(KEY_RIGHT)) {
            player.position.x += 16 * SCALING_FACTOR;
            player.owSprites.index = 8;
        } else if (IsKeyPressed(KEY_DOWN)) {
            player.position.y += 16 * SCALING_FACTOR;
            player.owSprites.index = 1;
        } else if (IsKeyPressed(KEY_UP)) {
            player.position.y -= 16 * SCALING_FACTOR;
            player.owSprites.index = 4;
        }

        // Debug ----------------------------------------------
        #ifdef DEBUG
            sprintf(textX, "%.0f", player.position.x);
            sprintf(textY, "%.0f", player.position.y);
            //sprintf(textFrame, "%d", playerIDX);
        #endif

        // Draw -----------------------------------------------
        BeginDrawing();
            ClearBackground(GRAY);


            // draw background tiles
            //DrawMapLayer(currentMap, 0, SCALING_FACTOR);
            //DrawSprite(map.tileset, 6, u8 scalingFactor, Vector2 position)



            DrawSprite(metang, metang.index, SCALING_FACTOR, (Vector2) {300, 50});
            //DrawSprite(rayquaza, rayquaza.index, SCALING_FACTOR, (Vector2) {30, 150});
            //DrawSprite(genesect, genesect.index, SCALING_FACTOR, (Vector2) {(float)screenWidth/2, (float)screenHeight/2});
            DrawSprite(player.owSprites, player.owSprites.index, SCALING_FACTOR, player.position);


            // draw front tiles

            #ifdef DEBUG
                // cross
                DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, RAYWHITE);
                DrawLine(0, screenHeight/2, screenWidth, screenHeight/2, RAYWHITE);

                DrawText("x:", 10, 0, 20, LIGHTGRAY);
                DrawText(textX, 40, 0, 20, LIGHTGRAY);
                DrawText("y:", 10, 20, 20, LIGHTGRAY);
                DrawText(textY, 40, 20, 20, LIGHTGRAY);
                //DrawText("frame:", 10, 40, 20, LIGHTGRAY);
                //DrawText(textFrame, 80, 40, 20, LIGHTGRAY);
                // DrawText(GetWorkingDirectory(), 10, 40, 20, LIGHTGRAY); // show working directory
                DrawFPS(screenWidth-75, 0);
            #endif
        EndDrawing();
    }

    // De-Initialization --------------------------------------
    DestroySprite(player.owSprites);
    DestroySprite(metang);
    DestroyMap(currentMap);
    //DestroySprite(rayquaza);
    //DestroySprite(genesect);

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
