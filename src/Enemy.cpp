#include "Enemy.h"
#include "raymath.h"


Enemy::Enemy() {

    position= {
        ( float)GetRandomValue(0,800),
        ( float)GetRandomValue(0,600)
     };
    speed = 100.0f;

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
}

void Enemy::Draw() {
    DrawCircle(
        position.x,
        position.y,
        10,
        GRAY
    );
}
