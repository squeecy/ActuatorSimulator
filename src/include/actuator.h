#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "hydraulics.h"

using namespace MsfsAeroFramework;

namespace aileron_actuator_params
{
    extern ActuatorParams params;
}extern DC10ActuatorSystem aileron_actuator_system;

namespace rudder_actuator_params
{
    extern ActuatorParams params;
}extern DC10ActuatorSystem rudder_actuator_system;



template <typename T, typename U>
void aileron_actuator(DC10ActuatorSystem& attrib, T& system_1, U& system_2, double yoke_x, double delta_time);

template <typename T, typename U>
void rudder_actuator(DC10ActuatorSystem& attrib, T& system_1, U& system_2, double axis, double delta_time);

#endif