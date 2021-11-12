# tp-2020-1c-UNIX

## Ficha Técnica

* Nombre del proyecto: Delibird
* Nombre del equipo: UNIX
* Tiempo de desarrollo: 80 dias
* Lenguajes utilizados: C, Bash, Python
* Enunciado/Requerimientos: [LINK](https://docs.google.com/document/d/1be91Gn93O2Vp8frZoV1i5CmtOG0scE1PS8dMHsCP314/edit)
* Integrantes: 
  * [Leo Bazán](https://www.linkedin.com/in/bazanotin/): Lider Técnico. Team, Broker, GameCard, GameBoy
  * [Eze Laime](https://www.linkedin.com/in/ezequiel-laime/): Team, Broker
  * [Eze Gauna](https://www.linkedin.com/in/ezequielgauna/): GameCard, GameBoy
  * [Diego Perez](https://github.com/dprez8): Broker, GameCard, GameBoy


## Resumen

Delibird es un sistema distribuido que simula el comportamiento de un sistema operativo, se tocan temas tales como algoritmos de gestion de memoria, algoritmos de planificación de procesos, sistemas de archivos, kernel, etc. Todos los módulos trabajan de forma concurrente y deben poder pasar 25 tests de integración que simulen distintos comportamientos en base a distintas configuraciones.

Se aclara que estas son versiones simplificadas o alteradas de los componentes reales de hardware y de sistemas operativos modernos, a fin de resaltar aspectos de diseño y permitir que los mismos sean realizables dentro del marco de un cuatrimestre.


## Módulos del sistema


### Team 
Es un planificador de procesos, donde cada proceso es un entrenador que compite por usar ciclos de CPU. Soporta varios algoritmos de planificación los cuales son parametrizables, un sistema de estados y un gestor de resolución de  deadlocks. 

El objetivo de un team es que todos los entrenadores que formen parte de el, capturen a los pokemons que necesitan. 

Un team, a su vez compite con otros teams por recursos compartidos que se encuentran en los gamecards. (Spoiler: ¡Los recursos son los pókemons!)

Un módulo Team cumple su objetivo, cuando todos sus entrenadores lo hayan hecho (Los entrenadores cumplen su objetivo cuando capturan a todos lo ~~bichos~~ pókemons que necesitan)

Algoritmos de planificación soportados: FIFO, Round Robin y Shortest job first con y sin desalojo



### Gamecard
Es el FileSystem, que almacena y gestiona todos los pokemones en el mapa. Permite crear, editar y borrar archivos.
Los archivos están conformados por bloques (Por ej 1.bin, 2.bin, 3.bin) y representados por un archivo de metadata al igual que en un Sistema de archivos real.

### Gameboy

Envía mensajes a todos los módulos (publisher), y es el tester del sistema. Permite crear todos los mensajes del sistema: crear nuevos pokemones en el mapa, solicitar capturas,confirmar capturas, etc.

### Broker

Recibe y distribuye los mensajes entre los distintos módulos. Utiliza tecnología de Colas de Mensajes (MQ), en la cuál existen módulos suscriptores que esperan por mensajes específicos.
Soporta e implementa por parametrización los siguientes mecanismos de gestión de memoria:
  - Particiones dinámicas con compactación
  - Buddy System
  - Permite realizar dump de la Caché

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

