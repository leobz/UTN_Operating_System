# Test Team

## Inicializacion

Cargo configuración del test

```shell
$ cat byexample.config                                                  # byexample: +fail-fast
sleep-time=<sleep-time>
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
<...>[AGREGADO]: Pikachu 6 6 [TOTAL EN MAPA]: 1<...>
<...>Tamaño de rafaga: 3  Posicion del TCB (5, 5)<...>
```

#### Se agrega al Mapa -> Se planifica entrenador

El entrenador mas cercano a (6,6) está en la posicion (5,5) asi que va a capturarlo.
Luego de capturarlo, pasa a la cola de ReadyToExchange, ya que capturo su maximo permitido

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 6 6
<...>[CAMBIO DE COLA] TID:2 (New->Ready) (5, 5) Motivo:CAPTURA
<...>[INSTRUCCION] TID:2, MOVIMIENTO Posición:(6, 5)
<...>[INSTRUCCION] TID:2, MOVIMIENTO Posición:(6, 6)
<...>[INSTRUCCION] TID:2, CATCH Pikachu 6 6
<...>[CAPTURA] TID:2 Capturó pokemon. Total capturados:2<...>
<...>[CAMBIO DE COLA] TID:2 (Exec->Ready to Exchange) (6, 6) Motivo:Capturó máximo permitido(2)
<...>[DEADLOCK] Inicio de detección de deadlock
<...>[DEADLOCK] No se detectó espera circular entre los TCBs
```

### APPEARED_POKEMON 2

#### Se agrega al Mapa -> Se planifica entrenador 

Como el entrandor capturo el pokemon, la cantidad en mapa volvio a 0.
Envio otro Pikachu (6,6). Quedan 2 entrenadores (1,1) y (3,3), como el segundo es el más cercano, va a capturarlo.

```bash
$  ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 6 6; sleep <sleep-time> # byexample: +timeout=4 +paste +fail-fast
<...>[AGREGADO]: Pikachu 6 6 [TOTAL EN MAPA]: 1<...>
<...>Tamaño de rafaga: 7  Posicion del TCB (3, 3)<...>
```

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 6 6
<...>[CAMBIO DE COLA] TID:1 (New->Ready) (3, 3) Motivo:CAPTURA
<...>[INSTRUCCION] TID:1, MOVIMIENTO Posición:(4, 3)
<...>[INSTRUCCION] TID:1, MOVIMIENTO Posición:(5, 3)
<...>[INSTRUCCION] TID:1, MOVIMIENTO Posición:(6, 3)
<...>[INSTRUCCION] TID:1, MOVIMIENTO Posición:(6, 4)
<...>[INSTRUCCION] TID:1, MOVIMIENTO Posición:(6, 5)
<...>[INSTRUCCION] TID:1, MOVIMIENTO Posición:(6, 6)
<...>[INSTRUCCION] TID:1, CATCH Pikachu 6 6
<...>[CAPTURA] TID:1 Capturó pokemon. Total capturados:2
<...>[CAMBIO DE COLA] TID:1 (Exec->Unblocked) (6, 6) Motivo:Capturó pokemon y puede seguir capturando

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
