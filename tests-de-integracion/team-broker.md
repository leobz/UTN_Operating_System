# Test Team-Broker

## Introducción

En este test demostramos:

1) Suscribirnos con el Gameboy a la cola Catch del Broker
2) Mandar un Appeared Pokémon de Gameboy a Team
3) Team seleccionara al entrenador más cercano al Pokémon
4) Una vez seleccionado, el entrenador se moverá X cantidad de posiciones al Pokémon a capturar
5) una vez llegado a esa posición, se mandará un mensaje Catch a Broker para capturar ese Pokémon
6) como Gameboy estaba suscripto a la cola Catch, el broker le tiene que mandar el mensaje que recibió en dicha cola (el mensaje Catch enviado por Team)

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
$ ../team/Debug/team &       # byexample: +fail-fast
<...>[<job-team-id>] <team-pid>
```

- - - - - - - - - - - - -

## Suscripcion del Gameboy a CATCH

Suscripción

```  bash
$ ../gameboy/Debug/gameboy SUSCRIPCION CATCH_POKEMON 200 & # byexample: +timeout=100 +fail-fast +paste
[<job-gameboy-id>] <gameboy-pid>
```

Comprobación

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>[SUSCRIPCION] Cola:CATCH_POKEMON<...>
```

## APPEARED_POKEMON a Team

Envio el mensaje APPEARED_POKEMON

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 6 6 10; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```

Comprobación de recepción de APPEARED y envio de CATCH

```bash
$  sleep <sleep-time>; cat team.log     # byexample: +timeout=10 +fail-fast +paste
<...>[MSG_RECIBIDO] APPEARED_POKEMON:<...>
<...>[CATCH] POKEMON<...>
```

## CATCH Team a Broker

Comprobación de recepción de APPEARED y envio de CATCH

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>recibido CATCH_POKEMON<...>
<...>enviado CATCH_POKEMON<...>
```

## CATCH Broker a Suscriptor

Comprobación

```bash
$ sleep <sleep-time>; cat gameboy.log   # byexample: +timeout=10 +fail-fast +paste
<...>recibido de [Broker]: CATCH_POKEMON Pikachu 6 6<...>
```

## Finalización

Cierro broker (De otro modo el puerto queda sin poder usarse)

```bash
$ rm *.log; kill <gameboy-pid> ; kill <broker-pid> ; kill <team-pid>; sleep <sleep-time>     # byexample: +timeout=20 +norm-ws +paste -skip
<...>Term<...>
<...>Term<...>
<...>Term<...>
```
