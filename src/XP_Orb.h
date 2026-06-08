#pragma once

#include<raylib.h>
#include "raymath.h"

class XPOrb {
    public:

    Vector2 position;
    int value;
    bool active;

    XPOrb(Vector2 spawnPosition,int value);

    void update(Vector2 playerPosition);

    void Draw();
};