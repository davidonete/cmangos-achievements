#include "AchievementsConfig.h"

#include "Log.h"

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

    enabled = config.GetBoolDefault("Achievements.Enable", false);
    sendMessage = config.GetBoolDefault("Achievements.SendMessage", false);
    sendAddon = config.GetBoolDefault("Achievements.SendAddon", false);
    sendVisual = config.GetBoolDefault("Achievements.SendVisual", false);
    randomBots = config.GetBoolDefault("Achievements.RandomBots", false);
    randomBotsRealmFirst = config.GetBoolDefault("Achievements.RandomBotsRealmFirst", false);
    accountAchievements = config.GetBoolDefault("Achievements.AccountAchievenemts", false);
    effectId = config.GetIntDefault("Achievements.EffectId", 0);

    sLog.outString("Achievements configuration loaded");
    return true;
}
