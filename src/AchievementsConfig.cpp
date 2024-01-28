#include "achievementspch.h"
#include "SystemConfig.h"
#include "AchievementsConfig.h"

AchievementsConfig::AchievementsConfig()
: enabled(false)
, sendMessage(false)
, sendAddon(false)
, sendVisual(false)
, randomBots(false)
, randomBotsRealmFirst(false)
, accountAchievements(false)
, effectId(0)
{

}

INSTANTIATE_SINGLETON_1(AchievementsConfig);

bool AchievementsConfig::Initialize()
{
    sLog.outString("Initializing Achievements by ike3");

    if (!config.SetSource(SYSCONFDIR"achievements.conf"))
    {
        sLog.outError("Failed to open configuration file achievements.conf");
        return false;
    }

    enabled = config.GetBoolDefault("Immersive.Enable", false);
    sendMessage = config.GetBoolDefault("Immersive.SendMessage", false);
    sendAddon = config.GetBoolDefault("Immersive.SendAddon", false);
    sendVisual = config.GetBoolDefault("Immersive.SendVisual", false);
    randomBots = config.GetBoolDefault("Immersive.RandomBots", false);
    randomBotsRealmFirst = config.GetBoolDefault("Immersive.RandomBotsRealmFirst", false);
    accountAchievements = config.GetBoolDefault("Immersive.AccountAchievenemts", false);
    effectId = config.GetIntDefault("Immersive.EffectId", 0);

    sLog.outString("Achievements configuration loaded");
    return true;
}