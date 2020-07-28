# Test Team-Broker

## Introducción

En este test demostramos:

### Suscripcion de TEAM a colas APPEARED y CAUGHT del broker

1) Suscripcion automatica de Team a las 3 colas

### Normal ejecucion de TEAM para APPEARED

1) Envio de mensaje APPEARED de: Gameboy => Broker => Team
2) Team recibe mensaje APPEARED, envia al entrenador mas cercano y hace un CATCH a Broker
3) Team recibe ID_CORRELATIVO

### Normal ejecucion de TEAM para CAUGHT

1) Envio de mensaje CAUGHT de: Gameboy => Broker => Team
2) Team recibe CAUGHT con ID_CORRELATIVO valido y asigna pokemon a entrenador
 
## Inicializacion del Test

Cargo configuración del test

```shell
$ cat byexample.config      # byexample: +fail-fast
sleep-time=<sleep-time>
```

Levanto Broker en segundo plano

```bash
$ ../broker/Debug/broker &   # byexample: +fail-fast
<...>[<job-broker-id>] <broker-pid>
```

- - - - - - - - - - - - -

## TEST 1: Suscripcion de TEAM a colas APPEARED, LOCALIZED y CAUGHT del broker

Levanto Team en segundo plano

```bash
$ ../team/Debug/team &       # byexample: +fail-fast
<...>[<job-team-id>] <team-pid>
```

Comprobación : Suscripcion automatica de Team a la cola APPEARED_POKEMON

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>[SUSCRIPCION] Cola:APPEARED_POKEMON ID_Proceso:<...>
```

Comprobación : Suscripcion automatica de Team a la cola CAUGHT_POKEMON

```bash
$ cat broker.log
<...>[SUSCRIPCION] Cola:CAUGHT_POKEMON ID_Proceso:<...>
```

Comprobación : Suscripcion automatica de Team a la cola LOCALIZED_POKEMON

```bash
$ cat broker.log
<...>[SUSCRIPCION] Cola:LOCALIZED_POKEMON ID_Proceso:<...>
```
## TEST 2: Normal ejecucion de TEAM para APPEARED

Envio de mensaje APPEARED de: Gameboy => Broker => Team

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER APPEARED_POKEMON Pikachu 6 6 10; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```

Comprobación:  
Team recibe mensaje APPEARED, envia al entrenador mas cercano y hace un CATCH a Broker  
Team recibe ID_CORRELATIVO

```bash
$  sleep <sleep-time>; cat team.log     # byexample: +timeout=10 +fail-fast +paste
<...>[MSG_RECIBIDO] APPEARED_POKEMON:<...>
<...>[INSTRUCCION]<...>CATCH<...>
<...>[MSG_RECIBIDO] ID_CORRELATIVO para CATCH:<ID-CORRELATIVO>
<...>
```

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>recibido CATCH_POKEMON<...>
```

### TEST 3: Normal ejecucion de TEAM para CAUGHT

Envio de mensaje CAUGHT de: Gameboy => Broker => Team

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER CAUGHT_POKEMON <ID-CORRELATIVO> OK; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```
Team recibe CAUGHT con ID_CORRELATIVO invalido y asigna pokemon a entrenador

```bash
$ sleep 1; cat team.log    # byexample: +timeout=10 +paste
<...>[MSG_RECIBIDO] CAUGHT_POKEMON: ID_Correlativo:<ID-CORRELATIVO> Resultado:1
<...>[CAMBIO DE COLA]<...>
```

## Finalización de Tests

Cierro broker, gameboy suscriptor y team (De otro modo los puertos quedan sin poder usarse)

```bash
$ rm *.log; kill <broker-pid> ; kill <team-pid>; sleep <sleep-time>     # byexample: +timeout=20 +norm-ws +paste -skip
<...>Term<...>
<...>Term<...>
```
