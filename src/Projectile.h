#pragma once

#include "raylib.h"

class Projectile {
    public:
    Vector2 position;
    Vector2 direction;
    float speed;
    bool active;

    Projectile(Vector2 startPosition, Vector2 targetDirection);

    void Update();

    void Draw();

};