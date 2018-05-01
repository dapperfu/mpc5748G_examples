################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sd.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sdmmc.c" \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sdspi.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sd.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sdmmc.c \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sdspi.c 

OBJS_OS_FORMAT += \
./SDK/middleware/sdhc/sd/sd.o \
./SDK/middleware/sdhc/sd/sdmmc.o \
./SDK/middleware/sdhc/sd/sdspi.o 

C_DEPS_QUOTED += \
"./SDK/middleware/sdhc/sd/sd.d" \
"./SDK/middleware/sdhc/sd/sdmmc.d" \
"./SDK/middleware/sdhc/sd/sdspi.d" 

OBJS += \
./SDK/middleware/sdhc/sd/sd.o \
./SDK/middleware/sdhc/sd/sdmmc.o \
./SDK/middleware/sdhc/sd/sdspi.o 

OBJS_QUOTED += \
"./SDK/middleware/sdhc/sd/sd.o" \
"./SDK/middleware/sdhc/sd/sdmmc.o" \
"./SDK/middleware/sdhc/sd/sdspi.o" 

C_DEPS += \
./SDK/middleware/sdhc/sd/sd.d \
./SDK/middleware/sdhc/sd/sdmmc.d \
./SDK/middleware/sdhc/sd/sdspi.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/middleware/sdhc/sd/sd.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sd.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/sdhc/sd/sd.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/sdhc/sd/sd.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/middleware/sdhc/sd/sdmmc.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sdmmc.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/sdhc/sd/sdmmc.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/sdhc/sd/sdmmc.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/middleware/sdhc/sd/sdspi.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc/sd/sdspi.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/sdhc/sd/sdspi.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/sdhc/sd/sdspi.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


