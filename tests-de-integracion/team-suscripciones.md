# Test Team-Broker

## Inicializacion del Test

Cargo configuración del test

```shell
$ cat byexample.config      # byexample: +fail-fast
sleep-time=<sleep-time>
```

Elimino logs antiguos

```shell
$ rm *.log      # byexample: +fail-fast
<...>
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


-------------------------------------------------------------------------------------

## TEST 0: Suscripcion de TEAM a colas APPEARED, LOCALIZED y CAUGHT del broker

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

-------------------------------------------------------------------------------------

### TEST 1: Normal ejecucion de TEAM para LOCALIZED

Al iniciar Team:  
Envio de mensaje GET por cada especie: Team => Broker
Envio de Confirmacion de GET: Broker => Team

```bash
$ sleep 1; cat team.log    # byexample: +timeout=10 +fail-fast
<...>CONFIRMACION: GET_POKEMON Pokemon:Squirtle ID_Correlativo:<ID-MSJ-SQUIRTLE>
<...>CONFIRMACION: GET_POKEMON Pokemon:Pikachu ID_Correlativo:<...>
```

-------------------------------------------------------------------------------------

### TEST 1.A: LOCALIZED con id INVALIDO no es procesado

Envio de mensaje LOCALIZED Squirtle con ID **Invalido** de: Gameboy => Broker => Team

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER LOCALIZED_POKEMON Squirtle 2 1 1 2 2 999; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```

La ultima linea de team.log **no indica** que hubo procesamiento posterior al test 4

```bash
$ sleep 1; tail -1 team.log    # byexample: +timeout=10 +fail-fast
<...>CONFIRMACION: GET_POKEMON Pokemon:Pikachu ID_Correlativo:<...>
```

-------------------------------------------------------------------------------------

### TEST 1.B: LOCALIZED con id VALIDO es procesado

Envio de mensaje LOCALIZED Squirtle con ID **Valido** de: Gameboy => Broker => Team

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER LOCALIZED_POKEMON Squirtle 2 1 1 2 2 <ID-MSJ-SQUIRTLE>; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```

Team procesa solicitud LOCALIZED

```bash
$ sleep <sleep-time>; cat team.log    # byexample: +timeout=10 +fail-fast +paste
<...>Motivo:CAPTURA de Squirtle (1,1)<...>
<...>Motivo:CAPTURA de Squirtle (2,2)<...>
```

-------------------------------------------------------------------------------------

## TEST 2: Normal ejecucion de TEAM para APPEARED

Envio de mensaje APPEARED Pikachu de: Gameboy => Broker => Team

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER APPEARED_POKEMON Pikachu 6 6 10; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```

Team recibe APPEARED => Planifica entrenador => envia CATCH a Broker  
Envio de mensaje ID_CORRELATIVO de: Broker => Team 

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

-------------------------------------------------------------------------------------

### TEST 3: Normal ejecucion de TEAM para CAUGHT

Envio de mensaje CAUGHT de: Gameboy => Broker => Team

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER CAUGHT_POKEMON <ID-CORRELATIVO> OK; sleep <sleep-time> # byexample: +timeout=8 +paste
<...>
```
Team recibe CAUGHT => Comprueba que ID_CORRELATIVO es valido => Asigna pokemon a entrenador

```bash
$ sleep 1; cat team.log    # byexample: +timeout=10 +paste
<...>[MSG_RECIBIDO] CAUGHT_POKEMON: ID_Correlativo:<ID-CORRELATIVO> Resultado:1
<...>[CAPTURA]<...>
```

-------------------------------------------------------------------------------------


## Finalización de Tests

Cierro broker, gameboy suscriptor y team (De otro modo los puertos quedan sin poder usarse)

```bash
$ rm *.log; kill <broker-pid> ; kill <team-pid>; sleep <sleep-time>     # byexample: +timeout=20 +norm-ws +paste -skip
<...>Hecho<...>
<...>Term<...>
```
