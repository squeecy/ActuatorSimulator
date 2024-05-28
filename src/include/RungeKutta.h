#pragma once

#include "Aircraft.h"


class RungeKutta {
private:
	#define LBS_2_N(x) ((x) * 4.4482)
	#define PSI_2_PA(x) ((x) * 6894.0)
	double TIME_STEP;
	double find_actuator_derivative(double pos, double fluid_velocity, double valve_position, double area, double load, double P);
public:
	RungeKutta(double TIME_STEP_INIT);
	double RK4_actuator(double position, double fluid_velocity, double valve_position, double area, double load, double dt, double P);

};