#include "achievementspch.h"

class achievement_resilient_victory: public AchievementCriteriaScript 
{
public:
    achievement_resilient_victory() : AchievementCriteriaScript("achievement_resilient_victory") { }

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        BattleGround* bg = source->GetBattleGround();
        if (bg)
        {
            if (bg->GetTypeId() == BATTLEGROUND_AB)
            {
                const PvpTeamIndex team = source->GetTeamId();
                if (team != TEAM_INDEX_NEUTRAL)
                {
                    const uint32 teamScore = sAchievementsMgr.GetTeamScore(bg, team);
                    const uint32 otherTeamScore = sAchievementsMgr.GetTeamScore(bg, BattleGround::GetOtherTeamIndex(team));
                    return teamScore > (otherTeamScore + 500);
                }
            }
        }

        return false;
    }
};

class achievement_bg_control_all_nodes : public AchievementCriteriaScript
{
public:
    achievement_bg_control_all_nodes() : AchievementCriteriaScript("achievement_bg_control_all_nodes") { }

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        BattleGround* bg = source->GetBattleGround();
        if (bg)
        {
            if (bg->GetTypeId() == BATTLEGROUND_AB)
            {
                const PvpTeamIndex team = source->GetTeamId();
                return bg->GetBgMap()->GetVariableManager().GetVariable(team == TEAM_INDEX_ALLIANCE ? BG_AB_OP_OCCUPIED_BASES_ALLY : BG_AB_OP_OCCUPIED_BASES_HORDE) == BG_AB_MAX_NODES;
            }

            // TO DO: Add other bg types
            // ...
        }

        return false;
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
        {
            if (victim->IsMounted())
            {
                return true;
            }
        }

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
        if (bg)
        {
            if (bg->GetTypeId() == BATTLEGROUND_AV)
            {
                const PvpTeamIndex team = source->GetTeamId();
                if (team != TEAM_INDEX_NEUTRAL)
                {
                    const uint32 irondeepMine = team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_IRONDEEP_MINE_A : BG_AV_STATE_IRONDEEP_MINE_H;
                    const uint32 coldtoothMine = team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_COLDTOOTH_MINE_A : BG_AV_STATE_COLDTOOTH_MINE_H;    
                    const bool irondeepMineOwned = bg->GetBgMap()->GetVariableManager().GetVariable(irondeepMine);
                    const bool coldtoothMineOwned = bg->GetBgMap()->GetVariableManager().GetVariable(coldtoothMine);
                    return irondeepMineOwned && coldtoothMineOwned;
                }
            }
        }

        return false;
    }
};

class achievement_bg_av_perfection : public AchievementCriteriaScript
{
public:
    achievement_bg_av_perfection() : AchievementCriteriaScript("achievement_bg_av_perfection") { }

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
        BattleGround* bg = source->GetBattleGround();
        if (bg)
        {
            if (bg->GetTypeId() == BATTLEGROUND_AV)
            {
                const PvpTeamIndex team = source->GetTeamId();
                if (team != TEAM_INDEX_NEUTRAL)
                {
                    BattleGroundAV* bgAV = static_cast<BattleGroundAV*>(bg);
                    const auto& nodes = bgAV->GetNodes();
                    if (team == TEAM_INDEX_ALLIANCE)
                    {
                        // alliance towers controlled
                        for (uint8 i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_STONEHEART_BUNKER; ++i)
                        {
                            if (nodes[i].state == POINT_CONTROLLED)
                            {
                                if (nodes[i].owner != TEAM_INDEX_ALLIANCE)
                                    return false;
                            }
                            else
                            {
                                return false;
                            }
                        }

                        // horde towers destroyed
                        for (uint8 i = BG_AV_NODES_ICEBLOOD_TOWER; i <= BG_AV_NODES_FROSTWOLF_WTOWER; ++i)
                        {
                            if (nodes[i].state != POINT_ASSAULTED)
                            {
                                return false;
                            }
                        }

                        return bgAV->IsActiveEvent(BG_AV_NODE_CAPTAIN_DEAD_A, 0);
                    }
                    else
                    {
                        // horde towers controlled
                        for (uint8 i = BG_AV_NODES_ICEBLOOD_TOWER; i <= BG_AV_NODES_FROSTWOLF_WTOWER; ++i)
                        {
                            if (nodes[i].state == POINT_CONTROLLED)
                            {
                                if (nodes[i].owner != TEAM_INDEX_HORDE)
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }
                        }

                        // alliance towers destroyed
                        for (uint8 i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_STONEHEART_BUNKER; ++i)
                        {
                            if (nodes[i].state != POINT_ASSAULTED)
                            {
                                return false;
                            }
                        }

                        return bgAV->IsActiveEvent(BG_AV_NODE_CAPTAIN_DEAD_H, 0);
                    }
                }
            }
        }
        
        return false;
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
