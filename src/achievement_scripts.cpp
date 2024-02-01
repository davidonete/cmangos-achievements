#include "AchievementScriptMgr.h"

#include "BattleGround/BattleGroundAB.h"
#include "BattleGround/BattleGroundAV.h"
#include "BattleGround/BattleGroundWS.h"
#include "Entities/Player.h"

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
                const PvpTeamIndex team = GetTeamIndexByTeamId(source->GetTeam());
                if (team != TEAM_INDEX_NEUTRAL)
                {
                    const uint32 teamScore = sAchievementsMgr.GetBGTeamScore(bg, source->GetTeam());
                    const uint32 otherTeamScore = sAchievementsMgr.GetBGTeamScore(bg, BattleGround::GetOtherTeam(source->GetTeam()));
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
                const PvpTeamIndex team = GetTeamIndexByTeamId(source->GetTeam());
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
            return bg && bg->GetTypeId() == BATTLEGROUND_WS && bg->IsActiveEvent(WS_EVENT_FLAG_A, GetTeamIndexByTeamId(player->GetTeam()));
        }

        return false;
    }
};

class achievement_arena_by_type : public AchievementCriteriaScript
{
public:
    achievement_arena_by_type(char const* name, uint8 arenaType) : AchievementCriteriaScript(name), arenaType(arenaType) {}

    bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
    {
#if defined(MANGOSBOT_ONE) || defined(MANGOSBOT_TWO) || MAX_EXPANSION >= 1
        return source->InArena() && source->GetBattleGround()->GetArenaType() == arenaType;
#else
        return false;
#endif
    }

private:
    const uint8 arenaType;
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
                const PvpTeamIndex team = GetTeamIndexByTeamId(source->GetTeam());
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
                const PvpTeamIndex team = GetTeamIndexByTeamId(source->GetTeam());
                if (team != TEAM_INDEX_NEUTRAL)
                {
                    constexpr uint8 MAX_TOWERS = 8;
                    const AVWorldStates towers[MAX_TOWERS] =
                    {
                        team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_SOUTH_BUNKER_A : BG_AV_STATE_SOUTH_BUNKER_H,
                        team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_NORTH_BUNKER_A : BG_AV_STATE_NORTH_BUNKER_H,
                        team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_ICEWING_BUNKER_A : BG_AV_STATE_ICEWING_BUNKER_H,
                        team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_STONE_BUNKER_A : BG_AV_STATE_STONE_BUNKER_H,
                        team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_ICEBLOOD_TOWER_A : BG_AV_STATE_ICEBLOOD_TOWER_H,
                        team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_TOWER_POINT_A : BG_AV_STATE_TOWER_POINT_H,
                        team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_FROSTWOLF_EAST_A : BG_AV_STATE_FROSTWOLF_EAST_H,
                        team == TEAM_INDEX_ALLIANCE ? BG_AV_STATE_FROSTWOLF_WEST_A : BG_AV_STATE_FROSTWOLF_WEST_H
                    };

                    for (uint8 i = 0; i < MAX_TOWERS; ++i)
                    {
                        const bool isTowerControlled = bg->GetBgMap()->GetVariableManager().GetVariable(towers[i]);
                        if (!isTowerControlled)
                        {
                            return false;
                        }
                    }

                    const uint32 captainState = team == TEAM_INDEX_ALLIANCE ? BG_AV_NODE_CAPTAIN_DEAD_A : BG_AV_NODE_CAPTAIN_DEAD_H;
                    return !bg->IsActiveEvent(captainState, 0);
                }
            }
        }
        
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
#if defined(MANGOSBOT_ONE) || defined(MANGOSBOT_TWO) || MAX_EXPANSION >= 1
    new achievement_arena_by_type("achievement_arena_2v2_check", ARENA_TYPE_2v2);
    new achievement_arena_by_type("achievement_arena_3v3_check", ARENA_TYPE_3v3);
    new achievement_arena_by_type("achievement_arena_5v5_check", ARENA_TYPE_5v5);
#else
    new achievement_arena_by_type("achievement_arena_2v2_check", 0);
    new achievement_arena_by_type("achievement_arena_3v3_check", 0);
    new achievement_arena_by_type("achievement_arena_5v5_check", 0);
#endif
    new achievement_killed_exp_or_honor_target();
}
