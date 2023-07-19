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
Vector2 gravity(double m1, double m2, double rx, double ry, double t)
{
    double G = 0.01;
    double soft = 0.001;
    double r = sqrt((rx * rx) + (ry * ry));
    double Fx = (G * m1 * m2 * rx) / (soft + r * r);
    double Fy = (G * m1 * m2 * ry) / (soft + r * r);
    double gx = Fx / m1;
    double gy = Fy / m1;
    double vx = gx * t;
    double vy = gy * t;
    Vector2 v;
    v.x = vx;
    v.y = vy;
    return v;
}
void rolldice(int32_t *dice)
{
    *dice = randi(1, 6);
}
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

        DrawRectangle(screenWidth / div, screenHeight / div, screenHeight / div, screenHeight / div, RED);
        DrawRectangle(screenWidth / div, (screenHeight / div) * (div - 2), screenHeight / div, screenHeight / div, RED);
        DrawRectangle((screenWidth / div) * (div - 2), screenHeight / div, screenHeight / div, screenHeight / div, RED);
        DrawRectangle((screenWidth / div) * (div - 2), (screenHeight / div) * (div - 2), screenHeight / div, screenHeight / div, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}