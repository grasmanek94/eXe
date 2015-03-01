@echo off
cd /d %~dp0
tf resolve /auto:TakeTheirs /noprompt
tf get /noprompt
tf resolve /auto:TakeTheirs /noprompt
tf get /noprompt
cd /d %~dp0
start /wait TFS_VERSION.bat
echo updated TFS version
cd /d %~dp0
cd src/gamemode
copy /b VERSION.cxx +,,
echo updated version...
cd /d %~dp0
echo compiling...
Devenv SampGamemode.sln /build ReleaseDedicated
echo done
exit 0