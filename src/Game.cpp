#include "Game.h"
#include<raymath.h>
#include <cmath>
#include <math.h>


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
            enemy.Draw();
        }
    }

    for (Projectile& projectile : projectiles)
    {
        projectile.Draw();
    }
    Player.Draw();
}
