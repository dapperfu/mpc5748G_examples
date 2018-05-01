################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/pal/timing/src/timing_irq.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/pal/timing/src/timing_pal.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/pal/timing/src/timing_irq.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/pal/timing/src/timing_pal.c 

OBJS_OS_FORMAT += \
./SDK/platform/pal/timing/src/timing_irq.o \
./SDK/platform/pal/timing/src/timing_pal.o 

C_DEPS_QUOTED += \
"./SDK/platform/pal/timing/src/timing_irq.d" \
"./SDK/platform/pal/timing/src/timing_pal.d" 

OBJS += \
./SDK/platform/pal/timing/src/timing_irq.o \
./SDK/platform/pal/timing/src/timing_pal.o 

OBJS_QUOTED += \
"./SDK/platform/pal/timing/src/timing_irq.o" \
"./SDK/platform/pal/timing/src/timing_pal.o" 

C_DEPS += \
./SDK/platform/pal/timing/src/timing_irq.d \
./SDK/platform/pal/timing/src/timing_pal.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/pal/timing/src/timing_irq.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/pal/timing/src/timing_irq.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/pal/timing/src/timing_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/pal/timing/src/timing_irq.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/pal/timing/src/timing_pal.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/pal/timing/src/timing_pal.c
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/pal/timing/src/timing_pal.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/platform/pal/timing/src/timing_pal.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


