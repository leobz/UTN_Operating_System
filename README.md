# tp-2020-1c-UNIX

![img-portada-utnso](https://www.utnso.com.ar/wp-content/uploads/2017/11/main-logo-utnso.png)

- - - - - - - - - - - - - - -

## Ficha Técnica

* Nombre del proyecto: Delibird
* Nombre del equipo: UNIX
* Tiempo de desarrollo: 3 meses
* Lenguajes utilizados: C, Bash, Python
* Enunciado/Requerimientos: [LINK](https://docs.google.com/document/d/19vU1ztYVl_6QmbjWz6DXAKHA1F1z8dyLcxsg12YhCR0/edit)
* Documentación: [LINK](https://github.com/leobz/SO-UNIX-Delibird/wiki)
* Integrantes: 
  * [Leo Bazán](https://www.linkedin.com/in/bazanotin/): Lider Técnico. Team, Broker, GameCard, GameBoy
  * [Diego Perez](https://github.com/dprez8): Broker, GameCard, GameBoy
  * [Eze Laime](https://www.linkedin.com/in/josu%C3%A9-ezequiel-laime-calizaya-98184a138/): Team, Broker
  * [Eze Gauna](https://www.linkedin.com/in/ezequielgauna/): GameCard, GameBoy

## Módulos del sistema

### Broker
Recibe y distribuye los mensajes entre los distintos módulos. Utiliza tecnología de Colas de Mensajes (MQ), en la cuál existen módulos suscriptores que esperan por mensajes específicos.

### Team 
Es un planificador de procesos, donde cada proceso es un entrenador que compite por usar ciclos de CPU. Un team, a su vez compite con otros teams por recursos compartidos que se encuentran en los gamecards. (Spoiler: ¡Los recursos son los pókemons!)

Un módulo Team cumple su objetivo, cuando todos sus entrenadores lo hayan hecho (Los entrenadores cumplen su objetivo cuando capturan a todos lo ~~bichos~~ pókemons que necesitan)

### Gamecard
Es el FileSystem, que almacena y gestiona todos los pokemones en el mapa. Permite crear, editar y borrar archivos.

### Gameboy

Envía mensajes a todos los módulos (publisher), y es el tester del sistema. Permite crear todos los mensajes del sistema: crear nuevos pokemones en el mapa, solicitar capturas,confirmar capturas, etc.


## Instalación

1) Instalar proyecto
```shell
$ sh instalacion.sh
```

2) Reiniciar Terminal para que se apliquen las variables de entorno (Basta con abrir otra terminal)

3) Se pueden levantar los módulos individualmente (los ejecutables estan dentro de la carpeta 'nombre-modulo/Debug/nombre-modulo') ó ejecutar los tests automatizados


## Pruebas automatizadas

Nota: Link al [Documento de pruebas](https://docs.google.com/document/d/1_MHl52R-wPyL4SaWYok4Crf47dVwlqZOQn1MWouo3T8/edit)

1) Instalar byexample: Es una herramienta que permite ejecutar comandos de bash y comprobar los logs de los módulos, permitiendo automatizar las ejecuciones y las comprobaciones. Pueden ver el siguiente [ejemplo de test](https://github.com/leobz/SO-UNIX-Delibird/blob/master/tests-de-integracion/team-broker.md) usado para comprobar la integración entre el Team y el Broker... =>
[Guía de instalación](https://github.com/leobz/SO-UNIX-Delibird/blob/master/tests-de-integracion/README.md)

2) Ejecutar tests básicos:

```shell
$ cd tests-de-integracion
$ sh broker.sh
$ sh team.sh
$ sh regresion.sh
```

3) Ejecutar tests 
```shell
$ cd test-catedra
$ sh <nombre-de-prueba>.sh
```

