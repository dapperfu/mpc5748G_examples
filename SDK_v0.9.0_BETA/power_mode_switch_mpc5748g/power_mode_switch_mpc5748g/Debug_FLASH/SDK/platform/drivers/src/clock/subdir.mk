################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/clock/clock_manager.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/clock/clock_manager.c 

OBJS_OS_FORMAT += \
./SDK/platform/drivers/src/clock/clock_manager.o 

C_DEPS_QUOTED += \
"./SDK/platform/drivers/src/clock/clock_manager.d" 

OBJS += \
./SDK/platform/drivers/src/clock/clock_manager.o 

OBJS_QUOTED += \
"./SDK/platform/drivers/src/clock/clock_manager.o" 

C_DEPS += \
./SDK/platform/drivers/src/clock/clock_manager.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/clock/clock_manager.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/clock/clock_manager.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/clock/clock_manager.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/drivers/src/clock/clock_manager.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


