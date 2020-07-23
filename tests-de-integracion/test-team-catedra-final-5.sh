echo
echo Ejecutando solicitudes APPEARED para RR:
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 9 9
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Squirtle 6 7
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Onix 2 8
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Gengar 6 6
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Squirtle 5 5

echo
echo Restaurando configuracion inicial 
rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar