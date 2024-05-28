#include <iostream>
#include "actuator.h"
#include "aircraft.h"

using namespace MsfsAeroFramework;



namespace aileron_actuator_params
{
    ActuatorParams params = {
        0.145277,   // bore_area_init
        0.104682,   // rod_area_init
        0.05908,    // actuator_value_radius_init
        0.0,        // actuator_position_init
        0.08,       // ideal_flow_init
        true        // active_control_init
    };

}DC10ActuatorSystem aileron_actuator_system(aileron_actuator_params::params);

namespace rudder_actautor_params 
{
    ActuatorParams params = {
        0.145277,   // bore_area_init
        0.104682,   // rod_area_init
        0.05908,    // actuator_value_radius_init
        0.0,        // actuator_position_init
        0.08,       // ideal_flow_init
        true        // active_control_init
    };

}DC10ActuatorSystem rudder_actuator_system(rudder_actautor_params::params);


template <typename T, typename U>
void aileron_actuator(DC10ActuatorSystem& attrib, T& system_1, U& system_2, double axis, double delta_time)
{

    //attrib.fe_hydro_pressure_1.current_value = system_1.pressure;
    //attrib.fe_hydro_flow_1.current_value = system_1.flow_rate;
    attrib.yoke_x_position.current_value = axis * 0.01;
    attrib.actuator_load_var.current_value = 100.0;

    attrib.update_requirments(false);

    attrib.update_request_control_surface(AILERON);

    attrib.update_actuator_position(delta_time);
    attrib.update_actuator_control_surface(AILERON);

    //std::cout << attrib.dc_left_in_aileron.current_value << std::endl;

}template void aileron_actuator<HydraulicSystem, HydraulicSystem>(DC10ActuatorSystem& attrib, HydraulicSystem&, HydraulicSystem&, double yoke_x, double delta_time);



template <typename T, typename U>
void rudder_actuator(DC10ActuatorSystem& attrib, T& system_1, U& system_2, double axis, double delta_time)
{

    //attrib.fe_hydro_pressure_1.current_value = system_1.pressure;
    //attrib.fe_hydro_flow_1.current_value = system_1.flow_rate;
    attrib.rudder_position.current_value = axis * 0.01;
    attrib.actuator_load_var.current_value = 100.0;

    attrib.update_requirments(false);

    attrib.update_request_control_surface(RUDDER);

    attrib.update_actuator_position(delta_time);
    attrib.update_actuator_control_surface(RUDDER);

    //std::cout << attrib.dc_upper_rudder.current_value << std::endl;

}template void rudder_actuator<HydraulicSystem, HydraulicSystem>(DC10ActuatorSystem& attrib, HydraulicSystem&, HydraulicSystem&, double yoke_x, double delta_time);
