################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_common.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_hw_access.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_ic_driver.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_mc_driver.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_oc_driver.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_pwm_driver.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_common.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_hw_access.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_ic_driver.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_mc_driver.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_oc_driver.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_pwm_driver.c 

OBJS_OS_FORMAT += \
./SDK/platform/drivers/src/emios/emios_common.o \
./SDK/platform/drivers/src/emios/emios_hw_access.o \
./SDK/platform/drivers/src/emios/emios_ic_driver.o \
./SDK/platform/drivers/src/emios/emios_mc_driver.o \
./SDK/platform/drivers/src/emios/emios_oc_driver.o \
./SDK/platform/drivers/src/emios/emios_pwm_driver.o 

C_DEPS_QUOTED += \
"./SDK/platform/drivers/src/emios/emios_common.d" \
"./SDK/platform/drivers/src/emios/emios_hw_access.d" \
"./SDK/platform/drivers/src/emios/emios_ic_driver.d" \
"./SDK/platform/drivers/src/emios/emios_mc_driver.d" \
"./SDK/platform/drivers/src/emios/emios_oc_driver.d" \
"./SDK/platform/drivers/src/emios/emios_pwm_driver.d" 

OBJS += \
./SDK/platform/drivers/src/emios/emios_common.o \
./SDK/platform/drivers/src/emios/emios_hw_access.o \
./SDK/platform/drivers/src/emios/emios_ic_driver.o \
./SDK/platform/drivers/src/emios/emios_mc_driver.o \
./SDK/platform/drivers/src/emios/emios_oc_driver.o \
./SDK/platform/drivers/src/emios/emios_pwm_driver.o 

OBJS_QUOTED += \
"./SDK/platform/drivers/src/emios/emios_common.o" \
"./SDK/platform/drivers/src/emios/emios_hw_access.o" \
"./SDK/platform/drivers/src/emios/emios_ic_driver.o" \
"./SDK/platform/drivers/src/emios/emios_mc_driver.o" \
"./SDK/platform/drivers/src/emios/emios_oc_driver.o" \
"./SDK/platform/drivers/src/emios/emios_pwm_driver.o" 

C_DEPS += \
./SDK/platform/drivers/src/emios/emios_common.d \
./SDK/platform/drivers/src/emios/emios_hw_access.d \
./SDK/platform/drivers/src/emios/emios_ic_driver.d \
./SDK/platform/drivers/src/emios/emios_mc_driver.d \
./SDK/platform/drivers/src/emios/emios_oc_driver.d \
./SDK/platform/drivers/src/emios/emios_pwm_driver.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/emios/emios_common.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_common.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/emios/emios_common.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/emios/emios_common.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/emios/emios_hw_access.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_hw_access.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/emios/emios_hw_access.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/emios/emios_hw_access.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/emios/emios_ic_driver.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_ic_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/emios/emios_ic_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/emios/emios_ic_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/emios/emios_mc_driver.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_mc_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/emios/emios_mc_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/emios/emios_mc_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/emios/emios_oc_driver.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_oc_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/emios/emios_oc_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/emios/emios_oc_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/emios/emios_pwm_driver.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/emios/emios_pwm_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/emios/emios_pwm_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/emios/emios_pwm_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


