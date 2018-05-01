################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/eee/source/eee_common.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/eee/source/eee_driver.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/eee/source/eee_common.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/eee/source/eee_driver.c 

OBJS_OS_FORMAT += \
./SDK/middleware/eee/source/eee_common.o \
./SDK/middleware/eee/source/eee_driver.o 

C_DEPS_QUOTED += \
"./SDK/middleware/eee/source/eee_common.d" \
"./SDK/middleware/eee/source/eee_driver.d" 

OBJS += \
./SDK/middleware/eee/source/eee_common.o \
./SDK/middleware/eee/source/eee_driver.o 

OBJS_QUOTED += \
"./SDK/middleware/eee/source/eee_common.o" \
"./SDK/middleware/eee/source/eee_driver.o" 

C_DEPS += \
./SDK/middleware/eee/source/eee_common.d \
./SDK/middleware/eee/source/eee_driver.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/middleware/eee/source/eee_common.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/eee/source/eee_common.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/eee/source/eee_common.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/eee/source/eee_common.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/middleware/eee/source/eee_driver.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/eee/source/eee_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/eee/source/eee_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/eee/source/eee_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


