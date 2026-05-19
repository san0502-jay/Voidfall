#include "Projectile.h"

Projectile::Projectile(Vector2 startPosition, Vector2 targetDirection) {

    position = startPosition;
    direction = targetDirection;
    speed = 500.0f;
    active = true;
}

void Projectile::Update() {

    position.x += speed * direction.x * GetFrameTime();
    position.y += speed * direction.y * GetFrameTime();
}
void Projectile::Draw() {
    DrawCircle(position.x, position.y,5,YELLOW);
}
