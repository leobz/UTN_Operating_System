### Pasos a seguir para importar y configurar entorno

### 1) Configuracion general
Crear archivo 'gameboy.config', puede ser una copia de 'gameboy.config.sample'
o tener su configuracion personalizada.

### 2) Importar Shared Library (

- Ir a File -> New -> C/C++ Project
- Elegir opcion All -> C Managed Build
- Hacer click en Next
- Ingresar el Project name "utils"
- Destildar la opcion Use default location
- Hacer click en Browse 
- Elegir el directorio donde estan las utils
- Elegir el Project type: Shared Library -> Empty Proyect
- Elegir la Toolchains: Linux GCC
- Hacer click en Finish

### 3) Configurar Shared Library (utils)

- Click derecho en el proyecto "utils"
- Elegir opcion Properties -> C/C++ Build -> Settings
- En la pestaña de Tool Settings, seleccionar GCC C Compiler ->  Miscellaneous
- Seleccionar la opcion Position Independent Code (-fPIC)
- Hacer click en Apply and Close
- Compilamos el proyecto "utils"
	- Click derecho en proyecto "utils"
 	- Elegir opcion Build Project

### 4) Importar proyecto GameBoy

- Ir a File -> New -> C/C++ Project
- Elegir opcion All -> C Managed Build
- Hacer click en Next
- Ingresar el Project name: gameboy
- Destildar la opcion Use default location
- Hacer click en Browse 
- Elegir el directorio del proyecto del modulo a importar
- Elegir el Project type: Executable -> Empty Proyect
- Elegir la Toolchains: Linux GCC
- Hacer click en Finish

### 5) Configurar proyecto GameBoy

- Click derecho en el proyecto gameboy
- Elegir opcion Properties:
	-  En C/C++ Build -> Settings -> GCC C Linker -> Libraries, agregar las librerias "commons", "cunit" y "utils"
	-  En C/C++ General -> Paths and Symbols -> References, tildar la opcion del proyecto "utils"
- Hacer click en Apply and Close

### 6) Run Configurations 

- Antes de correr el proyecto, ir a Run Configurations
- Hacer click derecho en C/C++ Applications -> New
- Setear el nombre: gameboy
- En la pestaña Environment, hacer click en New:
	- Name: LD_LIBRARY_PATH
	- {path_directorio_utils}/Debug
- Hacer click en Apply y luego Run





