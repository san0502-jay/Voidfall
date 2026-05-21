#include <raylib.h>
#include "Player.h"

Player::Player() {
    position = {400,350};
    speed = 300.0f;
    radius = 45.0f;
    health = 100.0f;
    xp = 0;
}

void Player::Update() {

    float deltaTime = GetFrameTime();

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {position.y -= config.playerSpeed * deltaTime;}
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {position.y += config.playerSpeed * deltaTime;}
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {position.x -= config.playerSpeed * deltaTime;}
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {position.x += config.playerSpeed * deltaTime;}


}

void Player::Draw() {

    DrawRectangle(position.x - 20, position.y - 20, radius, radius,RED);
}