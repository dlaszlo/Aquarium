################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/display/DisplayMain.cpp \
../src/display/DisplaySetClock.cpp \
../src/display/DisplaySetCooler.cpp \
../src/display/DisplaySetLight.cpp \
../src/display/DisplaySetSilentHours.cpp \
../src/display/DisplaySettings.cpp \
../src/display/DisplayWelcome.cpp 

OBJS += \
./src/display/DisplayMain.o \
./src/display/DisplaySetClock.o \
./src/display/DisplaySetCooler.o \
./src/display/DisplaySetLight.o \
./src/display/DisplaySetSilentHours.o \
./src/display/DisplaySettings.o \
./src/display/DisplayWelcome.o 

CPP_DEPS += \
./src/display/DisplayMain.d \
./src/display/DisplaySetClock.d \
./src/display/DisplaySetCooler.d \
./src/display/DisplaySetLight.d \
./src/display/DisplaySetSilentHours.d \
./src/display/DisplaySettings.d \
./src/display/DisplayWelcome.d 


# Each subdirectory must supply rules for building sources it contributes
src/display/%.o: ../src/display/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -Os -ffunction-sections -fdata-sections -ffunction-sections -fdata-sections -fno-exceptions -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


