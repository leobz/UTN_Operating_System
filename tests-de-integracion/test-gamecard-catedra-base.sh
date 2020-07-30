sleep_time=0.5

echo  "## Test Gamecard: Base ##\n"

# INICIO DEL TEST
echo "  Cargando configuracion de la Cátedra"
cp gamecard.config gamecard.config.auxiliar; rm gamecard.config; cp gamecard.config.catedra.base gamecard.config

echo "  Eliminando archivos existentes del FileSystem"
rm -r ../tall_grass/Files/* 
rm ../tall_grass/Blocks/*
rm *.log

echo "  Actualizando Metadata.bin"
echo -e "BLOCK_SIZE=64\nBLOCKS=4096\nMAGIC_NUMBER=TALL_GRASS" > ../tall_grass/Metadata/Metadata.bin

echo "  Levanto gamecard en segundo plano"
1>/dev/null 2>/dev/null ../gamecard/Debug/gamecard &
sleep 0.1

# PETICIONES new_pikachu.sh
echo "\nEjecutando el script new_pikachu.sh"
../gameboy/Debug/gameboy GAMECARD NEW_POKEMON Pikachu 2 5 10 9
sleep $sleep_time

echo "Comprobacion: Se creo la carpeta Pikachu y su metadata indica que el tamaño es 7 bytes.\n"
cat ../tall_grass/Files/Pikachu/Metadata.bin



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
sleep $(calc $sleep_time \* 8)

echo "Comprobacion: El tamaño del archivo Pikachu se haya actualizado a 13 bytes.\n"
cat ../tall_grass/Files/Pikachu/Metadata.bin

echo "\nComprobacion: Se creo la carpeta Charmander y su metadata indique
 que posee dos bloques y su tamaño es 70 bytes\n"
cat ../tall_grass/Files/Charmander/Metadata.bin


# PETICIONES catch_charmander.sh
echo "\nEjecutar el script catch_charmander.sh"
../gameboy/Debug/gameboy GAMECARD CATCH_POKEMON Charmander 413 17 1
sleep $sleep_time

echo "Comprobacion: Verificar que el archivo Charmander ahora indique que posee
solo un bloque y su tamaño es 61 bytes.\n"
cat ../tall_grass/Files/Charmander/Metadata.bin



#COMPROBACION A NIVEL DE LINUX

echo "\n\n  Comprobaciones a nivel FileSystem de Linux\n\n"
echo "Verificar que ejecutando el script pikachu (una única sentencia), el file system cree  el archivo y asigne correctamente los bloques y bytes.
Verificar que al realizar múltiples NEW sobre un mismo pokémon, el archivo crezca en tamaño de bloques.
La asignación de bloques se realice correctamente.
Verificar que al realizar catch se libere espacio en el archivo.
Verificar que al liberar espacio suficiente, se liberen los bloques innecesarios\n"

ls -l ../tall_grass/Blocks/



# TEST BYEXAMPLE
byexample -l shell gamecard-catedra-base.md


# FINALIZACION

echo "\n  Finalizando Gamecard"
1>/dev/null 2>/dev/null sh mataProcesos.sh

