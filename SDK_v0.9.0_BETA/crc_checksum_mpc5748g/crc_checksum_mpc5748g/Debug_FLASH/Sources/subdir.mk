################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/input_data.c" \
"../Sources/main.c" 

C_SRCS += \
../Sources/input_data.c \
../Sources/main.c 

OBJS_OS_FORMAT += \
./Sources/input_data.o \
./Sources/main.o 

C_DEPS_QUOTED += \
"./Sources/input_data.d" \
"./Sources/main.d" 

OBJS += \
./Sources/input_data.o \
./Sources/main.o 

OBJS_QUOTED += \
"./Sources/input_data.o" \
"./Sources/main.o" 

C_DEPS += \
./Sources/input_data.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/input_data.o: ../Sources/input_data.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/input_data.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/input_data.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


