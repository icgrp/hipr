ifndef XILINX_VIVADO
  $(error Environment variable XILINX_VIVADO is required and should point to Vitis install area)
endif

null  :=
space := $(null) #
comma := ,
vitis_impl_tcl_name=level0_wrapper
app=digit_reg
freq=0M
kl_name=increment
platform=xilinx_u50_gen3x16_xdma_5_202210_1
workspace=../../F000_increment_200M/increment/au50
impl_dir=$(workspace)/_x.hw.$(platform)/link/vivado/vpl/prj/prj.runs/impl_1
base_list=update_knn9 update_knn8 update_knn7 update_knn6 update_knn5 update_knn4 update_knn3 update_knn2 update_knn20 update_knn1 update_knn19 update_knn18 update_knn17 update_knn16 update_knn15 update_knn14 update_knn13 update_knn12 update_knn11 update_knn10 

floorplan_fixed=no
dummy_dcps=$(addprefix ../place_holder/, $(addsuffix _netlist.dcp, $(subst page, p_, $(base_list))))
dummy_depend=$(subst $(space),$(comma),$(strip $(addsuffix _$(app)_$(freq), $(addprefix hls_, $(base_list)))))
context_dcps=$(addprefix checkpoint/, $(addsuffix .dcp, $(subst page, p_, $(base_hipr_list))))
abs_depend=$(subst $(space),$(comma),$(strip $(addsuffix _$(app)_$(freq),$(addprefix gen_abs_,$(base_hipr_list)))))
page_dcps=$(addsuffix .dcp, $(addprefix ./checkpoint/, $(base_list)))
page_context_dcps=$(addsuffix .dcp, $(addprefix ./checkpoint/context_, $(base_list)))
page_bits=$(addsuffix .bit, $(addprefix bitstream/, $(base_list)))
xclbins=$(addsuffix .xclbin, $(addprefix xclbin/, $(base_list))) xclbin/dynamic_region.xclbin
abs_gen_tcls=$(addprefix ../_x/link/vivado/vpl/prj/prj.runs/impl_1/, $(addsuffix .tcl, $(subst page, abs_gen, $(base_list))))
VIVADO_DIR=$(XILINX_VIVADO)/settings64.sh
back_end=local


all: ../__overlay_is_ready__
# all: ./checkpoint/overlay.dcp
# all: ./xdc/sub.xdc

../__overlay_is_ready__: $(context_dcps) 
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e"                                                  > run_touch.sh
	echo "touch ../__overlay_is_ready__"                                  >> run_touch.sh
	echo "cd ../../.../  && make prj_name=$(app) freq=$(freq) bits"            >> run_touch.sh
	chmod +x                                                                 run_touch.sh
ifeq ($(back_end), qsub)
	qsub -N touch_$(app)_$(freq) -hold_jid $(abs_depend) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_touch.sh
else
	./run_touch.sh
endif

ylxiao:$(context_dcps)
$(context_dcps):checkpoint/%.dcp:tcl/gen_abs_%.tcl ./checkpoint/overlay.dcp
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e"                                                  > run_$(subst tcl/,,$(subst .tcl,,$<)).sh
	echo "source $(VIVADO_DIR)"                                           >> run_$(subst tcl/,,$(subst .tcl,,$<)).sh
	echo "./shell/run_tcl.sh $<"                                          >> run_$(subst tcl/,,$(subst .tcl,,$<)).sh
	chmod +x                                                                 run_$(subst tcl/,,$(subst .tcl,,$<)).sh
ifeq ($(back_end), qsub)
	qsub -N $(subst tcl/,,$(subst .tcl,,$<))_$(app)_$(freq) -hold_jid impl_$(app)_$(freq) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_$(subst tcl/,,$(subst .tcl,,$<)).sh
else
	./run_$(subst tcl/,,$(subst .tcl,,$<)).sh
endif

./checkpoint/overlay.dcp: $(impl_dir)/$(vitis_impl_tcl_name)_mk_overlay_$(app).tcl $(impl_dir)/run_tcl.sh ./checkpoint/hw_bb_divided.dcp ./xdc/sub.xdc
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e"                                                  >  run_impl.sh
	echo "source $(VIVADO_DIR)"                                           >>  run_impl.sh
	echo "cd $(subst $(vitis_impl_tcl_name)_mk_overlay_$(app),, $(basename $<)) && ./run_tcl.sh $(vitis_impl_tcl_name)_mk_overlay_$(app).tcl" >> run_impl.sh
	chmod +x                                                                  run_impl.sh
ifeq ($(back_end), qsub)
	qsub -N impl_$(app)_$(freq) -hold_jid div_$(app)_$(freq),xdc_$(app)_$(freq) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_impl.sh
else
	./run_impl.sh
endif

$(impl_dir)/run_tcl.sh: ./shell/run_tcl.sh
	mkdir -p workspace/qsub
	cp ./shell/run_tcl.sh $(impl_dir)

./checkpoint/hw_bb_divided.dcp: ./checkpoint/pfm_dynamic_new_bb.dcp ./checkpoint/hw_bb_locked.dcp ./tcl/sub_divided.tcl 
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e"                                                  > run_sub_div.sh
	echo "source $(VIVADO_DIR)"                                           >> run_sub_div.sh
	echo "./shell/run_tcl.sh ./tcl/sub_divided.tcl"                       >> run_sub_div.sh
	chmod +x                                                                 run_sub_div.sh
ifeq ($(back_end), qsub)
	qsub -N div_$(app)_$(freq) -hold_jid load_$(app)_$(freq) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_sub_div.sh
else
	./run_sub_div.sh
endif

./checkpoint/pfm_dynamic_new_bb.dcp:./checkpoint/pfm_dynamic_bb.dcp ./tcl/replace_sub_module_level1.tcl ./checkpoint/$(kl_name)_bb.dcp 
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e"                                                  > run_load.sh
	echo "source $(VIVADO_DIR)"                                           >> run_load.sh
	echo "./shell/run_tcl.sh ./tcl/replace_sub_module_level1.tcl"         >> run_load.sh
	chmod +x                                                                 run_load.sh
ifeq ($(back_end), qsub)
	qsub -N load_$(app)_$(freq) -hold_jid ooc_bb_$(app)_$(freq),pfm_dynamic_bb_$(app)_$(freq) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_load.sh
else
	./run_load.sh
endif

./checkpoint/$(kl_name)_bb.dcp: ./src4level2/increment_bb/* ./tcl/out_of_context_syn_$(kl_name)_bb.tcl
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e"                                                  > run_ooc_bb.sh
	echo "source $(VIVADO_DIR)"                                           >> run_ooc_bb.sh
	echo "./shell/run_tcl.sh ./tcl/out_of_context_syn_$(kl_name)_bb.tcl"  >> run_ooc_bb.sh	
	chmod +x                                                                 run_ooc_bb.sh
ifeq ($(back_end), qsub)
	qsub -N ooc_bb_$(app)_$(freq)  -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_ooc_bb.sh
else
	./run_ooc_bb.sh
endif


./checkpoint/pfm_dynamic_bb.dcp:./checkpoint/pfm_dynamic.dcp ./tcl/empty_pfm_dynamic.tcl
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e"                                  > run_bb_pfm_dynamic_$(app)_tcl.sh
	echo "source $(VIVADO_DIR)"                           >> run_bb_pfm_dynamic_$(app)_tcl.sh
	echo "./shell/run_tcl.sh ./tcl/empty_pfm_dynamic.tcl" >> run_bb_pfm_dynamic_$(app)_tcl.sh
	chmod +x                                                 run_bb_pfm_dynamic_$(app)_tcl.sh
ifeq ($(back_end), qsub)
	qsub -N pfm_dynamic_bb_$(app)_$(freq) -hold_jid pfm_dynamic_$(app)_$(freq) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_bb_pfm_dynamic_$(app)_tcl.sh
else
	./run_bb_pfm_dynamic_$(app)_tcl.sh
endif

./checkpoint/pfm_dynamic.dcp: $(impl_dir)/gen_pfm_dynamic_$(app).tcl
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e" > run_gen_pfm_dynamic_$(app)_tcl.sh
	echo "source $(VIVADO_DIR)"  >> run_gen_pfm_dynamic_$(app)_tcl.sh
	echo "./shell/run_gen_pfm_dymanic.sh $(impl_dir) $(app)"  >> run_gen_pfm_dynamic_$(app)_tcl.sh
	chmod +x run_gen_pfm_dynamic_$(app)_tcl.sh
ifeq ($(back_end), qsub)
	qsub -N pfm_dynamic_$(app)_$(freq) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_gen_pfm_dynamic_$(app)_tcl.sh
else
	./run_gen_pfm_dynamic_$(app)_tcl.sh
endif

$(impl_dir)/gen_pfm_dynamic_$(app).tcl:$(impl_dir)/$(vitis_impl_tcl_name).tcl ./python/mk_gen_pfm_tcl.py
	mkdir -p workspace/qsub
	./shell/run_python.sh ./python/mk_gen_pfm_tcl.py $(impl_dir) $(kl_name) $(vitis_impl_tcl_name) $(app)

$(impl_dir)/$(vitis_impl_tcl_name)_mk_overlay_$(app).tcl:$(impl_dir)/$(vitis_impl_tcl_name).tcl ./python/mk_overlay_tcl.py
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e" > run_mk_overlay_tcl.sh
	echo "./shell/run_python.sh ./python/mk_overlay_tcl.py $(impl_dir) $(kl_name) $(vitis_impl_tcl_name) $(app)" >> run_mk_overlay_tcl.sh
	chmod +x run_mk_overlay_tcl.sh
ifeq ($(back_end), qsub)
	qsub -N mkovl_$(app)_$(freq) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_mk_overlay_tcl.sh
else
	./run_mk_overlay_tcl.sh
endif

./xdc/sub.xdc: ./cpp/src/app/$(app)/dfx.txt
	mkdir -p workspace/qsub
	mkdir -p xdc
	echo "#!/bin/bash -e" > run_xdc.sh
ifeq ($(floorplan_fixed), yes)
	echo cp ../../../input_src/$(app)/xdc/sub.xdc ./xdc/sub.xdc >> run_xdc.sh
else
	echo 'cd ./cpp && ./run.sh $(app)'                          >> run_xdc.sh
endif
	chmod +x run_xdc.sh
ifeq ($(back_end), qsub)
	qsub -N xdc_$(app)_$(freq) -hold_jid dfx_$(app)_$(freq) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_xdc.sh
else
	./run_xdc.sh
endif

./cpp/src/app/$(app)/dfx.txt: ./python/report.py $(dummy_dcps)
	mkdir -p workspace/qsub
	echo "#!/bin/bash -e" > run_report.sh
	echo python3 $< '../place_holder' -op \'$(base_list)\' -app $(app)  >> run_report.sh
	chmod +x run_report.sh
ifeq ($(back_end), qsub)
	qsub -N dfx_$(app)_$(freq) -hold_jid $(dummy_depend) -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd run_report.sh
else
	./run_report.sh
endif

$(dummy_dcps):../place_holder/%_netlist.dcp:../place_holder/main_%.sh
	mkdir -p workspace/qsub
	cd ../place_holder && ./$<

checkpoint/hw_bb_locked.dcp: $(workspace)/_x.hw.$(platform)/link/vivado/vpl/.local/hw_platform/hw_bb_locked.dcp
	mkdir -p checkpoint
	cp $< $@
echo:
	echo $(abs_depend)

clean:
	rm -rf *.log *.jou *.rpt .Xil hd_visual *.str *.txt checkpoint/* xdc/*
	touch checkpoint/dummy
	touch xdc/dummy
	rm -rf $(impl_dir)/run_tcl.sh
	rm -rf $(impl_dir)/gen_pfm_dynamic_$(app).tcl
	rm -rf $(impl_dir)/gen_pfm_dynamic.tcl
	rm -rf $(abs_gen_tcls)
	rm -rf $(impl_dir)/$(vitis_impl_tcl_name)_mk_overlay_$(app).tcl
	rm -rf ./xdc/sub.xdc
	echo $(impl_dir)/$(vitis_impl_tcl_name)_mk_overlay_$(app).tcl
