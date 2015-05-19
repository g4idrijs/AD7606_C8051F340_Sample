################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Keil tools exit code 1 is warnings only, so don't treat as an error in make
RC := test $$? -lt 2

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: emptyProject.omf

dependents:
	-cd G:/AD7606_C8051F340_Sample/AD7606_C8051F340_SimplicityStudio/Keil\ 8051\ v9.53\ -\ Release && $(MAKE) clean all

# Tool invocations
emptyProject.omf: echo_path $(OBJS) G:/AD7606_C8051F340_Sample/AD7606_C8051F340_SimplicityStudio/Keil\ 8051\ v9.53\ -\ Release/emptyProject.omf
	@echo 'Building target: $@'
	@echo 'Invoking: Keil 8051 Linker'
	LX51 "@emptyProject.lnp" || $(RC)
	@echo 'Finished building target: $@'

# Change from all upper case to the expected case
	@-mv "EMPTYPROJECT.OMF.CRBUILD" "$@"

# Generate hex file: emptyProject.hex
	@Ohx51 "emptyProject.omf" "HEXFILE (emptyProject.hex)" "H386" 2>&1 >/dev/null

# Other Targets
clean:
	-$(RM) $(OBJS)$(EXECUTABLES) emptyProject.omf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
G:/AD7606_C8051F340_Sample/AD7606_C8051F340_SimplicityStudio/Keil\ 8051\ v9.53\ -\ Release/emptyProject.omf:

-include ../makefile.targets
# echo the path
echo_path:
	@echo PATH=$$PATH
	@echo ' '

