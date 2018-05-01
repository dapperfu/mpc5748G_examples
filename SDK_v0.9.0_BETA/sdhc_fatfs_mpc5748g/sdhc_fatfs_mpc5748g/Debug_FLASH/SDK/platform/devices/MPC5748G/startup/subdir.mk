################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup/system_MPC5748G.c 

OBJS += \
./SDK/platform/devices/MPC5748G/startup/system_MPC5748G.o 

C_DEPS += \
./SDK/platform/devices/MPC5748G/startup/system_MPC5748G.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/devices/MPC5748G/startup/system_MPC5748G.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup/system_MPC5748G.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc -std=c99 -DSTART_FROM_FLASH -DCPU_MPC5748G -DTURN_ON_CPU0 -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include/pa" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/common" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/inc" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Generated_Code" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Sources" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/clock/MPC57xx" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/source" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc" -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -ffunction-sections -fdata-sections -mcpu=e200z4 -specs=ewl_c9x.specs -mbig -mvle -mregnames -mhard-float --sysroot="/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


