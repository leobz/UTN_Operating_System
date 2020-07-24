################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../test/test_peticiones_gamecard.c 

OBJS += \
./test/test_peticiones_gamecard.o 

C_DEPS += \
./test/test_peticiones_gamecard.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/diego/TPCuatrimestral/tp-2020-1c-UNIX/utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" -lm
	@echo 'Finished building: $<'
	@echo ' '


