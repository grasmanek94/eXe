@echo off
for /F "usebackq tokens=1,2 delims==" %%i in (`wmic os get LocalDateTime /VALUE 2^>NUL`) do if '.%%i.'=='.LocalDateTime.' set ldt=%%j
set ldt=%ldt:~0,4%%ldt:~4,2%%ldt:~6,2%%ldt:~8,2%%ldt:~10,2%%ldt:~12,6%
set "CURRENTDATETIME=[%ldt%]"
set "NEWFILELOC=B:\%CURRENTDATETIME%-users.sqlite3"
set "SRCLOCFILE=S:\users.sqlite3"
COPY /Y "%SRCLOCFILE%" "%NEWFILELOC%"
B:
cd \
pushd B:\
for /f "skip=6 delims=" %%F in ('dir /b /a-d /o-n "[*]-users.sqlite3"') do del "%%F"
popd