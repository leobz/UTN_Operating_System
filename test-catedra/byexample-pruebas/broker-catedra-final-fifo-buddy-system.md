## Prueba Final Broker FIFO Buddy System

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento del esquema de memoria Buddy System. Para esto, será necesario la ejecución del proceso broker y de los scripts proporcionados por la cátedra.

## Actividades:

1) Ejecutar Script buddy_basico.sh
2) Se deben cargar los mensajes en las siguientes posiciones de memoria:
	- Caught 0
	- Caught 4
	- New pikachu 32
	- Catch onyx 16
3) Bajo FIFO, se eliminan las siguientes particiones:
	- Caught 0
	- Caught 4
	- New pikachu 32
4) Se guarda el Catch Charmander en la posición 32

## Prueba FIFO

1) Validar el funcionamiento del sistema con el esquema de Buddy System para el algoritmo de reemplazo FIFO


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
