echo Restaurando makefiles desde backups contenidos en deploy/ ...

echo   Restaurando team..
cp deploy/team_makefile team/Debug/makefile

echo   Restaurando gamecard..
cp deploy/gamecard_makefile gamecard/Debug/makefile

echo   Restaurando broker..
cp deploy/broker_makefile broker/Debug/makefile

echo   Restaurando gameboy..
cp deploy/gameboy_makefile gameboy/Debug/makefile

echo   Restaurando utils..
cp deploy/utils_makefile utils/Debug/makefile