################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/osif_baremetal.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/osif_baremetal.c 

OBJS_OS_FORMAT += \
./SDK/rtos/osif/osif_baremetal.o 

C_DEPS_QUOTED += \
"./SDK/rtos/osif/osif_baremetal.d" 

OBJS += \
./SDK/rtos/osif/osif_baremetal.o 

OBJS_QUOTED += \
"./SDK/rtos/osif/osif_baremetal.o" 

C_DEPS += \
./SDK/rtos/osif/osif_baremetal.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/osif/osif_baremetal.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/osif_baremetal.c
	@echo 'Building file: $<'
	@echo 'Executing target #23 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/rtos/osif/osif_baremetal.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/rtos/osif/osif_baremetal.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


