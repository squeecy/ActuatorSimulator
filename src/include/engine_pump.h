#ifndef ENGINE_PUMP_H
#define ENGINE_PUMP_H

#include "hydraulics.h"
using namespace MsfsAeroFramework;

extern HydraulicSystem system_1_1;
extern HydraulicSystem system_1_2;
extern HydraulicSystem system_2_1;
extern HydraulicSystem system_2_2;
extern HydraulicSystem system_3_1;
extern HydraulicSystem system_3_2;

namespace default_engine_pump_params
{
    extern EnginePumpParams params;
}
extern DC10HydraulicEnginePumpSystem engine_pump_1_1;
extern DC10HydraulicEnginePumpSystem engine_pump_3_1;
extern DC10ReversibleMotorPump RMP_test;

//void create_engine_pump(DC10HydraulicEnginePumpSystem& attrib, HydraulicSystem* sys, bool RMP, double N2, double temperature, double dt);
void create_engine_pump(DC10HydraulicEnginePumpSystem& attrib, DC10ReversibleMotorPump& attribrmp, 
    HydraulicSystem* sys, bool RMP, double N2, double temperature, int id, double dt);
void simulate_engine_pump_1_1_state();
void update_hydroline_variables();

class DC10EnginePump //: public system
{

    //ect. 
    //ect.

    public:
        DC10HydraulicEnginePumpSystem engine_pump_1_1_test = DC10HydraulicEnginePumpSystem(default_engine_pump_params::params);
        DC10HydraulicEnginePumpSystem engine_pump_3_1_test = DC10HydraulicEnginePumpSystem(default_engine_pump_params::params);
        //DC10EnginePump() : //System("DC 10 Engine Pump System") {};
        void update();

};




#endif