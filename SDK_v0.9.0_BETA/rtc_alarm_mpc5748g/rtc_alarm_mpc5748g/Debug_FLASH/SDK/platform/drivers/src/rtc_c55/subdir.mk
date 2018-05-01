################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_driver.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_hw_access.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_irq.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_driver.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_hw_access.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_irq.c 

OBJS_OS_FORMAT += \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o 

C_DEPS_QUOTED += \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.d" \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.d" \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.d" 

OBJS += \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o 

OBJS_QUOTED += \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o" \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o" \
"./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o" 

C_DEPS += \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.d \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.d \
./SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/rtc_c55/rtc_c55_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_hw_access.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/rtc_c55/rtc_c55_hw_access.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/rtc_c55/rtc_c55_irq.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/rtc_c55/rtc_c55_irq.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


