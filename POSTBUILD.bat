powershell start-process "taskkill" -ArgumentList '/f','/im','samp-server.exe'
C:
cd /d %~dp0
powershell start-process "cmd" -ArgumentList '/c','SERVER.bat'
powershell start-process "cmd" -ArgumentList '/c','DELAYKILL.bat'
echo "DONE, CLOSING VISUAL STUDIO"
exit 0