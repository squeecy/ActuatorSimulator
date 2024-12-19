#include "simvar.h"


//switches
MsfsAeroFramework::SimVar fe_aux_hyd_pump_1_switch_master = MsfsAeroFramework::SimVar(
	"FE_AUX1HYDPUMP",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_aux_hyd_pump_2_switch_master = MsfsAeroFramework::SimVar(
	"FE_AUX2HYDPUMP",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_1_hyd_pump_1_switch_master = MsfsAeroFramework::SimVar(
	"FE_ENG1HYDPUMP1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_1_hyd_pump_2_switch_master = MsfsAeroFramework::SimVar(
	"FE_ENG1HYDPUMP1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_2_hyd_pump_1_switch_master = MsfsAeroFramework::SimVar(
	"FE_ENG1HYDPUMP1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_2_hyd_pump_2_switch_master = MsfsAeroFramework::SimVar(
	"FE_ENG1HYDPUMP1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_3_hyd_pump_1_switch_master = MsfsAeroFramework::SimVar(
	"FE_ENG1HYDPUMP1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_13_hydro_switch_master = MsfsAeroFramework::SimVar(
	"FE_13HYDPUMP",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_23_hydro_switch_master = MsfsAeroFramework::SimVar(
	"FE_23HYDPUMP",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);


MsfsAeroFramework::SimVar fe_eng_3_hyd_pump_2_switch_master = MsfsAeroFramework::SimVar(
	"FE_ENG1HYDPUMP1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_hydro_pressure_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroPres1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_hydro_pressure_2_master = MsfsAeroFramework::SimVar(
	"FE_HYDROPRES2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_hydro_pressure_3_master = MsfsAeroFramework::SimVar(
	"FE_HYDROPRES3",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_pressure_1_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngPres1_1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_pressure_1_2_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngPres1_2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_pressure_2_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngPres2_1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_pressure_2_2_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngPres2_2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);


MsfsAeroFramework::SimVar fe_eng_pump_pressure_3_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngPres3_1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_pressure_3_2_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngPres3_2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_line_hydro_pressure_aux_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroAuxPres1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_line_hydro_pressure_aux_2_master = MsfsAeroFramework::SimVar(
	"FE_HydroAuxPres2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_line_hydro_pressure_1_1_master = MsfsAeroFramework::SimVar(
	"FE_HYD_ENG1_LINE1_PRESS",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_line_hydro_pressure_1_2_master = MsfsAeroFramework::SimVar(
	"FE_HYD_ENG1_LINE2_PRESS",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_line_hydro_pressure_2_1_master = MsfsAeroFramework::SimVar(
	"FE_HYD_ENG2_LINE1_PRESS",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_line_hydro_pressure_2_2_master = MsfsAeroFramework::SimVar(
	"FE_HYD_ENG2_LINE2_PRESS",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_line_hydro_pressure_3_1_master = MsfsAeroFramework::SimVar(
	"FE_HYD_ENG3_LINE1_PRESS",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_line_hydro_pressure_3_2_master = MsfsAeroFramework::SimVar(
	"FE_HYD_ENG3_LINE2_PRESS",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_hydro_flow_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroFlow1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_hydro_flow_2_master = MsfsAeroFramework::SimVar(
	"FE_HydroFlow1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_hydro_flow_3_master = MsfsAeroFramework::SimVar(
	"FE_HydroFlow1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);


MsfsAeroFramework::SimVar fe_eng_pump_flow_1_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngFlow1_1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_flow_1_2_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngFlow1_2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_flow_2_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngFlow2_1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_flow_2_2_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngFlow2_2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_flow_3_1_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngFlow3_1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_eng_pump_flow_3_2_master = MsfsAeroFramework::SimVar(
	"FE_HydroEngFlow3_2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

//new
MsfsAeroFramework::SimVar fe_opposite_pump_operation_master = MsfsAeroFramework::SimVar(
	"FE_HydroOppositeEngPumpOperation",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar normal_n2_1_master = MsfsAeroFramework::SimVar(
	"ENG N2 RPM:1",
	MsfsAeroFramework::SimVarType::NORMAL,
	"Rpm"
);

MsfsAeroFramework::SimVar normal_n2_2_master = MsfsAeroFramework::SimVar(
	"ENG N2 RPM:2",
	MsfsAeroFramework::SimVarType::NORMAL,
	"Rpm"
);

MsfsAeroFramework::SimVar fe_aux_motor_1_rpm_master = MsfsAeroFramework::SimVar(
	"FE_HydroAuxMotorRPM_1",
	MsfsAeroFramework::SimVarType::NORMAL,
	"Number"
);

MsfsAeroFramework::SimVar fe_aux_motor_2_rpm_master = MsfsAeroFramework::SimVar(
	"FE_HydroAuxMotorRPM_2",
	MsfsAeroFramework::SimVarType::NORMAL,
	"Number"
);

MsfsAeroFramework::SimVar eng_1_n2_percent_master = MsfsAeroFramework::SimVar(
	"TURB ENG N2:1",
	MsfsAeroFramework::SimVarType::NORMAL,
	"Percent"
);

MsfsAeroFramework::SimVar eng_2_n2_percent_master = MsfsAeroFramework::SimVar(
	"TURB ENG N2:2",
	MsfsAeroFramework::SimVarType::NORMAL,
	"Percent"
);

MsfsAeroFramework::SimVar normal_n2_3_master = MsfsAeroFramework::SimVar(
	"ENG N2 RPM:3",
	MsfsAeroFramework::SimVarType::NORMAL,
	"Rpm"
);

MsfsAeroFramework::SimVar ambient_temperature_master = MsfsAeroFramework::SimVar(
	"AMBIENT TEMPERATURE",
	MsfsAeroFramework::SimVarType::NORMAL,
	"Celsius"
);

MsfsAeroFramework::SimVar fe_rmp_13_operation_master = MsfsAeroFramework::SimVar(
	"FE_RMP_13_OPERATION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar fe_rmp_23_operation_master = MsfsAeroFramework::SimVar(
	"FE_RMP_23_OPERATION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar hydro_line_1_1_operation_master = MsfsAeroFramework::SimVar(
	"HYDRO_LINE_1_1_OPEREATION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar hydro_line_1_2_operation_master = MsfsAeroFramework::SimVar(
	"HYDRO_LINE_1_2_OPEREATION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar hydro_line_2_1_operation_master = MsfsAeroFramework::SimVar(
	"HYDRO_LINE_2_1_OPEREATION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar hydro_line_2_2_operation_master = MsfsAeroFramework::SimVar(
	"HYDRO_LINE_2_2_OPEREATION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar hydro_line_3_1_operation_master = MsfsAeroFramework::SimVar(
	"HYDRO_LINE_3_1_OPEREATION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar hydro_line_3_2_operation_master = MsfsAeroFramework::SimVar(
	"HYDRO_LINE_3_2_OPEREATION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);


MsfsAeroFramework::SimVar dc_left_out_aileron_master = MsfsAeroFramework::SimVar(
 	"DC_LEFT_OUT_AILERON",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar dc_left_in_aileron_master = MsfsAeroFramework::SimVar(
 	"DC_LEFT_IN_AILERON",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar dc_right_out_aileron_master = MsfsAeroFramework::SimVar(
 	"DC_RIGHT_OUT_AILERON",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar dc_right_in_aileron_master = MsfsAeroFramework::SimVar(
 	"DC_RIGHT_IN_AILERON",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar dc_upper_rudder_master = MsfsAeroFramework::SimVar(
 	"DC_UPPER_RUDDER",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar dc_lower_rudder_master = MsfsAeroFramework::SimVar(
 	"DC_LOWER_RUDDER",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar actuator_load_var_master = MsfsAeroFramework::SimVar(
 	"ACTUATOR LOAD",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar yoke_x_position_master = MsfsAeroFramework::SimVar(
 	"YOKE X POSITION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar yoke_y_position_master = MsfsAeroFramework::SimVar(
 	"YOKE Y POSITION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar rudder_position_master = MsfsAeroFramework::SimVar(
 	"RUDDER POSITION",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Number"
);

MsfsAeroFramework::SimVar cir_hydro_aux_pump_1_master = MsfsAeroFramework::SimVar(
 	"CIRCUIT_AUX_HYD_PMP_1",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar cir_hydro_aux_pump_2_master = MsfsAeroFramework::SimVar(
 	"CIRCUIT_AUX_HYD_PMP_2",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);

MsfsAeroFramework::SimVar o_adg_sw_master = MsfsAeroFramework::SimVar(
 	"O_ADG_SW",
	MsfsAeroFramework::SimVarType::LOCAL,
	"Bool"
);







