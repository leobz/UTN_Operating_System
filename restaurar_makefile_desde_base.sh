echo Restaurando makefiles desde backups contenidos en deploy/ ...

echo   Restaurando team..
mkdir team/Debug 2>/dev/null
cp deploy/team_makefile team/Debug/makefile

echo   Restaurando gamecard..
mkdir gamecard/Debug 2>/dev/null
cp deploy/gamecard_makefile gamecard/Debug/makefile

echo   Restaurando broker..
mkdir broker/Debug 2>/dev/null
cp deploy/broker_makefile broker/Debug/makefile

echo   Restaurando gameboy..
mkdir gameboy/Debug 2>/dev/null

cp deploy/gameboy_makefile gameboy/Debug/makefile

echo   Restaurando utils..
mkdir utils/Debug 2>/dev/null
cp deploy/utils_makefile utils/Debug/makefile