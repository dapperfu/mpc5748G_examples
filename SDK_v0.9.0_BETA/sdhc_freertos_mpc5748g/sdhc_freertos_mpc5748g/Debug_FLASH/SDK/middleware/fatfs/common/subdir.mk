################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/common/fatfs_impl.c" 

C_SRCS += \
/opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/common/fatfs_impl.c 

OBJS_OS_FORMAT += \
./SDK/middleware/fatfs/common/fatfs_impl.o 

C_DEPS_QUOTED += \
"./SDK/middleware/fatfs/common/fatfs_impl.d" 

OBJS += \
./SDK/middleware/fatfs/common/fatfs_impl.o 

OBJS_QUOTED += \
"./SDK/middleware/fatfs/common/fatfs_impl.o" 

C_DEPS += \
./SDK/middleware/fatfs/common/fatfs_impl.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/middleware/fatfs/common/fatfs_impl.o: /opt/NXP/S32DS_Power_v2017.R1/S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/common/fatfs_impl.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/middleware/fatfs/common/fatfs_impl.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "SDK/middleware/fatfs/common/fatfs_impl.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


