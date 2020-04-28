## Correr programa por primera vez

### 1) Configuracion general
Crear archivo 'gamecard.config', puede ser una copia de 'gamecard.config.sample'
o tener su configuracion personalizada.

### 2) Configuracion de Eclipse

#### a-Linkear librerias generales
Botón derecho en el proyecto -> Properties -> C/C++ Build -> Settings -> GCC C Linker ->
Libraries
... y agregamos las librerías 'cunit', 'commons' y 'utils' en “Libraries”.


#### b-Configurar Shared Folder Utils
Botón derecho en el proyecto -> Properties -> C/C++ General -> Path and Symbols -> Preferences
... y seleccionar 'utils'.

#### c-Run Configuration
Antes de correr el proyecto ir a:
~~~
Run Configurations -> {RunConfiguration del proyecto} -> Enviroment -> New 
~~~

Se abrira una ventana, ingresar:
~~~
Name: LD_LIBRARY_PATH  
Value:  {Path de la libreria utils}/Debug
~~~
