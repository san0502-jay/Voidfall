#include "Config.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

Config config;

static std::filesystem::file_time_type lastWriteTime;

void LoadConfig()
{
    std::ifstream file("config.txt");

    if(!file.is_open())
    {
        std::cout << "Could not open config.txt\n";
        return;
    }

    std::string line;

    while(std::getline(file, line))
    {
        if(line.find("player_speed=") == 0)
        {
            config.playerSpeed =
                std::stof(line.substr(13));
        }
        else if (line.find("enemy_Spawn_Timer=") == 0) {
            config.enemySpawnTimer = std::stof(line.substr(19));
        }

        else if (line.find("enemy_Spawn_Interval=") == 0) {
            config.enemySpawnInterval = std::stof(line.substr(22));}

        else if(line.find("camera_zoom=") == 0)
        {
            config.cameraZoom =
                std::stof(line.substr(12));
        }
    }

    std::cout << "Config Reloaded!\n";
}

void UpdateHotReload()
{
    auto currentWriteTime =
        std::filesystem::last_write_time("config.txt");

    if(currentWriteTime != lastWriteTime)
    {
        lastWriteTime = currentWriteTime;

        LoadConfig();
    }
}