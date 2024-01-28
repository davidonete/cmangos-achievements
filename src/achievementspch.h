//add here most rarely modified headers to speed up debug build compilation
#include "WorldSocket.h"                                    // must be first to make ACE happy with ACE includes in it
#include "Common.h"

#include "AccountMgr.h"
#include "BattleGround.h"
#include "BattleGroundAB.h"
#include "BattleGroundAV.h"
#include "BattleGroundWS.h"
#include "Chat.h"
#include "GuildMgr.h"
#include "Guild.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "GameEventMgr.h"
#include "World.h"

#include "AchievementsMgr.h"
#include "AchievementsConfig.h"
#include "AchievementScriptMgr.h"