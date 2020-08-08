echo Backupeando makefiles en carpeta deploy/ ...
#python3 deploy/backupear_makefiles_base.py

cp broker/Debug/*.mk deploy/broker/
cp broker/Debug/makefile deploy/broker/
cp broker/Debug/test/*.mk deploy/broker/test/

cp gameboy/Debug/*.mk deploy/gameboy/
cp gameboy/Debug/makefile deploy/gameboy/
cp gameboy/Debug/test/*.mk deploy/gameboy/test/

cp gamecard/Debug/*.mk deploy/gamecard/
cp gamecard/Debug/makefile deploy/gamecard/
cp gamecard/Debug/test/*.mk deploy/gamecard/test/

cp team/Debug/*.mk deploy/team/
cp team/Debug/makefile deploy/team/
cp team/Debug/test/*.mk deploy/team/test/

cp utils/Debug/*.mk deploy/utils/
cp utils/Debug/makefile deploy/utils/
