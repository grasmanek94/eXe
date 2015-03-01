@echo off
cd /d %~dp0
git fetch origin
git reset --hard origin/master
cd /d %~dp0
start /wait TFS_VERSION.bat
echo updated TFS version
cd /d %~dp0
cd src/gamemode
copy /b VERSION.cxx +,,
type nul >>GoldCoinsAchievements.inline & copy GoldCoinsAchievements.inline +,,
echo updated version...
cd /d %~dp0
echo compiling...
Devenv SampGamemode.sln /build ReleaseDedicated
echo done
exit 0