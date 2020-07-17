#
#
# Este archivo debe llamarse siempre desde el root del TP
#

import os
import re

# Directorios
root_tp   = os.path.abspath(os.getcwd())
path_deploy = root_tp + "/deploy"
path_utils= root_tp + "/utils/Debug"


def ruta_makefile(module_makefile):
    return root_tp + "/" + module_makefile + "/Debug/makefile"

def reemplazar_makefile_base(module_makefile):
	reading_file = open(ruta_makefile(module_makefile), "r")

	new_file_content = ""
	
	while(True):
		linea = reading_file.readline()
		new_file_content += linea
		if not linea:
			break

	reading_file.close()
	writing_file = open(path_deploy + "/" + module_makefile + "_makefile", "w")
	writing_file.write(new_file_content)
	writing_file.close()

def actualizar_todos_los_makefiles():
	print("Actualizando makefiles base desde makefile reales...")
	reemplazar_makefile_base("gamecard")
	reemplazar_makefile_base("broker")
	reemplazar_makefile_base("team")
	reemplazar_makefile_base("gameboy")
	reemplazar_makefile_base("utils")

	print("Finalizado")


actualizar_todos_los_makefiles()


