#pragma once

#include "Config.h"

class AchievementsConfig
{
public:
    AchievementsConfig();

    static AchievementsConfig& instance()
    {
        static AchievementsConfig instance;
        return instance;
    }

    bool Initialize();

public:
    bool enabled;
    bool sendMessage;
    bool sendAddon;
    bool sendVisual;
    bool randomBots;
    bool randomBotsRealmFirst;
    bool accountAchievements;
    uint32 effectId;

private:
    Config config;
};

#define sAchievementsConfig MaNGOS::Singleton<AchievementsConfig>::Instance()

