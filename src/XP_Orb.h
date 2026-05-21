#pragma once

#include<raylib.h>

class XPOrb {
    public:

    Vector2 position;
    int value;
    bool active;

    XPOrb(Vector2 spawnPosition);

    void Draw();
};