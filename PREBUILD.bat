start /wait taskkill /f /im procdump.exe
start /wait wmic Path win32_process Where "CommandLine Like '%%/c%%SERVER.bat%%'" Call Terminate
start /wait taskkill /f /im samp-server.exe
S:
cd \
cd mm
powershell start-process "S:\mm\samp-server.exe"
exit 0