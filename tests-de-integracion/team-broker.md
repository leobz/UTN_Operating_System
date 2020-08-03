# Test Team-Broker

## Introducción

En este test demostramos:

1) suscribirnos con el Gameboy a la cola Catch del Broker
2) mandar un Appeared Pokémon de Gameboy a Team
3) team seleccionara al entrenador más cercano al Pokémon
4) una vez seleccionado, el entrenador se moverá X cantidad de posiciones al Pokémon a capturar
5) una vez llegado a esa posición, se mandará un mensaje Catch a Broker para capturar ese Pokémon
6) cómo Gameboy estaba suscripto a la cola Catch, el broker le tiene que mandar el mensaje que recibió en dicha cola (el mensaje Catch enviado por Team)

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

Levanto Team en segundo plano

```bash
$ sleep <sleep-time>; ../team/Debug/team &       # byexample: +fail-fast +paste
<...>[<job-team-id>] <team-pid>
```

- - - - - - - - - - - - -

## 1 Suscripción de Gameboy a cola Catch del Broker

Suscripción

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy SUSCRIPCION CATCH_POKEMON 200 120 & # byexample: +timeout=100 +fail-fast +paste
<...>[<job-gameboy-id>] <gameboy-pid>
```

Comprobación

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste +fail-fast
<...>[SUSCRIPCION] Cola:CATCH_POKEMON ID_Proceso:120<...>
```

## 2 Mandar un Appeared Pokémon de Gameboy a Team

Envio el mensaje APPEARED_POKEMON

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 6 6 10; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```

Comprobación de recepción de mensaje Appeared Pokemon, selección de TCB, movimiento de TCB y envío de mensaje Catch al Broker (Pasos 3, 4 y 5)

```bash
$  sleep <sleep-time>; cat team.log     # byexample: +timeout=10 +fail-fast +paste
<...>[MSG_RECIBIDO] APPEARED_POKEMON:<...>
<...>[INSTRUCCION]<...>CATCH<...>
<...>[MSG_RECIBIDO] CONFIRMACION: CATCH_POKEMON<...>
```

## 5 Team envía el mensaje Catch al Broker correctamente

Comprobación de recepción de APPEARED y envio de CATCH

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>recibido CATCH_POKEMON<...>
<...>enviado CATCH_POKEMON<...>
```

## 6 Cómo Gameboy estaba suscripto a la cola Catch, el broker le reenviará el mensaje Catch

Comprobación

```bash
$ sleep <sleep-time>; cat gameboy.log   # byexample: +timeout=10 +fail-fast +paste
<...>recibido de [Broker]: CATCH_POKEMON Pikachu 6 6<...>
```

## Finalización

Cierro broker, gameboy suscriptor y team (De otro modo los puertos quedan sin poder usarse)

```bash
$ rm *.log ; kill <broker-pid> ;  kill <gameboy-pid>; kill <team-pid>; sleep <sleep-time>     # byexample: +timeout=20 +norm-ws +paste -skip
<...>Hecho<...>
<...>Terminado<...>
<...>Terminado<...>
``` 
