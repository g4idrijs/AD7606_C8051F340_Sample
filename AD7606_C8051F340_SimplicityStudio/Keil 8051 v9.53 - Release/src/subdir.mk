################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
A51_UPPER_SRCS += \
../src/SILABS_STARTUP.A51 

C_SRCS += \
../src/Delay.c \
../src/USB_MAIN.c 

OBJS += \
./src/Delay.OBJ \
./src/SILABS_STARTUP.OBJ \
./src/USB_MAIN.OBJ 


# Each subdirectory must supply rules for building sources it contributes
src/%.OBJ: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/Delay.OBJ: G:/AD7606_C8051F340_Sample/AD7606_C8051F340_SimplicityStudio/src/Delay.h

src/%.OBJ: ../src/%.A51
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Assembler'
	AX51 "@$(patsubst %.OBJ,%.__ia,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/USB_MAIN.OBJ: C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/si8051/v3/Device/shared/si8051Base/compiler_defs.h C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/si8051/v3/Device/C8051F340/inc/c8051F340.h G:/AD7606_C8051F340_Sample/AD7606_C8051F340_SimplicityStudio/src/USB_API.h G:/AD7606_C8051F340_Sample/AD7606_C8051F340_SimplicityStudio/src/Delay.h C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/si8051/v3/Device/shared/si8051Base/stdbool.h C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/si8051/v3/Device/shared/si8051Base/stdint.h


