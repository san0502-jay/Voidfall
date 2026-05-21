#include "Game.h"
#include <raymath.h>
#include <cmath>
#include <math.h>
#include <algorithm>

Game::Game() {
    camera.target = Player.position;
    camera.offset = {640/2.0f,360/2.0f};

    camera.rotation = 0.0f;

    enemySpawnTimer = config.enemySpawnTimer;
    enemySpawnInterval = config.enemySpawnInterval;
    shootTimer = 0.0f;
    shootInterval = 0.5f;



 }


void Game::Update() {
    camera.target = Player.position;
    camera.zoom = config.cameraZoom;
    enemySpawnTimer += GetFrameTime();
    shootTimer += GetFrameTime();

    Player.Update();

    if (enemySpawnTimer >= enemySpawnInterval)
    {
        enemySpawnTimer = 0.0f;

        float angle =
            GetRandomValue(0, 360) * DEG2RAD;

        float distance = 800.0f;

        Vector2 spawnPosition =
        {
            Player.position.x + cos(angle) * distance,

            Player.position.y + sin(angle) * distance
        };

        Enemy enemy;

        enemy.position = spawnPosition;

        enemies.push_back(enemy);
    }

    for (Enemy& enemy : enemies)
    {
        enemy.Update(Player.position);
    }

    if (shootTimer >= shootInterval ) {
        shootTimer = 0.0f;
        Enemy* nearestEnemy = nullptr;
        float closestDistance = 999999.0f;


        for (Enemy& enemy : enemies) {
            float distance = Vector2Distance(Player.position,enemy.position);

            if (distance < 60)
            {
                Player.health -= 1;

                //enemy.active = false;
            }

            if (distance < closestDistance) {
                closestDistance = distance;
                nearestEnemy = &enemy;
            }
        }
        if (nearestEnemy != nullptr) {
            Vector2 direction = {
                nearestEnemy->position.x - Player.position.x,
                nearestEnemy->position.y - Player.position.y
            };
            direction = Vector2Normalize(direction);

            Projectile projectile(Player.position,direction);
            projectiles.push_back(projectile);
        }

    }
    for (Projectile& projectile : projectiles)
    {
        projectile.Update();
    }

    for (Projectile& projectile : projectiles) {
        for (Enemy& enemy : enemies) {
            if (enemy.active && projectile.active) {
                float distance = Vector2Distance(projectile.position,enemy.position);
                if (distance < 15) {
                    enemy.active = false;
                    XPOrb orb(enemy.position);
                    xpOrbs.push_back(orb);
                    projectile.active = false;
                }
            }
        }
    }

    for (XPOrb& orb : xpOrbs) {
        float distance = Vector2Distance(Player.position,orb.position);
        if (distance < 40) {
            Player.xp += orb.value;
            orb.active = false;
        }
    }

    auto isDead = [](auto& x){ return !x.active; };

    projectiles.erase(
        std::remove_if(projectiles.begin(),projectiles.end(),isDead),projectiles.end());

    enemies.erase(
        std::remove_if(enemies.begin(),enemies.end(),isDead),enemies.end());

    xpOrbs.erase(std::remove_if(xpOrbs.begin(),xpOrbs.end(),isDead),xpOrbs.end());
}

void Game::Draw() {


    Player.Draw();
    for (int x = -5000; x <= 5000; x += 64)
    {
        DrawLine(x, -5000, x, 5000, DARKGRAY);
    }

    for (int y = -5000; y <= 5000; y += 64)
    {
        DrawLine(-5000, y, 5000, y, DARKGRAY);
    }
    for (Enemy& enemy : enemies) {
        {
            if (enemy.active) {
                enemy.Draw();
            }
        }
    }

    for (Projectile& projectile : projectiles)
    {
        if (projectile.active) {
            projectile.Draw();
        }
    }

    DrawText(
    TextFormat("Health: %i",Player.health),
    camera.target.x - 300,
    camera.target.y - 160,
    20,
    WHITE
);
    for (XPOrb& orb : xpOrbs) {
        {
            if (orb.active) {
                orb.Draw();
            }
        }
    }
    DrawText(
    TextFormat("XP: %i", Player.xp),
    camera.target.x - 300,
    camera.target.y - 130,
    20,
    GREEN
);
    Player.Draw();
    DrawCircleLines(
    Player.position.x,
    Player.position.y,
    40,
    GREEN
);
}
