# Test Team

## Inicializacion

Cargo configuración del test

```shell
$ cat byexample.config                                                  # byexample: +fail-fast
sleep-time=<sleep-time>
```

Compilo Team

```bash
$ make clean -C ../team/Debug; make -C ../team/Debug; sleep <sleep-time> # byexample: +timeout=10 +fail-fast +paste
<...>Finished building target: team<...>
```

Eliminamos Logs antiguos y levantamos Team en segundo plano.

```bash
$ rm *.log; ../team/Debug/team &
<...>[<job-id>] <pid>
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
WARNING: Error al conectar con IP:<...> Puerto:<...>
[TCB-info] TID:0 Capturó pokemon. Total capturados:2
[TCB-info] TID:0 Capturó máximo permitido(2)
[TCB-info] TID:0 Pasó a lista Deadlock
```

#### Se agrega al Mapa -> Se planifica entrenador

El entrenador mas cercano a (6,6) está en la posicion (5,5) asi que va a capturarlo.

TODO: Arreglar logs para que sean mas expresivos (En el codigo y luego en el test)

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 6 6
<...>[CAMBIO ENTRENADOR] (NEW -> READY) MOTIVO:CAPTURA ID_ENTRENADOR:<...> POSICION:(5,5)
<...>[MOVIMIENTO] ID_ENTRENADOR:<...>, POSICION:(6, 5)
<...>[MOVIMIENTO] ID_ENTRENADOR:<...>, POSICION:(6, 6)
<...>[CATCH] POKEMON: Pikachu, POSICION:(6, 6)
```

### APPEARED_POKEMON 2

#### Misma especie -> Se aumenta cantidad

Envio otro Pikachu (6,6). Como ya habia uno, la cantidad aumenta a dos.
Quedan 2 entrenadores (1,1) y (3,3), como el segundo es el más cercano, va a capturarlo.

```bash
$  ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 6 6; sleep <sleep-time> # byexample: +timeout=4 +paste +fail-fast
[AGREGADO]: Pikachu 6 6 [TOTAL]: 2
Tamaño de rafaga: 7  Posicion del TCB (3, 3)
WARNING: Error al conectar con IP:<...> Puerto:<...>
[TCB-info] TID:0 Capturó pokemon. Total capturados:2
[TCB-info] TID:0 Pasó a lista Unblocked
```

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 6 6
<...>[CAMBIO ENTRENADOR] (NEW -> READY) MOTIVO:CAPTURA ID_ENTRENADOR:0 POSICION:(3,3)
<...>[MOVIMIENTO] ID_ENTRENADOR:0, POSICION:(4, 3)
<...>[MOVIMIENTO] ID_ENTRENADOR:0, POSICION:(5, 3)
<...>[MOVIMIENTO] ID_ENTRENADOR:0, POSICION:(6, 3)
<...>[MOVIMIENTO] ID_ENTRENADOR:0, POSICION:(6, 4)
<...>[MOVIMIENTO] ID_ENTRENADOR:0, POSICION:(6, 5)
<...>[MOVIMIENTO] ID_ENTRENADOR:0, POSICION:(6, 6)
<...>[CATCH] POKEMON: Pikachu, POSICION:(6, 6)
```

#### Pokemon No Pertenece a Objetivo Global -> No se agrega

Envío Charmander.  
Como no pertenece al Objetivo Global no se agrega.

```bash
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Charmander 1 1; sleep <sleep-time> # byexample: +timeout=4 +paste +fail-fast
```

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Charmander 1 1
```

## Finalizacion

Cerramos el proceso Team (de otra manera el puerto quedara en uso)

```bash
$ rm *.log; kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste -skip
[<job-id>]+ Term<...>
```
