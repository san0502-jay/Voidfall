#pragma once

#include <raylib.h>
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"
#include "XP_Orb.h"


enum class GameState {
    playing,paused,GameOver,levelup
};

class Game {
    public:
    GameState currentGameState = GameState::playing;

    Camera2D camera;

    Game();

    void Update();
    void Draw();
    void DrawUI();
    void Reset();
    void HandleInput(bool& shouldExit);

private:

    Player player;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    std::vector<XPOrb> xpOrbs;
    float enemySpawnTimer;
    float enemySpawnInterval;
    float shootTimer;
    float shootInterval;
    float difficultyTimer;




    Rectangle RestartButton;
    Rectangle PauseButton;
    Rectangle ResumeButton;
    Rectangle ExitButton;
    Rectangle upgradeButton1;
    Rectangle upgradeButton2;
    Rectangle upgradeButton3;

};
