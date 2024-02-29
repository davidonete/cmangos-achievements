# Achievements
Achievements mod to make WoTLK achievements work on cmangos classic and tbc cores.

![image](https://github.com/davidonete/cmangos-achievements/assets/11618807/caa813e9-0053-4405-8d00-cf04fe5c205f)

This mod was ported from https://github.com/tsaah/core/tree/hb-achievements and modified to become a independent module as well as adding extra features.

# Available Cores
Classic and TBC

# How to install
1. Follow the instructions in https://github.com/davidonete/cmangos-modules?tab=readme-ov-file#how-to-install
2. Enable the `BUILD_MODULE_ACHIEVEMENTS` flag in cmake and run cmake. The module should be installed in `src/modules/achievements`
4. Copy the configuration file from `src/modules/achievements/src/achievements.conf.dist.in` and place it where your mangosd executable is. Also rename it to `achievements.conf`.
5. Remember to edit the config file and modify the options you want to use.
6. Lastly you will have to install the database changes located in the `src/modules/achievements/sql/install` folder, each folder inside represents where you should execute the queries. E.g. The queries inside of `src/modules/achievements/sql/install/world` will need to be executed in the world/mangosd database, the ones in `src/modules/achievements/sql/install/characters` in the characters database, etc...
7. For being able to see the UI on the client you will need to download and install the following addon https://github.com/celguar/Achiever

# How to uninstall
To remove achievements from your server you have multiple options, the first and easiest is to disable it from the `achievements.conf` file. The second option is to completely remove it from the server and db:
1. Remove the `BUILD_MODULE_ACHIEVEMENTS` flag from your cmake configuration and recompile the game
2. Execute the sql queries located in the `src/modules/achievements/sql/uninstall` folder. Each folder inside represents where you should execute the queries. E.g. The queries inside of `src/modules/achievements/sql/uninstall/world` will need to be executed in the world/mangosd database, the ones in `src/modules/achievements/sql/uninstall/characters` in the characters database, etc...
