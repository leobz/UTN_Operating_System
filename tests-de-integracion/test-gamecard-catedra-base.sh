echo  "## Test Gamecard: Base ##\n"

# INICIO DEL TEST
echo "  Cargando configuracion de la Cátedra"
cp gamecard.config gamecard.config.auxiliar; rm gamecard.config; cp gamecard.config.catedra.base gamecard.config

echo "  Eliminando archivos existentes del FileSystem"
rm -r ../tall_grass/Files/* 
rm ../tall_grass/Blocks/*

echo "  Actualizando Metadata.bin"
echo -e "BLOCK_SIZE=64\nBLOCKS=4096\nMAGIC_NUMBER=TALL_GRASS" > ../tall_grass/Metadata/Metadata.bin

echo "  Levanto gamecard en segundo plano"
1>/dev/null 2>/dev/null ../gamecard/Debug/gamecard &


# PETICIONES new_pikachu.sh
echo "\nEjecutando el script new_pikachu.sh"
sleep 0.5
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Pikachu 2 5 10 9

echo "Comprobacion: Se creo la carpeta Pikachu y su metadata indica que el tamaño es 7 bytes.\n"
sleep 0.5
cat ../tall_grass/Files/Pikachu/Metadata.txt



# PETICIONES new_pokemons_varios.sh
echo "\nEjecutando el script new_pokemons_varios.sh"
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Pikachu 1 9 3 1
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Charmander 4 1 101 2
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Charmander 517 2046 15 3
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Charmander 413 17 1 4
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Charmander 723 97 29 5
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Charmander 65 93 3001 6
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Charmander 12 17 34 7
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Charmander 129 547 11 8

echo "Comprobacion: El tamaño del archivo Pikachu se haya actualizado a 13 bytes.\n"
sleep 0.5
cat ../tall_grass/Files/Pikachu/Metadata.txt

# FINALIZACION

echo "\n  Finalizando Gamecard"
rm -r ../tall_grass/Files/*
rm ../tall_grass/Blocks/*
1>/dev/null 2>/dev/null sh mataProcesos.sh


