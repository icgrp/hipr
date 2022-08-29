ifndef XILINX_VIVADO
  $(error Environment variable XILINX_VIVADO is required and should point to Vitis install area)
endif

vitis_impl_tcl_name=pfm_top_wrapper
kl_name=ydma
benchmark_name=xxx
workspace=..
impl_dir=$(workspace)/_x/link/vivado/vpl/prj/prj.runs/impl_1
base_list=zculling_bot zculling_top coloringFB_bot_m coloringFB_top_m data_redir_m rasterization2_m data_transfer 
context_dcps=$(addprefix checkpoint/, $(addsuffix .dcp, $(subst page, p_, $(base_hipr_list))))
page_dcps=$(addsuffix .dcp, $(addprefix ./checkpoint/, $(base_list)))
page_context_dcps=$(addsuffix .dcp, $(addprefix ./checkpoint/context_, $(base_list)))
page_bits=$(addsuffix .bit, $(addprefix bitstream/, $(base_list)))
xclbins=$(addsuffix .xclbin, $(addprefix xclbin/, $(base_list))) xclbin/dynamic_region.xclbin
abs_gen_tcls=$(addprefix ../_x/link/vivado/vpl/prj/prj.runs/impl_1/, $(addsuffix .tcl, $(subst page, abs_gen, $(base_list))))


all: ../../../__overlay_is_ready__

../../../__overlay_is_ready__: $(context_dcps) 
	touch ../../../__overlay_is_ready__

$(context_dcps):checkpoint/%.dcp:tcl/gen_abs_%.tcl ./checkpoint/overlay.dcp
	./shell/run_tcl.sh $< 

overlay:./checkpoint/overlay.dcp

./checkpoint/overlay.dcp: $(impl_dir)/$(vitis_impl_tcl_name)_mk_overlay.tcl $(impl_dir)/run_tcl.sh ./checkpoint/hw_bb_divided.dcp ./xdc/sub.xdc
	cd $(subst $(vitis_impl_tcl_name)_mk_overlay,, $(basename $<)) && ./run_tcl.sh $(vitis_impl_tcl_name)_mk_overlay.tcl
	cp $(impl_dir)/design_route.dcp ./checkpoint/overlay.dcp

$(impl_dir)/run_tcl.sh: ./shell/run_tcl.sh
	cp ./shell/run_tcl.sh $(impl_dir)

./checkpoint/page.dcp: ./src4level2/page/* ./tcl/out_of_context_syn_page.tcl
	./shell/run_tcl.sh ./tcl/out_of_context_syn_page.tcl	

./checkpoint/hw_bb_divided.dcp: ./checkpoint/pfm_dynamic_new_bb.dcp ./checkpoint/hw_bb_locked.dcp ./tcl/sub_divided.tcl 
	./shell/run_tcl.sh ./tcl/sub_divided.tcl

./checkpoint/pfm_dynamic_new_bb.dcp:./checkpoint/pfm_dynamic_bb.dcp ./tcl/replace_sub_module_level1.tcl ./checkpoint/$(kl_name)_bb.dcp 
	./shell/run_tcl.sh ./tcl/replace_sub_module_level1.tcl

./checkpoint/$(kl_name)_bb.dcp: ./src4level2/ydma_bb/* ./tcl/out_of_context_syn_$(kl_name)_bb.tcl
	./shell/run_tcl.sh ./tcl/out_of_context_syn_$(kl_name)_bb.tcl	

./checkpoint/pfm_dynamic_bb.dcp:./checkpoint/pfm_dynamic.dcp ./tcl/empty_pfm_dynamic.tcl
	./shell/run_tcl.sh ./tcl/empty_pfm_dynamic.tcl

./checkpoint/pfm_dynamic.dcp: $(impl_dir)/gen_pfm_dynamic.tcl checkpoint/hw_bb_locked.dcp
	./shell/run_gen_pfm_dymanic.sh $(workspace) 

modify:$(impl_dir)/gen_pfm_dynamic.tcl

$(impl_dir)/gen_pfm_dynamic.tcl:$(impl_dir)/$(vitis_impl_tcl_name).tcl ./python/mk_gen_pfm_tcl.py
	./shell/run_python.sh ./python/mk_gen_pfm_tcl.py $(workspace) $(kl_name) $(vitis_impl_tcl_name)

overlay_tcl:$(impl_dir)/$(vitis_impl_tcl_name)_mk_overlay.tcl

$(abs_gen_tcls): ./python/mk_abs_tcl.py
	./shell/run_python.sh ./python/mk_abs_tcl.py $(workspace) $(kl_name) $(vitis_impl_tcl_name)

$(impl_dir)/$(vitis_impl_tcl_name)_mk_overlay.tcl:$(impl_dir)/$(vitis_impl_tcl_name).tcl ./python/mk_overlay_tcl.py
	./shell/run_python.sh ./python/mk_overlay_tcl.py $(workspace) $(kl_name) $(vitis_impl_tcl_name)

./xdc/sub.xdc: ./cpp/src/app/dfx.txt
	mkdir -p xdc
	cd ./cpp && ./run.sh


./cpp/src/app/dfx.txt: ./python/report.py
	python $< '../../../place_holder' -op '$(base_list)' -app $(benchmark_name)

checkpoint/hw_bb_locked.dcp: ../_x/link/vivado/vpl/.local/hw_platform/hw_bb_locked.dcp
	mkdir -p checkpoint
	cp $< $@

clean:
	rm -rf *.log *.jou *.rpt .Xil hd_visual *.str *.txt checkpoint/* 
