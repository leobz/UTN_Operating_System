## Prueba Final Broker Compactación FIFO Particiones Dinamicas

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la compactación de particiones dinámicas del proceso broker independiente de los demás. Para esto, será necesario la ejecución del proceso broker y de scripts proporcionados por la cátedra.

## Actividades:

1) Ejecutar Script compactacion_basico.sh
2) Se debe llenar la memoria y luego realizar una suscripción. Finalizada la misma se realiza un nuevo mensaje NEW.
3) Bajo FIFO se eliminan las primera partición, se ejecuta la compactación, y se repite el mismo proceso 2 veces más. Al finalizar, se almacena el mensaje en la posición 36.


## Prueba FIFO

1) Validar el funcionamiento de la compactación para el algoritmo de reemplazo FIFO.


```bash
$ cat broker.log | grep "PARTICIONES DINAMICAS"
<...>Eliminación de partición con posición de inicio: 0<...>
<...>Compactación de particiones<...>
<...>Eliminación de partición con posición de inicio: 0<...>
<...>Compactación de particiones<...>
<...>Eliminación de partición con posición de inicio: 0<...>
<...>Compactación de particiones<...>
```

```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 6 en cache en posicion 36