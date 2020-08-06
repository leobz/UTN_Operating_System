################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../test/test_appeared_pokemon.c \
../test/test_catch_pokemon.c \
../test/test_caught_pokemon.c \
../test/test_configuracion.c \
../test/test_get_pokemon.c \
../test/test_new_pokemon.c \
../test/test_parcer.c \
../test/test_utils.c \
../test/testing.c 

OBJS += \
./test/test_appeared_pokemon.o \
./test/test_catch_pokemon.o \
./test/test_caught_pokemon.o \
./test/test_configuracion.o \
./test/test_get_pokemon.o \
./test/test_new_pokemon.o \
./test/test_parcer.o \
./test/test_utils.o \
./test/testing.o 

C_DEPS += \
./test/test_appeared_pokemon.d \
./test/test_catch_pokemon.d \
./test/test_caught_pokemon.d \
./test/test_configuracion.d \
./test/test_get_pokemon.d \
./test/test_new_pokemon.d \
./test/test_parcer.d \
./test/test_utils.d \
./test/testing.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/leo/Programacion/C/EclipseWorkSpace/tp-2020-1c-UNIX/utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


