echo Team: Memory Leacks

echo "  Cargando configuracion de Memory Leacks"
cp team.config team.config.auxiliar; rm team.config; cp team.config.memory.leacks team.config
rm *.log

echo "  Levanto Team en segundo plano"
valgrind --leak-check=yes ../team/Debug/team 2> memory-leaks.log &

echo "  Ejecutando solicitudes"
sleep 2
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 9 9
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Squirtle 0 0

echo "  Restaurando configuracion inicial"
rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar


echo "  Memory leaks:"
sleep 2
less memory-leaks.log





