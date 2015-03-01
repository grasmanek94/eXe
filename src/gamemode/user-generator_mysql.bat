@echo off
cls
:Loop

IF [%1] EQU [] GOTO Continue
IF [%1] EQU [""] GOTO Continue

Set filename=%1

For %%A in ("%filename%") do (
    Set Folder=%%~dpA
    Set Name=%%~nxA
)

echo "------------ %Name% ------------"

%~dp0..\..\bin\tools\bin\odb.exe -d mysql %Name%

SHIFT
GOTO Loop

:Continue
echo "--------------------------------"
echo "              DONE              "