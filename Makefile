
freq=200M


#prj_name=datamover
#prj_name=zcu102_1
#prj_name=zcu102_2
#prj_name=ultra96_1
#prj_name=optical_flow_8
#prj_name=optical_flow_1x
#prj_name=optical_flow_2x
#prj_name=optical_flow_4x
#prj_name=optical_flow_5x
#prj_name=optical_flow_6x
#prj_name=optical_flow_2x_op
#prj_name=spam_filter_1x
#prj_name=rendering_dw
#prj_name=rendering_1x
#prj_name=rendering_2x
#prj_name=rendering_12x
#prj_name=rendering_16x
#prj_name=rendering_32x
#prj_name=digit_reg_1x_small
#prj_name=digit_reg_1x
#prj_name=digit_reg_2x
#prj_name=digit_reg_12x
#prj_name=digit_reg_dyn__1280_20_1
#prj_name=face_detect_1x
#prj_name=face_detect_12x
#prj_name=face_detect_2x
#prj_name=face_detect_4x
#prj_name=digit_reg512
#prj_name=digit_reg_dyn_50p
#prj_name=digit_reg_dyn_yboard_80k
#prj_name=digit_reg_dyn_yboard_164k
# prj_name=digit_reg_dyn_yboard_265k
#prj_name=digit_reg_dyn_yboard_339k
#prj_name=ultra6_1


# prj_name=digit_reg
# prj_name=hibft
# prj_name=optical_flow
# prj_name=spam_filter_xysa_50p
# prj_name=spam_filter_sqsa_50p
prj_name=rendering
# prj_name=rendering_fix
# prj_name=bnn
# prj_name=face_detect
# prj_name=digit_reg_yboard_164k_200M
# prj_name=face_detect_simple


# prj_name=yboard_80k
# prj_name=yboard_91k
# prj_name=yboard_101k
# prj_name=yboard_112k
# prj_name=yboard_119k
# prj_name=yboard_128k
# prj_name=yboard_139k
# prj_name=yboard_152k
# prj_name=yboard_164k


# prj_name=digit_reg_dyn_sqsa_70p
# prj_name=hibft_51p
# prj_name=hibft
# prj_name=spam_filter_60p

prefix=/opt
platform_name=zcu102
src=./common/verilog_src
ws=workspace

ws_increment=$(ws)/F000_increment_$(freq)
ws_sdk=$(ws)/F007_sdk_$(prj_name)
ws_overlay=$(ws)/F001_overlay
ws_hipr=$(ws)/F001_hipr
ws_hls=$(ws)/F002_hls_$(prj_name)_$(freq)
ws_syn=$(ws)/F003_syn_$(prj_name)_$(freq)
ws_impl=$(ws)/F004_impl_$(prj_name)_$(freq)
ws_bit=$(ws)/F005_bits_$(prj_name)_$(freq)
ws_mono=$(ws)/F007_mono_$(prj_name)
ws_mbft=$(ws)/F007_mono_bft_$(prj_name)

operators_dir=./input_src/$(prj_name)/operators
operators_src=$(wildcard $(operators_dir)/*.cpp)
operators=$(basename $(notdir $(operators_src)))
operators_hls_targets=$(foreach n, $(operators), $(ws_hls)/runLog$(n).log)
operators_syn_targets=$(foreach n, $(operators), $(ws_syn)/$(n)/page_netlist.dcp)
operators_impl_targets=$(foreach n, $(operators), $(ws_impl)/$(n)/page_routed.dcp)
operators_bit_targets=$(foreach n, $(operators), $(ws_bit)/$(n).bit)
operators_xclbin_targets=$(foreach n, $(operators), $(ws_bit)/$(n).xclbin)
operators_runtime_target=$(ws_bit)/app.exe
mono_target=$(ws_mono)/increment.xclbin
operators_ip_targets=$(foreach n, $(operators), $(ws_mbft)/ip_repo/$(n)/prj/floorplan_static.xpr)
mono_bft_target=$(ws_mbft)/prj/floorplan_static.runs/impl_1/floorplan_static_wrapper.bit
download_target=$(ws_bit)/download.tcl 
config_target=$(ws_mbft)/config.cpp 

freq_start=100
freq_end  =401
freq_diff = 10
freq_sweep=$(shell seq $(freq_start) $(freq_diff) $(freq_end))

increment_targets=$(foreach n, $(freq_sweep), $(ws)/F000_increment_$(n)M/__increment_is_ready__)
increment=$(ws)/F000_increment_$(freq)/__increment_is_ready__

m=$(shell date)


overlay_type=hipr
ifeq ($(overlay_type),hipr)
overlay_suffix=_$(prj_name)_$(freq)
endif


all: $(operators_runtime_target)

mono: $(mono_target)

$(mono_target):./input_src/$(prj_name)/host/top.cpp ./pr_flow/monolithic.py $(operators_hls_targets)
	python3 pr_flow.py $(prj_name) -monolithic -op '$(basename $(notdir $(operators_bit_targets)))'
	cd $(ws_mono) && ./main.sh
	
$(operators_runtime_target):./input_src/$(prj_name)/host/host.cpp ./input_src/$(prj_name)/host/typedefs.h $(operators_xclbin_targets) ./pr_flow/runtime.py
	python3 pr_flow.py $(prj_name) -runtime -op '$(basename $(notdir $(operators_bit_targets)))' -f $(freq)
	cp $(operators_xclbin_targets) $(ws_bit)/sd_card
	cd $(ws_bit) && ./main.sh
	
$(operators_xclbin_targets):$(ws_bit)/%.xclbin:$(ws_bit)/%.bit
	python3 pr_flow.py $(prj_name) -xclbin -op $(basename $(notdir $@)) -f $(freq)
	cd $(ws_bit) && ./main_$(basename $(notdir $@)).sh

$(download_target): $(operators_bit_targets)
	python3 pr_flow.py $(prj_name) -bit -op '$(basename $(notdir $^))'

bits:$(operators_bit_targets)
# Implementation from post-synthesis DCP to bitstreams
# generate bitstream for each operator
$(operators_bit_targets):$(ws_bit)/%.bit:$(ws_overlay)$(overlay_suffix)/__overlay_is_ready__ $(ws_syn)/%/page_netlist.dcp 
	python3 pr_flow.py $(prj_name) -impl -op $(basename $(notdir $@)) -f $(freq)
	cd $(ws_impl)/$(basename $(notdir $@)) && ./main.sh

syn:$(operators_syn_targets)
# Out-of-Context Synthesis from Verilog to post-synthesis DCP
$(operators_syn_targets):$(ws_syn)/%/page_netlist.dcp:$(ws_hls)/runLog%.log ./pr_flow/syn.py
	python3 pr_flow.py $(prj_name) -syn -op $(subst runLog,,$(basename $(notdir $<))) -f $(freq)
	#cd $(ws_syn)/$(subst runLog,,$(basename $(notdir $<)))/riscv && ./qsub_run.sh
	cd $(ws_syn)/$(subst runLog,,$(basename $(notdir $<))) && ./main.sh

hls: $(operators_hls_targets)
# High-Level-Synthesis from C to Verilog
$(operators_hls_targets):$(ws_hls)/runLog%.log:$(operators_dir)/%.cpp $(operators_dir)/%.h ./pr_flow/hls.py
	python3 pr_flow.py $(prj_name) -hls -op $(basename $(notdir $<)) -f $(freq) 
	cd $(ws_hls) && ./main_$(basename $(notdir $<)).sh


#$(ws_overlay)$(overlay_suffix)/overlay.dcp: $(ws_overlay)/src  
#	cp -rf ./common/overlay/riscv_bit_lib ./workspace/F001_overlay
#	cp -rf ./common/verilog_src ./workspace/F001_overlay/src
#	cp -rf ./common/overlay/size_table/* ./workspace/F001_overlay
#	cp -rf ./common/overlay/dcp_lib/* ./workspace/F001_overlay
#	cp -rf ./common/overlay/main.bit ./workspace/F001_overlay
#	touch ./workspace/F001_overlay/*


$(ws_overlay)$(overlay_suffix)/src : common/verilog_src/*  common/script_src/project_syn_gen_zcu102.tcl
	rm -rf ./workspace/F001_overlay
	mkdir -p ./workspace/F001_overlay
	python3 pr_flow.py $(prj_name) -g


config: $(config_target)

$(config_target): $(operators_src)
	python3 pr_flow.py $(prj_name) -cfg -op '$(basename $(notdir $^))'
	cp $(ws)/F008_sdk_$(prj_name)/cpp_src/* $(ws)/vitis/$(prj_name)/src/



HW:
	python3 ./pr_flow/riscv2HW.py $(prj_name)

riscv:
	python3 ./pr_flow/HW2riscv.py $(prj_name)


mono_prj: $(mono_bft_target)

# prepare the logic equivalent monolithic project 
$(mono_bft_target): $(ws_overlay)$(overlay_suffix)/src  $(operators_ip_targets)
	python3 pr_flow.py $(prj_name) -mbft
	cd $(ws_mbft) && ./main.sh


# prepare the ip package for monolithic project
$(operators_ip_targets):$(ws_mbft)/ip_repo/%/prj/floorplan_static.xpr:$(ws_hls)/runLog%.log
	echo $@
	python3 pr_flow.py $(prj_name) -ip -op $(subst runLog,,$(basename $(notdir $<)))
	cd $(ws_mbft)/ip_repo/$(subst runLog,,$(basename $(notdir $<))) && ./qsub_run.sh

cp_mono_prj: ./workspace/vitis/floorplan_static_wrapper.xsa 

./workspace/vitis/floorplan_static_wrapper.xsa: ./workspace/F007_mono_bft_$(prj_name)/prj/floorplan_static.sdk/floorplan_static_wrapper.xsa
	mkdir -p workspace/vitis
	cp $< ./workspace/vitis

overlay: $(ws_overlay)$(overlay_suffix)/__overlay_is_ready__

$(ws_overlay)$(overlay_suffix)/__overlay_is_ready__: $(increment)
	python3 pr_flow.py $(prj_name) -g -f $(freq) -op '$(basename $(notdir $(operators_bit_targets)))'
	cd ./workspace/F001_overlay$(overlay_suffix) && ./main.sh

# 
#increment:$(increment) 
#$(increment):./common/increment/src/increment.cpp 
#	python3 pr_flow.py  $(prj_name) -y -f $(freq)
#	cd $(ws_increment) && ./main.sh


install: $(increment_targets) 

$(increment_targets):$(ws)/F000_increment_%/__increment_is_ready__:./common/increment/src/increment.cpp
	python3 pr_flow.py $(prj_name) -y -f $(subst /__increment_is_ready__,,$(subst $(ws)/F000_increment_,,$@)) && cd $(subst /__increment_is_ready__,,$@) && ./main.sh


	
run:
	cd $(ws_bit) && ./run_app.sh

touch:
	touch $(operators_hls_targets)
	touch $(operators_syn_targets)
	touch $(operators_bit_targets)
	touch $(operators_xclbin_targets)
	touch $(operators_runtime_target)

platform: $(prefix)/xilinx/platforms/xilinx_$(platform_name)_base_dfx_202110_1

$(prefix)/xilinx/platforms/xilinx_$(platform_name)_base_dfx_202110_1:
	cd $(platform_name)_dfx_platform && ./setup.sh
	mkdir -p $(prefix)/xilinx/platforms 
	cp  -rf $(platform_name)_dfx_platform/xilinx_$(platform_name)_base_dfx_202110_1 $(prefix)/xilinx/platforms
	echo "Hello"
	echo $(prefix)


.PHONY: report 
report: 
	 python3 ./pr_flow.py $(prj_name) -op '$(basename $(notdir $(operators_bit_targets)))' -rpt -f $(freq)

upload:
	scp $(ws_bit)/sd_card/* root@yueboard.seas.upenn.edu:/mnt


git_clean:
	rm -rf ./pr_flow/*.pyc

git:
	git add .
	git commit -m "$(m)"
	git push origin au280

clear:
	rm -rf ./workspace/F002_hls_$(prj_name)_$(freq)
	rm -rf ./workspace/F003_syn_$(prj_name)_$(freq)
	rm -rf ./workspace/F004_impl_$(prj_name)_$(freq)
	rm -rf ./workspace/F005_bits_$(prj_name)_$(freq)
	rm -rf ./workspace/qsub/*



clean:
	rm -rf ./workspace/F001*
	rm -rf ./workspace/F002*
	rm -rf ./workspace/F003*
	rm -rf ./workspace/F004*
	rm -rf ./workspace/F005*
	rm -rf ./workspace/F006*
	rm -rf ./workspace/F007*
	rm -rf ./workspace/qsub
	rm -rf ./workspace/report

cleanall:
	rm -rf ./workspace
	rm -rf ./pr_flow/*.pyc


