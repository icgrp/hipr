# High-priority clocks
# --------------------


# Package pins
# ------------

# Due to lack of cell attachment points in upper hierarchies, re-apply QSFP HSIO and refclk package_pin constraints at this scope
set_property PACKAGE_PIN T42 [get_ports io_clk_qsfp0_refclka_00_clk_p] -quiet
set_property PACKAGE_PIN T43 [get_ports io_clk_qsfp0_refclka_00_clk_n] -quiet
set_property PACKAGE_PIN P42 [get_ports io_clk_qsfp1_refclka_00_clk_p] -quiet
set_property PACKAGE_PIN P43 [get_ports io_clk_qsfp1_refclka_00_clk_n] -quiet

set_property PACKAGE_PIN R40 [get_ports io_clk_qsfp0_refclkb_00_clk_p] -quiet
set_property PACKAGE_PIN R41 [get_ports io_clk_qsfp0_refclkb_00_clk_n] -quiet
set_property PACKAGE_PIN M42 [get_ports io_clk_qsfp1_refclkb_00_clk_p] -quiet
set_property PACKAGE_PIN M43 [get_ports io_clk_qsfp1_refclkb_00_clk_n] -quiet

set_property PACKAGE_PIN L48 [get_ports io_gt_qsfp0_00_gtx_p[0]] -quiet
set_property PACKAGE_PIN L49 [get_ports io_gt_qsfp0_00_gtx_n[0]] -quiet
set_property PACKAGE_PIN L53 [get_ports io_gt_qsfp0_00_grx_p[0]] -quiet
set_property PACKAGE_PIN L54 [get_ports io_gt_qsfp0_00_grx_n[0]] -quiet
set_property PACKAGE_PIN L44 [get_ports io_gt_qsfp0_00_gtx_p[1]] -quiet
set_property PACKAGE_PIN L45 [get_ports io_gt_qsfp0_00_gtx_n[1]] -quiet
set_property PACKAGE_PIN K51 [get_ports io_gt_qsfp0_00_grx_p[1]] -quiet
set_property PACKAGE_PIN K52 [get_ports io_gt_qsfp0_00_grx_n[1]] -quiet
set_property PACKAGE_PIN K46 [get_ports io_gt_qsfp0_00_gtx_p[2]] -quiet
set_property PACKAGE_PIN K47 [get_ports io_gt_qsfp0_00_gtx_n[2]] -quiet
set_property PACKAGE_PIN J53 [get_ports io_gt_qsfp0_00_grx_p[2]] -quiet
set_property PACKAGE_PIN J54 [get_ports io_gt_qsfp0_00_grx_n[2]] -quiet
set_property PACKAGE_PIN J48 [get_ports io_gt_qsfp0_00_gtx_p[3]] -quiet
set_property PACKAGE_PIN J49 [get_ports io_gt_qsfp0_00_gtx_n[3]] -quiet
set_property PACKAGE_PIN H51 [get_ports io_gt_qsfp0_00_grx_p[3]] -quiet
set_property PACKAGE_PIN H52 [get_ports io_gt_qsfp0_00_grx_n[3]] -quiet

set_property PACKAGE_PIN G48 [get_ports io_gt_qsfp1_00_gtx_p[0]] -quiet
set_property PACKAGE_PIN G49 [get_ports io_gt_qsfp1_00_gtx_n[0]] -quiet
set_property PACKAGE_PIN G53 [get_ports io_gt_qsfp1_00_grx_p[0]] -quiet
set_property PACKAGE_PIN G54 [get_ports io_gt_qsfp1_00_grx_n[0]] -quiet
set_property PACKAGE_PIN E48 [get_ports io_gt_qsfp1_00_gtx_p[1]] -quiet
set_property PACKAGE_PIN E49 [get_ports io_gt_qsfp1_00_gtx_n[1]] -quiet
set_property PACKAGE_PIN F51 [get_ports io_gt_qsfp1_00_grx_p[1]] -quiet
set_property PACKAGE_PIN F52 [get_ports io_gt_qsfp1_00_grx_n[1]] -quiet
set_property PACKAGE_PIN C48 [get_ports io_gt_qsfp1_00_gtx_p[2]] -quiet
set_property PACKAGE_PIN C49 [get_ports io_gt_qsfp1_00_gtx_n[2]] -quiet
set_property PACKAGE_PIN E53 [get_ports io_gt_qsfp1_00_grx_p[2]] -quiet
set_property PACKAGE_PIN E54 [get_ports io_gt_qsfp1_00_grx_n[2]] -quiet
set_property PACKAGE_PIN A49 [get_ports io_gt_qsfp1_00_gtx_p[3]] -quiet
set_property PACKAGE_PIN A50 [get_ports io_gt_qsfp1_00_gtx_n[3]] -quiet
set_property PACKAGE_PIN D51 [get_ports io_gt_qsfp1_00_grx_p[3]] -quiet
set_property PACKAGE_PIN D52 [get_ports io_gt_qsfp1_00_grx_n[3]] -quiet


# set power budget
# --------------------

