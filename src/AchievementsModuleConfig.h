#pragma once
#include "ModuleConfig.h"

namespace cmangos_module
{
    #define MAX_ACHIEVEMENT_LOCALE     16

    enum AchievementFaction
    {
        ACHIEVEMENT_FACTION_HORDE = 0,
        ACHIEVEMENT_FACTION_ALLIANCE = 1,
        ACHIEVEMENT_FACTION_ANY = -1,
    };

    enum AchievementFlags
    {
        ACHIEVEMENT_FLAG_COUNTER = 0x00000001,        // Just count statistic (never stop and complete)
        ACHIEVEMENT_FLAG_HIDDEN = 0x00000002,        // Not sent to client - internal use only
        ACHIEVEMENT_FLAG_STORE_MAX_VALUE = 0x00000004,        // Store only max value? used only in "Reach level xx"
        ACHIEVEMENT_FLAG_SUMM = 0x00000008,        // Use summ criteria value from all reqirements (and calculate max value)
        ACHIEVEMENT_FLAG_MAX_USED = 0x00000010,        // Show max criteria (and calculate max value ??)
        ACHIEVEMENT_FLAG_REQ_COUNT = 0x00000020,        // Use not zero req count (and calculate max value)
        ACHIEVEMENT_FLAG_AVERAGE = 0x00000040,        // Show as average value (value / time_in_days) depend from other flag (by def use last criteria value)
        ACHIEVEMENT_FLAG_BAR = 0x00000080,        // Show as progress bar (value / max vale) depend from other flag (by def use last criteria value)
        ACHIEVEMENT_FLAG_REALM_FIRST_REACH = 0x00000100,        //
        ACHIEVEMENT_FLAG_REALM_FIRST_KILL = 0x00000200,        //
    };

    #define MAX_CRITERIA_REQUIREMENTS 2

    enum AchievementCriteriaCondition
    {
        ACHIEVEMENT_CRITERIA_CONDITION_NONE = 0,
        ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH = 1,    // reset progress on death
        ACHIEVEMENT_CRITERIA_CONDITION_UNK1 = 2,    // only used in "Complete a daily quest every day for five consecutive days"
        ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP = 3,    // requires you to be on specific map, reset at change
        ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE = 4,    // only used in "Win 10 arenas without losing"
        ACHIEVEMENT_CRITERIA_CONDITION_NO_SPELL_HIT = 9,    // requires the player not to be hit by specific spell
        ACHIEVEMENT_CRITERIA_CONDITION_NOT_IN_GROUP = 10,   // requires the player not to be in group
        ACHIEVEMENT_CRITERIA_CONDITION_UNK3 = 13,   // unk
        ACHIEVEMENT_CRITERIA_CONDITION_TOTAL = 14
    };

    enum AchievementCriteriaFlags
    {
        ACHIEVEMENT_CRITERIA_FLAG_SHOW_PROGRESS_BAR = 0x00000001,         // Show progress as bar
        ACHIEVEMENT_CRITERIA_FLAG_HIDDEN = 0x00000002,         // Not show criteria in client
        ACHIEVEMENT_CRITERIA_FLAG_FAIL_ACHIEVEMENT = 0x00000004,         // BG related??
        ACHIEVEMENT_CRITERIA_FLAG_RESET_ON_START = 0x00000008,         //
        ACHIEVEMENT_CRITERIA_FLAG_IS_DATE = 0x00000010,         // not used
        ACHIEVEMENT_CRITERIA_FLAG_MONEY_COUNTER = 0x00000020          // Displays counter as money
    };

    enum AchievementCriteriaTimedTypes
    {
        ACHIEVEMENT_TIMED_TYPE_EVENT = 1,    // Timer is started by internal event with id in timerStartEvent
        ACHIEVEMENT_TIMED_TYPE_QUEST = 2,    // Timer is started by accepting quest with entry in timerStartEvent
        ACHIEVEMENT_TIMED_TYPE_SPELL_CASTER = 5,    // Timer is started by casting a spell with entry in timerStartEvent
        ACHIEVEMENT_TIMED_TYPE_SPELL_TARGET = 6,    // Timer is started by being target of spell with entry in timerStartEvent
        ACHIEVEMENT_TIMED_TYPE_CREATURE = 7,    // Timer is started by killing creature with entry in timerStartEvent
        ACHIEVEMENT_TIMED_TYPE_ITEM = 9,    // Timer is started by using item with entry in timerStartEvent

        ACHIEVEMENT_TIMED_TYPE_MAX,
    };

    enum AchievementCriteriaAssetsIds
    {
        ACHIEVEMENT_CRITERIA_ASSET_ID_BG_WS_OBJECTIVE_CAPTURE_FLAG = 42,
        ACHIEVEMENT_CRITERIA_ASSET_ID_BG_WS_OBJECTIVE_RETURN_FLAG = 44
    };

    enum AchievementCriteriaTimedAssetsIds
    {
        ACHIEVEMENT_CRITERIA_TIMED_ASSET_ID_BG_WS_EVENT_START_BATTLE = 8563,
        ACHIEVEMENT_CRITERIA_TIMED_ASSET_ID_BG_WS_SPELL_SILVERWING_FLAG_PICKED = 61265, // fake spell, does not exist but used as timer start event
        ACHIEVEMENT_CRITERIA_TIMED_ASSET_ID_BG_WS_SPELL_WARSONG_FLAG_PICKED = 61266  // fake spell, does not exist but used as timer start event
    };

    enum AchievementCriteriaTypes
    {
        ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE = 0,
        ACHIEVEMENT_CRITERIA_TYPE_WIN_BG = 1,
        ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL = 5,
        ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL = 7,
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT = 8,
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT = 9,
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10, // you have to complete a daily quest x times in a row
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11,
        ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE = 13,
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST = 14,
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND = 15,
        ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP = 16,
        ACHIEVEMENT_CRITERIA_TYPE_DEATH = 17,
        ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON = 18,
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID = 19,
        ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE = 20,
        ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER = 23,
        ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING = 24,
        ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM = 26,
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST = 27,
        ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET = 28,
        ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL = 29,
        ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE = 30,
        ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31,
        ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA = 32,
        ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA = 33,
        ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL = 34,
        ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL = 35,
        ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM = 36,
        ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA = 37, // TODO: the archievements 1162 and 1163 requires a special rating which can't be found in the dbc
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING = 38,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING = 39,
        ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL = 40,
        ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM = 41,
        ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM = 42,
        ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA = 43,
        ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK = 44,
        ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT = 45,
        ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION = 46,
        ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION = 47,
        ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP = 48, // note: rewarded as soon as the player payed, not at taking place at the seat
        ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM = 49,
        ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT = 50, // TODO: itemlevel is mentioned in text but not present in dbc
        ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT = 51,
        ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS = 52,
        ACHIEVEMENT_CRITERIA_TYPE_HK_RACE = 53,
        ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE = 54,
        ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE = 55,
        ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS = 56, // TODO: in some cases map not present, and in some cases need do without die
        ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM = 57,
        ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS = 59,
        ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS = 60,
        ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS = 61,
        ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD = 62,
        ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING = 63,
        ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER = 65,
        ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL = 66,
        ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY = 67,
        ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT = 68,
        ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2 = 69,
        ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL = 70,
        ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT = 72,
        ACHIEVEMENT_CRITERIA_TYPE_ON_LOGIN = 74, // TODO: title id is not mentioned in dbc
        ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS = 75,
        ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL = 76,
        ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL = 77,
        ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE = 78, // TODO: creature type (demon, undead etc.) is not stored in dbc
        ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS = 80,
        ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION = 82,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID = 83,
        ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS = 84,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD = 85,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED = 86,
        ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION = 87,
        ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION = 88,
        ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS = 89,
        ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM = 90,
        ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM = 91,
        ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED = 93,
        ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED = 94,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALTH = 95,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER = 96,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT = 97,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER = 98,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_ARMOR = 99,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING = 100,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT = 101,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED = 102,
        ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED = 103,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED = 104,
        ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED = 105,
        ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED = 106,
        ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED = 107,
        ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN = 108,
        ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE = 109,
        ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2 = 110, // TODO: target entry is missing
        ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE = 112,
        ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL = 113,
        ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS = 114,
        ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS = 115,
        ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS = 119,
        ACHIEVEMENT_CRITERIA_TYPE_JOINED_GROUP = 120,
        ACHIEVEMENT_CRITERIA_TYPE_MAIL_ITEMS = 121,
        ACHIEVEMENT_CRITERIA_TYPE_MAIL_GOLD = 122,
        ACHIEVEMENT_CRITERIA_TYPE_TRADES_DONE = 123,
        ACHIEVEMENT_CRITERIA_TYPE_NON_CRAFTED_ITEMS_USED = 124,
        ACHIEVEMENT_CRITERIA_TYPE_TOTAL = 125, // 0..124 => 125 criteria types total
    };

    enum AchievementCategory
    {
        CATEGORY_CHILDRENS_WEEK = 163,
    };

    enum AchievementCriteriaDataType
    {
                                                                    // value1         value2        comment
        ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE = 0,                    // 0              0
        ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE = 1,              // creature_id    0
        ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE = 2,     // class_id       race_id
        ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH = 3,    // health_percent 0
        ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_DEAD = 4,           // own_team       0             not corpse (not released body), own_team == false if enemy team expected
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA = 5,                  // spell_id       effect_idx
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AREA = 6,                  // area id        0
        ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA = 7,                  // spell_id       effect_idx
        ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE = 8,                   // minvalue       0             value provided with achievement update must be not less that limit
        ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL = 9,                 // minlevel       0             minlevel of target
        ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER = 10,               // gender         0             0=male; 1=female
        ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT = 11,                 // 0              0             scripted requirement
        ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_DIFFICULTY = 12,         // difficulty     0             normal/heroic difficulty for current event map
        ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT = 13,       // count          0             "with less than %u people in the zone"
        ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM = 14,                 // team           0             HORDE(67), ALLIANCE(469)
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK = 15,                // drunken_state  0             (enum DrunkenState) of player
        ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY = 16,                // holiday_id     0             event in holiday time
        ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE = 17,     // min_score      max_score     player's team win bg and opposition team have team score in range
        ACHIEVEMENT_CRITERIA_DATA_TYPE_INSTANCE_SCRIPT = 18,        // 0              0             maker instance script call for check current criteria requirements fit
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM = 19,         // item_level     item_quality  for equipped item in slot to check item level and quality
        ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_ID = 20,                 // map_id         0             player must be on map with id in map_id
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE = 21,    // class_id       race_id
        ACHIEVEMENT_CRITERIA_DATA_TYPE_NTH_BIRTHDAY = 22,           // N              0             login on day of N-th Birthday
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_KNOWN_TITLE = 23,          // title_id       0             known (pvp) title, values from dbc
        ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_TEAMS_SCORES = 24,        // winner_score   loser score   player's team win bg and their teams have exact scores
        ACHIEVEMENT_CRITERIA_DATA_TYPE_NO_DEATH = 25,               // 0              0             must achieve criteria without ever dying
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_LEVEL = 26,                // max_level      0             max_level of player
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYED_TIME = 27,          // max_playtime   0             max_playtime of player
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_SELF_FOUND = 28,           // 0              0             must achieve criteria without grouping, trading, using mailbox or auction house
        ACHIEVEMENT_CRITERIA_DATA_TYPE_S_SELF_CRAFTER = 29          // 0              0             must achieve criteria by only using items crafted by the player           
    };

    #define MAX_ACHIEVEMENT_CRITERIA_DATA_TYPE               30     // maximum value in AchievementCriteriaDataType enum

    enum AchievementCommonCategories
    {
        ACHIEVEMENT_CATEOGRY_GENERAL = -1,
        ACHIEVEMENT_CATEGORY_STATISTICS = 1
    };

    class AchievementsModuleConfig : public ModuleConfig
    {
    public:
        AchievementsModuleConfig();
        bool OnLoad();

    public:
        bool enabled;
        bool sendMessage;
        bool sendAddon;
        bool sendVisual;
        bool randomBots;
        bool randomBotsRealmFirst;
        bool accountAchievements;
        uint32 effectId;
    };
}