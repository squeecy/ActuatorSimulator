#ifndef HYDRAULICS_H
#define HYDRAULICS_H

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>

#define TIME_STEP 0.05

double lerp(double x, double x1, double y1, double x2, double y2);

double M_area_to_in_Area(double input);

double GPM2cbmSEC(double x);

class PIDController {
	double Kp;
	double Ki;
	double Kd;
	double setpoint;
	double lastError;
	double integral;
private:
    double lastError_init;
    double integral_init;
public:
	double control;

public:
    PIDController(double Kp_init, double Ki_init, double Kd_init, double setpoint_init);

    double calculate(double displacement, double pressure, double min, double max);
    double publishRegulatePressure(); 
};

class HydraulicEngPump{
private:
	double Q;
	double VALVE_COUNT;
	double L_PIPE;
	double D_PIPE;
	double VALVE_BORE;
	double displacementStroke;
	double pumpDisplacement;
	double fluidViscosity;
	double fluidViscosityPercent;
	double deltaP;
	double f;
	double flowFactor;
	double Q_pressureRelief;
	double effectiveArea;
	double relievedPressure;
	double pumpNumber;
	PIDController classRegulate;

public:

	HydraulicEngPump(double VALVE_COUNT_init, double L_PIPE_init, double D_PIPE_init, double VALVE_BORE_init, PIDController& regulate, double PumpNumber_init);

	double hydraulicPressure;

	double cbM2Gal(double cbM);

	double GPM2VEL(double x, double D);

	double toPercent(double x, double x1, double x2);

	void hydraulicFluidViscosity(double fluidTemp);

	void regulateDisplacement();

	double orificeEquation();

	void simulateEngPump(double driverRPM, double fluidTemp, double hydroPressure);

	void findDarcyFriction(double driverRPM, double fluidTemp);

	void systemDeltaP(double driverRPM, double fluidTemp, double rho);

	double systemPressure(double driverRPM, double fluidTemp);

	double simulatePressureDisipate(double t);

	void updateFlow(double flowRateDelta);

	double publishFlow();

	double publishPressure();


};


class LinearActuator {
private:
    double previousPosition;
    double currentPosition;
    double requestedPosition;
    double positionOfActuator;
    double deltaPosition;
    double deltaRequest;
    double actuatorVelocity;
    double actuatorEntryFlowRate;
	double actuatorFlowRate;
    double boreArea;
    double rodArea;
    double actuatorLength;
    double cylinder_entrence_velocity;
    double cylinder_exit_velocity;
    double extensionRatio;
    double fluidPressure;
    double deltaP_actuator;
    double deltaP_valve;
	double Q;
    double bore_side_area;
    double rod_side_area;
	double L;
	double D;
	double pos;
	double last_pos;
	double delta_displacement;
	double volume_extension_ratio;
	double total_volume_to_actuator;
	double total_volume_to_reservoir;
	double entry_valve_opening;
	double hydro_press_loss;
	double actuator_fluid_velocity;
	double valve_cross_area;
	double actuator_valve_radius;

public:
    LinearActuator(double boreArea, double rodArea, double actuatorLength, double valve_radius); 

	void actuator_entry_fluid_velocity(double Q);

    double actuator_entry_pressure(double Q); 

	double simulate_actuator_fluid_velocity();

	void update_position();

	void actuator_valve_displacement(double requested_position);

	double simulatePressureLoss(HydraulicEngPump& hydro);

	double position(double time, double request, bool retract, double Q, double P);

	double valve_requested_flow_rate();

	double publish_position();

	double publish_actuator_vol();

	double publish_actuator_fluid_flow();
};

class AuxPump{
	private:
		double auxPumpPSI;
		int lastPumpPosition;
		HydraulicEngPump pump;
	public:

		AuxPump(HydraulicEngPump hydroPump);
		void lastAuxPosition(int auxPumpSwitch);
		void simulateAuxPump(int auxPumpSwitch, double time);
		double publishAuxPressure();
};

#endif
