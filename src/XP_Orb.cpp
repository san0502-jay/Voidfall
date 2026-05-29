#include "XP_Orb.h"

XPOrb::XPOrb(Vector2 spawnPosition) {
    position = spawnPosition;

    value = 10;

    active = true;
}

void XPOrb::update(Vector2 playerPosition) {
    float pickupRadius = 100.0f;

    float magnetSpeed = 300.0f;

    float distance =
        Vector2Distance(position, playerPosition);

    if (distance < pickupRadius)
    {
        Vector2 direction =
        {
            playerPosition.x - position.x,
            playerPosition.y - position.y
        };

        direction = Vector2Normalize(direction);

        position.x +=
            direction.x *
            magnetSpeed *
            GetFrameTime();

        position.y +=
            direction.y *
            magnetSpeed *
            GetFrameTime();
    }

}

void XPOrb::Draw() {
    DrawCircle(position.x,position.y,6,PINK);
}
