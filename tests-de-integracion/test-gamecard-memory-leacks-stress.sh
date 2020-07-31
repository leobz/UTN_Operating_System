echo  "## Test Gamecard: Base ##\n"

# INICIO DEL TEST
echo "  Cargando configuracion de la Cátedra"
cp gamecard.config gamecard.config.auxiliar; rm gamecard.config; cp gamecard.config.catedra.base gamecard.config

echo "  Eliminando archivos existentes del FileSystem"
rm -r ../tall_grass/Files/*
rm ../tall_grass/Blocks/*
rm *.log

echo "  Actualizando Metadata.bin"
echo -e "BLOCK_SIZE=64\nBLOCKS=96\nMAGIC_NUMBER=TALL_GRASS" > ../tall_grass/Metadata/Metadata.bin

echo "  Levanto gamecard en segundo plano"
valgrind --leak-check=yes ../gamecard/Debug/gamecard 2> memory-leaks-stress-gamecard.log &
sleep 3

echo "  Ejecutando miles de solicitudes"
i=0
while [ "$i" -le 250 ]; do
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Pikachu 1 $((i+1)) 45 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Charmander 1 $((i+1)) 30 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Bulbasur 1 $((i+1)) 30 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Squirtle 1 $((i+1)) 30 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Caterpie 1 $((i+1)) 30 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Butterfree 1 $((i+1)) 30 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Pidgeotto 1 $((i+1)) 30 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Rattata 1 $((i+1)) 30 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Bulbasur 1 $((i+1)) 30 1
    sleep 0.5
    ../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Weedle 1 $((i+1)) 30 1
    sleep 0.5
    i=$(( i + 1 ))
done 

echo "\n\n  Listado de archivos blocks\n\n"
ls -l ../tall_grass/Blocks/

echo "\n  Finalizando Gamecard"
sh mataProcesos.sh &