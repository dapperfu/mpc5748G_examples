################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/console/echoEmulation.c" \
"../Sources/console/ringBuffer.c" \
"../Sources/console/uart.c" \
"../Sources/console/uart_console_io.c" 

C_SRCS += \
../Sources/console/echoEmulation.c \
../Sources/console/ringBuffer.c \
../Sources/console/uart.c \
../Sources/console/uart_console_io.c 

OBJS_OS_FORMAT += \
./Sources/console/echoEmulation.o \
./Sources/console/ringBuffer.o \
./Sources/console/uart.o \
./Sources/console/uart_console_io.o 

C_DEPS_QUOTED += \
"./Sources/console/echoEmulation.d" \
"./Sources/console/ringBuffer.d" \
"./Sources/console/uart.d" \
"./Sources/console/uart_console_io.d" 

OBJS += \
./Sources/console/echoEmulation.o \
./Sources/console/ringBuffer.o \
./Sources/console/uart.o \
./Sources/console/uart_console_io.o 

OBJS_QUOTED += \
"./Sources/console/echoEmulation.o" \
"./Sources/console/ringBuffer.o" \
"./Sources/console/uart.o" \
"./Sources/console/uart_console_io.o" 

C_DEPS += \
./Sources/console/echoEmulation.d \
./Sources/console/ringBuffer.d \
./Sources/console/uart.d \
./Sources/console/uart_console_io.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/console/echoEmulation.o: ../Sources/console/echoEmulation.c
	@echo 'Building file: $<'
	@echo 'Executing target #42 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/console/echoEmulation.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/console/echoEmulation.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/console/ringBuffer.o: ../Sources/console/ringBuffer.c
	@echo 'Building file: $<'
	@echo 'Executing target #43 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/console/ringBuffer.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/console/ringBuffer.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/console/uart.o: ../Sources/console/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #44 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/console/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/console/uart.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/console/uart_console_io.o: ../Sources/console/uart_console_io.c
	@echo 'Building file: $<'
	@echo 'Executing target #45 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Sources/console/uart_console_io.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Sources/console/uart_console_io.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


