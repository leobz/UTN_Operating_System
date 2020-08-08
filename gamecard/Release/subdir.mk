################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bloques.c \
../directorios.c \
../gamecard.c \
../main.c \
../servidor_gamecard.c 

OBJS += \
./bloques.o \
./directorios.o \
./gamecard.o \
./main.o \
./servidor_gamecard.o 

C_DEPS += \
./bloques.d \
./directorios.d \
./gamecard.d \
./main.d \
./servidor_gamecard.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


