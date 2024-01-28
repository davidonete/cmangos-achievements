#include "achievementspch.h"

class achievement_resilient_victory: public AchievementCriteriaScript 
{
public:
    achievement_resilient_victory() : AchievementCriteriaScript("achievement_resilient_victory") { }

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        BattleGround* bg = source->GetBattleGround();
        return bg && bg->GetTypeId() == BATTLEGROUND_AB && reinterpret_cast<BattleGroundAB*>(bg)->IsTeamScores500Disadvantage(source->GetTeamId());
    }
};

class achievement_bg_control_all_nodes : public AchievementCriteriaScript
{
public:
    achievement_bg_control_all_nodes() : AchievementCriteriaScript("achievement_bg_control_all_nodes") { }

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        BattleGround* bg = source->GetBattleGround();
        return bg && bg->AllNodesConrolledByTeam(source->GetTeamId());
    }
};

class achievement_save_the_day : public AchievementCriteriaScript
{
public:
    achievement_save_the_day() : AchievementCriteriaScript("achievement_save_the_day") { }

    bool OnCheck(Player* source, Unit* target, uint32 /*criteria_id*/) override
    {
        if (!target)
            return false;

        if (Player const* player = static_cast<Player*>(target))
        {
            BattleGround* bg = source->GetBattleGround();
            return bg && bg->GetTypeId() == BATTLEGROUND_WS && bg->IsActiveEvent(WS_EVENT_FLAG_A, player->GetTeamId());
        }
        return false;
    }
};

class achievement_sickly_gazelle : public AchievementCriteriaScript
{
public:
    achievement_sickly_gazelle() : AchievementCriteriaScript("achievement_sickly_gazelle") { }

    bool OnCheck(Player* /*source*/, Unit* target, uint32 /*criteria_id*/) override
    {
        if (!target)
            return false;

        if (Player* victim = static_cast<Player*>(target))
            if (victim->IsMounted())
                return true;

        return false;
    }
};

class achievement_everything_counts : public AchievementCriteriaScript
{
public:
    achievement_everything_counts() : AchievementCriteriaScript("achievement_everything_counts") { }

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        BattleGround* bg = source->GetBattleGround();
        return bg && bg->GetTypeId() == BATTLEGROUND_AV && reinterpret_cast<BattleGroundAV*>(bg)->IsBothMinesControlledByTeam(source->GetTeamId());
    }
};

class achievement_bg_av_perfection : public AchievementCriteriaScript
{
public:
    achievement_bg_av_perfection() : AchievementCriteriaScript("achievement_bg_av_perfection") { }

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        BattleGround* bg = source->GetBattleGround();
        return bg && bg->GetTypeId() == BATTLEGROUND_AV && reinterpret_cast<BattleGroundAV*>(bg)->IsAllTowersControlledAndCaptainAlive(source->GetTeamId());
    }
};

class achievement_tilted : public AchievementCriteriaScript
{
public:
    achievement_tilted() : AchievementCriteriaScript("achievement_tilted") {}

    bool OnCheck(Player* player, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        if (!player)
            return false;

        uint32 areaid = player->GetAreaId();
        // bool checkArea =    areaid == AREA_ARGENT_TOURNAMENT_FIELDS ||
        //                     areaid == AREA_RING_OF_ASPIRANTS ||
        //                     areaid == AREA_RING_OF_ARGENT_VALIANTS ||
        //                     areaid == AREA_RING_OF_ALLIANCE_VALIANTS ||
        //                     areaid == AREA_RING_OF_HORDE_VALIANTS ||
        //                     areaid == AREA_RING_OF_CHAMPIONS;

        bool checkArea = false;

        return checkArea && player->duel;// && player->duel->isMounted;
    }
};

class achievement_not_even_a_scratch : public AchievementCriteriaScript
{
public:
    achievement_not_even_a_scratch() : AchievementCriteriaScript("achievement_not_even_a_scratch") { }

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        if (!source)
            return false;

        BattleGround* battleground = source->GetBattleGround();
        // return battleground && battleground->GetTypeID() == BATTLEGROUND_SA && battleground->ToBattleGroundSA()->notEvenAScratch(source->GetTeamId());
        return false;
    }
};

class achievement_killed_exp_or_honor_target : public AchievementCriteriaScript
{
public:
    achievement_killed_exp_or_honor_target() : AchievementCriteriaScript("achievement_killed_exp_or_honor_target") { }

    bool OnCheck(Player* player, Unit* target, uint32 /*criteria_id*/) override
    {
        return target && player->isHonorOrXPTarget(target);
    }
};

void AddSC_achievement_scripts()
{
    new achievement_resilient_victory();
    new achievement_bg_control_all_nodes();
    new achievement_save_the_day();
    new achievement_sickly_gazelle();
    new achievement_everything_counts();
    new achievement_bg_av_perfection();
    new achievement_tilted();
    new achievement_not_even_a_scratch();
    new achievement_killed_exp_or_honor_target();
}
