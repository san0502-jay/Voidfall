#include "Game.h"
#include <raymath.h>
#include <cmath>
#include <algorithm>
#include <iostream>

Game::Game() {
    camera.target = player.position;
    camera.offset = {640/2.0f,360/2.0f};

    camera.rotation = 0.0f;

    RestartButton = { 300, 300, 200, 60 };
    PauseButton   = { 700,  10,  80, 40 };
    ResumeButton  = { 300, 300, 200, 60 };
    ExitButton    = { 300, 400, 200, 60 };
    upgradeButton1 = {150, 200, 150, 200};
    upgradeButton2 = {325, 200, 150, 200};
    upgradeButton3 = {500, 200, 150, 200};

    enemySpawnTimer = 0.0f;
    enemySpawnInterval = 1.0f;
    shootTimer = 0.0f;
    shootInterval = 0.5f;
    difficultyTimer = 0.0f;
    threat = 0;
    bossAlive = false;
    bossRollTimer = 0.0f;



 }

void Game::HandleInput(bool& shouldExit) {
    Vector2 mouse = GetMousePosition();
    bool clicked  = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (currentGameState == GameState:: GameOver) {
        if (clicked && CheckCollisionPointRec(mouse, RestartButton)) {
            Reset();
        }
    }

    if (currentGameState == GameState:: playing) {
        if (clicked && CheckCollisionPointRec(mouse, PauseButton)) {
            currentGameState = GameState:: paused;
        }
    }
    if (currentGameState == GameState::paused) {
        if (clicked && CheckCollisionPointRec(mouse, ResumeButton))
            currentGameState = GameState::playing;
    }

    if (currentGameState == GameState::paused ||
        currentGameState == GameState::GameOver) {
        if (clicked && CheckCollisionPointRec(mouse, ExitButton))
            shouldExit = true;
        }

    if (currentGameState == GameState::levelup) {
        if (
    CheckCollisionPointRec(
        GetMousePosition(),
        upgradeButton1
    )

    &&

    IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
)
        {
            player.weapon.fireRate -= 0.1f;

            if (player.weapon.fireRate < 0.1f)
            {
                player.weapon.fireRate = 0.1f;
            }

            currentGameState = GameState::playing;
        }

        if (
    CheckCollisionPointRec(
        GetMousePosition(),
        upgradeButton2
    )

    &&

    IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
)
        {
            player.speed *= 1.25f;

            currentGameState = GameState::playing;
        }
        if (
    CheckCollisionPointRec(
        GetMousePosition(),
        upgradeButton3
    )

    &&

    IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
)
        {
            player.maxhealth += 25;
            player.health += 25;
            currentGameState = GameState::playing;
        }
    }


}

void Game::Update() {
    camera.target = player.position;
    camera.zoom = config.cameraZoom;
    enemySpawnTimer += GetFrameTime();
    shootTimer += GetFrameTime();
    difficultyTimer += GetFrameTime();
    player.damageTimer += GetFrameTime();
    bossRollTimer += GetFrameTime();


    if (currentGameState == GameState::playing) {
        player.Update();

        if (difficultyTimer >= 5.0f) {
            difficultyTimer = 0.0f;
            enemySpawnInterval -= 0.1f;

            if (enemySpawnInterval < 0.5f) {
                enemySpawnInterval = 0.5f;
            }
        }

        if (enemySpawnTimer >= enemySpawnInterval)
        {
            enemySpawnTimer = 0.0f;

            float angle =
                GetRandomValue(0, 360) * DEG2RAD;

            float distance = 800.0f;

            Vector2 spawnPosition =
            {
                player.position.x + cos(angle) * distance,

                player.position.y + sin(angle) * distance
            };

            int enemyRoll = GetRandomValue(0, 9);
            EnemyType enemyType;

            if(enemyRoll < 6)
            {
                enemyType = EnemyType::Normal;
            }
            else if(enemyRoll < 9)
            {
                enemyType = EnemyType::Fast;
            }
            else {
                enemyType = EnemyType::Tank;
            }

            Enemy enemy(enemyType);

            enemy.position = spawnPosition;

            enemies.push_back(enemyType);
        }
        for (Enemy& enemyA :enemies) {
            for (Enemy& enemyB : enemies) {
                if (&enemyA == &enemyB)
                    continue;

                float distance = Vector2Distance(enemyA.position,enemyB.position);
                float minDistance = 25.0f;

                if (distance < minDistance) {
                    Vector2 push  = {enemyA.position.x - enemyB.position.x, enemyA.position.y - enemyB.position.y};
                    push = Vector2Normalize(push);

                    enemyA.position.x += push.x * 50 * GetFrameTime();
                    enemyA.position.y += push.y * 50 * GetFrameTime();
                }
            }
        }

        for (Enemy& enemy : enemies)
        {
            enemy.Update(player.position);
        }

        if (shootTimer >= player.weapon.fireRate) {
            shootTimer = 0.0f;
            Enemy* nearestEnemy = nullptr;
            float closestDistance = 999999.0f;


            for (Enemy& enemy : enemies) {
                if (!enemy.active)
                    continue;
                float distance = Vector2Distance(player.position,enemy.position);
                float collisionRadius = player.radius + enemy.radius;

                if (distance < collisionRadius)
                {
                    if(player.damageTimer >= player.damageCooldown)
                    {
                        player.health = player.health - enemy.damage;
                        std::cout << player.health << std::endl;
                        std::cout << enemy.damage << std::endl;
                        player.damageTimer = 0.0f;
                        if (player.health <= 0 ) {
                            currentGameState = GameState::GameOver;
                        }
                    }
                }

                if (distance < closestDistance) {
                    closestDistance = distance;
                    nearestEnemy = &enemy;
                }
            }
            if (nearestEnemy != nullptr) {
                Vector2 direction = {
                    nearestEnemy->position.x - player.position.x,
                    nearestEnemy->position.y - player.position.y
                };
                direction = Vector2Normalize(direction);

                Projectile projectile(player.position,direction);
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
                    if (distance < 15)
                    {
                        enemy.health -= player.weapon.damage;
                        enemy.isHit = true;
                        enemy.hitFlashTimer = 0.1f;
                        Vector2 knockbaack = {enemy.position.x - projectile.position.x,enemy.position.y - projectile.position.y};

                        knockbaack = Vector2Normalize(knockbaack);

                        enemy.position.x += knockbaack.x * 20;
                        enemy.position.y += knockbaack.y * 20;

                        projectile.active = false;

                        if (enemy.health <= 0)
                        {
                            if (enemy.type == EnemyType::Boss)
                            {
                                bossAlive = false;
                            }
                            enemy.active = false;

                            threat += enemy.xpReward;

                            XPOrb orb(enemy.position,enemy.xpReward);

                            xpOrbs.push_back(orb);
                        }

                        break;
                    }
                }
            }
        }
        if (!bossAlive && threat >= 500 && bossRollTimer >= 2.0f) {
            bossRollTimer = 0.0f;
            int roll = GetRandomValue(0,99);

            if (roll < 25) {
                Enemy boss(EnemyType::Boss);

                boss.position ={
                player.position.x + 100, player.position.x};
                enemies.push_back(boss);
                bossAlive = true;
                threat = 0;
            }
        }

        for (XPOrb& orb : xpOrbs) {
            orb.update(player.position);
            float distance = Vector2Distance(player.position,orb.position);
            if (distance < 50) {
                player.xp += orb.value;
                orb.active = false;
            }

        }
    }

        if (currentGameState == GameState::playing && player.xp >= player.xpToNextLevel) {
            player.level++;
            player.xp -= player.xpToNextLevel;
            player.xpToNextLevel = static_cast<int> (player.xpToNextLevel * 1.45f);
            currentGameState = GameState::levelup;
        }

        auto isDead = [](auto& x){ return !x.active; };

        projectiles.erase(
            std::remove_if(projectiles.begin(),projectiles.end(),isDead),projectiles.end());

        enemies.erase(
            std::remove_if(enemies.begin(),enemies.end(),isDead),enemies.end());

        xpOrbs.erase(std::remove_if(xpOrbs.begin(),xpOrbs.end(),isDead),xpOrbs.end());


}

void Game::Draw() {


    player.Draw();
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


    for (XPOrb& orb : xpOrbs) {
        {
            if (orb.active) {
                orb.Draw();
            }
        }
    }

}

void Game::DrawUI() {
    if (currentGameState == GameState::playing) {
        DrawRectangleRec(PauseButton,GRAY);
        DrawText("PAUSE",710,20, 18,WHITE);
    }
    if (currentGameState == GameState::paused) {
        DrawRectangle(0,0, GetScreenWidth(),GetScreenHeight(),Fade(BLACK,0.7f));
        DrawText("PAUSED",300,250,50,WHITE);
        DrawRectangleRec(ResumeButton,GREEN);
        DrawText("RESUME",350,320, 25,WHITE);
        DrawRectangleRec(ExitButton,RED);
        DrawText("EXIT",370,420, 25,WHITE);
    }
    if (currentGameState == GameState::GameOver) {
        DrawText("GAME OVER ", 280, 250, 40, RED);
        DrawRectangleRec(RestartButton, RED);
        DrawText("RESTART",340,320, 25,WHITE);
        DrawRectangleRec(ExitButton,RED);
        DrawText("EXIT",370,420, 25,WHITE);

    }
    DrawText(
    TextFormat(
        "HP: %i / %i",
        player.health,
        player.maxhealth
    ),
    30,
    15,
    20,
    WHITE
);
    DrawText(
    TextFormat("XP: %i", player.xp,player.xpToNextLevel
    ),
     30,
     45,
    20,
    PINK
);
    DrawText(
    TextFormat("Level: %i", player.level),

    30,
    75,

    20,
    YELLOW
);
    if (currentGameState == GameState::levelup)
    {
        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            Fade(BLACK, 0.8f)
        );

        DrawText(
            "LEVEL UP",
            280,
            100,
            40,
            YELLOW
        );

        DrawRectangleRec(upgradeButton1, DARKGRAY);

        DrawRectangleRec(upgradeButton2, DARKGRAY);

        DrawRectangleRec(upgradeButton3, DARKGRAY);

        DrawText(
    "FIRE RATE",
    170,
    300,
    20,
    WHITE
);

        DrawText(
            "MOVE SPEED",
            335,
            300,
            20,
            WHITE
        );

        DrawText(
            "MAX HEALTH",
            515,
            300,
            20,
            WHITE
        );
    }

    DrawText(
    TextFormat("Spawn: %.2f", enemySpawnInterval),
    20,
    100,
    20,
    ORANGE
);
    DrawText(
    TextFormat("Difficulty: %.2f", difficultyTimer),
    20,
    160,
    20,
    WHITE
);
    DrawText(
    TextFormat("FireSpeed: %.2f", player.weapon.fireRate),
    20,
    190,
    20,
    WHITE
);
    DrawText(
        TextFormat("Threat: %d", threat),
        20,
        320,
        20,
        ORANGE
    );


}

void Game::Reset() {
    player.xp = 0;
    player.health = 100;
    player.level = 0;
}
