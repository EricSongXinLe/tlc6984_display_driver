################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS

GEN_OPTS__FLAG := --cmd_file="syscfg/board.opt" --cmd_file="syscfg/c2000ware_libraries.opt" 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./CCSI_Socket.obj" \
"./TLC698x_LP589x_APIs.obj" \
"./frames.obj" \
"./led_hal.obj" \
"./main.obj" \
"./system_info.obj" \
"./syscfg/board.obj" \
"./syscfg/clb_config.obj" \
"./syscfg/clb_sim.obj" \
"./syscfg/c2000ware_libraries.obj" \
"./device/device.obj" \
"./device/f28003x_codestartbranch.obj" \
"../28003x_generic_flash_lnk.cmd" \
"/Applications/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28003x/driverlib/ccs/Debug/driverlib.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include device/subdir_vars.mk
-include subdir_rules.mk
-include device/subdir_rules.mk
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
tlc698x_lp589x_f28003x.out 

EXE_OUTPUTS__QUOTED += \
"tlc698x_lp589x_f28003x.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "tlc698x_lp589x_f28003x.out"

# Tool invocations
tlc698x_lp589x_f28003x.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -O3 --opt_for_speed=4 --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi -z -m"tlc698x_lp589x_f28003x.map" --heap_size=0x100 --stack_size=0x200 --warn_sections -i"/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/lib" -i"/Applications/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --reread_libs --define=_FLASH --diag_wrap=off --display_error_number --xml_link_info="tlc698x_lp589x_f28003x_linkInfo.xml" --entry_point=code_start --rom_model -o "tlc698x_lp589x_f28003x.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "CCSI_Socket.obj" "TLC698x_LP589x_APIs.obj" "frames.obj" "led_hal.obj" "main.obj" "system_info.obj" "syscfg/board.obj" "syscfg/clb_config.obj" "syscfg/clb_sim.obj" "syscfg/c2000ware_libraries.obj" "device/device.obj" "device/f28003x_codestartbranch.obj" 
	-$(RM) "CCSI_Socket.d" "TLC698x_LP589x_APIs.d" "frames.d" "led_hal.d" "main.d" "system_info.d" "syscfg/board.d" "syscfg/clb_config.d" "syscfg/c2000ware_libraries.d" "device/device.d" 
	-$(RM) "syscfg/clb_sim.d" 
	-$(RM) "device/f28003x_codestartbranch.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-if 0 == 1  mkdir "/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/simulation"
	-if 0 == 1  /C:/TDM-GCC-64/bin/g++  -c -DCLB_SIM -I/Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc-2.3.3/src -I/Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/include/type3 -I/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x -I/C:/TDM-GCC-64/bin/include -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/simulation/clb_sim.d -MT/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/simulation/clb_sim.o -I/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg   -fno-threadsafe-statics  -o/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/simulation/clb_sim.o /Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg/clb_sim.cpp
	-if 0 == 1  /C:/TDM-GCC-64/bin/g++ -DCLB_SIM -Og -g -gdwarf-3 -gstrict-dwarf -Wall -Wl,-Map,/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/simulation/simulation_output.map -L/Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc-2.3.3/build/src -o/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/simulation/simulation_output.exe /Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/simulation/clb_sim.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_FSM_SC_model.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_HLC_SC_model.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_LUT4_SC_model.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_OutputLUT_SC_model.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_counter_SC_model.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_counter_top_SC_model.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_FSM_top_SC_model.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_LUT4_top_SC_model.o /Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type3/CLB_OutputLUT_top_SC_model.o  -Wl,--start-group -lsystemc -Wl,--end-group
	-if 0 == 1  "./simulation/simulation_output.exe"
	-if 0 == 1  /Applications/ti/ccs1260/ccs/tools/node/node "/Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/dot_file_libraries/clbDotUtility.js"  "/Applications/ti/c2000/C2000Ware_5_01_00_00/utilities/clb_tool/clb_syscfg/" "/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg" "/Users/eric/Desktop/UCLA/ChipsLab/tlc698x_lp589x_f28003x/CPU1_FLASH/syscfg/clb.dot"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

