################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -O3 --opt_for_speed=4 --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/device" --include_path="/Applications/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28003x/driverlib" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/led_driver" --include_path="/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1898265823: ../tlc698x_lp589x_f28003x.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/Applications/ti/ccs1260/ccs/utils/sysconfig_1.19.0/sysconfig_cli.sh" --script "/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/tlc698x_lp589x_f28003x.syscfg" -o "syscfg" -s "/Applications/ti/c2000/C2000Ware_5_01_00_00/.metadata/sdk.json" -d "F28003x" --package 100PZ --part F28003x_100PZ --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-1898265823 ../tlc698x_lp589x_f28003x.syscfg
syscfg/board.h: build-1898265823
syscfg/board.cmd.genlibs: build-1898265823
syscfg/board.opt: build-1898265823
syscfg/pinmux.csv: build-1898265823
syscfg/epwm.dot: build-1898265823
syscfg/clb_config.h: build-1898265823
syscfg/clb_config.c: build-1898265823
syscfg/clb.dot: build-1898265823
syscfg/clb_sim.cpp: build-1898265823
syscfg/clb_simulation.sh: build-1898265823
syscfg/c2000ware_libraries.cmd.genlibs: build-1898265823
syscfg/c2000ware_libraries.opt: build-1898265823
syscfg/c2000ware_libraries.c: build-1898265823
syscfg/c2000ware_libraries.h: build-1898265823
syscfg/clocktree.h: build-1898265823
syscfg: build-1898265823

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -O3 --opt_for_speed=4 --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/device" --include_path="/Applications/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28003x/driverlib" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/led_driver" --include_path="/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/%.obj: ./syscfg/%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -O3 --opt_for_speed=4 --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/device" --include_path="/Applications/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28003x/driverlib" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/led_driver" --include_path="/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


