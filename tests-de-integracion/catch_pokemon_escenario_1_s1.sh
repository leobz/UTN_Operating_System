echo **** Parte 2 *****

echo
echo Cargando configuracion de la Cátedra
cp team.config team.config.auxiliar; rm team.config; cp team_catedra_base.config team.config

echo
echo Levanto Team:
../team/Debug/team&

echo
echo Ejecutando solicitudes Appeared:
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 1 1
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Squirtle 9 7
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Onix 2 2

echo
echo Restaurando configuracion original
cp team.config team_catedra_base.config; rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar
