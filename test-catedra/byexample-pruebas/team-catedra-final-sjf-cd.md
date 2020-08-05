## Prueba Final Team

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la planificación y el proceso Team en particular, sin la intervención de los demás módulos. La misma se encargará de verificar los distintos algoritmos de planificación, la detección y resolución de deadlocks.

## Actividades:

1) Iniciar el proceso con el algoritmo FIFO, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
2) Iniciar el proceso con el algoritmo RR con Quantum 2, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
3) Iniciar el proceso con el algoritmo SJF, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
4) Iniciar el proceso con el algoritmo SJF con Desalojo, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice

## Prueba SJF Con Desalojo

5) El funcionamiento del algoritmo SJF con Desalojo debería ser similar al SJF pero con distintas  métricas

```bash
$ cat team.log | grep "DEADLOCK"
<...>Se detectó espera circular entre  2 TCBs.<...>
<...>El entrenador TID: 2<...>TID: 0 para intercambiar<...>
```

```bash
$ cat team.log | grep "FIN"
<...>[FIN DEL PROCESO] ¡Team cumplió objetivo!
```
