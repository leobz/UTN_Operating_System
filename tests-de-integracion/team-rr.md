# Test Team con Round Robin

## Inicializacion

Cargo configuración del test

```shell
$ cat byexample.config                                                  # byexample: +fail-fast
sleep-time=<sleep-time>
```

Modifico Archivo de configuracion para que el algoritmo sea Round Robin y no FIFO

```bash
$ sed -i 's/FIFO/RR/g' team.config
```

Levanto Team en segundo plano.

```bash
$ ../team/Debug/team &
[<job-id>] <pid>
```

## Tests

En el **Objetivo Global** hay: **3 Pikachus** y **2 Squirtles**

Posición entrenadores: (1,1) (3,3) (5,5)

### APPEARED_POKEMON 1

- - -

#### Pokemon Pertenece a Objetivo Global -> Se agrega al Mapa

Envío Pikachu en posicion (6,6). Como pertenece al Objetivo global lo agrego al mapa.

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 6 6; sleep <sleep-time> # byexample: +timeout=7 +paste +fail-fast
[AGREGADO]: Pikachu 6 6 [TOTAL]: 1
Tamaño de rafaga: 3  Posicion del TCB (5, 5)
```

#### Se agrega al Mapa -> Se planifica entrenador

El entrenador mas cercano a (6,6) está en la posicion (5,5) asi que va a capturarlo.
Cómo la ráfaga es de 3 instrucciones y el Quantum es de 2, el tcb no termina de ejecutar toda su ráfaga, y pasará a Ready con una instrucción pendiente.

TODO: Arreglar logs para que sean mas expresivos (En el codigo y luego en el test)

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 6 6<...>
<...>[MOVIMIENTO] ID_ENTRENADOR:2, POSICION:(6, 5)<...>
<...>[MOVIMIENTO] ID_ENTRENADOR:2, POSICION:(6, 6)<...>
<...>[TCB-info] TID:2 Pasó a lista Ready<...>
```

### APPEARED_POKEMON 2

#### Misma especie -> Se aumenta cantidad

Envio otro Pikachu (6,6). Como ya habia uno, la cantidad aumenta a dos.
Quedan 2 entrenadores (1,1) y (3,3), como el segundo es el más cercano, va a capturarlo.

```bash
$  ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 6 6; sleep <sleep-time> # byexample: +timeout=4 +paste +fail-fast
[AGREGADO]: Pikachu 6 6 [TOTAL]: 2
Tamaño de rafaga: 7  Posicion del TCB (3, 3)
```

Cómo la ráfaga es de 7 instrucciones pero el quantum de 2, el entrenador ejecuta y pasa a Ready con 5 instrucciones pendientes.

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 6 6<...>
<...>[MOVIMIENTO] ID_ENTRENADOR:1, POSICION:(4, 3)<...>
<...>[MOVIMIENTO] ID_ENTRENADOR:1, POSICION:(5, 3)<...>
<...>[TCB-info] TID:1 Pasó a lista Ready<...>
```

## Finalizacion

Cerramos el proceso Team (de otra manera el puerto quedara en uso)

```bash
$ rm *.log; kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste -skip
[<job-id>]+ Term<...>
```

Restauro Archivo de configuración a su estado original

```bash
$ sed -i 's/RR/FIFO/g' team.config
```
