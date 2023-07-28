#include "raylib.h"
#include "raymath.h"
#include <stdint.h>
#include <float.h>
#include <limits.h>

#include "ccols.h"
#include "cpath.h"



const Color colors[] = {RED, BLUE, GREEN, YELLOW, VIOLET, BLACK, WHITE, RAYWHITE};

int32_t NumPlayers = 4;
int32_t randi(int32_t min, int32_t max){
    return GetRandomValue(min, max);
}
void irand(){
    SetRandomSeed(32132);
}

typedef struct pawn{
    Color _color;
    int x, y;
    int px, py;
    int place;
} pawn;
int rolldice(){
    return randi(1, 6);
}
typedef struct tile{
    Color _color;
} tile;
void movepawnone(pawn *p){
    const int posiblemoves[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int i = 0; i < 4; i++)
    {
        const int nx = posiblemoves[i][0] + p->x;
        const int ny = posiblemoves[i][1] + p->y;

        if (cpath[ny][nx] == 1){
            if (nx != p->px || ny != p->py){
                p->px = p->x;
                p->py = p->y;
                p->x = nx;
                p->y = ny;
                p->place++;
                return;
            }
        }
    }
    return;
}
void movepawn(pawn *p, int k){
    for(int i = 0; i<k; i++){
        movepawnone(p);
    }
}
int main(void){
    //struct sockaddr_in addr = {0};
    //addr.sin_family = AF_INET;
    //addr.sin_port = htons(PORT);
    //addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    //int sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    //send(sock, "HA", 2, 0);
    //char buf[2];
    //recv(sock, buf, 2, 0);
    irand();
    // Initialization
    //--------------------------------------------------------------------------------------
    int32_t screenWidth = 800;
    int32_t screenHeight = 800;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
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

    tile tilemap[13][13];
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            tilemap[i][j]._color = colors[ccols[j][i]];
        }
    }
    pawn mpawn = {DARKGREEN, 1, 7, 1, 6};
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        const int tilesizex = screenWidth / 13;
        const int tilesizey = screenHeight / 13;
        
        if (IsKeyPressed(KEY_SPACE))
        {
            const int rn = rolldice();
            movepawn(&mpawn, rn);
        }
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        for (int i = 0; i < 13; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                DrawRectangle(i * tilesizex + tilesizex / 20, j * tilesizey + tilesizey / 20, (tilesizex * 9) / 10, (tilesizey * 9) / 10, tilemap[i][j]._color);
            }
        }
        DrawEllipse(mpawn.x * tilesizex + tilesizex / 2, mpawn.y * tilesizey + tilesizey / 2, tilesizex / 3, tilesizey / 3, mpawn._color);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    //close(sock);
    return 0;
}