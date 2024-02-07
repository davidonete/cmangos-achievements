#include "AchievementScriptMgr.h"

#include "Util/Timer.h"

INSTANTIATE_SINGLETON_1(AchievementScriptMgr);

extern void AddSC_achievement_scripts();

// Utility macros to refer to the script registry.
#define SCR_REG_MAP(T) ScriptRegistry<T>::ScriptMap
#define SCR_REG_ITR(T) ScriptRegistry<T>::ScriptMapIterator
#define SCR_REG_LST(T) ScriptRegistry<T>::ScriptPointerList

// Utility macros for looping over scripts.
#define FOR_SCRIPTS(T, C, E) \
    if (!SCR_REG_LST(T).empty()) \
        for (SCR_REG_ITR(T) C = SCR_REG_LST(T).begin(); \
            C != SCR_REG_LST(T).end(); ++C)

#define FOR_SCRIPTS_RET(T, C, E, R) \
    if (SCR_REG_LST(T).empty()) \
        return R; \
    for (SCR_REG_ITR(T) C = SCR_REG_LST(T).begin(); \
        C != SCR_REG_LST(T).end(); ++C)

#define FOREACH_SCRIPT(T) \
    FOR_SCRIPTS(T, itr, end) \
    itr->second

// Utility macros for finding specific scripts.
#define GET_SCRIPT(T, I, V) \
    T* V = ScriptRegistry<T>::GetScriptById(I); \
    if (!V) \
        return;

#define GET_SCRIPT_RET(T, I, V, R) \
    T* V = ScriptRegistry<T>::GetScriptById(I); \
    if (!V) \
        return R;

void AchievementScriptMgr::Initialize()
{
    sLog.outBasic("server.loading, > Loading C++ scripts");
    sLog.outBasic("server.loading,  ");
    loadScriptNames();

    AddSC_achievement_scripts();
}

void AchievementScriptMgr::Unload()
{
#define SCR_CLEAR(T) \
        for (SCR_REG_ITR(T) itr = SCR_REG_LST(T).begin(); itr != SCR_REG_LST(T).end(); ++itr) \
            delete itr->second; \
        SCR_REG_LST(T).clear();

    SCR_CLEAR(AchievementCriteriaScript);
    SCR_CLEAR(AchievementScript);

#undef SCR_CLEAR
}

void AchievementScriptMgr::LoadDatabase()
{
    uint32 oldMSTime = WorldTimer::getMSTime();

    ScriptRegistry<AchievementCriteriaScript>::AddALScripts();

    ScriptRegistry<AchievementCriteriaScript>::MatchScriptIDs();

    CheckIfScriptsInDatabaseExist();

    sLog.outBasic("server.loading, >> Loaded %u C++ scripts in %u ms", GetScriptCount(), WorldTimer::getMSTimeDiff(oldMSTime, WorldTimer::getMSTime()));
    sLog.outBasic("server.loading, ");
}

void AchievementScriptMgr::loadScriptNames() 
{
    // We insert an empty placeholder here so we can use the
    // script id 0 as dummy for "no script found".
    _scriptNamesStore.emplace_back("");

    std::unique_ptr<QueryResult> result(WorldDatabase.Query("SELECT DISTINCT(`ScriptName`) FROM `achievement_criteria_data` WHERE `ScriptName` <> '' AND `type` = 11"));

    if (!result)
    {
        MANGOS_ASSERT(false);
        return;
    }

    _scriptNamesStore.reserve(result->GetRowCount() + 1);

    do
    {
        _scriptNamesStore.push_back((*result)[0].GetString());
    } 
    while (result->NextRow());

    std::sort(_scriptNamesStore.begin(), _scriptNamesStore.end());
}

void AchievementScriptMgr::CheckIfScriptsInDatabaseExist()
{
    for (auto const& scriptName : _scriptNamesStore)
    {
        if (uint32 sid = getScriptId(scriptName.c_str()))
        {
            if (!ScriptRegistry<AchievementCriteriaScript>::GetScriptById(sid) &&
                !ScriptRegistry<AchievementScript>::GetScriptById(sid))
            {
                sLog.outError("Script named '%s' is assigned in the database, but has no code!", scriptName.c_str());
            }
        }
    }
}

bool AchievementScriptMgr::OnCriteriaCheck(uint32 scriptId, Player* source, Unit* target, uint32 criteria_id)
{
    MANGOS_ASSERT(source);
    // target can be nullptr.

    GET_SCRIPT_RET(AchievementCriteriaScript, scriptId, tmpscript, false);
    return tmpscript->OnCheck(source, target, criteria_id);
}

bool AchievementScriptMgr::OnBeforeAchievementComplete(Player* player, AchievementEntry const* achievement)
{
    bool ret = true;

    FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
    if (!itr->second->OnBeforeAchiComplete(player, achievement))
        ret = false; // we change ret value only when scripts return false

    return ret;
}

void AchievementScriptMgr::OnAchievementComplete(Player* player, AchievementEntry const* achievement)
{
    FOREACH_SCRIPT(PlayerScript)->OnAchiComplete(player, achievement);
}

bool AchievementScriptMgr::OnBeforeCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria)
{
    bool ret = true;

    FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
    if (!itr->second->OnBeforeCriteriaProgress(player, criteria))
        ret = false; // we change ret value only when scripts return false

    return ret;
}

void AchievementScriptMgr::OnCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria)
{
    FOREACH_SCRIPT(PlayerScript)->OnCriteriaProgress(player, criteria);
}

void AchievementScriptMgr::OnAchievementSave(/*CharacterDatabaseTransaction trans, */Player* player, uint16 achiId, CompletedAchievementData achiData)
{
    FOREACH_SCRIPT(PlayerScript)->OnAchiSave(/*trans, */player, achiId, achiData);
}

void AchievementScriptMgr::OnCriteriaSave(/*CharacterDatabaseTransaction trans, */Player* player, uint16 critId, CriteriaProgress criteriaData)
{
    FOREACH_SCRIPT(PlayerScript)->OnCriteriaSave(/*trans, */player, critId, criteriaData);
}

void AchievementScriptMgr::SetRealmCompleted(AchievementEntry const* achievement)
{
    FOREACH_SCRIPT(AchievementScript)->SetRealmCompleted(achievement);
}

bool AchievementScriptMgr::IsCompletedCriteria(PlayerAchievementMgr* mgr, AchievementCriteriaEntry const* achievementCriteria, AchievementEntry const* achievement, CriteriaProgress const* progress)
{
    bool ret = true;

    FOR_SCRIPTS_RET(AchievementScript, itr, end, ret) // return true by default if not scripts
        if (!itr->second->IsCompletedCriteria(mgr, achievementCriteria, achievement, progress))
            ret = false; // we change ret value only when scripts return false

    return ret;
}

bool AchievementScriptMgr::IsRealmCompleted(AchievementsMgr const* globalmgr, AchievementEntry const* achievement, std::chrono::system_clock::time_point completionTime)
{
    bool ret = true;

    FOR_SCRIPTS_RET(AchievementScript, itr, end, ret) // return true by default if not scripts
        if (!itr->second->IsRealmCompleted(globalmgr, achievement, completionTime))
            ret = false; // we change ret value only when scripts return false

    return ret;
}

void AchievementScriptMgr::OnBeforeCheckCriteria(PlayerAchievementMgr* mgr, AchievementCriteriaEntryList const* achievementCriteriaList)
{
    FOREACH_SCRIPT(AchievementScript)->OnBeforeCheckCriteria(mgr, achievementCriteriaList);
}

bool AchievementScriptMgr::CanCheckCriteria(PlayerAchievementMgr* mgr, AchievementCriteriaEntry const* achievementCriteria)
{
    bool ret = true;

    FOR_SCRIPTS_RET(AchievementScript, itr, end, ret) // return true by default if not scripts
        if (!itr->second->CanCheckCriteria(mgr, achievementCriteria))
            ret = false; // we change ret value only when scripts return false

    return ret;
}

AchievementCriteriaScript::AchievementCriteriaScript(const char* name)
    : AchievementScriptObject(name)
{
    ScriptRegistry<AchievementCriteriaScript>::AddScript(this);
}

AchievementScript::AchievementScript(const char* name)
    : AchievementScriptObject(name)
{
    ScriptRegistry<AchievementScript>::AddScript(this);
}

template class ScriptRegistry<AchievementCriteriaScript>;
template class ScriptRegistry<AchievementScript>;

uint32 AchievementScriptMgr::getScriptId(std::string const& name) const
{
    // use binary search to find the script name in the sorted vector
    // assume "" is the first element
    if (name.empty())
        return 0;

    auto itr = std::lower_bound(_scriptNamesStore.begin(), _scriptNamesStore.end(), name);
    if (itr == _scriptNamesStore.end() || (*itr != name))
        return 0;

    return uint32(itr - _scriptNamesStore.begin());
}
