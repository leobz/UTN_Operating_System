# Delibird: Operating System Simulator

**Nota**: Encuentra la versión en español en el [siguiente link](https://github.com/leobz/SO-UNIX-Delibird/blob/master/SPANISH_README.md)

**Note**: All the codebase of this project is written in Spanish due to this project had an academic purpose.


## Technical Specs

* Project name: Delibird
* Team name: UNIX
* Development time: 2 months
* Languages used: C, Bash, Python
* University: 
* Statement/Requirements: [LINK](https://docs.google.com/document/d/1be91Gn93O2Vp8frZoV1i5CmtOG0scE1PS8dMHsCP314/edit)
* Team members: 
  * [Leo Bazán](https://www.linkedin.com/in/bazanotin/): Technical leader. Team, Broker, GameCard, GameBoy
  * [Eze Laime](https://www.linkedin.com/in/ezequiel-laime/): Team, Broker
  * [Eze Gauna](https://www.linkedin.com/in/ezequielgauna/): GameCard, GameBoy
  * [Diego Perez](https://github.com/dprez8): Broker, GameCard, GameBoy

## Summary

Delibird is a distributed system that simulates the behavior of an operating system, covering topics such as memory management algorithms, process scheduling algorithms, file systems, kernels, etc. All modules work concurrently and must be able to pass 25 integration tests that simulate different behaviors based on different configurations.

It should be noted that these are simplified or altered versions of real hardware components and modern operating systems, in order to highlight design aspects and allow them to be feasible within the framework of a semester.

## System Modules

### Team 

It is a process scheduler, where each process is a trainer that competes for CPU cycles. It supports several scheduling algorithms which are parametrizable, a state system, and a deadlock resolution manager.

The objective of a team is that all the trainers that are part of it, capture the Pokémon they need.

A team, in turn, competes with other teams for shared resources found in the gamecards. (Spoiler: The resources are the Pokémon!)

A Team module fulfills its objective when all its trainers have done so (Trainers fulfill their objective when they capture all the bugs Pokémon they need).

Supported scheduling algorithms: FIFO, Round Robin, and Shortest Job First with and without preemption.

### Gamecard

It is the FileSystem, which stores and manages all the Pokémon in the map. It allows creating, editing, and deleting files. The files are made up of blocks (e.g., 1.bin, 2.bin, 3.bin) and represented by a metadata file just like in a real file system.

### Gameboy

Sends messages to all modules (publisher), and is the system tester. It allows creating all the system messages: creating new Pokémon on the map, requesting captures, confirming captures, etc.

### Broker

Receives and distributes messages among the different modules. It uses Message Queues (MQ) technology, in which there are subscriber modules waiting for specific messages.
It supports and implements by parametrization the following memory management mechanisms:
  - Dynamic partitions with compaction
  - Buddy System
  - Allows cache dump

## Installation

1) Install project

```shell
$ sh instalacion.sh
```

2) Restart Terminal so that environment variables are applied (Just open another terminal)

3) Modules can be started individually (the executables are inside the 'module-name/Debug/module-name' folder) or run automated tests

## Automated Tests

Note: Link to the [Testing Document](https://docs.google.com/document/d/1_MHl52R-wPyL4SaWYok4Crf47dVwlqZOQn1MWouo3T8/edit)

1) Install byexample: It is a tool that allows running bash commands and checking module logs, enabling the automation of executions and checks. You can see the following [test example](https://github.com/leobz/SO-UNIX-Delibird/blob/master/tests-de-integracion/team-broker.md)  used to verify the integration between the Team and the Broker... =>
[Installation guide](https://github.com/leobz/SO-UNIX-Delibird/blob/master/tests-de-integracion/README.md)


1) Run basic tests:

```shell
$ cd tests-de-integracion
$ sh broker.sh
$ sh team.sh
$ sh regresion.sh
```

3) Run tests:

```shell
$ cd test-catedra
$ sh <nombre-de-prueba>.sh
```

