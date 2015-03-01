@echo off
cd /d %~dp0
cd src/gamemode
copy /b VERSION.cxx +,,
type nul >>GoldCoinsAchievements.inline & copy GoldCoinsAchievements.inline +,,
type nul >>ipwhitelist.inline & copy ipwhitelist.inline +,,
echo updated version...

git rev-list HEAD --count > auto_version.h
FOR /f "tokens=1" %%a in ('findstr /R "^[0-9][0-9]*" auto_version.h') do set _TFS_BUILD_VERSION=%%a

cd /d %~dp0
echo #define TFS_BUILD_VERSION %_TFS_BUILD_VERSION% > auto_version.h
echo Updated GameMode version to %_TFS_BUILD_VERSION%
exit 0