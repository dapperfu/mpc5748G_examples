################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 4
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Debug_Configurations/subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Sources/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
flexcan_mpc5748g.elf \

EXECUTABLES_QUOTED += \
"flexcan_mpc5748g.elf" \

EXECUTABLES_OS_FORMAT += \
flexcan_mpc5748g.elf \

SECONDARY_SIZE += \
flexcan_mpc5748g.siz \

SECONDARY_SIZE_QUOTED += \
"flexcan_mpc5748g.siz" \

SECONDARY_SIZE_OS_FORMAT += \
flexcan_mpc5748g.siz \


# All Target
all: warning-messages explicit-dependencies flexcan_mpc5748g.elf secondary-outputs

# Tool invocations
flexcan_mpc5748g.elf: $(OBJS) /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/linker/gcc/MPC5748G_flash.ld $(OSPT) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #5 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	powerpc-eabivle-gcc -o "flexcan_mpc5748g.elf" "@flexcan_mpc5748g.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) $(OSPT)
	@echo 'Finished building target: $@'
	@echo ' '

flexcan_mpc5748g.siz: $(EXECUTABLES)
	@echo 'Executing target #6 $@'
	@echo 'Invoking: Standard S32DS Print Size'
	powerpc-eabivle-size --format=berkeley $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*.d  ./*/*.o ./*/*/*.o  $(SECONDARY_SIZE_QUOTED) $(EXECUTABLES_QUOTED)
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

