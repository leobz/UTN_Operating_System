## Prueba Catedra Final Completa

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento del esquema de memoria Buddy System. Para esto, será necesario la ejecución del proceso broker y de los scripts proporcionados por la cátedra.

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


```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 1 en cache en posicion 0<...>
<...>Almacenando mensaje con ID 2 en cache en posicion 4<...>
<...>Almacenando mensaje con ID 3 en cache en posicion 32<...>
<...>Almacenando mensaje con ID 4 en cache en posicion 16<...>
```


```bash
$ cat broker.log | grep "BUDDY SYSTEM"
<...>Eliminación de partición con posición de inicio: 0<...>
<...>Eliminación de partición con posición de inicio: 4<...>
<...>Eliminación de partición con posición de inicio: 32<...>
```

```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 5 en cache en posicion 32
```
