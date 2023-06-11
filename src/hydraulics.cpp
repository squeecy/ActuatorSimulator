#include <hydraulics.h>

#define TIME_STEP 0.05

double lerp(double x, double x1, double y1, double x2, double y2)
{
	return (y1 + (x - x1) * ((y2 - y1) / (x2 - x1)));
}

double M_area_to_in_Area(double input)
{
	return input * 1550.0;
}

double GPM2cbmSEC(double x)
{
	return x * 1.0;
}


PIDController::PIDController(double Kp_init, double Ki_init, double Kd_init, double setpoint_init)
{
	Kp = Kp_init;
	Ki = Ki_init;
	Kd = Kd_init;
	setpoint = setpoint_init;
	lastError = 0.0;
	integral = 0.0;
}

double PIDController::calculate(double displacement, double pressure, double min, double max) {
	double error = setpoint - pressure;
	double proportional = Kp * error;
	integral += Ki * error;
	double integralLimited = std::min(std::max(integral, min), max);
	double derivative = Kd * (error - lastError);
	double controlSignal = displacement + proportional + integralLimited + derivative;
	this->control = std::min(std::max(controlSignal, min), max);
	lastError = error;
	return this->control;
}

double PIDController::publishRegulatePressure() {
	return setpoint;
}


HydraulicEngPump::HydraulicEngPump
	   (double VALVE_COUNT_init, double L_PIPE_init, double D_PIPE_init, double VALVE_BORE_init, PIDController& regulate, double PumpNumber_init):
		   VALVE_COUNT(VALVE_COUNT_init), L_PIPE(L_PIPE_init), D_PIPE(D_PIPE_init), VALVE_BORE(VALVE_BORE_init),
			classRegulate(regulate), pumpNumber(PumpNumber_init)
{
	Q = 0.0;
	displacementStroke = 0.0;
	pumpDisplacement = 0.0;
	fluidViscosity = 0.0;
	fluidViscosityPercent = 0.0;
	deltaP = 0.0;
	hydraulicPressure = 0.0;
	f = 0.0;
	flowFactor = 0.0;
	Q_pressureRelief = 0.0;
	effectiveArea = 0.0;
	relievedPressure = 0.0;
}

double HydraulicEngPump::cbM2Gal(double cbM)
{
	return (cbM * 264.172);
}

double HydraulicEngPump::GPM2VEL(double x, double D)
{
	return (x/D);
}

double HydraulicEngPump::toPercent(double x, double x1, double x2)
{
	return lerp(x, x1, 0.0, x2, 1.0);
}

void HydraulicEngPump::hydraulicFluidViscosity(double fluidTemp)
{
	this->fluidViscosity = lerp(fluidTemp, -40.0, 2600, 100.0, 3.5);
	this->fluidViscosityPercent = (this->toPercent(this->fluidViscosity, -40.0, 100.0));
}

void HydraulicEngPump::regulateDisplacement()
{
	this->displacementStroke = classRegulate.calculate(this->displacementStroke, this->hydraulicPressure, 0.0, 10.0);
	this->effectiveArea = classRegulate.calculate(this->effectiveArea, this->hydraulicPressure, 0.0, 7.5);
}

double HydraulicEngPump::orificeEquation()
{
	return 1.2 * (M_PI * std::pow(this->D_PIPE / 2.0, 2)) * std::sqrt(std::abs(2.0 * (this->hydraulicPressure / 0.69)));
}

void HydraulicEngPump::simulateEngPump(double driverRPM, double fluidTemp, double hydroPressure)
{
	this->hydraulicFluidViscosity(fluidTemp);
	this->regulateDisplacement();

	this->pumpDisplacement = (this->VALVE_COUNT) * M_PI * pow(this->VALVE_BORE / 2.0, 2) * this->displacementStroke;

	//multiplaying by 0.0001 because of unit error/divide error somewhere,
	//if this is not done 34000 GPM will be the flow rate instead of 34GPM
	//this is not a factor in the aircraft build, so it is fine for testing
	if(this->pumpDisplacement != 0.0)
		this->Q = ((this->pumpDisplacement * (driverRPM * 1000.0)) / this->cbM2Gal(this->pumpDisplacement)) * 0.001;
	else
		this->Q = ((this->pumpDisplacement * (driverRPM * 1000.0)) / this->cbM2Gal(1)) * 0.001;

	if(this->Q - this->orificeEquation() > 0.0)
		this->Q -= this->orificeEquation();
}

void HydraulicEngPump::findDarcyFriction(double driverRPM, double fluidTemp)
{
	this->simulateEngPump(driverRPM, fluidTemp, this->hydraulicPressure);
	double Re = (4.0 * this->Q) / (M_PI * this->D_PIPE * this->fluidViscosity);
	if(Re != 0.0)
		this->f = 0.3164 / (pow(Re, 0.25));
	else
		this->f = 0.0;
}

void HydraulicEngPump::systemDeltaP(double driverRPM, double fluidTemp, double rho)
{
	double V = this->GPM2VEL(this->Q, this->D_PIPE);

	this->findDarcyFriction(driverRPM, fluidTemp);
	this->deltaP = this->f * (this-> L_PIPE / this->D_PIPE) * (rho * pow(V, 2.0)) / 2.0;
}

double HydraulicEngPump::systemPressure(double driverRPM, double fluidTemp)
{
	this->systemDeltaP(driverRPM, fluidTemp, 0.69);
	if(this->f != 0.0)
	{
		this->flowFactor = this->Q / (sqrt(std::abs(this->deltaP)));
		this->hydraulicPressure += (sqrt(std::abs(this->Q) / this->flowFactor));
	}
	else{
		this->flowFactor = 0.0;
		this->hydraulicPressure += 0.0;
	}

	return this->hydraulicPressure;
}

double HydraulicEngPump::simulatePressureDisipate(double t)
{
	this->hydraulicPressure -= this->hydraulicPressure * (std::pow(exp(1.0), -(0.265 * t)));
	return this->hydraulicPressure;
}


void HydraulicEngPump::updateFlow(double flowRateDelta)
{
	if(this->pumpNumber == 3)
		this->Q = this->Q - flowRateDelta;
}

double HydraulicEngPump::publishFlow()
{
	return GPM2cbmSEC(this->Q);
}

double HydraulicEngPump::publishPressure()
{
	return this->hydraulicPressure;
}

LinearActuator::LinearActuator(double boreArea, double rodArea, double actuatorLength, double valve_radius) {
	previousPosition = 0.0;
	currentPosition = 0.0;
	requestedPosition = 0.0;
	positionOfActuator = 0.0;
	deltaPosition = 0.0;
	deltaRequest = 0.0;
	actuatorVelocity = 0.0;
	actuatorFlowRate = 0.0;
	delta_displacement = 0.0;
	actuatorEntryFlowRate = 0.0;
	cylinder_entrence_velocity = 0.0;
	cylinder_exit_velocity = 0.0;
	this->boreArea = boreArea;
	this->rodArea = M_PI * pow((rodArea / 2.0), 2);
	this->actuatorLength = actuatorLength;
	this->fluidPressure = fluidPressure;
	extensionRatio = this->boreArea / this->rodArea;
	deltaP_actuator = 0.0;
	deltaP_valve = 0.0;
	Q = 0.0; 
	bore_side_area = M_PI * pow((boreArea / 2.0), 2);
	rod_side_area = bore_side_area - this->rodArea;
	L = 0.0762;
	D = 0.1016;
	pos = 1.0;
	last_pos = 0.0;
	delta_displacement = 0.0;
	//TODO
	volume_extension_ratio = 0.1;
	total_volume_to_actuator = 0.0;
	total_volume_to_reservoir = 0.0;
	entry_valve_opening = 0.0;
	hydro_press_loss = 0.0;
	actuator_fluid_velocity = 0.0;
	valve_cross_area = 0.0;
	actuator_valve_radius = valve_radius;

}



	void LinearActuator::actuator_entry_fluid_velocity(double Q)
	{
		this->valve_cross_area = (M_PI * pow((this->actuator_valve_radius / 2.0), 2));
		this->actuatorEntryFlowRate = Q;
	}

    double LinearActuator::actuator_entry_pressure(double Q) {
		this->actuator_entry_fluid_velocity(Q);
		double f = 0.00015;
		double rho = 0.69;
		this->deltaP_actuator =  f * (this->L / this->D) * (rho * pow(this->actuatorEntryFlowRate, 2) / 2.0);

		return this->deltaP_actuator;
    }

	double LinearActuator::simulate_actuator_fluid_velocity()
	{
		//Cv * A * v(dP / rho))
		this->actuator_fluid_velocity = 100.6 * this->valve_cross_area * std::sqrt(deltaP_actuator / 0.8);

	}

	void LinearActuator::update_position()
	{
		this->delta_displacement = this->pos - this->last_pos;
		this->last_pos = this->pos;
	}

	void LinearActuator::actuator_valve_displacement(double requested_position)
	{
		this->update_position();
		double vol_to_actuator = 0.0;
		double vol_to_reservoir = 0.0;

		if(this->delta_displacement > 0.0)
		{
			vol_to_actuator = this->delta_displacement * this->bore_side_area;
			vol_to_reservoir = vol_to_actuator / this->volume_extension_ratio;
		}else if(this->delta_displacement < 0.0)
		{
			vol_to_actuator = -this->delta_displacement * this->rod_side_area;
			vol_to_reservoir = vol_to_actuator * this->volume_extension_ratio;
		}

		this->total_volume_to_actuator += vol_to_actuator;
		this->total_volume_to_reservoir += vol_to_reservoir;
	}


	double LinearActuator::simulatePressureLoss(HydraulicEngPump& hydro)
	{
		//hydro.hydraulicPressure -= (0.5 * pow((this->actuator_fluid_velocity / this->boreArea), 2) * (1.0 / 840.0));
		//this->hydro_press_loss =  (0.5 * pow((2.94/this->boreArea), 2) * (1.0 / 840.0));
		this->hydro_press_loss =  (0.5 * pow((16.94/this->boreArea), 2) * (1.0 / 840.0));
		hydro.hydraulicPressure -= this->hydro_press_loss;
	}

	double LinearActuator::position(double time, double request, bool retract, double Q, double P)
	{
		actuator_entry_pressure(Q); 
		simulate_actuator_fluid_velocity();
		actuator_valve_displacement(0.0);
		update_position();
		double currentTime = 0.0;
		double direction = 1.0;
		double deltaTime = std::min(TIME_STEP, time - currentTime);

		//speed = (Q * dT ( 1 - loadFactor)) / (Force * Pressure)
			//loadFactor is the resistance of the opposing force being moved
		if(retract)
			this->pos -=  (this->actuator_fluid_velocity * deltaTime * (1.0 - 0.9)) / (200.0 / P);
		else
			this->pos += (this->actuator_fluid_velocity * deltaTime * (1.0 - 0.9)) / (200.0 / P);

		//limit actuator to physical boundaries
		if(this->pos > 1.0)
			this->pos = 1.0;
		if(this->pos < -1.0)
			this->pos = -1.0;

		currentTime += deltaTime;

		return this->pos;
	}

	double LinearActuator::valve_requested_flow_rate()
	{
		//ratio (0<->1)
		//this->entry_valve_opening = 1.0/(this->ACTUATOR_MAX_FLOW_RATE, req_Q); 
		return this->entry_valve_opening;
	}

	double LinearActuator::publish_position()
	{
		return this->delta_displacement;
	}

	double LinearActuator::publish_actuator_vol()
	{
		return this->total_volume_to_reservoir;
	}

	double LinearActuator::publish_actuator_fluid_flow()
	{
		return this->actuator_fluid_velocity;
	}


	AuxPump::AuxPump(HydraulicEngPump hydroPump) : auxPumpPSI(0.0), lastPumpPosition(1), pump(hydroPump){}

	void AuxPump::lastAuxPosition(int auxPumpSwitch)
	{
		if(auxPumpSwitch == 2)
			this->lastPumpPosition = 2;
		if(auxPumpSwitch == 0)
			this->lastPumpPosition = 0;
	}

	void AuxPump::simulateAuxPump(int auxPumpSwitch, double time)
	{
		this->lastAuxPosition(auxPumpSwitch);
		//switch set to on
		if(auxPumpSwitch == 1)
		{
			//electrical check is not preformed 
			//driven electrically, so give a constant value that is only operating when ACGS power is on
			this->auxPumpPSI = pump.systemPressure(500.0, 18.0);
		}
	}

	double AuxPump::publishAuxPressure()
	{
		return this->auxPumpPSI;
	}



