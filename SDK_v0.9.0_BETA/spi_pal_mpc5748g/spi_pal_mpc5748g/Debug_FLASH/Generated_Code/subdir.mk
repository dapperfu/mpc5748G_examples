################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Generated_Code/Cpu.c" \
"../Generated_Code/clockMan1.c" \
"../Generated_Code/dmaController1.c" \
"../Generated_Code/pin_mux.c" \
"../Generated_Code/spi1.c" \
"../Generated_Code/spi2.c" 

C_SRCS += \
../Generated_Code/Cpu.c \
../Generated_Code/clockMan1.c \
../Generated_Code/dmaController1.c \
../Generated_Code/pin_mux.c \
../Generated_Code/spi1.c \
../Generated_Code/spi2.c 

OBJS_OS_FORMAT += \
./Generated_Code/Cpu.o \
./Generated_Code/clockMan1.o \
./Generated_Code/dmaController1.o \
./Generated_Code/pin_mux.o \
./Generated_Code/spi1.o \
./Generated_Code/spi2.o 

C_DEPS_QUOTED += \
"./Generated_Code/Cpu.d" \
"./Generated_Code/clockMan1.d" \
"./Generated_Code/dmaController1.d" \
"./Generated_Code/pin_mux.d" \
"./Generated_Code/spi1.d" \
"./Generated_Code/spi2.d" 

OBJS += \
./Generated_Code/Cpu.o \
./Generated_Code/clockMan1.o \
./Generated_Code/dmaController1.o \
./Generated_Code/pin_mux.o \
./Generated_Code/spi1.o \
./Generated_Code/spi2.o 

OBJS_QUOTED += \
"./Generated_Code/Cpu.o" \
"./Generated_Code/clockMan1.o" \
"./Generated_Code/dmaController1.o" \
"./Generated_Code/pin_mux.o" \
"./Generated_Code/spi1.o" \
"./Generated_Code/spi2.o" 

C_DEPS += \
./Generated_Code/Cpu.d \
./Generated_Code/clockMan1.d \
./Generated_Code/dmaController1.d \
./Generated_Code/pin_mux.d \
./Generated_Code/spi1.d \
./Generated_Code/spi2.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/Cpu.o: ../Generated_Code/Cpu.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/Cpu.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/Cpu.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/clockMan1.o: ../Generated_Code/clockMan1.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/clockMan1.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/clockMan1.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/dmaController1.o: ../Generated_Code/dmaController1.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/dmaController1.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/dmaController1.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/pin_mux.o: ../Generated_Code/pin_mux.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/pin_mux.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/pin_mux.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/spi1.o: ../Generated_Code/spi1.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/spi1.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/spi1.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/spi2.o: ../Generated_Code/spi2.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@Generated_Code/spi2.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "Generated_Code/spi2.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


