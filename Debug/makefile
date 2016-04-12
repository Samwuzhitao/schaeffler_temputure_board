################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2

ORDERED_OBJS += \
"./TMS570LS1227_DEMO/source/can.obj" \
"./TMS570LS1227_DEMO/source/dabort.obj" \
"./TMS570LS1227_DEMO/source/esm.obj" \
"./TMS570LS1227_DEMO/source/gio.obj" \
"./TMS570LS1227_DEMO/source/notification.obj" \
"./TMS570LS1227_DEMO/source/pinmux.obj" \
"./TMS570LS1227_DEMO/source/sci.obj" \
"./TMS570LS1227_DEMO/source/sys_core.obj" \
"./TMS570LS1227_DEMO/source/sys_dma.obj" \
"./TMS570LS1227_DEMO/source/sys_intvecs.obj" \
"./TMS570LS1227_DEMO/source/sys_main.obj" \
"./TMS570LS1227_DEMO/source/sys_mpu.obj" \
"./TMS570LS1227_DEMO/source/sys_pcr.obj" \
"./TMS570LS1227_DEMO/source/sys_phantom.obj" \
"./TMS570LS1227_DEMO/source/sys_pmm.obj" \
"./TMS570LS1227_DEMO/source/sys_pmu.obj" \
"./TMS570LS1227_DEMO/source/sys_selftest.obj" \
"./TMS570LS1227_DEMO/source/sys_startup.obj" \
"./TMS570LS1227_DEMO/source/sys_vim.obj" \
"./TMS570LS1227_DEMO/source/system.obj" \
"./TMS570LS1227_DEMO/App/CanProtocol.obj" \
"./TMS570LS1227_DEMO/App/printf.obj" \
"../TMS570LS1227_DEMO/source/sys_link.cmd" \
$(GEN_CMDS__FLAG) \
-l"rtsv7R4_T_be_v3D16_eabi.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include TMS570LS1227_DEMO/source/subdir_vars.mk
-include TMS570LS1227_DEMO/App/subdir_vars.mk
-include TMS570LS1227_DEMO/source/subdir_rules.mk
-include TMS570LS1227_DEMO/App/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
TMS570LS1227_DEMO.out \

EXE_OUTPUTS__QUOTED += \
"TMS570LS1227_DEMO.out" \

BIN_OUTPUTS += \
TMS570LS1227_DEMO.txt \

BIN_OUTPUTS__QUOTED += \
"TMS570LS1227_DEMO.txt" \


# All Target
all: TMS570LS1227_DEMO.out secondary-outputs

# Tool invocations
TMS570LS1227_DEMO.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --abi=eabi -g --display_error_number --diag_warning=225 --diag_wrap=off --enum_type=packed -z -m"TMS570LS1227_DEMO.map" --heap_size=0x800 --stack_size=0x800 -i"D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/lib" -i"D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="TMS570LS1227_DEMO_linkInfo.xml" --rom_model --be32 -o "TMS570LS1227_DEMO.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

TMS570LS1227_DEMO.txt: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armhex" --ti_txt -o "TMS570LS1227_DEMO.txt" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "TMS570LS1227_DEMO\source\can.pp" "TMS570LS1227_DEMO\source\esm.pp" "TMS570LS1227_DEMO\source\gio.pp" "TMS570LS1227_DEMO\source\notification.pp" "TMS570LS1227_DEMO\source\pinmux.pp" "TMS570LS1227_DEMO\source\sci.pp" "TMS570LS1227_DEMO\source\sys_dma.pp" "TMS570LS1227_DEMO\source\sys_main.pp" "TMS570LS1227_DEMO\source\sys_pcr.pp" "TMS570LS1227_DEMO\source\sys_phantom.pp" "TMS570LS1227_DEMO\source\sys_pmm.pp" "TMS570LS1227_DEMO\source\sys_selftest.pp" "TMS570LS1227_DEMO\source\sys_startup.pp" "TMS570LS1227_DEMO\source\sys_vim.pp" "TMS570LS1227_DEMO\source\system.pp" "TMS570LS1227_DEMO\App\CanProtocol.pp" "TMS570LS1227_DEMO\App\printf.pp" 
	-$(RM) "TMS570LS1227_DEMO\source\can.obj" "TMS570LS1227_DEMO\source\dabort.obj" "TMS570LS1227_DEMO\source\esm.obj" "TMS570LS1227_DEMO\source\gio.obj" "TMS570LS1227_DEMO\source\notification.obj" "TMS570LS1227_DEMO\source\pinmux.obj" "TMS570LS1227_DEMO\source\sci.obj" "TMS570LS1227_DEMO\source\sys_core.obj" "TMS570LS1227_DEMO\source\sys_dma.obj" "TMS570LS1227_DEMO\source\sys_intvecs.obj" "TMS570LS1227_DEMO\source\sys_main.obj" "TMS570LS1227_DEMO\source\sys_mpu.obj" "TMS570LS1227_DEMO\source\sys_pcr.obj" "TMS570LS1227_DEMO\source\sys_phantom.obj" "TMS570LS1227_DEMO\source\sys_pmm.obj" "TMS570LS1227_DEMO\source\sys_pmu.obj" "TMS570LS1227_DEMO\source\sys_selftest.obj" "TMS570LS1227_DEMO\source\sys_startup.obj" "TMS570LS1227_DEMO\source\sys_vim.obj" "TMS570LS1227_DEMO\source\system.obj" "TMS570LS1227_DEMO\App\CanProtocol.obj" "TMS570LS1227_DEMO\App\printf.obj" 
	-$(RM) "TMS570LS1227_DEMO\source\dabort.pp" "TMS570LS1227_DEMO\source\sys_core.pp" "TMS570LS1227_DEMO\source\sys_intvecs.pp" "TMS570LS1227_DEMO\source\sys_mpu.pp" "TMS570LS1227_DEMO\source\sys_pmu.pp" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"D:/Program Files (x86)/ccs/ccsv6/utils/tiobj2bin/tiobj2bin" "TMS570LS1227_DEMO.out" "TMS570LS1227_DEMO.bin" "D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armofd" "D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armhex" "D:/Program Files (x86)/ccs/ccsv6/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

