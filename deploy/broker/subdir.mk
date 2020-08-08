################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../administracion-de-mensajes.c \
../broker-commons.c \
../cache.c \
../colas.c \
../main.c \
../server.c 

OBJS += \
./administracion-de-mensajes.o \
./broker-commons.o \
./cache.o \
./colas.o \
./main.o \
./server.o 

C_DEPS += \
./administracion-de-mensajes.d \
./broker-commons.d \
./cache.d \
./colas.d \
./main.d \
./server.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/leo/Programacion/C/EclipseWorkSpace/tp-2020-1c-UNIX/utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


