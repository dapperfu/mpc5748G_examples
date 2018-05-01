################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/source/ff.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/source/ffunicode.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/source/ff.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/source/ffunicode.c 

OBJS_OS_FORMAT += \
./SDK/middleware/fatfs/source/ff.o \
./SDK/middleware/fatfs/source/ffunicode.o 

C_DEPS_QUOTED += \
"./SDK/middleware/fatfs/source/ff.d" \
"./SDK/middleware/fatfs/source/ffunicode.d" 

OBJS += \
./SDK/middleware/fatfs/source/ff.o \
./SDK/middleware/fatfs/source/ffunicode.o 

OBJS_QUOTED += \
"./SDK/middleware/fatfs/source/ff.o" \
"./SDK/middleware/fatfs/source/ffunicode.o" 

C_DEPS += \
./SDK/middleware/fatfs/source/ff.d \
./SDK/middleware/fatfs/source/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/middleware/fatfs/source/ff.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/source/ff.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/fatfs/source/ff.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/fatfs/source/ff.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/middleware/fatfs/source/ffunicode.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/source/ffunicode.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/fatfs/source/ffunicode.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/fatfs/source/ffunicode.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


