################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
App/Inv.obj: ../App/Inv.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"D:/RatioSoft/ccsv7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O2 --include_path="G:/CCSV7workspace/28377_UPS/28377D_INV" --include_path="D:/RatioSoft/ccsv7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.5.LTS/include" --advice:performance=all --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="App/Inv.d_raw" --obj_directory="App" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


