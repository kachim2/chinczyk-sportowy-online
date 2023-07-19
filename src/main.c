#include "raylib.h"
#include "raymath.h"
#include <stdint.h>
#include <float.h>
#include <limits.h>
// #include <time.h>
//------------------------------------------------------------------------------------
//  Program main entry point
//------------------------------------------------------------------------------------
const Color colors[] = {RED, BLUE, GREEN, YELLOW, VIOLET, BLACK};

int32_t NumPlayers = 4;
int32_t randi(int32_t min, int32_t max)
{
    return GetRandomValue(min, max);
}
void irand()
{
    SetRandomSeed(32132);
}
typedef struct particle
{
    Vector2 p;
    Vector2 v;
    Color color;
} particle;

void rolldice(int32_t *dice)
{
    *dice = randi(1, 6);
}
typedef struct tile
{
    Color _color = BLACK;
} tile;

int main(void)
{
#ifdef __TINYC__
    TraceLog(LOG_WARNING, "tcc");
#endif
    int32_t numofparticles = 100;
    irand();
    // Initialization
    //--------------------------------------------------------------------------------------
    int32_t screenWidth = 1280;
    int32_t screenHeight = 720;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60); // Set our game to run at refresh rate
    //--------------------------------------------------------------------------------------
    const float div = 7;
    // Main game loop
    int32_t positions[4][4];
    for (int32_t i = 0; i < 4; i++)
    {
        for (int32_t j = 0; j < 4; j++)
        {
            positions[i][j] = -INT32_MAX;
        }
    }
    tile tilemap[11][11];
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);



        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}