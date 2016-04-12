################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
TMS570LS1227_DEMO/App/CanProtocol.obj: ../TMS570LS1227_DEMO/App/CanProtocol.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --abi=eabi --include_path="D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="D:/My_TEMP_Project/schaeffler/workspace/TMS570LS1227_DEMO/TMS570LS1227_DEMO/App" --include_path="D:/My_TEMP_Project/schaeffler/workspace/TMS570LS1227_DEMO/TMS570LS1227_DEMO/include" -g --display_error_number --diag_warning=225 --diag_wrap=off --enum_type=packed --preproc_with_compile --preproc_dependency="TMS570LS1227_DEMO/App/CanProtocol.pp" --obj_directory="TMS570LS1227_DEMO/App" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

TMS570LS1227_DEMO/App/printf.obj: ../TMS570LS1227_DEMO/App/printf.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --abi=eabi --include_path="D:/Program Files (x86)/ccs/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="D:/My_TEMP_Project/schaeffler/workspace/TMS570LS1227_DEMO/TMS570LS1227_DEMO/App" --include_path="D:/My_TEMP_Project/schaeffler/workspace/TMS570LS1227_DEMO/TMS570LS1227_DEMO/include" -g --display_error_number --diag_warning=225 --diag_wrap=off --enum_type=packed --preproc_with_compile --preproc_dependency="TMS570LS1227_DEMO/App/printf.pp" --obj_directory="TMS570LS1227_DEMO/App" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


