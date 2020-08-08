# Test Gamecard-Broker

## Introducción

En este test demostramos: 
 
1) Gamecard se suscribe a las colas de Broker (NEW_POKEMON, GET_POKEMON CATCH_POKEMON)
2) Gameboy envia New Pokemon a Broker
3) Broker establece conexion con Gamecard
4) Gamecard procesa New Pokemon

## Inicializacion

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

Levanto Gamecard en segundo plano

```bash
$ ../gamecard/Debug/gamecard &       # byexample: +fail-fast
<...>[<job-gamecard-id>] <gamecard-pid>
```

- - - - - - - - - - - - -

## 1 Suscripción de Gamecard a las colas de Broker

Suscripción

> Como a penas se inicia Gammecard se suscribe a las tres colas solamente procedemos a verificar si es asi.

Comprobación

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>[SUSCRIPCION] Cola:NEW_POKEMON<...>
<...>[SUSCRIPCION] Cola:GET_POKEMON<...>
<...>[SUSCRIPCION] Cola:CATCH_POKEMON<...>
```

## 2 New Pokemon de Gameboy a Broker

Envio de mensaje NEW_POKEMON

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER NEW_POKEMON "Pikachu" 2 5 1; sleep <sleep-time>   # byexample: +timeout=10 +paste
<...>
```

Comprobacion

```bash
$  sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>[CONEXION] COD_OP:NEW_POKEMON<...>
```

## 3 Broker establece conexion con Gamecard

Conexion

> Como recibido el mensaje desde Gameboy, Broker debe enviar el mensaje a Gamecard.

Comprobacion

```bash
$  sleep <sleep-time>; cat gamecard.log    # byexample: +timeout=10 +paste
<...>[CONEXION] COD_OP:NEW_POKEMON<...>
```

<!-- Hay que hacer una comprobacion de confirmacion -->
