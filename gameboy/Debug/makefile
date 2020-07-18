################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include test/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: gameboy

dependents:
	-cd /home/leo/Música/tp-2020-1c-UNIX/utils/Debug && $(MAKE) clean all

# Tool invocations
gameboy: $(OBJS) $(USER_OBJS) /home/leo/Música/tp-2020-1c-UNIX/utils/Debug/libutils.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/leo/Música/tp-2020-1c-UNIX/utils/Debug" -o "gameboy" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) gameboy
	-@echo ' '

.PHONY: all clean dependents
/home/leo/Música/tp-2020-1c-UNIX/utils/Debug/libutils.so:

-include ../makefile.targets
