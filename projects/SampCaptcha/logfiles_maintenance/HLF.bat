C:
cd \
cd SCRIPTS
ncftpput -f login.txt / S:/logs/*.*
ROBOCOPY S:\Logs B:\Logs /move /minage:1 > nul