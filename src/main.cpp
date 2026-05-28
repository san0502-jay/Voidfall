#include <raylib.h>
#include "Game.h"
#include "Config.h"

int main () {

    InitWindow(800,600,"Voidfall");
    SetTargetFPS(60);

    Game  game;

    bool shouldExit = false;

    while (!WindowShouldClose() && !shouldExit) {

        game.HandleInput(shouldExit);

        if (game.currentGameState == GameState::playing) {
            game.Update();
        }

        UpdateHotReload();
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(game.camera);
        game.Draw();
        EndMode2D();
        game.DrawUI();

        EndDrawing();
    }
    LoadConfig();
}