#include <iostream>
#include "engine_pump.h"

using namespace MsfsAeroFramework;

HydraulicSystem system_1_1;
HydraulicSystem system_1_2;
HydraulicSystem system_2_1;
HydraulicSystem system_2_2;
HydraulicSystem system_3_1;
HydraulicSystem system_3_2;

namespace default_engine_pump_params
{
    EnginePumpParams params = {
        7,
        3.048,
        0.5144,
        0.0508,
        1

    };

}
DC10HydraulicEnginePumpSystem engine_pump_1_1(default_engine_pump_params::params);
DC10HydraulicEnginePumpSystem engine_pump_3_1(default_engine_pump_params::params);
DC10ReversibleMotorPump RMP_test;



void operate_engine_pump_1_1(DC10HydraulicEnginePumpSystem& attrib, HydraulicSystem* sys, bool RMP, double N2, double temperature, double dt)
{
    attrib.update_environmnet(N2, temperature, dt);
    attrib.systemPressure(RMP);

    sys->pressure = attrib.hydraulic_pressure;
    sys->flow_rate = attrib.hydraulic_Q;

}

void create_engine_pump(DC10HydraulicEnginePumpSystem& engine_pump, 
    DC10ReversibleMotorPump& reverse_pump, 
    HydraulicSystem* sys, bool RMP, 
    double N2, double temperature, 
    int id, 
    double dt)
{
    engine_pump.check_pump_operation();
    engine_pump.update_environmnet(N2, temperature, dt);

    if(engine_pump.operating)
    {
        engine_pump.systemPressure(RMP);
    }

    if(RMP)
    {
        
        if(id != 3)
        {
            //reverse motor pump always uses pump 3 to supply support pressure
            reverse_pump.simulate_pressure_exchange(engine_pump, engine_pump_3_1.hydraulic_pressure, 1, dt);
            engine_pump_3_1.hydraulic_pressure -= reverse_pump.pressure_change;
        }
    }


    sys->pressure = engine_pump.hydraulic_pressure;
    sys->flow_rate = engine_pump.hydraulic_Q;

}

void update_hydroline_variables()
{
    fe_hydro_pressure_1.current_value = engine_pump_1_1.hydraulic_pressure;
    fe_hydro_pressure_3.current_value = engine_pump_3_1.hydraulic_pressure;

    fe_hydro_flow_1.current_value = engine_pump_1_1.hydraulic_Q;
    fe_hydro_flow_3.current_value = engine_pump_3_1.hydraulic_Q;

}
