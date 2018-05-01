################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/edmaTransfer.c" \
"../Sources/main.c" 

C_SRCS += \
../Sources/edmaTransfer.c \
../Sources/main.c 

OBJS_OS_FORMAT += \
./Sources/edmaTransfer.o \
./Sources/main.o 

C_DEPS_QUOTED += \
"./Sources/edmaTransfer.d" \
"./Sources/main.d" 

OBJS += \
./Sources/edmaTransfer.o \
./Sources/main.o 

OBJS_QUOTED += \
"./Sources/edmaTransfer.o" \
"./Sources/main.o" 

C_DEPS += \
./Sources/edmaTransfer.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/edmaTransfer.o: ../Sources/edmaTransfer.c
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/edmaTransfer.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/edmaTransfer.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


