@echo off
cd /d %~dp0
cd src/gamemode
copy /b VERSION.cxx +,,
echo updated version...

set _aPath=%CD%
set "TFSX=..\IDE"
set _TFPath="%VS120COMNTOOLS%%TFSX%"

pushd %_aPath%
%_TFPath%\TF.exe history . /r /noprompt /stopafter:1 /Version:W > auto_version.h
FOR /f "tokens=1" %%a in ('findstr /R "^[0-9][0-9]*" auto_version.h') do set _TFS_BUILD_VERSION=%%a

popd
cd /d %~dp0
echo #define TFS_BUILD_VERSION %_TFS_BUILD_VERSION% > auto_version.h
exit 0