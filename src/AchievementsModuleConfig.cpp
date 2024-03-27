#include "AchievementsModuleConfig.h"

namespace cmangos_module
{
    AchievementsModuleConfig::AchievementsModuleConfig()
    : ModuleConfig("achievements.conf")
    , enabled(false)
    , sendMessage(false)
    , sendAddon(false)
    , sendVisual(false)
    , randomBots(false)
    , randomBotsRealmFirst(false)
    , accountAchievements(false)
    , effectId(0)
    {

    }

    bool AchievementsModuleConfig::OnLoad()
    {
        enabled = config.GetBoolDefault("Achievements.Enable", false);
        sendMessage = config.GetBoolDefault("Achievements.SendMessage", false);
        sendAddon = config.GetBoolDefault("Achievements.SendAddon", false);
        sendVisual = config.GetBoolDefault("Achievements.SendVisual", false);
        randomBots = config.GetBoolDefault("Achievements.RandomBots", false);
        randomBotsRealmFirst = config.GetBoolDefault("Achievements.RandomBotsRealmFirst", false);
        accountAchievements = config.GetBoolDefault("Achievements.AccountAchievenemts", false);
        effectId = config.GetIntDefault("Achievements.EffectId", 0);
        return true;
    }
}