## Prueba Catedra Final Completa

## Disclaimer:

El objetivo de la prueba es verificar el correcto funcionamiento del sistema global incluyendo mensajes Localized en el sistema.

## Actividades:

1) Iniciar el proceso Broker
2) Iniciar el proceso GameCard
3) Ejecutar el script new_pokemon_antes_team.sh
4) Ejecutar el proceso Team 1 y Team 2
5) Una vez que ambos procesos Teams queden sin moverse, ejecutar el script new_pokemon_post_team.sh
6) Esperar a que ambos procesos Team finalicen


## Prueba Completa

1) Verificar el estado del file system previa a la ejecución de los procesos Team
2) Verificar el estado del file system post ejecución de ambos proceso Team y ejecución de catch
3) Verificar que ambos procesos Team finalicen cumpliendo su objetivo
4) Verificar que la memoria vaya realizando los reemplazos necesarios para el correcto funcionamiento

-------------------------------------------------------------------------------------
## Comprobar que finalizan los teams

```bash
$ cat team.log | grep "FIN DEL PROCESO"
<...>[FIN DEL PROCESO] ¡Team cumplió objetivo!<...>
```


```bash
$ cat ../team/Debug/team.log | grep "FIN DEL PROCESO"
<...>[FIN DEL PROCESO] ¡Team cumplió objetivo!<...>
```


-------------------------------------------------------------------------------------
## Comprobar que el FS esta vacio

```bash
$ cat ../tall-grass/Files/Espeon/Metadata.bin
<...>SIZE=0
```

```bash
$ cat ../tall-grass/Files/Flareon/Metadata.bin
<...>SIZE=0
```

```bash
$ cat ../tall-grass/Files/Jolteon/Metadata.bin
<...>SIZE=0
```

```bash
$ cat ../tall-grass/Files/Gengar/Metadata.bin
<...>SIZE=0
```

```bash
$ cat ../tall-grass/Files/Onix/Metadata.bin
<...>SIZE=0
```

```bash
$ cat ../tall-grass/Files/Pikachu/Metadata.bin
<...>SIZE=0
```

```bash
$ cat ../tall-grass/Files/Squirtle/Metadata.bin
<...>SIZE=0
```

```bash
$ cat ../tall-grass/Files/Umbreon/Metadata.bin
<...>SIZE=0
```


```bash
$ cat ../tall-grass/Files/Vaporeon/Metadata.bin
<...>SIZE=0
```

