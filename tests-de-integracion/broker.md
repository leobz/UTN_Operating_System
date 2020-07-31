# Test de Broker MEnsajes

## Inicializacion

Cargo configuración del test

```shell
$ cat byexample.config                                        # byexample: +fail-fast
sleep-time=<sleep-time>
```

Levanto el Broker en segundo plano

```bash
$ ../broker/Debug/broker &
[<job-broker-id>] <broker-pid>
```

- - - - - - - - - - - - -

## Suscripción y envío de mensajes Catch

Suscripción a CATCH_POKEMON

```  bash
$ ../gameboy/Debug/gameboy SUSCRIPCION CATCH_POKEMON 200 120 & # byexample: +timeout=100 +fail-fast +paste
[<job-gameboy-id>] <gameboy-pid>
```

Comprobación de suscripción

```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=10 +paste
<...>[SUSCRIPCION] Cola:CATCH_POKEMON ID_Proceso:120<...>
```

Envió Catch Pokemon y compruebo que le haya llegado al gameboy que estaba suscripto

```  bash
$ ../gameboy/Debug/gameboy BROKER CATCH_POKEMON Pikachu 6 6 # byexample: +timeout=10 +fail-fast
<...>
```

Comprobación

```bash
$ sleep <sleep-time>; cat gameboy.log   # byexample: +timeout=10 +fail-fast +paste
<...>recibido de [Broker]: CATCH_POKEMON Pikachu 6 6<...>
```

## Suscripción desde otro modulo recibe el mensaje catcheado

Ahora hacemos que otro gameboy se suscriba a la cola Catch, al detectar un nuevo suscriptor, el broker le enviará el mensaje anterior, que estarpa guardado en la memoria caché.

```  bash
$ ../gameboy/Debug/gameboy SUSCRIPCION CATCH_POKEMON 200 121 & # byexample: +timeout=100 +fail-fast +paste
[<job-gameboy-id2>] <gameboy-pid2>
```

Comprobamos que el gameboy haya recibido nuevamente el CATCH_POKEMON

```bash
$ sleep 1; cat gameboy.log   # byexample: +timeout=10 +fail-fast +paste
<...>recibido de [Broker]: CATCH_POKEMON Pikachu 6 6<...>
<...>recibido de [Broker]: CATCH_POKEMON Pikachu 6 6<...>
```

## Finalización

Cierro broker y gameboy (De otro modo los puertos quedan sin poder usarse)

```bash
$ rm *.log; kill <gameboy-pid> ; kill <gameboy-pid2> ; kill <broker-pid> ; sleep <sleep-time>     # byexample: +timeout=20 +norm-ws +paste -skip
<...>Hecho<...>
<...>Term<...>
<...>Term<...>
```
