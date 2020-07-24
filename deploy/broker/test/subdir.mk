################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../test/test_administrador_de_mensajes.c \
../test/test_algoritmos_dinamicas.c \
../test/test_buddy_system.c \
../test/test_cache_particiones_dinamicas.c \
../test/test_configuration.c \
../test/test_new_pokemon.c \
../test/testing.c 

OBJS += \
./test/test_administrador_de_mensajes.o \
./test/test_algoritmos_dinamicas.o \
./test/test_buddy_system.o \
./test/test_cache_particiones_dinamicas.o \
./test/test_configuration.o \
./test/test_new_pokemon.o \
./test/testing.o 

C_DEPS += \
./test/test_administrador_de_mensajes.d \
./test/test_algoritmos_dinamicas.d \
./test/test_buddy_system.d \
./test/test_cache_particiones_dinamicas.d \
./test/test_configuration.d \
./test/test_new_pokemon.d \
./test/testing.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/leo/Programacion/C/EclipseWorkSpace/pruebaTP/tp-2020-1c-UNIX/utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


