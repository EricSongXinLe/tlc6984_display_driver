################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -O3 --opt_for_speed=4 --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/device" --include_path="/Applications/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28003x/driverlib" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/led_driver" --include_path="/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/%.obj: ../device/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -O3 --opt_for_speed=4 --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/device" --include_path="/Applications/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28003x/driverlib" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/led_driver" --include_path="/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


