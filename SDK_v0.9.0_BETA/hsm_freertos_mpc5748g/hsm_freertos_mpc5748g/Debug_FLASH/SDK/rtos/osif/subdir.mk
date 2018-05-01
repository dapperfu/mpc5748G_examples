################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/osif_freertos.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/osif_freertos.c 

OBJS_OS_FORMAT += \
./SDK/rtos/osif/osif_freertos.o 

C_DEPS_QUOTED += \
"./SDK/rtos/osif/osif_freertos.d" 

OBJS += \
./SDK/rtos/osif/osif_freertos.o 

OBJS_QUOTED += \
"./SDK/rtos/osif/osif_freertos.o" 

C_DEPS += \
./SDK/rtos/osif/osif_freertos.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/osif/osif_freertos.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/osif_freertos.c
	@echo 'Building file: $<'
	@echo 'Executing target #34 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/osif/osif_freertos.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/osif/osif_freertos.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


