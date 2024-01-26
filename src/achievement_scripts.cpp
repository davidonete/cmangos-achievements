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

// class achievement_bg_ic_resource_glut : public AchievementCriteriaScript
// {
// public:
//     achievement_bg_ic_resource_glut() : AchievementCriteriaScript("achievement_bg_ic_resource_glut") { }

//     bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
//     {
//         BattleGround* bg = source->GetBattleGround();
//         return bg && bg->GetBgTypeID(true) == BATTLEGROUND_IC && bg->ToBattleGroundIC()->IsResourceGlutAllowed(source->GetTeamId());
//     }
// };

// class achievement_bg_ic_glaive_grave : public AchievementCriteriaScript
// {
// public:
//     achievement_bg_ic_glaive_grave() : AchievementCriteriaScript("achievement_bg_ic_glaive_grave") { }

//     bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
//     {
//         if (Creature* vehicle = source->GetVehicleCreatureBase())
//             return vehicle->GetEntry() == NPC_GLAIVE_THROWER_H ||  vehicle->GetEntry() == NPC_GLAIVE_THROWER_A;

//         return false;
//     }
// };

// class achievement_bg_ic_mowed_down : public AchievementCriteriaScript
// {
// public:
//     achievement_bg_ic_mowed_down() : AchievementCriteriaScript("achievement_bg_ic_mowed_down") { }

//     bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
//     {
//         if (Creature* vehicle = source->GetVehicleCreatureBase())
//             return vehicle->GetEntry() == NPC_KEEP_CANNON;

//         return false;
//     }
// };

// class achievement_bg_sa_artillery : public AchievementCriteriaScript
// {
// public:
//     achievement_bg_sa_artillery() : AchievementCriteriaScript("achievement_bg_sa_artillery") { }

//     bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
//     {
//         if (Creature* vehicle = source->GetVehicleCreatureBase())
//             return vehicle->GetEntry() == NPC_ANTI_PERSONNAL_CANNON;

//         return false;
//     }
// };

// class achievement_arena_by_type : public AchievementCriteriaScript
// {
// public:
//     achievement_arena_by_type(char const* name, uint8 arenaType) : AchievementCriteriaScript(name),
//         _arenaType(arenaType)
//     {
//     }

//     bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
//     {
//         return source->InArena() && source->GetBattleGround()->GetArenaType() == _arenaType;
//     }

// private:
//     uint8 const _arenaType;
// };

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

// class achievement_sa_defense_of_the_ancients : public AchievementCriteriaScript
// {
// public:
//     achievement_sa_defense_of_the_ancients() : AchievementCriteriaScript("achievement_sa_defense_of_the_ancients") { }

//     bool OnCheck(Player* source, Unit* /*target*/, uint32 /*criteria_id*/) override
//     {
//         BattleGround* bg = source->GetBattleGround();
//         return bg && bg->GetBgTypeID(true) == BATTLEGROUND_SA && bg->ToBattleGroundSA()->AllowDefenseOfTheAncients(source);
//     }
// };

// enum ArgentTournamentAreas
// {
//     AREA_ARGENT_TOURNAMENT_FIELDS  = 4658,
//     AREA_RING_OF_ASPIRANTS         = 4670,
//     AREA_RING_OF_ARGENT_VALIANTS   = 4671,
//     AREA_RING_OF_ALLIANCE_VALIANTS = 4672,
//     AREA_RING_OF_HORDE_VALIANTS    = 4673,
//     AREA_RING_OF_CHAMPIONS         = 4669,
// };

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
    // new achievement_bg_ic_resource_glut();
    // new achievement_bg_ic_glaive_grave();
    // new achievement_bg_ic_mowed_down();
    // new achievement_bg_sa_artillery();
    new achievement_sickly_gazelle();
    new achievement_everything_counts();
    new achievement_bg_av_perfection();
    // new achievement_arena_by_type("achievement_arena_2v2_check", ARENA_TYPE_2v2);
    // new achievement_arena_by_type("achievement_arena_3v3_check", ARENA_TYPE_3v3);
    // new achievement_arena_by_type("achievement_arena_5v5_check", ARENA_TYPE_5v5);
    // new achievement_sa_defense_of_the_ancients();
    new achievement_tilted();
    new achievement_not_even_a_scratch();
    new achievement_killed_exp_or_honor_target();
}
