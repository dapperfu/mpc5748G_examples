################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 8
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
hello+pll+interrupt_Z2_3.elf \

EXECUTABLES_QUOTED += \
"hello+pll+interrupt_Z2_3.elf" \

EXECUTABLES_OS_FORMAT += \
hello+pll+interrupt_Z2_3.elf \


# All Target
all: warning-messages explicit-dependencies hello+pll+interrupt_Z2_3.elf secondary-outputs

# Tool invocations
hello+pll+interrupt_Z2_3.elf: $(OBJS) C:/Users/B55457/workspace.s32DS.DEVKIT-MPC5748G\ Rev-B/hello+pll+interrupt/hello+pll+interrupt_Z2_3/Project_Settings/Linker_Files/mem.ld C:/Users/B55457/workspace.s32DS.DEVKIT-MPC5748G\ Rev-B/hello+pll+interrupt/hello+pll+interrupt_Z2_3/Project_Settings/Linker_Files/sections.ld C:/Users/B55457/workspace.s32DS.DEVKIT-MPC5748G\ Rev-B/hello+pll+interrupt/hello+pll+interrupt_Z2_3/Project_Settings/Linker_Files/libs.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #9 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	powerpc-eabivle-gcc -o "hello+pll+interrupt_Z2_3.elf" "@hello+pll+interrupt_Z2_3.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*/*.o ./*/*.o  $(EXECUTABLES_QUOTED) ./*/*.d  C:\Users\B55457\workspace.s32DS.DEVKIT-MPC5748G Rev-B\hello+pll+interrupt\hello+pll+interrupt_Z2_3\Debug\hello+pll+interrupt_Z2_3.elf
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

