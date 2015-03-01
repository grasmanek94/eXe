@echo off
:loop
B:
cd \
cd DUMPS
start procdump -mp -t -w samp-server.exe
start powershell -ExecutionPolicy unrestricted -Command "(New-Object Net.WebClient).DownloadString(\"http://exe24.info/notification/start.php\")"
S:
cd \
.\samp-server.exe > NUL
start powershell -ExecutionPolicy unrestricted -Command "(New-Object Net.WebClient).DownloadString(\"http://exe24.info/notification/shutdown.php\")"
goto loop