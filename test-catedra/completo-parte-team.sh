#!/bin/sh

ciclo_cpu_time=1
sleep_time=1


echo "Levanto Team 1 en segundo plano"
1>/dev/null 2>/dev/null     ../team/Debug/team &
sleep $sleep_time

echo "Levanto Team 2 en segundo plano"
cd ../team/Debug/
1>/dev/null 2>/dev/null     ./team & 
cd ../../test-catedra/


echo "\nComprobacion: Se hicieron 4 Catch => Se eliminan bloques"
sleep $ciclo_cpu_time

echo "\nMetadata Pikachu:"
cat ../tall-grass/Files/Pikachu/Metadata.bin
echo "\nMetadata Squirtle:"
cat ../tall-grass/Files/Squirtle/Metadata.bin
echo "\nMetadata Jolteon:"
cat ../tall-grass/Files/Jolteon/Metadata.bin
echo "\nMetadata Flareon:"
cat ../tall-grass/Files/Flareon/Metadata.bin
echo



sleep $(expr $ciclo_cpu_time \* 4)

echo "\nComprobacion: Team 1 tiene 2 TCBs Unblocked:"
cat team.log | grep Unblocked

echo "\nComprobacion: Team 2 tiene 2 TCBs Ready to Exchange:"
cat ../team/Debug/team.log | grep "Ready to Exchange"


echo "\n\nEjecutar 'new_pokemon_post_team.sh' ?"
read "foo"

sh delibird-pruebas/new_pokemon_post_team.sh

sleep $(expr $ciclo_cpu_time \* 8)


echo "\nComprobacion: Ambos team cumplieron objetivo:\n"

tail -10 team.log
echo
tail -10 ../team/Debug/team.log


echo "\n\nAprete enter para correr test de by-example "
read "foo"

echo "\n\nEjecutando test de byexample\n"
byexample -l shell byexample-pruebas/catedra-final-completo.md