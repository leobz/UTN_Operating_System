## Prueba Final Gamecard

## Disclaimer:

El objetivo de la prueba es verificar el correcto funcionamiento del File System individualmente. Para esto, se requerirá la ejecución de una instancia del Proceso GameCard y de los scripts que provee la cátedra.

## Actividades:

1) Ejecutar Script consolidacion_basico.sh
2) Se debe llenar la memoria y luego realizar una suscripción. Finalizada la misma se realiza un nuevo mensaje CATCH
3) Bajo FIFO se eliminan las tres primeras particiones consolidándose y se guarda el último mensaje en la posición 0
4) Bajar el sistema, cambiar algoritmo de reemplazo a LRU y volverlo a levantar ejecutando el script del paso 1.
5) Al ejecutar la suscripción se actualizan los tiempos de LRU de los primeros dos mensajes por lo que se deben borrar los dos siguientes (Catch Pikachu y Catch Squirtle), guardándose posteriormente el nuevo mensaje en la posición 8.

## Resultados Esperados
1) Validar el funcionamiento de la consolidación para el algoritmo de reemplazo FIFO.
2) Validar el funcionamiento de la consolidación para el algoritmo de reemplazo LRU.

## Finalizacion por cumplir objetivo

