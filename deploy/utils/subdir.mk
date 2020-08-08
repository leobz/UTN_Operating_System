################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cliente.c \
../config.c \
../diccionarios.c \
../listas.c \
../log.c \
../serializacion.c \
../servidor.c 

OBJS += \
./cliente.o \
./config.o \
./diccionarios.o \
./listas.o \
./log.o \
./serializacion.o \
./servidor.o 

C_DEPS += \
./cliente.d \
./config.d \
./diccionarios.d \
./listas.d \
./log.d \
./serializacion.d \
./servidor.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


