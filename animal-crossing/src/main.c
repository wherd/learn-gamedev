#include <raylib.h>
#include "defines.h"

typedef enum direction
{
    DOWN = 0,
    UP = 1,
    LEFT = 2,
    RIGHT = 3,
    NONE = 4
} direction_t;

global i32 screenWidth = 1000;
global i32 screenHeight = 480;

global b8 running;
global Color backgroundColor = {147, 211, 196, 255};
global Texture2D grassSprite;

global Texture2D playerSprite;
global Rectangle playerSrc;
global Rectangle playerDest;
global Vector2 playerOrigin;
global f32 playerSpeed = 3.0f;
global direction_t playerDir = DOWN;
global i8 playerFrame;
global Camera2D cam;

global i32 mapW = 5;
global i32 mapH = 5;
global Rectangle tileDest;
global Rectangle tileSrc;
global i32 tileMap[25] = {1};
global i32 tileMapSize = 25;
// global char srcMap[][];

global i32 frameCount;
global b8 musicPaused;
global Music music;

internal void
LoadMap(void)
{
    for (int i = 0; i < tileMapSize; ++i)
    {
        tileMap[i] = 1;
    }

    tileMap[6] = 2;
    tileMap[11] = 2;
    tileMap[16] = 2;
}

internal void
InitGame(void)
{
    running = 1;
    InitWindow(screenWidth, screenHeight, "Sprout Lands");
    SetExitKey(0);

    grassSprite = LoadTexture("res/grass.png");
    tileDest = (Rectangle){0.0f, 0.0f, 16.0f, 16.0f};
    tileSrc = (Rectangle){0.0f, 0.0f, 16.0f, 16.0f};

    playerSprite = LoadTexture("res/player.png");

    playerSrc = (Rectangle){0, 0, 48, 48};
    playerDest = (Rectangle){200, 200, 100, 100};
    playerOrigin = (Vector2){playerDest.width, playerDest.height};

    InitAudioDevice();
    musicPaused = 0;
    music = LoadMusicStream("res/harrys-background-music.mp3");
    PlayMusicStream(music);

    cam = (Camera2D){
        .offset = (Vector2){screenWidth / 2, screenHeight / 2},
        .target = (Vector2){playerDest.x - (playerDest.width / 2), playerDest.y - (playerDest.height / 2)},
        .rotation = 0.0f,
        .zoom = 1.5f,
    };

    LoadMap();
}

internal void
UpdateInput(void)
{
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        playerDir = UP;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        playerDir = DOWN;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        playerDir = LEFT;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        playerDir = RIGHT;
    }
    if (IsKeyPressed(KEY_Q))
    {
        musicPaused = 1 - musicPaused;
    }
}

internal void
UpdateGame(void)
{
    running = !WindowShouldClose();

    switch (playerDir)
    {
    case DOWN:
        playerDest.y += playerSpeed;
        if ((frameCount % 8) == 1)
        {
            playerFrame = (playerFrame + 1) % 4;
        }
        break;
    case UP:
        playerDest.y -= playerSpeed;
        if ((frameCount % 8) == 1)
        {
            playerFrame = (playerFrame + 1) % 4;
        }
        break;
    case LEFT:
        playerDest.x -= playerSpeed;
        if ((frameCount % 8) == 1)
        {
            playerFrame = (playerFrame + 1) % 4;
        }
        break;
    case RIGHT:
        playerDest.x += playerSpeed;
        if ((frameCount % 8) == 1)
        {
            playerFrame = (playerFrame + 1) % 4;
        }
        break;
    case NONE:
        if ((frameCount % 24) == 1)
        {
            playerFrame = 1 - playerFrame;
        }
        else if (playerFrame > 1)
        {
            playerFrame = 0;
        }
        break;
    }

    frameCount++;
    playerSrc.x = playerSrc.width * (f32)playerFrame;
    if (playerDir != NONE)
    {
        playerSrc.y = playerSrc.height * (f32)playerDir;
    }
    playerDir = NONE;

    UpdateMusicStream(music);
    if (musicPaused)
    {
        PauseMusicStream(music);
    }
    else
    {
        ResumeMusicStream(music);
    }

    cam.target = (Vector2){playerDest.x - (playerDest.width / 2), playerDest.y - (playerDest.height / 2)};
}

internal void
DrawScene(void)
{
    // DrawTexture(grassSprite, 100, 50, WHITE);
    for (int i = 0; i < tileMapSize; ++i)
    {
        if (tileMap[i] != 0)
        {
            tileDest.x = tileDest.width * (f32)(i % mapW);
            tileDest.y = tileDest.height * (f32)(i / mapW);
            tileSrc.x = tileSrc.width * (f32)((tileMap[i] - 1) % ((int)grassSprite.width / (int)tileSrc.width));
            tileSrc.y = tileSrc.height * (f32)((tileMap[i] - 1) % ((int)grassSprite.width / (int)tileSrc.width));
            DrawTexturePro(grassSprite, tileSrc, tileDest, (Vector2){tileDest.width, tileDest.height}, 0, WHITE);
        }
    }
    DrawTexturePro(playerSprite, playerSrc, playerDest, playerOrigin, 0, WHITE);
}

internal void
DrawGame(void)
{
    BeginDrawing();
    ClearBackground(backgroundColor);
    BeginMode2D(cam);
    DrawScene();
    EndMode2D();
    EndDrawing();
}

internal void
UpdateAndDrawGame(void)
{
    UpdateInput();
    UpdateGame();
    DrawGame();
}

int main(void)
{
    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateAndDrawGame, 60, 1);
#else
    SetTargetFPS(60);

    while (running)
    {
        UpdateAndDrawGame();
    }
#endif

    return 0;
}
