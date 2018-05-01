################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/main.c" \
"../Sources/test.c" 

C_SRCS += \
../Sources/main.c \
../Sources/test.c 

OBJS_OS_FORMAT += \
./Sources/main.o \
./Sources/test.o 

C_DEPS_QUOTED += \
"./Sources/main.d" \
"./Sources/test.d" 

OBJS += \
./Sources/main.o \
./Sources/test.o 

OBJS_QUOTED += \
"./Sources/main.o" \
"./Sources/test.o" 

C_DEPS += \
./Sources/main.d \
./Sources/test.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/test.o: ../Sources/test.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/test.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/test.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


