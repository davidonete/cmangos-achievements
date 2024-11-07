#ifndef CMANGOS_MODULE_ACHIEVEMENTS_H
#define CMANGOS_MODULE_ACHIEVEMENTS_H

#include "Module.h"
#include "AchievementsModuleConfig.h"

#include "Database/SQLStorage.h"

#include <map>
#include <unordered_map>
#include <string>
#include <chrono>
#include <array>

namespace cmangos_module
{
    class AchievementsModule;
    class PlayerAchievementMgr;
    class AchievementScriptMgr;

    struct AchievementEntry
    {
        uint32 ID;                                           // 0
        int32 requiredFaction;                               // 1 -1=all, 0=horde, 1=alliance
        int32 mapID;                                         // 2 -1=none
        uint32 parentAchievement;                            // 3 its Achievement parent (can`t start while parent uncomplete, use its Criteria if don`t have own, use its progress on begin)
        std::string name[16];                                // 4-19
        uint32 name_flags;                                   // 20
        std::string description[16];                         // 21-36
        uint32 desc_flags;                                   // 37
        uint32 categoryId;                                   // 38
        uint32 points;                                       // 39 reward points
        uint32 OrderInCategory;                              // 40
        uint32 flags;                                        // 41
        uint32 icon;                                         // 42 icon (from SpellIcon.dbc)
        std::string titleReward[16];                         // 43-58
        uint32 titleReward_flags;                            // 59
        uint32 count;                                        // 60 - need this count of completed criterias (own or referenced achievement criterias)
        uint32 refAchievement;                               // 61 - referenced achievement (counting of all completed criterias)
        uint32 patch;

    public:
        const std::string& GetName(uint32 locale) const;
        const std::string& GetDescription(uint32 locale) const;
        const std::string& GetTitleReward(uint32 locale) const;
    };

    struct AchievementCategoryEntry
    {
        int32 ID;                                            // 0
        int32 parentCategory;                                // 1 -1 for main category
        std::string name[16];                                // 2-17
        uint32 name_flags;                                   // 18
        uint32 sortOrder;                                    // 19
        uint32 patch;

    public:
        const std::string& GetName(uint32 locale) const;
    };

    struct AchievementCriteriaEntry
    {
        uint32  ID;                                             // 0
        uint32  referredAchievement;                            // 1
        uint32  requiredType;                                   // 2
        union
        {
            // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE          = 0
            // TODO: also used for player deaths..
            struct
            {
                uint32  creatureID;                             // 3
                uint32  creatureCount;                          // 4
            } kill_creature;

            // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                 = 1
            struct
            {
                uint32  bgMapID;                                // 3
                uint32  winCount;                               // 4
            } win_bg;

            // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL            = 5
            struct
            {
                uint32  unused;                                 // 3
                uint32  level;                                  // 4
            } reach_level;

            // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL      = 7
            struct
            {
                uint32  skillID;                                // 3
                uint32  skillLevel;                             // 4
            } reach_skill_level;

            // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT   = 8
            struct
            {
                uint32  linkedAchievement;                      // 3
            } complete_achievement;

            // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT   = 9
            struct
            {
                uint32  unused;                                 // 3
                uint32  totalQuestCount;                        // 4
            } complete_quest_count;

            // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
            struct
            {
                uint32  unused;                                 // 3
                uint32  numberOfDays;                           // 4
            } complete_daily_quest_daily;

            // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
            struct
            {
                uint32  zoneID;                                 // 3
                uint32  questCount;                             // 4
            } complete_quests_in_zone;

            // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST   = 14
            struct
            {
                uint32  unused;                                 // 3
                uint32  questCount;                             // 4
            } complete_daily_quest;

            // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND  = 15
            struct
            {
                uint32  mapID;                                  // 3
            } complete_battleground;

            // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP           = 16
            struct
            {
                uint32  mapID;                                  // 3
            } death_at_map;

            // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON       = 18
            struct
            {
                uint32  manLimit;                               // 3
            } death_in_dungeon;

            // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID          = 19
            struct
            {
                uint32  groupSize;                              // 3 can be 5, 10 or 25
            } complete_raid;

            // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE     = 20
            struct
            {
                uint32  creatureEntry;                          // 3
            } killed_by_creature;

            // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING     = 24
            struct
            {
                uint32  unused;                                 // 3
                uint32  fallHeight;                             // 4
            } fall_without_dying;

            // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM            = 26
            struct
            {
                uint32 type;                                    // 3, see enum EnviromentalDamage
            } death_from;

            // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST         = 27
            struct
            {
                uint32  questID;                                // 3
                uint32  questCount;                             // 4
            } complete_quest;

            // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET        = 28
            // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2       = 69
            struct
            {
                uint32  spellID;                                // 3
                uint32  spellCount;                             // 4
            } be_spell_target;

            // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL             = 29
            // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2            = 110
            struct
            {
                uint32  spellID;                                // 3
                uint32  castCount;                              // 4
            } cast_spell;

            // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE
            struct
            {
                uint32 objectiveId;                             // 3
                uint32 completeCount;                           // 4
            } bg_objective;

            // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
            struct
            {
                uint32  areaID;                                 // 3 Reference to AreaTable.dbc
                uint32  killCount;                              // 4
            } honorable_kill_at_area;

            // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA              = 32
            struct
            {
                uint32 mapID;                                   // 3 Reference to Map.dbc
                uint32 count;                                   // 4 Number of times that the arena must be won.
            } win_arena;

            // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA             = 33
            struct
            {
                uint32  mapID;                                  // 3 Reference to Map.dbc
            } play_arena;

            // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL            = 34
            struct
            {
                uint32  spellID;                                // 3 Reference to Map.dbc
            } learn_spell;

            // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM               = 36
            struct
            {
                uint32  itemID;                                 // 3
                uint32  itemCount;                              // 4
            } own_item;

            // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA        = 37
            struct
            {
                uint32  unused;                                 // 3
                uint32  count;                                  // 4
            } win_rated_arena;

            // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING    = 38
            struct
            {
                uint32  teamtype;                               // 3 {2, 3, 5}
            } highest_team_rating;

            // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING      = 39
            struct
            {
                uint32  teamtype;                               // 3 {2, 3, 5}
                uint32  PersonalRating;                         // 4
            } highest_personal_rating;

            // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL      = 40
            struct
            {
                uint32  skillID;                                // 3
                uint32  skillLevel;                             // 4 apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
            } learn_skill_level;

            // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM               = 41
            struct
            {
                uint32  itemID;                                 // 3
                uint32  itemCount;                              // 4
            } use_item;

            // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM              = 42
            struct
            {
                uint32  itemID;                                 // 3
                uint32  itemCount;                              // 4
            } loot_item;

            // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA           = 43
            struct
            {
                uint32  areaReference;                          // 3
            } explore_area;

            // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK               = 44
            struct
            {
                // TODO: This rank is _NOT_ the index from CharTitles.dbc
                uint32  rank;                                   // 3
            } own_rank;

            // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT          = 45
            struct
            {
                uint32  unused;                                 // 3
                uint32  numberOfSlots;                          // 4
            } buy_bank_slot;

            // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION        = 46
            struct
            {
                uint32  factionID;                              // 3
                uint32  reputationAmount;                       // 4 Total reputation amount, so 42000 = exalted
            } gain_reputation;

            // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
            struct
            {
                uint32  unused;                                 // 3
                uint32  numberOfExaltedFactions;                // 4
            } gain_exalted_reputation;

            // ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP      = 48
            struct
            {
                uint32 unused;                                  // 3
                uint32 numberOfVisits;                          // 4
            } visit_barber;

            // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM        = 49
            // TODO: where is the required itemlevel stored?
            struct
            {
                uint32  itemSlot;                               // 3
                uint32  count;                                  // 4
            } equip_epic_item;

            // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT      = 50
            struct
            {
                uint32  rollValue;                              // 3
                uint32  count;                                  // 4
            } roll_need_on_loot;
            // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT      = 51
            struct
            {
                uint32  rollValue;                              // 3
                uint32  count;                                  // 4
            } roll_greed_on_loot;

            // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS               = 52
            struct
            {
                uint32  classID;                                // 3
                uint32  count;                                  // 4
            } hk_class;

            // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                = 53
            struct
            {
                uint32  raceID;                                 // 3
                uint32  count;                                  // 4
            } hk_race;

            // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE               = 54
            // TODO: where is the information about the target stored?
            struct
            {
                uint32  emoteID;                                // 3 enum TextEmotes
                uint32  count;                                  // 4 count of emotes, always required special target or requirements
            } do_emote;

            // ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE            = 13
            // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE           = 55
            struct
            {
                uint32  unused;                                 // 3
                uint32  count;                                  // 4
            } healing_done;

            // ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS      = 56
            struct
            {
                uint32  unused;
                uint32  killCount;
            } get_killing_blow;

            // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM             = 57
            struct
            {
                uint32  itemID;                                 // 3
                uint32  count;                                  // 4
            } equip_item;

            // ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD= 62
            struct
            {
                uint32  unused;                                 // 3
                uint32  goldInCopper;                           // 4
            } quest_reward_money;

            // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY             = 67
            struct
            {
                uint32  unused;                                 // 3
                uint32  goldInCopper;                           // 4
            } loot_money;

            // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT         = 68
            struct
            {
                uint32  goEntry;                                // 3
                uint32  useCount;                               // 4
            } use_gameobject;

            // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL       = 70
            // TODO: are those special criteria stored in the dbc or do we have to add another sql table?
            struct
            {
                uint32  unused;                                 // 3
                uint32  killCount;                              // 4
            } special_pvp_kill;

            // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT     = 72
            struct
            {
                uint32  goEntry;                                // 3
                uint32  lootCount;                              // 4
            } fish_in_gameobject;

            // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS = 75
            struct
            {
                uint32  skillLine;                              // 3
                uint32  spellCount;                             // 4
            } learn_skillline_spell;

            // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL               = 76
            struct
            {
                uint32  unused;                                 // 3
                uint32  duelCount;                              // 4
            } win_duel;

            // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER          = 96
            struct
            {
                uint32  powerType;                              // 3 mana=0, 1=rage, 3=energy, 6=runic power
            } highest_power;

            // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT           = 97
            struct
            {
                uint32  statType;                               // 3 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
            } highest_stat;

            // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER     = 98
            struct
            {
                uint32  spellSchool;                            // 3
            } highest_spellpower;

            // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING         = 100
            struct
            {
                uint32  ratingType;                             // 3
            } highest_rating;

            // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE              = 109
            struct
            {
                uint32  lootType;                               // 3 3=fishing, 2=pickpocket, 4=disentchant
                uint32  lootTypeCount;                          // 4
            } loot_type;

            // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE       = 112
            struct
            {
                uint32  skillLine;                              // 3
                uint32  spellCount;                             // 4
            } learn_skill_line;

            // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL    = 113
            struct
            {
                uint32  unused;                                 // 3
                uint32  killCount;                              // 4
            } honorable_kill;

            struct
            {
                uint32  unused;
                uint32  dungeonsComplete;
            } use_lfg;

            struct
            {
                uint32  field3;                                 // 3 main requirement
                uint32  count;                                  // 4 main requirement count
            } raw;
        };

        struct
        {
            uint32  additionalRequirement_type;
            uint32  additionalRequirement_value;
        } additionalRequirements[MAX_CRITERIA_REQUIREMENTS];

        std::string name[16];                                   // 9-24
        uint32  name_flags;                                     // 25
        uint32  flags;                                          // 26
        uint32  timedType;                                      // 27
        uint32  timerStartEvent;                                // 28 Alway appears with timed events
        // for timed spells it is spell id for
        // timed kills it is creature id
        uint32  timeLimit;                                      // 29 time limit in seconds
        uint32 showOrder;                                       // 30 show order

    public:
        const std::string& GetName(uint32 locale) const;
    };

    typedef std::list<AchievementCriteriaEntry const*> AchievementCriteriaEntryList;
    typedef std::list<AchievementEntry const*>         AchievementEntryList;

    typedef std::unordered_map<uint32, AchievementCriteriaEntryList> AchievementCriteriaListByAchievement;
    typedef std::map<uint32, AchievementEntryList>         AchievementListByReferencedId;

    struct CriteriaProgress
    {
        uint32 counter;
        time_t date;                                            // latest update time.
        bool changed;
    };

    struct AchievementCriteriaData
    {
        AchievementCriteriaDataType dataType{ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE};
        union
        {
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE              = 0 (no data)
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE        = 1
            struct
            {
                uint32 id;
            } creature;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE = 2
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE = 21
            struct
            {
                uint32 class_id;
                uint32 race_id;
            } classRace;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH = 3
            struct
            {
                uint32 percent;
            } health;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_DEAD     = 4
            struct
            {
                uint32 own_team_flag;
            } player_dead;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA            = 5
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA            = 7
            struct
            {
                uint32 spell_id;
                uint32 effect_idx;
            } aura;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AREA            = 6
            struct
            {
                uint32 id;
            } area;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE             = 8
            struct
            {
                uint32 value;
                uint32 compType;
            } value;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL           = 9
            struct
            {
                uint32 minlevel;
            } level;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER          = 10
            struct
            {
                uint32 gender;
            } gender;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT            = 11 (no data)
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_DIFFICULTY    = 12
            struct
            {
                uint32 difficulty;
            } difficulty;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT  = 13
            struct
            {
                uint32 maxcount;
            } map_players;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM            = 14
            struct
            {
                uint32 team;
            } team;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK           = 15
            struct
            {
                uint32 state;
            } drunk;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY           = 16
            struct
            {
                uint32 id;
            } holiday;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE= 17
            struct
            {
                uint32 min_score;
                uint32 max_score;
            } bg_loss_team_score;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_INSTANCE_SCRIPT   = 18 (no data)
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM    = 19
            struct
            {
                uint32 item_level;
                uint32 item_quality;
            } equipped_item;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_ID            = 20
            struct
            {
                uint32 mapId;
            } map_id;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_NTH_BIRTHDAY      = 22
            struct
            {
                uint32 nth_birthday;
            } birthday_login;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_KNOWN_TITLE       = 23
            struct
            {
                uint32 title_id;
            } known_title;
            // ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_TEAMS_SCORES   = 24
            struct
            {
                uint32 winner_score;
                uint32 loser_score;
            } teams_scores;
            // ...
            struct
            {
                uint32 value1;
                uint32 value2;
            } raw;
        };
        uint32 ScriptId;

        AchievementCriteriaData()
        {
            raw.value1 = 0;
            raw.value2 = 0;
            ScriptId = 0;
        }

        AchievementCriteriaData(uint32 _dataType, uint32 _value1, uint32 _value2, uint32 _scriptId) : dataType(AchievementCriteriaDataType(_dataType))
        {
            raw.value1 = _value1;
            raw.value2 = _value2;
            ScriptId = _scriptId;
        }

        bool IsValid(AchievementCriteriaEntry const* criteria);
        bool Meets(uint32 criteria_id, const PlayerAchievementMgr* playerAchievementMgr, const Unit* target, uint32 miscvalue1 = 0) const;
    };

    struct AchievementCriteriaDataSet
    {
        AchievementCriteriaDataSet()  {}
        typedef std::vector<AchievementCriteriaData> Storage;
        void Add(AchievementCriteriaData const& data) { storage.push_back(data); }
        bool Meets(const PlayerAchievementMgr* playerAchievementMgr, const Unit* target, uint32 miscvalue = 0) const;
        void SetCriteriaId(uint32 id) { criteria_id = id; }
        const Storage& GetStorage() const { return storage; }

    private:
        uint32 criteria_id{0};
        Storage storage;
    };

    typedef std::map<uint32, AchievementCriteriaDataSet> AchievementCriteriaDataMap;

    struct AchievementReward
    {
        uint32 titleId[2];
        uint32 itemId;
        uint32 sender;
        std::string subject;
        std::string text;
        uint32 mailTemplate;
    };

    typedef std::map<uint32, AchievementReward> AchievementRewards;

    struct AchievementRewardLocale
    {
        std::vector<std::string> Subject;
        std::vector<std::string> Text;
    };

    typedef std::map<uint32, AchievementRewardLocale> AchievementRewardLocales;

    struct CompletedAchievementData
    {
        time_t date;
        bool changed;
    };

    typedef std::unordered_map<uint32, CriteriaProgress> CriteriaProgressMap;
    typedef std::unordered_map<uint32, CompletedAchievementData> CompletedAchievementMap;

    class PlayerAchievementMgr
    {
        friend struct AchievementCriteriaData;

    public:
        explicit PlayerAchievementMgr(Player* player, AchievementsModule* module);
        ~PlayerAchievementMgr();

        void Reset();
        static void DeleteFromDB(uint32 playerId);
        void LoadFromDB(uint32 playerId);
        void SaveToDB();
        void ResetAchievementCriteria(AchievementCriteriaCondition condition, uint32 value, bool evenIfCriteriaComplete = false);
        void UpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 = 0, uint32 miscValue2 = 0, Unit* unit = nullptr);
        void CompletedAchievement(AchievementEntry const* entry);
        void CheckAllAchievementCriteria();
        void SendAllAchievementData() const;
        void SendRespondInspectAchievements(Player* player) const;
        [[nodiscard]] bool HasAchieved(uint32 achievementId) const;
        Player* GetPlayer() const { return m_player; }
        void UpdateTimedAchievements(uint32 timeDiff);
        void StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry, uint32 timeLost = 0);
        void RemoveTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);   // used for quest and scripted timed achievements

        void RemoveCriteriaProgress(AchievementCriteriaEntry const* entry);

        bool IsAddonEnabled() const { return m_addonEnabled; }
        void EnableAddon(uint32 version);
        uint32 GetVersion() { return m_version; }

        bool AddAchievement(uint32 achievementId, bool assert = true);
        bool AddAchievement(const AchievementEntry* entry);
        bool RemoveAchievement(const AchievementEntry* entry);
        const AchievementsModule* GetModule() const { return m_module; }

    private:
        enum ProgressType { PROGRESS_SET, PROGRESS_ACCUMULATE, PROGRESS_HIGHEST, PROGRESS_RESET };
        void SendAchievementEarned(AchievementEntry const* achievement) const;
        void SendCriteriaUpdate(AchievementCriteriaEntry const* entry, CriteriaProgress const* progress, uint32 timeElapsed, bool timedCompleted) const;
        CriteriaProgress* GetCriteriaProgress(AchievementCriteriaEntry const* entry);
        const CriteriaProgress* GetCriteriaProgress(AchievementCriteriaEntry const* entry) const;
        void SetCriteriaProgress(AchievementCriteriaEntry const* entry, uint32 changeValue, ProgressType ptype = PROGRESS_SET);
        void CompletedCriteriaFor(AchievementEntry const* achievement);
        bool IsCompletedCriteria(AchievementCriteriaEntry const* achievementCriteria, AchievementEntry const* achievement);
        bool IsCompletedAchievement(AchievementEntry const* entry);
        bool CanUpdateCriteria(AchievementCriteriaEntry const* criteria, AchievementEntry const* achievement);

        uint8 GetPlayerLocale() const;
        void SyncAccountAcchievements();

        Player* m_player;
        CriteriaProgressMap m_criteriaProgress;
        CompletedAchievementMap m_completedAchievements;
        typedef std::map<uint32, uint32> TimedAchievementMap;
        TimedAchievementMap m_timedAchievements;      // Criteria id/time left in MS

        bool m_addonEnabled;
        uint32 m_version;
        AchievementsModule* m_module;
    };

    class AchievementsModule : public Module
    {
        friend class PlayerAchievementMgr;
        friend class AchievementScriptMgr;
        friend struct AchievementCriteriaData;

    public:
        AchievementsModule();
        const AchievementsModuleConfig* GetConfig() const override;

        // Module Hooks
        void OnInitialize() override;
        void OnUpdate(uint32 elapsed) override;

        // Player Hooks
        void OnPreCharacterCreated(Player* player) override;
        void OnPreLoadFromDB(Player* player) override;
        void OnLogOut(Player* player) override;
        void OnLoadFromDB(Player* player) override;
        void OnDeleteFromDB(uint32 playerId) override;
        void OnSaveToDB(Player* player) override;
        void OnAddSpell(Player* player, uint32 spellId) override;
        void OnDuelComplete(Player* player, Player* opponent, uint8 duelCompleteType) override;
        void OnKilledMonsterCredit(Player* player, uint32 entry, ObjectGuid& guid) override;
        void OnRewardPlayerAtKill(Player* player, Unit* victim) override;
        void OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32 damage) override;
        void OnResetTalents(Player* player, uint32 cost) override;
        void OnStoreItem(Player* player, Item* item) override;
        void OnMoveItemToInventory(Player* player, Item* item) override;
        void OnDeath(Player* player, Unit* killer) override;
        void OnDeath(Player* player, uint8 environmentalDamageType) override;
        bool OnHandlePageTextQuery(Player* player, const WorldPacket& packet) override;
        void OnUpdateSkill(Player* player, uint16 skillId) override;
        void OnRewardHonor(Player* player, Unit* victim) override;
        void OnEquipItem(Player* player, Item* item);
        void OnRewardQuest(Player* player, const Quest* quest);
        void OnTaxiFlightRouteStart(Player* player, const Taxi::Tracker& taxiTracker, bool initial) override;
        void OnTaxiFlightRouteEnd(Player* player, const Taxi::Tracker& taxiTracker, bool final) override;
        void OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental);
        void OnEmote(Player* player, Unit* target, uint32 emote) override;
        void OnBuyBankSlot(Player* player, uint32 slot, uint32 price) override;
        void OnSellItem(Player* player, Item* item, uint32 money) override;
        void OnBuyBackItem(Player* player, Item* item, uint32 money) override;
        void OnModifyMoney(Player* player, int32 diff) override;
        void OnSummoned(Player* player, const ObjectGuid& summoner) override;
        void OnAreaExplored(Player* player, uint32 areaId) override;
        void OnUpdateHonor(Player* player) override;
        void OnGiveLevel(Player* player, uint32 level) override;
        void OnSendMail(Player* player, const ObjectGuid& receiver, uint32 cost) override;
        void OnAbandonQuest(Player* player, uint32 questId) override;

        // Battleground Hooks
        void OnStartBattleGround(BattleGround* battleground) override;
        void OnEndBattleGround(BattleGround* battleground, uint32 winnerTeam) override;
        void OnUpdatePlayerScore(BattleGround* battleground, Player* player, uint8 scoreType, uint32 value) override;
        void OnLeaveBattleGround(BattleGround* battleground, Player* player) override;
        void OnJoinBattleGround(BattleGround* battleground, Player* player) override;
        void OnPickUpFlag(BattleGroundWS* battleground, Player* player, uint32 team) override;

        // Game Object Hooks
        bool OnUse(GameObject* gameObject, Unit* user) override;

        // Unit Hooks
        void OnDealDamage(Unit* unit, Unit* victim, uint32 health, uint32 damage) override;
        void OnKill(Unit* unit, Unit* victim) override;
        void OnDealHeal(Unit* dealer, Unit* victim, int32 gain, uint32 addHealth) override;

        // Spell Hooks
        void OnHit(Spell* spell, Unit* caster, Unit* target) override;
        void OnCast(Spell* spell, Unit* caster, Unit* target) override;

        // Loot Hooks
        void OnHandleLootMasterGive(Loot* loot, Player* target, LootItem* lootItem) override;
        void OnPlayerRoll(Loot* loot, Player* player, uint32 itemSlot, uint8 rollType) override;
        void OnPlayerWinRoll(Loot* loot, Player* player, uint8 rollType, uint8 rollAmount, uint32 itemSlot, uint8 inventoryResult) override;
        void OnSendGold(Loot* loot, Player* player, uint32 gold, uint8 lootMethod) override;

        // Auction House Hooks
        void OnSellItem(AuctionEntry* auctionEntry, Player* player) override;
        void OnUpdateBid(AuctionEntry* auctionEntry, Player* player, uint32 newBid) override;

        // Player Dump Hooks
        void OnWriteDump(uint32 playerId, std::string& dump) override;
        bool IsModuleDumpTable(const std::string& dbTableName) override;

        // Commands
        std::vector<ModuleChatCommand>* GetCommandTable() override;
        const char* GetChatCommandPrefix() const override { return "achievements"; }
        bool HandleEnableAchiever(WorldSession* session, const std::string& args);
        bool HandleGetCategories(WorldSession* session, const std::string& args);
        bool HandleGetAchievements(WorldSession* session, const std::string& args);
        bool HandleGetCriteria(WorldSession* session, const std::string& args);
        bool HandleGetCharacterCriteria(WorldSession* session, const std::string& args);
        bool HandleAddAchievement(WorldSession* session, const std::string& args);
        bool HandleRemoveAchievement(WorldSession* session, const std::string& args);

    private:
        static uint32 GetCurrentPatch();
        static uint32 GetCurrentVersion() { return 1U; }

        bool IsStatisticCriteria(AchievementCriteriaEntry const* achievementCriteria) const;
        bool isStatisticAchievement(AchievementEntry const* achievement) const;

        void GetAllCategories(Player* player, uint32 version) const;
        void GetAllAchievements(Player* player, uint32 version) const;
        void GetAllCriteria(Player* player, uint32 version) const;

        void GetCharacterCriteria(Player* player) const;
        void GetCharacterAchievements(Player* player) const;

        bool HasAddon(Player* player) const;
        void EnableAddon(Player* player, uint32 version);

        bool AddAchievement(Player* player, uint32 achievementId);
        bool RemoveAchievement(Player* player, uint32 achievementId);

        [[nodiscard]] AchievementCriteriaEntryList const* GetAchievementCriteriaByType(AchievementCriteriaTypes type) const
        {
            return &m_achievementCriteriasByType[type];
        }

        AchievementCriteriaEntryList const* GetSpecialAchievementCriteriaByType(AchievementCriteriaTypes type, uint32 val)
        {
            if (m_specialList[type].find(val) != m_specialList[type].end())
                return &m_specialList[type][val];
            return nullptr;
        }

        AchievementCriteriaEntryList const* GetAchievementCriteriaByCondition(AchievementCriteriaCondition condition, uint32 val)
        {
            if (m_achievementCriteriasByCondition[condition].find(val) != m_achievementCriteriasByCondition[condition].end())
                return &m_achievementCriteriasByCondition[condition][val];
            return nullptr;
        }

        [[nodiscard]] AchievementCriteriaEntryList const& GetTimedAchievementCriteriaByType(AchievementCriteriaTimedTypes type) const
        {
            return m_achievementCriteriasByTimedType[type];
        }

        [[nodiscard]] AchievementCriteriaEntryList const* GetAchievementCriteriaByAchievement(uint32 id) const
        {
            AchievementCriteriaListByAchievement::const_iterator itr = m_achievementCriteriaListByAchievement.find(id);
            return itr != m_achievementCriteriaListByAchievement.end() ? &itr->second : nullptr;
        }

        [[nodiscard]] AchievementEntryList const* GetAchievementByReferencedId(uint32 id) const
        {
            AchievementListByReferencedId::const_iterator itr = m_achievementListByReferencedId.find(id);
            return itr != m_achievementListByReferencedId.end() ? &itr->second : nullptr;
        }

        AchievementReward const* GetAchievementReward(AchievementEntry const* achievement) const
        {
            AchievementRewards::const_iterator iter = m_achievementRewards.find(achievement->ID);
            return iter != m_achievementRewards.end() ? &iter->second : nullptr;
        }

        AchievementRewardLocale const* GetAchievementRewardLocale(AchievementEntry const* achievement) const
        {
            AchievementRewardLocales::const_iterator iter = m_achievementRewardLocales.find(achievement->ID);
            return iter != m_achievementRewardLocales.end() ? &iter->second : nullptr;
        }

        AchievementCriteriaDataSet const* GetCriteriaDataSet(AchievementCriteriaEntry const* achievementCriteria) const
        {
            AchievementCriteriaDataMap::const_iterator iter = m_criteriaDataMap.find(achievementCriteria->ID);
            return iter != m_criteriaDataMap.end() ? &iter->second : nullptr;
        }

        bool IsRealmCompleted(AchievementEntry const* achievement) const;
        void SetRealmCompleted(AchievementEntry const* achievement);

        [[nodiscard]] const AchievementEntry* GetAchievement(uint32 achievementId, bool assert = true) const;
        [[nodiscard]] const AchievementCriteriaEntry* GetAchievementCriteria(uint32 criteriaId, bool assert = true) const;
        [[nodiscard]] const AchievementCategoryEntry* GetAchievementCategory(uint32 categoryId) const;

        void LoadAchievementCriteriaList();
        void LoadAchievementCriteriaData();
        void LoadAchievementList();
        void LoadAchievementCategories();
        void LoadCompletedAchievements();
        void LoadRewards();
        void LoadRewardLocales();

        uint8 GetPlayerLocale(WorldSession* session) const;
        PlayerAchievementMgr* GetPlayerAchievementMgr(Player* player);
        const PlayerAchievementMgr* GetPlayerAchievementMgr(const Player* player) const;

    private:
        std::unordered_map<uint32, AchievementEntry> m_achievements;
        std::unordered_map<uint32, AchievementCategoryEntry> m_achievementCategories;
        std::unordered_map<uint32, AchievementCriteriaEntry> m_achievementCriterias;
        AchievementCriteriaDataMap m_criteriaDataMap;

        // store achievement criterias by type to speed up lookup
        AchievementCriteriaEntryList m_achievementCriteriasByType[ACHIEVEMENT_CRITERIA_TYPE_TOTAL];
        AchievementCriteriaEntryList m_achievementCriteriasByTimedType[ACHIEVEMENT_TIMED_TYPE_MAX];
        // store achievement criterias by achievement to speed up lookup
        AchievementCriteriaListByAchievement m_achievementCriteriaListByAchievement;
        // store achievements by referenced achievement id to speed up lookup
        AchievementListByReferencedId m_achievementListByReferencedId;

        typedef std::unordered_map<uint32 /*achievementId*/, std::chrono::system_clock::time_point /*completionTime*/> AllCompletedAchievements;
        AllCompletedAchievements m_allCompletedAchievements;

        AchievementRewards m_achievementRewards;
        AchievementRewardLocales m_achievementRewardLocales;

        std::unordered_map<uint32, AchievementCriteriaEntryList> m_specialList[ACHIEVEMENT_CRITERIA_TYPE_TOTAL];
        std::unordered_map<uint32, AchievementCriteriaEntryList> m_achievementCriteriasByCondition[ACHIEVEMENT_CRITERIA_CONDITION_TOTAL];

        std::unordered_map<uint32, PlayerAchievementMgr> m_playerMgrs;
    };
}
#endif
