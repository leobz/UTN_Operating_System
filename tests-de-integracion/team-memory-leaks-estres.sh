echo Team: Memory Leacks

echo "  Cargando configuracion de Memory Leacks"
#rm *.log
cp team.config team.config.auxiliar; rm team.config; cp team.config.estres team.config


echo "  Levanto Team en segundo plano"
1>/dev/null 2>memory-leaks.log valgrind --leak-check=yes ../team/Debug/team  &

echo "  Ejecutando miles de solicitudes (Estres)"
sleep 0.5

i=0
while [ "$i" -le 1000 ]; do
     2>/dev/null 1>/dev/null ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 6 6
    i=$(( i + 1 ))
done 

echo "Total de peticiones realizadas : ${i}"
#echo "  Restaurando configuracion inicial"
#rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar


echo "  Memory leaks:"
sleep 4
less memory-leaks.log





