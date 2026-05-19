#include <raylib.h>
#include "Game.h"
#include "Config.h"

int main () {

    InitWindow(800,600,"Voidfall");
    SetTargetFPS(60);

    Game  game;

    while (!WindowShouldClose()) {
        UpdateHotReload();
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(game.camera);
        game.Update();
        game.Draw();
        EndMode2D();
        EndDrawing();
    }
    LoadConfig();
}