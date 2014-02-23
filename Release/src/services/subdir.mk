################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/services/DS18B20.cpp \
../src/services/EE24LC256.cpp \
../src/services/GPIO.cpp \
../src/services/IRremote.cpp \
../src/services/LCD.cpp \
../src/services/OneWire.cpp \
../src/services/PCF8583.cpp \
../src/services/TWI.cpp 

OBJS += \
./src/services/DS18B20.o \
./src/services/EE24LC256.o \
./src/services/GPIO.o \
./src/services/IRremote.o \
./src/services/LCD.o \
./src/services/OneWire.o \
./src/services/PCF8583.o \
./src/services/TWI.o 

CPP_DEPS += \
./src/services/DS18B20.d \
./src/services/EE24LC256.d \
./src/services/GPIO.d \
./src/services/IRremote.d \
./src/services/LCD.d \
./src/services/OneWire.d \
./src/services/PCF8583.d \
./src/services/TWI.d 


# Each subdirectory must supply rules for building sources it contributes
src/services/%.o: ../src/services/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -Os -ffunction-sections -fdata-sections -ffunction-sections -fdata-sections -fno-exceptions -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


