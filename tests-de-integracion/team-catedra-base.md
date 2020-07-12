# Test Catedra: Team Base

## Inicializacion

Cargo configuración del test

```shell
$ cat byexample.config                                                  # byexample: +fail-fast
sleep-time=<sleep-time>
```

Modifico Archivo de configuracion como pide la catedra

```bash
$ cp team.config team.config.auxiliar; rm team.config; cp team_catedra_base.config team.config
<...>
```

Levanto Team en segundo plano.

```bash
$ cp team.config team_catedra_base.config; rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar  # byexample: +timeout=4 +norm-ws +paste -skip
<...>
```

```bash
$ ../team/Debug/team &  # byexample: +timeout=4 +norm-ws +paste -skip
[<job-id>] <pid>
```













## Finalizacion

Cerramos el proceso Team (de otra manera el puerto quedara en uso)

```bash
$ rm *.log; kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste -skip
[<job-id>]+ Term<...>
```

Restauro Archivo de configuración a su estado original

```bash
$ rm team.config; cp team.config.auxiliar team.config
<...>
```
