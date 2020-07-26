echo
echo Ejecutando VARIOS POKEMONES nuevos:
../gamecard/Debug/gamecard GAMECARD CATCH_POKEMON Charmander 413 17 1

echo
echo Restaurando configuracion inicial 
rm gamecard.config; cp gamecard.config.auxiliar gamecard.config; rm gamecard.config.auxiliar