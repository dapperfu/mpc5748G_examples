################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 12
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Generated_Code/subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include SDK/platform/devices/MPC5748G/startup/subdir.mk
-include SDK/platform/devices/subdir.mk
-include SDK/platform/drivers/src/interrupt/subdir.mk
-include SDK/platform/drivers/src/pins/subdir.mk
-include SDK/platform/drivers/src/sema42/subdir.mk
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
sema42_multicore_mpc5748g_z4_0.elf \

EXECUTABLES_QUOTED += \
"sema42_multicore_mpc5748g_z4_0.elf" \

EXECUTABLES_OS_FORMAT += \
sema42_multicore_mpc5748g_z4_0.elf \

SECONDARY_SIZE += \
sema42_multicore_mpc5748g_z4_0.siz \

SECONDARY_SIZE_QUOTED += \
"sema42_multicore_mpc5748g_z4_0.siz" \

SECONDARY_SIZE_OS_FORMAT += \
sema42_multicore_mpc5748g_z4_0.siz \


# All Target
all: warning-messages explicit-dependencies sema42_multicore_mpc5748g_z4_0.elf secondary-outputs

dependents:
	-cd "/home/jed/workspaceS32DS.Power.2017.R1/sema42_multicore_mpc5748g/sema42_multicore_mpc5748g_z2/Debug_FLASH" && $(MAKE) clean all
	-cd "/home/jed/workspaceS32DS.Power.2017.R1/sema42_multicore_mpc5748g/sema42_multicore_mpc5748g_z4_1/Debug_FLASH" && $(MAKE) clean all

# Tool invocations
sema42_multicore_mpc5748g_z4_0.elf: $(OBJS) /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/linker/gcc/MPC5748G_Z4_0/MPC5748G_flash.ld $(OSPT) $(USER_OBJS) /home/jed/workspaceS32DS.Power.2017.R1/sema42_multicore_mpc5748g/sema42_multicore_mpc5748g_z2/Debug_FLASH/sema42_multicore_mpc5748g_z2.elf /home/jed/workspaceS32DS.Power.2017.R1/sema42_multicore_mpc5748g/sema42_multicore_mpc5748g_z4_1/Debug_FLASH/sema42_multicore_mpc5748g_z4_1.elf
	@echo 'Building target: $@'
	@echo 'Executing target #13 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	powerpc-eabivle-gcc -o "sema42_multicore_mpc5748g_z4_0.elf" "@sema42_multicore_mpc5748g_z4_0.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) $(OSPT)
	@echo 'Finished building target: $@'
	@echo ' '

sema42_multicore_mpc5748g_z4_0.siz: $(EXECUTABLES)
	@echo 'Executing target #14 $@'
	@echo 'Invoking: Standard S32DS Print Size'
	powerpc-eabivle-size --format=berkeley $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*.d ./*/*/*/*/*/*.d ./*/*/*/*.d  ./*/*.o ./*/*/*/*/*/*.o ./*/*/*/*.o ./*/*/*.o  $(SECONDARY_SIZE_QUOTED) $(EXECUTABLES_QUOTED)
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:
/home/jed/workspaceS32DS.Power.2017.R1/sema42_multicore_mpc5748g/sema42_multicore_mpc5748g_z2/Debug_FLASH/sema42_multicore_mpc5748g_z2.elf:
/home/jed/workspaceS32DS.Power.2017.R1/sema42_multicore_mpc5748g/sema42_multicore_mpc5748g_z4_1/Debug_FLASH/sema42_multicore_mpc5748g_z4_1.elf:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

