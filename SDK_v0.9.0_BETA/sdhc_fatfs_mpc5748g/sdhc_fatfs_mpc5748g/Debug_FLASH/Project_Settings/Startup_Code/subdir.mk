################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup/gcc/core0_intc_sw_handlers.S \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup/gcc/interrupt_vectors.S \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup/gcc/startup_MPC5748G.S 

OBJS += \
./Project_Settings/Startup_Code/core0_intc_sw_handlers.o \
./Project_Settings/Startup_Code/interrupt_vectors.o \
./Project_Settings/Startup_Code/startup_MPC5748G.o 


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/core0_intc_sw_handlers.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup/gcc/core0_intc_sw_handlers.S
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include/pa" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/common" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/inc" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Generated_Code" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Sources" -x assembler-with-cpp -DTURN_ON_CPU0 -O0 -g3 -mcpu=e200z4 -specs=ewl_c9x.specs -mbig -mvle -mregnames --sysroot="/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/interrupt_vectors.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup/gcc/interrupt_vectors.S
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include/pa" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/common" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/inc" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Generated_Code" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Sources" -x assembler-with-cpp -DTURN_ON_CPU0 -O0 -g3 -mcpu=e200z4 -specs=ewl_c9x.specs -mbig -mvle -mregnames --sysroot="/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/startup_MPC5748G.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup/gcc/startup_MPC5748G.S
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS Assembler'
	powerpc-eabivle-gcc -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include/pa" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/common" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/inc" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Generated_Code" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Sources" -x assembler-with-cpp -DTURN_ON_CPU0 -O0 -g3 -mcpu=e200z4 -specs=ewl_c9x.specs -mbig -mvle -mregnames --sysroot="/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


