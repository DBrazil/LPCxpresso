################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/XBee/XBee.cpp 

OBJS += \
./src/XBee/XBee.o 

CPP_DEPS += \
./src/XBee/XBee.d 


# Each subdirectory must supply rules for building sources it contributes
src/XBee/%.o: ../src/XBee/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -DNO_EXTCLK -DNDEBUG -D__CODE_RED -I"/Users/sin/Documents/LPCxpresso/workspace/eXodusino/src" -I"/Users/sin/Documents/LPCxpresso/workspace/eXodusino/src/XBee" -I"/Users/sin/Documents/LPCxpresso/workspace/eXodusino/src/FatFs" -I"/Users/sin/Documents/LPCxpresso/workspace/eXodusino/src/core" -Os -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -DNON_EXTCLK -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

