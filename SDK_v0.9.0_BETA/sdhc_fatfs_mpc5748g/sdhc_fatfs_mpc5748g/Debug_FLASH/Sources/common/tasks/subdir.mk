################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/common/tasks/task_fatfs_diskio.c \
../Sources/common/tasks/task_fatfs_f_open_f_read.c \
../Sources/common/tasks/task_fatfs_f_open_f_write.c \
../Sources/common/tasks/task_fatfs_f_opendir_f_readdir.c \
../Sources/common/tasks/task_fatfs_fdisk_mkfs.c \
../Sources/common/tasks/task_sd_eraseblocks.c \
../Sources/common/tasks/task_sd_writeblocks_readblocks.c \
../Sources/common/tasks/task_sdhc_init_deinit.c 

OBJS += \
./Sources/common/tasks/task_fatfs_diskio.o \
./Sources/common/tasks/task_fatfs_f_open_f_read.o \
./Sources/common/tasks/task_fatfs_f_open_f_write.o \
./Sources/common/tasks/task_fatfs_f_opendir_f_readdir.o \
./Sources/common/tasks/task_fatfs_fdisk_mkfs.o \
./Sources/common/tasks/task_sd_eraseblocks.o \
./Sources/common/tasks/task_sd_writeblocks_readblocks.o \
./Sources/common/tasks/task_sdhc_init_deinit.o 

C_DEPS += \
./Sources/common/tasks/task_fatfs_diskio.d \
./Sources/common/tasks/task_fatfs_f_open_f_read.d \
./Sources/common/tasks/task_fatfs_f_open_f_write.d \
./Sources/common/tasks/task_fatfs_f_opendir_f_readdir.d \
./Sources/common/tasks/task_fatfs_fdisk_mkfs.d \
./Sources/common/tasks/task_sd_eraseblocks.d \
./Sources/common/tasks/task_sd_writeblocks_readblocks.d \
./Sources/common/tasks/task_sdhc_init_deinit.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/common/tasks/%.o: ../Sources/common/tasks/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc -std=c99 -DSTART_FROM_FLASH -DCPU_MPC5748G -DTURN_ON_CPU0 -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2/EWL_C/include/pa" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/include" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G/startup" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/MPC5748G" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/devices/common" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/inc" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/rtos/osif/" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Generated_Code" -I"/home/jed/workspaceS32DS.Power.2017.R1/sdhc_fatfs_mpc5748g/sdhc_fatfs_mpc5748g/Sources" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/platform/drivers/src/clock/MPC57xx" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs/source" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/fatfs" -I"/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/S32_SDK_MPC574xx_BETA_0.9.0/middleware/sdhc" -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -ffunction-sections -fdata-sections -mcpu=e200z4 -specs=ewl_c9x.specs -mbig -mvle -mregnames -mhard-float --sysroot="/opt/NXP/S32DS_Power_v2017.R1/eclipse/../S32DS/e200_ewl2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


