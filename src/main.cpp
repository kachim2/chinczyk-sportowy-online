#include "raylib.h"
#include "raymath.h"
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <iostream>
#include <thread>
#include "ccols.h"
#include "cpath.h"
#include "net.hpp"

int tilesizex;
int tilesizey;

using namespace std;

const Color colors[] = {RED, BLUE, GREEN, YELLOW, VIOLET, BLACK, WHITE, RAYWHITE};
const short bpos[4][2]{
    {1, 1},
    {10, 1},
    {10, 10},
    {1, 10}

};
const short spos[4][2][2]{
    {{1,5}, {1,6}},
    {{7,1},{6,1}},
    {{11,7}, {11, 6}},
    {{5, 11}, 6,11}

};
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
    int place = 0;
    bool inplay = 0;
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
        if(p->place < 39){
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
        else{
            if (cpath[ny][nx] > 1){
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
    }
    return;
}
void movepawn(pawn *p, int k, unsigned char a){
    if(p->inplay == 0 && k!=0){
        p->x = spos[a][0][0];
        p->y = spos[a][0][1];
        p->px = spos[a][1][0];
        p->py = spos[a][1][1];
        p->inplay = 1;
        return;
    }
    for(int i = 0; i<k; i++){
        movepawnone(p);
    }
}
void sendback(pawn pawns[4][4], int pr, int pa){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
        if(i == pr && j == pa) continue;
        else
        if(pawns[i][j].x == pawns[pr][pa].x && pawns[i][j].y == pawns[pr][pa].y)
        {
            pawns[i][j].place = 0;
            pawns[i][j].inplay = 0;
            pawns[i][j].x = bpos[i][0] + (j) % 2;
            pawns[i][j].y = bpos[i][1] + (j) / 2;
        }
        }
    }
}
bool ispawnpressed(pawn *p){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
    && p->x == GetMouseX() /tilesizex
    && p->y == GetMouseY()/tilesizey
){
        return true;
    }else{
        return false;
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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_16X_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    SetTargetFPS(60); // Set our game to run at refresh rate
    //--------------------------------------------------------------------------------------
    const float div = 7;
    // Main game loop
    std::unique_ptr<netdata> sdata = std::make_unique<netdata>();
    std::thread nett(netf, sdata.get());
    
    pawn pawns[4][4];
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            pawns[i][j].place = 0;
            pawns[i][j].inplay = 0;
            pawns[i][j].x = bpos[i][0] + (j) % 2;
            pawns[i][j].y = bpos[i][1] + (j) / 2;
            pawns[i][j]._color = colors[i];
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
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        tilesizex = screenWidth / 13;
        tilesizey = screenHeight / 13;
        
        if (!sdata->done_main)
        {
            movepawn(&pawns[sdata->MovePlayerId][sdata->MovePawnId], sdata->Movement, sdata->MovePlayerId);
            sendback(pawns, sdata->MovePlayerId, sdata->MovePawnId);
            sdata->done_main = 1;
        }
        if(sdata->selecting){
            for (int i = 0; i < 4; i++){
                if(ispawnpressed(&pawns[sdata->MyPlayerId][i])){
                sdata->Selected = i;
                sdata->selecting = 0;
                break;
                }
            }

        }
        
        BeginDrawing();

        ClearBackground(RAYWHITE);
        for (int i = 0; i < 13; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                DrawRectangle(i * tilesizex + tilesizex / 20, j * tilesizey + tilesizey / 20, (tilesizex * 9) / 10, (tilesizey * 9) / 10, tilemap[i][j]._color);
            }
        }
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                DrawEllipse(pawns[i][j].x * tilesizex + tilesizex / 2, pawns[i][j].y * tilesizey + tilesizey / 2, tilesizex / 2.5, tilesizey / 2.5, BLACK);
                DrawEllipse(pawns[i][j].x * tilesizex + tilesizex / 2, pawns[i][j].y * tilesizey + tilesizey / 2, tilesizex / 3, tilesizey / 3, pawns[i][j]._color);
                
            }
        }
        DrawText((to_string(sdata->DiceRoll) + "    "+ to_string(sdata->MyPlayerId+1) + "    " + to_string(sdata->selecting)).c_str() , 0, 0, 72, BLACK);
        EndDrawing();
    }
    CloseWindow();
    //close(sock);
    return 0;
}
