################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./src/device/soft_i2c.obj" \
"./src/device/soft_spi.obj" \
"./src/device/ucs.obj" \
"./src/device/usci_a0_spi.obj" \
"./src/device/usci_a1_spi.obj" \
"./src/device/usci_b0_i2c.obj" \
"./src/device/usci_b0_spi.obj" \
"./src/device/usci_b1_i2c.obj" \
"./src/device/usci_b1_spi.obj" \
"./src/device/wdt.obj" \
"./src/driver/ads1118.obj" \
"./src/driver/dac8571.obj" \
"./src/driver/key.obj" \
"./src/driver/led.obj" \
"./src/driver/ssd1351.obj" \
"./src/main.obj" \
"./src/vector.obj" \
"./src/system/fonts.obj" \
"./src/system/init.obj" \
"./src/system/lut.obj" \
"./src/system/user.obj" \
"./src/user/display_value.obj" \
"../lnk_msp430f5529.cmd" \
$(GEN_CMDS__FLAG) \
-llibmath.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/device/subdir_vars.mk
-include src/driver/subdir_vars.mk
-include src/subdir_vars.mk
-include src/system/subdir_vars.mk
-include src/user/subdir_vars.mk
-include subdir_rules.mk
-include src/device/subdir_rules.mk
-include src/driver/subdir_rules.mk
-include src/subdir_rules.mk
-include src/system/subdir_rules.mk
-include src/user/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Minisys_new_msp430f5529_beta3.0.out \

EXE_OUTPUTS__QUOTED += \
"Minisys_new_msp430f5529_beta3.0.out" \

BIN_OUTPUTS += \
Minisys_new_msp430f5529_beta3.0.hex \

BIN_OUTPUTS__QUOTED += \
"Minisys_new_msp430f5529_beta3.0.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Minisys_new_msp430f5529_beta3.0.out"

# Tool invocations
Minisys_new_msp430f5529_beta3.0.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --advice:power=all --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 -z -m"Minisys_new_msp430f5529_beta3.0.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"C:/ti/ccs1010/ccs/ccs_base/msp430/include" -i"C:/ti/ccs1010/ccs/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/lib" -i"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="Minisys_new_msp430f5529_beta3.0_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "Minisys_new_msp430f5529_beta3.0.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Minisys_new_msp430f5529_beta3.0.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/bin/hex430" --memwidth=8 --romwidth=8 -o "Minisys_new_msp430f5529_beta3.0.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "src\device\soft_i2c.obj" "src\device\soft_spi.obj" "src\device\ucs.obj" "src\device\usci_a0_spi.obj" "src\device\usci_a1_spi.obj" "src\device\usci_b0_i2c.obj" "src\device\usci_b0_spi.obj" "src\device\usci_b1_i2c.obj" "src\device\usci_b1_spi.obj" "src\device\wdt.obj" "src\driver\ads1118.obj" "src\driver\dac8571.obj" "src\driver\key.obj" "src\driver\led.obj" "src\driver\ssd1351.obj" "src\main.obj" "src\vector.obj" "src\system\fonts.obj" "src\system\init.obj" "src\system\lut.obj" "src\system\user.obj" "src\user\display_value.obj" 
	-$(RM) "src\device\soft_i2c.d" "src\device\soft_spi.d" "src\device\ucs.d" "src\device\usci_a0_spi.d" "src\device\usci_a1_spi.d" "src\device\usci_b0_i2c.d" "src\device\usci_b0_spi.d" "src\device\usci_b1_i2c.d" "src\device\usci_b1_spi.d" "src\device\wdt.d" "src\driver\ads1118.d" "src\driver\dac8571.d" "src\driver\key.d" "src\driver\led.d" "src\driver\ssd1351.d" "src\main.d" "src\vector.d" "src\system\fonts.d" "src\system\init.d" "src\system\lut.d" "src\system\user.d" "src\user\display_value.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

