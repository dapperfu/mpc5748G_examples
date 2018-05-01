################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/main.c" \
"../Sources/run.c" \
"../Sources/setup.c" \
"../Sources/sys.c" 

C_SRCS += \
../Sources/main.c \
../Sources/run.c \
../Sources/setup.c \
../Sources/sys.c 

OBJS_OS_FORMAT += \
./Sources/main.o \
./Sources/run.o \
./Sources/setup.o \
./Sources/sys.o 

C_DEPS_QUOTED += \
"./Sources/main.d" \
"./Sources/run.d" \
"./Sources/setup.d" \
"./Sources/sys.d" 

OBJS += \
./Sources/main.o \
./Sources/run.o \
./Sources/setup.o \
./Sources/sys.o 

OBJS_QUOTED += \
"./Sources/main.o" \
"./Sources/run.o" \
"./Sources/setup.o" \
"./Sources/sys.o" 

C_DEPS += \
./Sources/main.d \
./Sources/run.d \
./Sources/setup.d \
./Sources/sys.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #46 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/run.o: ../Sources/run.c
	@echo 'Building file: $<'
	@echo 'Executing target #47 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/run.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/run.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/setup.o: ../Sources/setup.c
	@echo 'Building file: $<'
	@echo 'Executing target #48 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/setup.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/setup.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sys.o: ../Sources/sys.c
	@echo 'Building file: $<'
	@echo 'Executing target #49 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/sys.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/sys.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


