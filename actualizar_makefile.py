import os
import re

# Directorios
root_tp   = os.path.abspath(os.getcwd())
path_utils= root_tp + "/utils"

def reemplazar_ruta_utils_en_makefile(path_makefile):
	reading_file = open(path_makefile, "r")

	new_file_content = ""
	
	while(True):
		linea = reading_file.readline()
		new_line = re.sub('/home.*/utils', path_utils, linea)
		new_file_content += new_line
		if not linea:
			break

	reading_file.close()
	writing_file = open(path_makefile, "w")
	writing_file.write(new_file_content)
	writing_file.close()

def actualizar_todos_los_makefiles():
	print("Reemplazando la ruta Utils/Debug dentro de todos los makefile...")
	reemplazar_ruta_utils_en_makefile("gamecard/Debug/makefile")
	reemplazar_ruta_utils_en_makefile("broker/Debug/makefile")
	reemplazar_ruta_utils_en_makefile("team/Debug/makefile")
	reemplazar_ruta_utils_en_makefile("gameboy/Debug/makefile")

	reemplazar_ruta_utils_en_makefile("broker/Debug/subdir.mk")
	reemplazar_ruta_utils_en_makefile("broker/Debug/test/subdir.mk")

	print("Finalizado")

 

actualizar_todos_los_makefiles()


