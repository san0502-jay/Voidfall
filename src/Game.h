#pragma once

#include <raylib.h>
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"
#include "XP_Orb.h"

class Game {
    public:

    Camera2D camera;


    Game();

    void Update();
    void Draw();

private:
    Player Player;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    std::vector<XPOrb> xpOrbs;
    float enemySpawnTimer;
    float enemySpawnInterval;
    float shootTimer;
    float shootInterval;


};
