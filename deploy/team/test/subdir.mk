################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../test/common_testing.c \
../test/test_algoritmos_sjf.c \
../test/test_algoritmos_sjf_cd.c \
../test/test_appeared.c \
../test/test_caught.c \
../test/test_deadlock.c \
../test/test_entrenador_cercano_a_ready.c \
../test/test_lista_blocked.c \
../test/test_lista_ready.c \
../test/test_localized.c \
../test/test_movimiento_entrenador.c \
../test/test_rafagas.c \
../test/test_unblocked_a_ready.c \
../test/testing.c 

OBJS += \
./test/common_testing.o \
./test/test_algoritmos_sjf.o \
./test/test_algoritmos_sjf_cd.o \
./test/test_appeared.o \
./test/test_caught.o \
./test/test_deadlock.o \
./test/test_entrenador_cercano_a_ready.o \
./test/test_lista_blocked.o \
./test/test_lista_ready.o \
./test/test_localized.o \
./test/test_movimiento_entrenador.o \
./test/test_rafagas.o \
./test/test_unblocked_a_ready.o \
./test/testing.o 

C_DEPS += \
./test/common_testing.d \
./test/test_algoritmos_sjf.d \
./test/test_algoritmos_sjf_cd.d \
./test/test_appeared.d \
./test/test_caught.d \
./test/test_deadlock.d \
./test/test_entrenador_cercano_a_ready.d \
./test/test_lista_blocked.d \
./test/test_lista_ready.d \
./test/test_localized.d \
./test/test_movimiento_entrenador.d \
./test/test_rafagas.d \
./test/test_unblocked_a_ready.d \
./test/testing.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/leo/Repos/UTN-OS/utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


