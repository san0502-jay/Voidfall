#include "XP_Orb.h"

XPOrb::XPOrb(Vector2 spawnPosition) {
    position = spawnPosition;

    value = 10;

    active = true;
}


void XPOrb::Draw() {
    DrawCircle(position.x,position.y,6,GREEN);
}
