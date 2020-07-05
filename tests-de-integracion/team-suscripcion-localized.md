# Test Team-Broker

## Introducción

En este test demostramos:

### Suscripcion de TEAM a cola APPEARED del broker

1) Suscripcion automatica de Team a la cola APPEARED del Broker
2) Envio de mensaje APPEARED de Gameboy a Broker
3) Broker reenvia el mensaje APPEARED hacia Team

### Normal ejecucion de TEAM

1) Team recibe mensaje APPEARED, envia al entrenador mas cercano y hace un CATCH a Broker

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

## Test 1: Suscripcion de TEAM a cola APPEARED del broker

Levanto Team en segundo plano => Suscripcion automatica de Team a la cola APPEARED del Broker

```bash
$ ../team/Debug/team &       # byexample: +fail-fast
<...>[<job-team-id>] <team-pid>
```

Comprobación

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>[SUSCRIPCION] Cola:APPEARED_POKEMON ID_Proceso:<...>
```

Envio de mensaje APPEARED de Gameboy a Broker => Broker reenvia el mensaje APPEARED a Team

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER APPEARED_POKEMON Pikachu 6 6 10; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```

Comprobación

```bash
$  sleep <sleep-time>; cat team.log     # byexample: +timeout=10 +fail-fast +paste
<...>[MSG_RECIBIDO] APPEARED_POKEMON:<...>
```

## Test 2: Normal ejecucion de Team

1) Team recibe mensaje APPEARED, envia al entrenador mas cercano y hace un CATCH a Broker

Comprobación de recepción de APPEARED y envio de CATCH

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>recibido CATCH_POKEMON<...>
```

## Finalización de Tests

Cierro broker, gameboy suscriptor y team (De otro modo los puertos quedan sin poder usarse)

```bash
$ rm *.log; kill <broker-pid> ; kill <team-pid>; sleep <sleep-time>     # byexample: +timeout=20 +norm-ws +paste -skip
<...>Term<...>
<...>Term<...>
```
