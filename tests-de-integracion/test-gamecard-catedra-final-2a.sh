echo
echo Ejecutando UN POKEMON nuevo:
../gamecard/Debug/gamecard GAMECARD NEW_POKEMON Pikachu 2 5 10 9


echo
echo Restaurando configuracion inicial 
rm gamecard.config; cp gamecard.config.auxiliar gamecard.config; rm gamecard.config.auxiliar