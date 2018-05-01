################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Sources/console/subdir.mk
-include Sources/common/tasks/subdir.mk
-include Sources/subdir.mk
-include SDK/rtos/osif/subdir.mk
-include SDK/platform/drivers/src/usdhc/subdir.mk
-include SDK/platform/drivers/src/rtc_c55/subdir.mk
-include SDK/platform/drivers/src/pins/subdir.mk
-include SDK/platform/drivers/src/interrupt/subdir.mk
-include SDK/platform/drivers/src/clock/MPC57xx/subdir.mk
-include SDK/platform/drivers/src/clock/subdir.mk
-include SDK/platform/devices/MPC5748G/startup/subdir.mk
-include SDK/platform/devices/subdir.mk
-include SDK/middleware/sdhc/usdhc/subdir.mk
-include SDK/middleware/sdhc/sd/subdir.mk
-include SDK/middleware/sdhc/fatfs/subdir.mk
-include SDK/middleware/fatfs/source/subdir.mk
-include SDK/middleware/fatfs/rtc/subdir.mk
-include SDK/middleware/fatfs/common/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include Generated_Code/subdir.mk
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
all: sdhc_fatfs_mpc5748g.elf secondary-outputs

# Tool invocations
sdhc_fatfs_mpc5748g.elf: $(OBJS) /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/linker/gcc/MPC5748G_flash.ld $(OSPT) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Standard S32DS C Linker'
	powerpc-eabivle-gcc -Xlinker --defsym=__stack_size__=0x2000 -T "/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/linker/gcc/MPC5748G_flash.ld" -Wl,-Map,"sdhc_fatfs_mpc5748g.map" -Xlinker --gc-sections -mcpu=e200z4 -specs=ewl_c9x.specs -mhard-float --sysroot="/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2" -o "sdhc_fatfs_mpc5748g.elf" $(OBJS) $(USER_OBJS) $(LIBS) $(OSPT)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) sdhc_fatfs_mpc5748g.elf
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
