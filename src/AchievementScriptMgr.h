#ifndef ACHIEVEMENT_AchievementScriptMgr_H
#define ACHIEVEMENT_AchievementScriptMgr_H

#include "AchievementsMgr.h"

class AchievementScriptObject 
{
    friend class AchievementScriptMgr;

public:
    // Do not override this in scripts; it should be overridden by the various script type classes. It indicates
    // whether or not this script type must be assigned in the database.
    [[nodiscard]] virtual bool IsDatabaseBound() const { return false; }
    [[nodiscard]] virtual bool isAfterLoadScript() const { return IsDatabaseBound(); }
    virtual void checkValidity() { }

    [[nodiscard]] const std::string& GetName() const { return _name; }

    AchievementScriptObject() {}
    ~AchievementScriptObject() {}// = default;

protected:
    AchievementScriptObject(const char* name) : _name(std::string(name)) {}

private:
    const std::string _name;
};

template<class TObject> class UpdatableScript
{
protected:
    UpdatableScript() = default;

public:
    virtual void OnUpdate(TObject* /*obj*/, uint32 /*diff*/) { }
};

class AchievementCriteriaScript : public AchievementScriptObject
{
protected:
    AchievementCriteriaScript(const char* name);

public:
    [[nodiscard]] bool IsDatabaseBound() const override { return true; }

    [[nodiscard]] virtual bool OnCheck(Player* /*source*/, Unit* /*target*/, uint32 /*criteria_id*/) { return true; };
};

class PlayerScript : public AchievementScriptObject
{
protected:
    PlayerScript(const char* name) : AchievementScriptObject(name) {}

public:
    // Called when a player complete an achievement
    virtual void OnAchiComplete(Player* /*player*/, AchievementEntry const* /*achievement*/) { }

    // Called before player complete an achievement, can be used to disable achievements in certain conditions
    virtual bool OnBeforeAchiComplete(Player* /*player*/, AchievementEntry const* /*achievement*/) { return true; }

    // Called when a player complete an achievement criteria
    virtual void OnCriteriaProgress(Player* /*player*/, AchievementCriteriaEntry const* /*criteria*/) { }

    //  Called before player complete an achievement criteria, can be used to disable achievement criteria in certain conditions
    virtual bool OnBeforeCriteriaProgress(Player* /*player*/, AchievementCriteriaEntry const* /*criteria*/) { return true; }

    // Called when an Achievement is saved to DB
    virtual void OnAchiSave(/*CharacterDatabaseTransaction trans, */Player* /*player*/, uint16 /*achId*/, CompletedAchievementData /*achiData*/) { }

    // Called when an Criteria is saved to DB
    virtual void OnCriteriaSave(/*CharacterDatabaseTransaction trans, */Player* /*player*/, uint16 /*achId*/, CriteriaProgress /*criteriaData*/) { }
};

class AchievementScript : public AchievementScriptObject
{
protected:
    AchievementScript(const char* name);

public:
    bool IsDatabaseBound() const { return false; }

    // After complete global achievement
    virtual void SetRealmCompleted(AchievementEntry const* /*achievement*/) { }

    [[nodiscard]] virtual bool IsCompletedCriteria(PlayerAchievementMgr* /*mgr*/, AchievementCriteriaEntry const* /*achievementCriteria*/, AchievementEntry const* /*achievement*/, CriteriaProgress const* /*progress*/) { return true; }

    [[nodiscard]] virtual bool IsRealmCompleted(AchievementsMgr const* /*globalmgr*/, AchievementEntry const* /*achievement*/, std::chrono::system_clock::time_point /*completionTime*/) { return true; }

    virtual void OnBeforeCheckCriteria(PlayerAchievementMgr* /*mgr*/, AchievementCriteriaEntryList const* /*achievementCriteriaList*/) { }

    [[nodiscard]] virtual bool CanCheckCriteria(PlayerAchievementMgr* /*mgr*/, AchievementCriteriaEntry const* /*achievementCriteria*/) { return true; }
};

// Manages registration, loading, and execution of scripts.
class AchievementScriptMgr
{
    friend class AchievementScriptObject;

public: /* Initialization */
    AchievementScriptMgr() : _scriptCount(0), _scheduledScripts(0), _script_loader_callback(nullptr) {}
    ~AchievementScriptMgr() {}

    void Initialize();
    void LoadDatabase();
    //void FillSpellSummary();
    void CheckIfScriptsInDatabaseExist();

    const char* ScriptsVersion() const { return "Integrated Azeroth Scripts"; }

    void IncrementScriptCount() { ++_scriptCount; }
    uint32 GetScriptCount() { return _scriptCount; }

    typedef void(*ScriptLoaderCallbackType)();

    /// Sets the script loader callback which is invoked to load scripts
    /// (Workaround for circular dependency game <-> scripts)
    void SetScriptLoader(ScriptLoaderCallbackType script_loader_callback)
    {
        _script_loader_callback = script_loader_callback;
    }

public: /* Unloading */
    void Unload();

public: /* AchievementCriteriaScript */
    bool OnCriteriaCheck(uint32 scriptId, Player* source, Unit* target, uint32 criteria_id);

public: /* PlayerScript */
    void OnAchievementComplete(Player* player, AchievementEntry const* achievement);
    bool OnBeforeAchievementComplete(Player* player, AchievementEntry const* achievement);
    void OnCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria);
    bool OnBeforeCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria);
    void OnAchievementSave(/*CharacterDatabaseTransaction trans, */Player* player, uint16 achiId, CompletedAchievementData achiData);
    void OnCriteriaSave(/*CharacterDatabaseTransaction trans, */Player* player, uint16 critId, CriteriaProgress criteriaData);

public: /* AchievementScript */
    void SetRealmCompleted(AchievementEntry const* achievement);
    bool IsCompletedCriteria(PlayerAchievementMgr* mgr, AchievementCriteriaEntry const* achievementCriteria, AchievementEntry const* achievement, CriteriaProgress const* progress);
    bool IsRealmCompleted(AchievementsMgr const* globalmgr, AchievementEntry const* achievement, std::chrono::system_clock::time_point completionTime);
    void OnBeforeCheckCriteria(PlayerAchievementMgr* mgr, AchievementCriteriaEntryList const* achievementCriteriaList);
    bool CanCheckCriteria(PlayerAchievementMgr* mgr, AchievementCriteriaEntry const* achievementCriteria);

public:
    uint32 getScriptId(std::string const& name) const;

private:
    void loadScriptNames();
    std::vector<std::string> _scriptNamesStore;

    //atomic op counter for active scripts amount
    std::atomic<long> _scheduledScripts;
    uint32 _scriptCount;
    ScriptLoaderCallbackType _script_loader_callback;
};

#define sAchievementScriptMgr MaNGOS::Singleton<AchievementScriptMgr>::Instance()

template<class TScript>
class ScriptRegistry
{
public:
    typedef std::map<uint32, TScript*> ScriptMap;
    typedef typename ScriptMap::iterator ScriptMapIterator;

    typedef std::vector<TScript*> ScriptVector;
    typedef typename ScriptVector::iterator ScriptVectorIterator;

    // The actual list of scripts. This will be accessed concurrently, so it must not be modified
    // after server startup.
    static ScriptMap ScriptPointerList;
    // After database load scripts
    static ScriptVector ALScripts;

    static void AddScript(TScript* const script)
    {
        MANGOS_ASSERT(script);

        if (!_checkMemory(script))
            return;

        if (script->isAfterLoadScript())
        {
            ALScripts.push_back(script);
        }
        else
        {
            script->checkValidity();

            // We're dealing with a code-only script; just add it.
            ScriptPointerList[_scriptIdCounter++] = script;
            sAchievementScriptMgr.IncrementScriptCount();
        }
    }

    static void AddALScripts()
    {
        for (auto script : ALScripts)
        {
            script->checkValidity();

            // We're dealing with a code-only script; just add it.
            ScriptPointerList[_scriptIdCounter++] = script;
            sAchievementScriptMgr.IncrementScriptCount();
        }

        ALScripts.clear();
    }

    static void MatchScriptIDs()
    {
        // This is needed to match the id of the registry with the id of the AchievementScriptMgr
        ScriptMap matchedScriptPointerList;
        for (const auto& pair : ScriptPointerList)
        {
            TScript* script = pair.second;
            const uint32 scriptId = sAchievementScriptMgr.getScriptId(script->GetName());
            if (scriptId)
            {
                matchedScriptPointerList[scriptId] = script;
            }
            else
            {
                sLog.outError("Failed to match script '%s' with the DB loaded scripts.", script->GetName().c_str());
            }
        }

        if (!matchedScriptPointerList.empty())
        {
            ScriptPointerList = matchedScriptPointerList;
        }
    }

    // Gets a script by its ID (assigned by ObjectMgr).
    static TScript* GetScriptById(uint32 id)
    {
        ScriptMapIterator it = ScriptPointerList.find(id);
        if (it != ScriptPointerList.end())
            return it->second;

        return nullptr;
    }

private:
    // See if the script is using the same memory as another script. If this happens, it means that
    // someone forgot to allocate new memory for a script.
    static bool _checkMemory(TScript* const script)
    {
        // See if the script is using the same memory as another script. If this happens, it means that
        // someone forgot to allocate new memory for a script.
        for (ScriptMapIterator it = ScriptPointerList.begin(); it != ScriptPointerList.end(); ++it)
        {
            if (it->second == script)
            {
                sLog.outError("scripts, Script '%s' has same memory pointer as '%s'.",
                    script->GetName().c_str(), it->second->GetName().c_str());

                return false;
            }
        }

        return true;
    }

    // Counter used for code-only scripts.
    static uint32 _scriptIdCounter;
};

// Instantiate static members of ScriptRegistry.
template<class TScript> std::map<uint32, TScript*> ScriptRegistry<TScript>::ScriptPointerList;
template<class TScript> std::vector<TScript*> ScriptRegistry<TScript>::ALScripts;
template<class TScript> uint32 ScriptRegistry<TScript>::_scriptIdCounter = 0;

#endif
