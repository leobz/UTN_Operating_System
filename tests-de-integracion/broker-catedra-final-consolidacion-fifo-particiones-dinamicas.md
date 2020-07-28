## Prueba Final Broker Consolidacion FIFO Particiones Dinamicas

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la consolidación de particiones dinámicas del proceso broker independiente de los demás. Para esto, será necesario la ejecución del proceso broker y de scripts proporcionados por la cátedra.

## Actividades:

1) Ejecutar Script consolidacion_basico.sh
2) Se debe llenar la memoria y luego realizar una suscripción. Finalizada la misma se realiza un nuevo mensaje NEW.
3) Bajo FIFO se eliminan las tres primeras particiones consolidándose y se guarda el último mensaje en la posición 0.

## Prueba FIFO

1) Validar el funcionamiento de la consolidación para el algoritmo de reemplazo FIFO.


```bash
$ cat broker.log | grep "PARTICIONES DINAMICAS"
<...>Eliminación de partición con posición de inicio: 0<...>
<...>Eliminación de partición con posición de inicio: 4<...>
<...>Eliminación de partición con posición de inicio: 8<...>
```

```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 6 en cache en posicion 0
```
