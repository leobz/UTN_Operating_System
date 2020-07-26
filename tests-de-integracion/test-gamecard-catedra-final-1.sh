echo Test Gamecard: Final

echo
echo Cargando configuracion de la Cátedra
cp gamecard.config gamecard.config.auxiliar; rm gamecard.config; cp gamecard.config.catedra.final gamecard.config

echo
echo Levanto gamecard en segundo plano:
../gamecard/Debug/gamecard &

