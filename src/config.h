#pragma once

struct Config
{
    float playerSpeed = 250;
    float cameraZoom = 1.0f;
    float enemySpawnTimer = 0.0f;
    float enemySpawnInterval = 1.0f;
};

extern Config config;

void LoadConfig();

void UpdateHotReload();