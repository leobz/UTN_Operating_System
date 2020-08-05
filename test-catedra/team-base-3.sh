# NOTA: 
# Ejecutar el script test-catedra-2.sh previamente: Esperar a que los 
# tres entrenadores se muevan y atrapen los respectivo pokemon.

#echo Ejecutando solicitudes Appeared "Parte 2":

../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Squirtle 3 5
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Gengar 7 5


#echo Restaurando configuracion inicial 
cp team.config team.config.catedra.base; rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar

