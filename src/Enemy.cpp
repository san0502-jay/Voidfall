#include "Enemy.h"
#include "raymath.h"


Enemy::Enemy(EnemyType enemyType) {


    
    type = enemyType;
    switch (type) {
        case EnemyType::Normal:
            health = 3;
            radius = 10.0f;
            speed = 100.0f;
            xpReward = 10;
            color = WHITE;
            damage = 1;
            break;

        case EnemyType::Fast:
            health = 1;
            radius = 8.0f;
            speed = 180.0f;
            xpReward = 15;
            color = YELLOW;
            damage = 2;
            break;

        case EnemyType::Tank:
            xpReward = 25;
            radius = 16.0f;
            health = 10;
            speed = 60.0f;
            color = RED;
            damage = 3;
            break;

        case EnemyType::Boss:
            xpReward = 40;
            radius = 26.0f;
            health = 15;
            speed = 15.0f;
            color = PURPLE;
            damage = 5;
            break;
    }

    position= {
        (float)GetRandomValue(0,800),
        (float)GetRandomValue(0,600)
     };
    speed = 100.0f;

    active = true;

    hitFlashTimer = 0.0f;

    isHit = false;

}

void Enemy::Update(Vector2 PlayerPosition) {

    Vector2 direction =
    {
        PlayerPosition.x - position.x,
        PlayerPosition.y - position.y
    };

    direction = Vector2Normalize(direction);

    position.x += direction.x * speed * GetFrameTime();

    position.y += direction.y * speed * GetFrameTime();


    if (isHit) {
        hitFlashTimer -= GetFrameTime();

        if (hitFlashTimer <= 0.0f) {
            isHit = false;
        }
    }
}

void Enemy::Draw() {


    Color drawColor ;
    if (isHit) {
        drawColor = ORANGE;
    }
    else {
        drawColor  =  color;
    }

    switch(type) {
        case EnemyType::Normal:
            DrawCircle(position.x, position.y, radius, drawColor);
            break;

        case EnemyType::Fast:
            DrawCircle(position.x, position.y, radius, drawColor);
            break;

        case EnemyType::Tank:
            DrawCircle(position.x, position.y, radius, drawColor);
            break;

        case EnemyType::Boss:
            DrawCircle(position.x, position.y, radius, drawColor);
    }

    DrawText(
    TextFormat("%d", health),
    position.x - 10,
    position.y - 30,
    20,
    WHITE
);
}