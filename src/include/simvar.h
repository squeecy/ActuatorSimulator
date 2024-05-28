#ifndef SIMVAR_H
#define SIMVAR_H

#include "aircraft.h"


// switches
extern MsfsAeroFramework::SimVar fe_eng_1_hyd_pump_1_switch_master;
extern MsfsAeroFramework::SimVar fe_eng_1_hyd_pump_2_switch_master;
extern MsfsAeroFramework::SimVar fe_eng_2_hyd_pump_1_switch_master;
extern MsfsAeroFramework::SimVar fe_eng_2_hyd_pump_2_switch_master;
extern MsfsAeroFramework::SimVar fe_eng_3_hyd_pump_1_switch_master;
extern MsfsAeroFramework::SimVar fe_eng_3_hyd_pump_2_switch_master;
extern MsfsAeroFramework::SimVar fe_13_hydro_switch_master;
extern MsfsAeroFramework::SimVar fe_23_hydro_switch_master;

// hydro pressure vars
extern MsfsAeroFramework::SimVar fe_hydro_pressure_1_master;
extern MsfsAeroFramework::SimVar fe_hydro_pressure_2_master;
extern MsfsAeroFramework::SimVar fe_hydro_pressure_3_master;

// hydro pump pressure vars
extern MsfsAeroFramework::SimVar fe_eng_pump_pressure_1_1_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_pressure_1_2_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_pressure_2_1_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_pressure_2_2_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_pressure_3_1_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_pressure_3_2_master;

// hydro line pressure vars
extern MsfsAeroFramework::SimVar fe_line_hydro_pressure_1_1_master;
extern MsfsAeroFramework::SimVar fe_line_hydro_pressure_1_2_master;
extern MsfsAeroFramework::SimVar fe_line_hydro_pressure_2_1_master;
extern MsfsAeroFramework::SimVar fe_line_hydro_pressure_2_2_master;
extern MsfsAeroFramework::SimVar fe_line_hydro_pressure_3_1_master;
extern MsfsAeroFramework::SimVar fe_line_hydro_pressure_3_2_master;

// hydro flow vars
extern MsfsAeroFramework::SimVar fe_hydro_flow_1_master;
extern MsfsAeroFramework::SimVar fe_hydro_flow_2_master;
extern MsfsAeroFramework::SimVar fe_hydro_flow_3_master;

// hydro pump flow vars
extern MsfsAeroFramework::SimVar fe_eng_pump_flow_1_1_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_flow_1_2_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_flow_2_1_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_flow_2_2_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_flow_3_1_master;
extern MsfsAeroFramework::SimVar fe_eng_pump_flow_3_2_master;

//engine vars
extern MsfsAeroFramework::SimVar normal_n2_1_master;
extern MsfsAeroFramework::SimVar normal_n2_2_master;
extern MsfsAeroFramework::SimVar normal_n2_3_master;
extern MsfsAeroFramework::SimVar eng_1_n2_percent_master;
extern MsfsAeroFramework::SimVar eng_2_n2_percent_master;


// environment vars
extern MsfsAeroFramework::SimVar ambient_temperature_master;

// hydro rmp vars
extern MsfsAeroFramework::SimVar fe_rmp_13_operation_master;
extern MsfsAeroFramework::SimVar fe_rmp_23_operation_master;

#endif // 