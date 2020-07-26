echo
echo Ejecutando VARIOS POKEMONES nuevos:
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Pikachu 1 9 3 1
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Charmander 4 1 101 2
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Charmander 517 2046 15 3
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Charmander 413 17 1 4
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Charmander 723 97 29 5
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Charmander 65 93 3001 6
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Charmander 12 17 34 7
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Charmander 129 547 11 8

echo
echo Restaurando configuracion inicial 
rm gamecard.config; cp gamecard.config.auxiliar gamecard.config; rm gamecard.config.auxiliar