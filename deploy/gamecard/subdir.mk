################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bitmap.c \
../bloques.c \
../directorios.c \
../gamecard.c \
../main.c \
../servidor_gamecard.c 

OBJS += \
./bitmap.o \
./bloques.o \
./directorios.o \
./gamecard.o \
./main.o \
./servidor_gamecard.o 

C_DEPS += \
./bitmap.d \
./bloques.d \
./directorios.d \
./gamecard.d \
./main.d \
./servidor_gamecard.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/leo/Repos/UTN-OS/utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" -lm
	@echo 'Finished building: $<'
	@echo ' '


