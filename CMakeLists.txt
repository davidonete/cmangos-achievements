#
# Copyright (C) 2005-2018 MaNGOS project <http://getmangos.com/>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

set(LIBRARY_NAME Achievements)

#Add PCH Files
set(achievements_PCH "${CMAKE_CURRENT_SOURCE_DIR}/src/achievementspch.h")
if(PCH)
    LIST(APPEND achievements_PCH "${CMAKE_CURRENT_SOURCE_DIR}/src/achievementspch.cpp")
endif()

#Base files
file(GLOB achievements_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
    source_group("Achievements" FILES ${achievements_source})

#Define base source library
set(LIBRARY_SRCS
    ${achievements_source}
    ${achievements_PCH}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/game
    ${CMAKE_SOURCE_DIR}/src/game/AI
    ${CMAKE_SOURCE_DIR}/src/game/Accounts
    ${CMAKE_SOURCE_DIR}/src/game/Addons
    ${CMAKE_SOURCE_DIR}/src/game/Arena
    ${CMAKE_SOURCE_DIR}/src/game/AuctionHouse
    ${CMAKE_SOURCE_DIR}/src/game/BattleGround
    ${CMAKE_SOURCE_DIR}/src/game/Chat
    ${CMAKE_SOURCE_DIR}/src/game/ChatCommands
    ${CMAKE_SOURCE_DIR}/src/game/Combat
    ${CMAKE_SOURCE_DIR}/src/game/DBScripts
    ${CMAKE_SOURCE_DIR}/src/game/Entities
    ${CMAKE_SOURCE_DIR}/src/game/GMTickets
    ${CMAKE_SOURCE_DIR}/src/game/GameEvents
    ${CMAKE_SOURCE_DIR}/src/game/Globals
    ${CMAKE_SOURCE_DIR}/src/game/Grids
    ${CMAKE_SOURCE_DIR}/src/game/Groups
    ${CMAKE_SOURCE_DIR}/src/game/Guilds
    ${CMAKE_SOURCE_DIR}/src/game/LFG
    ${CMAKE_SOURCE_DIR}/src/game/Loot
    ${CMAKE_SOURCE_DIR}/src/game/Mails
    ${CMAKE_SOURCE_DIR}/src/game/Maps
    ${CMAKE_SOURCE_DIR}/src/game/MotionGenerators
    ${CMAKE_SOURCE_DIR}/src/game/Movement
    ${CMAKE_SOURCE_DIR}/src/game/Object
    ${CMAKE_SOURCE_DIR}/src/game/OutdoorPvP
    ${CMAKE_SOURCE_DIR}/src/game/Pools
    ${CMAKE_SOURCE_DIR}/src/game/Quests
    ${CMAKE_SOURCE_DIR}/src/game/References
    ${CMAKE_SOURCE_DIR}/src/game/Reputation
    ${CMAKE_SOURCE_DIR}/src/game/Server
    ${CMAKE_SOURCE_DIR}/src/game/Server
    ${CMAKE_SOURCE_DIR}/src/game/Skills
    ${CMAKE_SOURCE_DIR}/src/game/Social
    ${CMAKE_SOURCE_DIR}/src/game/Spells
    ${CMAKE_SOURCE_DIR}/src/game/Time
    ${CMAKE_SOURCE_DIR}/src/game/Tools
    ${CMAKE_SOURCE_DIR}/src/game/Trade
    ${CMAKE_SOURCE_DIR}/src/game/VoiceChat
    ${CMAKE_SOURCE_DIR}/src/game/Warden
    ${CMAKE_SOURCE_DIR}/src/game/Weather
    ${CMAKE_SOURCE_DIR}/src/game/World
    ${CMAKE_SOURCE_DIR}/src/game/WorldHandlers
    ${CMAKE_SOURCE_DIR}/src/game/movement
    ${CMAKE_SOURCE_DIR}/src/game/vmap
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/shared/Auth
    ${CMAKE_SOURCE_DIR}/src/shared/Config
    ${CMAKE_SOURCE_DIR}/src/shared/Common
    ${CMAKE_SOURCE_DIR}/src/shared/Database
    ${CMAKE_SOURCE_DIR}/src/shared/DataStores
    ${CMAKE_SOURCE_DIR}/src/shared/Network
    ${CMAKE_SOURCE_DIR}/src/shared/Log
    ${CMAKE_SOURCE_DIR}/src/shared/Threading
    ${CMAKE_SOURCE_DIR}/src/shared/Utilities
    ${CMAKE_SOURCE_DIR}/src/framework
    ${CMAKE_SOURCE_DIR}/src/framework/Platform
    ${CMAKE_SOURCE_DIR}/dep/g3dlite
    ${CMAKE_SOURCE_DIR}/dep/include/g3dlite
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src/shared
    ${MYSQL_INCLUDE_DIR}
    ${ACE_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

# Define ENABLE_Achievements if need
if (BUILD_ACHIEVEMENTS)
	add_definitions(-DENABLE_ACHIEVEMENTS)
endif()

if(PCH)
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SRCS})

target_link_libraries(${LIBRARY_NAME}
	shared
	RecastNavigation::Detour
)

if(UNIX)
	# Both systems don't have libdl and don't need them
	if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
		target_link_libraries(${LIBRARY_NAME} dl)
	endif()
endif()

# Install config files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/achievements.conf.dist.in ${CMAKE_CURRENT_BINARY_DIR}/achievements.conf.dist)
if (NOT CONF_INSTALL_DIR)
	set(CONF_INSTALL_DIR ${CONF_DIR})
endif()
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/achievements.conf.dist DESTINATION ${CONF_INSTALL_DIR})

# Generate PCH
if(PCH)
	if (ADD_CXX_PCH)
		ADD_CXX_PCH(Achievements ${achievements_PCH})
	elseif (MSVC)
		set(PCHBaseName "pchdef")
		set(PrecompiledBinary "${CMAKE_CURRENT_SOURCE_DIR}/src/achievementspch.pch")
		set(PrecompiledHeader "${CMAKE_CURRENT_SOURCE_DIR}/src/achievementspch.h")
		set(PrecompiledSource "${CMAKE_CURRENT_SOURCE_DIR}/src/achievementspch.cpp")
		set_target_properties(${LIBRARY_NAME} PROPERTIES COMPILE_FLAGS " /Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\" /bigobj")
		set_source_files_properties(${PrecompiledSource} PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\"")
		set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Achievements")
	elseif(NOT MINGW)
		# TODO: Resolve issues with order of includes before enabling Cotire for MinGW builds
		cotire(${LIBRARY_NAME})
	endif()
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()