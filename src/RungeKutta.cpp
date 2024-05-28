#include <iostream>
#include <cmath>
#include "RungeKutta.h"




RungeKutta::RungeKutta(double TIME_STEP_INIT)
{
	TIME_STEP = TIME_STEP_INIT;
}

double RungeKutta::find_actuator_derivative(double pos, double fluid_velocity, double valve_position, double area, double load, double P)
{
	return (fluid_velocity * valve_position* area) / (LBS_2_N(load) / (PSI_2_PA(P) * area));
}

double RungeKutta::RK4_actuator(double pos, double fluid_velocity, double valve_position, double area, double load, double dt, double P) 
{

	double k1 = find_actuator_derivative(pos, fluid_velocity, valve_position, area, load, P);
	double k2 = find_actuator_derivative(pos + 0.5 * (TIME_STEP)*k1, fluid_velocity, valve_position, area, load, P);
	double k3 = find_actuator_derivative(pos + 0.5 * (TIME_STEP)*k2, fluid_velocity, valve_position, area, load, P);
	double k4 = find_actuator_derivative(pos + (TIME_STEP)*k3, fluid_velocity, valve_position, area, load, P);

	return pos + ((TIME_STEP) / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
}