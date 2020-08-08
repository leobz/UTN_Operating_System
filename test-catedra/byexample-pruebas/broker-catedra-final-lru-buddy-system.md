## Prueba Final Broker LRU Buddy System

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento del esquema de memoria Buddy System. Para esto, será necesario la ejecución del proceso broker y de los scripts proporcionados por la cátedra.

## Actividades:

1) Bajar sistema, cambiar algoritmo de reemplazo a LRU y volverlo a levantar ejecutando el script.
2) Se debe cargar los mensajes en las siguientes posiciones de memoria:
	-Caught 0
	-Caught 4
	-New pikachu 32
	-Catch onyx 16
3) Bajo LRU se eliminaran la siguientes particiones:
	- Caught 0
	- Caught 4
	- Catch Onyx 16
4) Se guarda Catch Charmander en la posición 0.


## Prueba LRU

1) Validar el funcionamiento del sistema con el esquema de Buddy System para el algoritmo de reemplazo LRU


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
<...>Eliminación de partición con posición de inicio: 16<...>
```

```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 5 en cache en posicion 0
```
