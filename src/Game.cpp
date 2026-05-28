#include "Game.h"
#include <raymath.h>
#include <cmath>
#include <math.h>
#include <algorithm>

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

    enemySpawnTimer = config.enemySpawnTimer;
    enemySpawnInterval = config.enemySpawnInterval;
    shootTimer = 0.0f;
    shootInterval = 0.5f;



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
            player.fireRate -= 0.05f;

            if (player.fireRate < 0.1f)
            {
                player.fireRate = 0.1f;
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
            player.speed += 25.0f;

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


    if (currentGameState == GameState::playing) {
        player.Update();

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

            Enemy enemy;

            enemy.position = spawnPosition;

            enemies.push_back(enemy);
        }

        for (Enemy& enemy : enemies)
        {
            enemy.Update(player.position);
        }

        if (shootTimer >= player.fireRate) {
            shootTimer = 0.0f;
            Enemy* nearestEnemy = nullptr;
            float closestDistance = 999999.0f;


            for (Enemy& enemy : enemies) {
                float distance = Vector2Distance(player.position,enemy.position);

                if (distance < 60)
                {
                    player.health -= 1;

                    //enemy.active = false;
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
            float distance = Vector2Distance(player.position,orb.position);
            if (distance < 40) {
                player.xp += orb.value;
                orb.active = false;
            }
        }
    }

        if (player.xp >= player.xpToNextLevel) {
            player.level++;
            player.xp -= player.xpToNextLevel;
            player.xpToNextLevel +=  50;
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
    TextFormat("Health: %i",player.health),
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

}

void Game::Reset() {
    player.xp = 0;
    player.health = 0;
    player.level = 0;
}
