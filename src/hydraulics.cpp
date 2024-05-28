//#include <RungeKutta.h>
#include <algorithm>
#include <iostream>
#include "interp.h"
#include "hydraulics.h"

using namespace MsfsAeroFramework;

#define IN3_2_GPM(x) ((x) * 0.0043)
#define LBS_2_N(x) ((x) * 4.4482)
#define PSI_2_PA(x) ((x) * 6894.0)
#define M2_2_in2(x) ((x) * 1550.0)
#define cbM2Gal(x) (x * 264.172)
#define M_sqr_2_in_sqr(x) (x * 1550.0)
#define GPS_2_M3S(x) (x/264.2)
#define lerp(x, x1, y1, x2, y2) (y1 + (x - x1) * ((y2 - y1) / (x2 - x1)))

namespace MsfsAeroFramework {
	//RungeKutta RK(0.0007);



	double DC10HydraulicEnginePumpSystem::GPM2VEL(double x, double D)
	{
		return (x / D);
	}


	void DC10HydraulicEnginePumpSystem::hydraulicFluidViscosity()
	{
		this->fluidViscosity = lerp(this->fluid_temperature, -40.0, 2600, 100.0, 3.5);
	}

	void DC10HydraulicEnginePumpSystem::regulate_displacement()
	{

		const std::vector<DataPoint> displacement_data_points = {
			{0.0, 0.55},
			{500.0, 0.55},
			{750.0, 0.6},
			{1000.0, 0.8},
			{1500.0, 0.8},
			{2000.0, 0.9},
			{2100.0, 0.6},
			{2200.0, 0.5},
			{2300.0, 0.44},
			{2500.0, 0.33},
			{2700.0, 0.33},
			{2900.0, 0.25},
			{3025.0, 0.0}
		};
		double maximum_displacement = interpolate(displacement_data_points, this->hydraulic_pressure);

		PID pid = PID(this->dt, maximum_displacement, 0, 0.1, 0.01, 0.5);

		this->displacement_stroke = pid.calculate(3000, this->hydraulic_pressure);

		//std::cout << this->hydraulic_pressure << "   " << this->displacement_stroke << std::endl;
		//std::cout << this->hydraulic_Q << "  " << this->hydraulic_pressure << std::endl;


	}

	double DC10HydraulicEnginePumpSystem::orificeEquation()
	{
		return 1.2 * (M_PI * std::pow(this->D_pipe / 2.0, 2)) * std::sqrt(std::abs(2.0 * (this->hydraulic_pressure / 0.69)));
	}

	double val = 0;
	void DC10HydraulicEnginePumpSystem::simulateEngPump()
	{
		this->hydraulicFluidViscosity();
		this->regulate_displacement();



		double rpmAddition = (this->N2) * (1.0 - (this->N2) / 9000.0);
		double hydroEngPumpRPM = (this->N2) + rpmAddition;


		this->pump_displacement = (this->valve_count) * M_PI * std::pow(this->valve_bore / 2.0, 2) * this->displacement_stroke;

		if (!this->RMP)
		{
			if (this->pump_displacement != 0.0)
			{
				this->hydraulic_Q = IN3_2_GPM((this->displacement_stroke * (hydroEngPumpRPM)) / 0.95);
				//std::cout << "pump_displacement: " << this->pump_displacement << " : " << this->hydraulic_Q << std::endl;
			}
			else
			{
				this->hydraulic_Q = ((this->displacement_stroke * (hydroEngPumpRPM)) / cbM2Gal(1));
				//std::cout << "pump_displacement: " << this->pump_displacement << " : " << this->hydraulic_Q << std::endl;
			}

			if (this->hydraulic_Q <= 0.0)
				this->hydraulic_Q = 0.0;

		}
		if(this->switch_id == 1 && this->pump_id == 1)
		{
			//std::cout << this->hydraulic_Q << std::endl;
			
		}
	}


	void DC10HydraulicEnginePumpSystem::findDarcyFriction()
	{
		this->simulateEngPump();
		this->Re = (4.0 * this->hydraulic_Q) / (M_PI * this->D_pipe * this->fluidViscosity);
		if (this->Re != 0.0)
			this->f = 0.3164 / (pow(this->Re, 0.25));
		else
			this->f = 0.0;
	}

	void DC10HydraulicEnginePumpSystem::systemDeltaP()
	{
		const double rho = 0.69;
		double V = this->GPM2VEL(this->hydraulic_Q, this->D_pipe);
		this->dt = dt;

		this->findDarcyFriction();
		this->deltaP = this->f * (this->L_pipe / this->D_pipe) * (rho * pow(V, 2.0)) / 2.0;
	}

	void DC10HydraulicEnginePumpSystem::systemPressure()
	{
		this->systemDeltaP();
		if (this->f != 0.0)
		{
			this->flowFactor = this->hydraulic_Q / (sqrt(abs(this->deltaP)));
			this->hydraulic_pressure += (((this->hydraulic_Q * 9.27) / 50.27))  ;
		}
		else {
			this->flowFactor = 0.0;
			this->hydraulic_pressure += 0.0;
		}


	}

	void DC10HydraulicEnginePumpSystem::simulatePressureDisipate(double pressure, double t, double k)
	{
		double pressure_disipate;
		double acTime;
		acTime += t;

		pressure_disipate = pressure * (pow(exp(1.0), (-0.265 * t)));

		this->hydraulic_pressure -= pressure_disipate;

	}



	void DC10HydraulicEnginePumpSystem::update_context()
	{

		switch (this->pump_id)
		{
			case 1:
				this->N2 = normal_n2_1->current_value;
				break;
			case 2:
				this->N2 = normal_n2_1->current_value;
				break;
			case 3:
				this->N2 = normal_n2_3->current_value;
				break;

			//default:
				//std::cout << "Error: Unknown pump id from update_context function (HydraulicSystem.cpp)" << std::cout;
		}
	}

	void DC10HydraulicEnginePumpSystem::check_pump_operation()
	{
		const double FAILURE_PSI = 2400.0;

		switch (this->switch_id)
		{
		case 1:
			this->operating = (double)(this->switch_position != 0);
			break;
		case 2:
			//this->operating = (double)(this->switch_id == 2 && 
				//this->switch_position == SWITCH_AUTO && fe_eng_2_pump_pressure_member->current_value < FAILURE_PSI);
			if (this->switch_position == 1)
			{
				this->operating = (double)(this->switch_id == 1 && 
					this->switch_position == 2 && (*fe_eng_2_pump_pressure_member)->current_value < FAILURE_PSI);
			}
			else {
				this->operating = (double)(this->switch_position != -1);
			}
			break;
		default:
			break;
		}

		//std::cout << fe_eng_1_hyd_pump_1_switch.current_value << std::endl;
			
	}

	//FIXME in main
	void DC10HydraulicEnginePumpSystem::check_pump_dissipate()
	{


		if(this->pump_id != 3)
		{
			if(operating != 1 && (*fe_rmp_pump_operation_member)->current_value != 0)
			{
				this->pump_state = 0;
			}else if(this->operating != 0)
			{
				this->pump_state = 1;
			}


			//eng operating, rmp is not on, hydro pressure is > 0
			if(this->pump_state == 1)
			{
				this->dissipating = (double)(this->operating != 1 && (*fe_rmp_pump_operation_member)->current_value != 1 &&this->hydraulic_pressure > 0);
			}
			else if(this->pump_state == 0)
			{
				if((*fe_rmp_switch_position_member)->current_value != 0 && this->switch_position == 0 && (*fe_rmp_pump_operation_member)->current_value
					!= 0)
				{
						//std::cout << this->operating << std::endl;
						//std::cout << (*fe_rmp_pump_operation_member)->current_value << std::endl;
						// if(this->pump_id == 2 && this->switch_id == 1)
						// 	std::cout << this->pump_id << " " << this->switch_id << std::endl;
						//std::cout << this->pump_state << std::endl;
					//this->dissipating = true;
				}

			}
			


			//rmp switch is on, eng pump switch is off, rmp is operating
		}



		switch (this->pump_id)
		{
			case 3:
				this->dissipating = (double)!((*fe_rmp_partner_1_system_member)->current_value +
					(*fe_eng_2_pump_pressure_member)->current_value >
					(*fe_hydraulic_pressure_member)->current_value && this->rmp_switch_position_combo != 0);
		default:
			break;
		}

	}

	void DC10HydraulicEnginePumpSystem::assign_member()
	{
		//write based on combination of selected engine pump and selected switch
		//hex corresponds with the combination of each
		//ex. 0x00010001: first system, first (or primary) switch
		//ex. 0x00030002: third system, second (or secondary) switch

		#define FIRST_FIRST 0x00010001
		#define FIRST_SECOND 0x00010002
		#define SECOND_FIRST 0x00020001
		#define SECOND_SECOND 0x00020002
		#define THIRD_FIRST 0x00030001
		#define THIRD_SECOND 0x00030002

		int combined_id = (this->pump_id << 16) | this->switch_id;

		switch (combined_id)
		{
			case FIRST_FIRST:
				 fe_hydraulic_pressure_member = &this->fe_hydro_pressure_1;
				 fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_1_1;
				 fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_1_2;
				 fe_rmp_pump_operation_member = &this->fe_rmp_13_operation;
				 fe_rmp_switch_position_member = &this->fe_13_hydro_switch;

				this->secondary_pump = fe_line_hydro_pressure_1_2->current_value;
				this->switch_position = fe_eng_1_hyd_pump_1_switch->current_value;

				break;
			case FIRST_SECOND:
				 fe_hydraulic_pressure_member = &this->fe_hydro_pressure_1;
				 fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_1_2;
				 fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_1_1;
				 fe_rmp_pump_operation_member = &this->fe_rmp_13_operation;
				 fe_rmp_switch_position_member = &this->fe_13_hydro_switch;

				 this->secondary_pump = fe_line_hydro_pressure_1_1->current_value;
				 this->switch_position = fe_eng_1_hyd_pump_2_switch->current_value;
				break;
			case SECOND_FIRST:
				 fe_hydraulic_pressure_member = &this->fe_hydro_pressure_2;
				 fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_2_1;
				 fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_2_2;
				 fe_rmp_pump_operation_member = &this->fe_rmp_23_operation;
				 fe_rmp_switch_position_member = &this->fe_23_hydro_switch;

				this->secondary_pump = fe_line_hydro_pressure_2_2->current_value;
				this->switch_position = fe_eng_2_hyd_pump_1_switch->current_value;
				break;
			case SECOND_SECOND:
				 fe_hydraulic_pressure_member = &this->fe_hydro_pressure_2;
				 fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_2_2;
				 fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_2_1;
				 fe_rmp_pump_operation_member = &this->fe_rmp_23_operation;
				 fe_rmp_switch_position_member = &this->fe_23_hydro_switch;

				 this->secondary_pump = fe_line_hydro_pressure_2_1->current_value;
				 this->switch_position = fe_eng_2_hyd_pump_2_switch->current_value;
				break;
			case THIRD_FIRST:
				 fe_hydraulic_pressure_member = &this->fe_hydro_pressure_3;
				 fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_3_1;
				 fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_3_2;
				 

				 this->secondary_pump = fe_line_hydro_pressure_3_2->current_value;
				 this->switch_position = fe_eng_3_hyd_pump_1_switch->current_value;
				 this->rmp_switch_position_combo = fe_rmp_13_operation->current_value + fe_rmp_23_operation->current_value;
				 this->fe_rmp_partner_1_system_member = &this->fe_hydro_pressure_1;
				 this->fe_rmp_partner_2_system_member = &this->fe_hydro_pressure_2;
				break;
			case THIRD_SECOND:
				 fe_hydraulic_pressure_member = &this->fe_hydro_pressure_3;
				 fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_3_2;
				 fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_3_1;
				 this->rmp_switch_position_combo = fe_rmp_13_operation->current_value + fe_rmp_23_operation->current_value;

				 this->secondary_pump = fe_line_hydro_pressure_3_1->current_value;
				 this->switch_position = fe_eng_3_hyd_pump_2_switch->current_value;
				 this->fe_rmp_partner_1_system_member = &this->fe_hydro_pressure_1;
				 this->fe_rmp_partner_2_system_member = &this->fe_hydro_pressure_2;
				break;
			default:
				std::cout << "combined: " << combined_id << std::endl;
				std::cout << "Error: Error assigning nullptr in assign_member fucntion (HydraulicSystem.cpp)" << std::endl;
				break;

		}
	}

	void DC10HydraulicEnginePumpSystem::make_members_current()
	{
		if (this->hydraulic_pressure < 0.0)
			this->hydraulic_pressure = 0.0;

		(*fe_eng_1_pump_pressure_member)->current_value = this->hydraulic_pressure;

		//system pressure will be the highest between the two hydro pump lines
		(*fe_hydraulic_pressure_member)->current_value = std::max((*fe_eng_1_pump_pressure_member)->current_value, 
			secondary_pump) / 100.0;


	}

	void DC10HydraulicEnginePumpSystem::update_state()
	{
		//sim_context->write(*this->fe_eng_1_pump_pressure_member);
		//sim_context->write(*this->fe_hydraulic_pressure_member);
	}

	void DC10HydraulicEnginePumpSystem::update(double delta_time)
	{
		this->dt = delta_time;
		this->update_context();
		this->assign_member();
		this->check_pump_operation();
		normal_n2_1->current_value = 7000.0;
		normal_n2_3->current_value = 7000.0;
		this->check_pump_dissipate();


		if (this->dissipating)
		{
			this->simulatePressureDisipate((*fe_hydraulic_pressure_member)->current_value, delta_time, 0.266);
		}


		if (this->operating)
		{
			this->systemPressure();

			//std::cout << this->hydraulic_pressure << std::endl;
			//std::cout << (*fe_eng_1_pump_pressure_member)->current_value << std::endl;
			//std::cout << this->hydraulic_Q << std::endl;

		}
		//std::cout << this->operating << std::endl;
		//std::cout << this->switch_position << std::endl;

		this->make_members_current();

		//this->update_state();
	}

#define SWITCH_OFF 1 
#define SWITCH_ON 0
#define SWITCH_AUTO 2 
	


	void DC10ReversibleMotorPump::assign_member()
	{
		switch (this->switch_id)
		{
		case 1:
			fe_hydro_slave_member = &this->fe_hydro_pressure_1;
			fe_engine_n2_member = &this->eng_1_n2_percent;
			this->switch_position = fe_13_hydro_switch->current_value;
			fe_rmp_pump_operation_member = &fe_rmp_13_operation;
			break;
		case 2:
			fe_hydro_slave_member = &this->fe_hydro_pressure_2;
			fe_engine_n2_member = &this->eng_2_n2_percent;
			this->switch_position = fe_23_hydro_switch->current_value;
			fe_rmp_pump_operation_member = &fe_rmp_23_operation;

			break;

		default:
			break;
		}
	}

	void DC10ReversibleMotorPump::check_RMP_operation()
	{
		#define SWITCH_OFF 0 
		#define SWITCH_ON -1 
		#define SWITCH_AUTO 1

		const double MIN_N2 = 45.0;

		switch (this->switch_position)
		{
		case SWITCH_OFF:
			this->operating = 0;
			break;
		case SWITCH_ON:
			this->operating = 1;
			break;
		case SWITCH_AUTO:
			this->operating = (double)((*fe_engine_n2_member)->current_value < MIN_N2);
			break;

		default:
			break;
		}
	}

	void DC10ReversibleMotorPump::simulate_pressure_exchange(double delta_time)
	{
		const double cylinder_area = 40.75; 
		const double pump_diameter = 2.0; 
		double pressure_change;

		double area = (pump_diameter * pump_diameter * M_PI) / 4.0;

		double delta_p = (fe_hydro_pressure_3->current_value * 100.0) - slave_obj.hydraulic_pressure;
		//std::cout << delta_p << std::endl;

		double force = area * delta_p;

		this->pressure_change = ((force) / cylinder_area) * delta_time - 0.01 * 
			(slave_obj.hydraulic_pressure - (fe_hydro_pressure_3->current_value * 100.0)) / area;


		if (this->operating)
		{
			(*fe_rmp_pump_operation_member)->current_value = true;

			slave_obj.hydraulic_pressure += this->pressure_change;
			system_3_left_line.hydraulic_pressure += -(this->pressure_change);
			system_3_right_line.hydraulic_pressure += -(this->pressure_change);

			//"pressure regulate" TODO: make this more realistic
			if (slave_obj.hydraulic_pressure > 3000.0)
			{
				slave_obj.hydraulic_pressure -= 3.0;
			}
		}
		else {
			(*fe_rmp_pump_operation_member)->current_value = false;
		}



	}

	void DC10ReversibleMotorPump::update_state(SimContext* sim_context)
	{
		//sim_context->write(*this->fe_rmp_pump_operation_member);
	}

	void DC10ReversibleMotorPump::update(double delta_time)
	{
		this->assign_member();
		this->check_RMP_operation();
		this->simulate_pressure_exchange(delta_time);
	}







	void DC10HydraulicEnginePump::update(double delta_time)
	{
			this->engine_pump_1_1.update(delta_time);
			this->engine_pump_1_2.update(delta_time);
			this->engine_pump_2_1.update(delta_time);
			this->engine_pump_2_2.update(delta_time);
			this->engine_pump_3_1.update(delta_time);
			this->engine_pump_3_2.update(delta_time);

			this->reverse_motor_pump_1_3.update(delta_time);
			this->reverse_motor_pump_2_3.update(delta_time);
	}

}





/*OLD COLD, SAVING FOR REFERENCE*/

// using namespace MsfsAeroFramework;



// double DC10HydraulicEnginePumpSystem::GPM2VEL(double x, double D)
// {
// 	return (x/D);
// }


// void DC10HydraulicEnginePumpSystem::hydraulicFluidViscosity()
// {
// 	this->fluidViscosity = lerp(this->fluid_temperature, -40.0, 2600, 100.0, 3.5);
// }


// void DC10HydraulicEnginePumpSystem::regulate_displacement()
// {	
	
// 	const std::vector<DataPoint> displacement_data_points = {
// 		{0.0, 0.55},
// 		{500.0, 0.55},
// 		{750.0, 0.6},
// 		{1000.0, 0.8},
// 		{1500.0, 0.8},
// 		{2000.0, 0.9},
// 		{2100.0, 0.6},
// 		{2200.0, 0.5},
// 		{2300.0, 0.44},
// 		{2500.0, 0.33},
// 		{2700.0, 0.33},
// 		{2900.0, 0.25},
// 		{3025.0, 0.0}
// 	};
// 		double maximum_displacement = interpolate(displacement_data_points, this->hydraulic_pressure);

// 		PID pid = PID(this->dt, maximum_displacement, 0, 0.1, 0.01, 0.5);

// 		this->displacement_stroke = pid.calculate(3000, this->hydraulic_pressure);

// 		//std::cout << this->hydraulic_pressure << "   " << this->displacement_stroke << std::endl;
// 		//std::cout << this->hydraulic_Q << "  " << this->hydraulic_pressure << std::endl;
	

// }

// double DC10HydraulicEnginePumpSystem::orificeEquation()
// {
// 	return 1.2 * (M_PI * std::pow(this->D_pipe/2.0, 2)) * std::sqrt(std::abs(2.0 * (this->hydraulic_pressure / 0.69)));
// }

// double val = 0;
// void DC10HydraulicEnginePumpSystem::simulateEngPump(bool RMP)
// {
// 	this->hydraulicFluidViscosity();
// 	this->regulate_displacement();


// 	double rpmAddition = (this->N2) * (1.0 - (this->N2) / 9000.0);
// 	double hydroEngPumpRPM = (this->N2) + rpmAddition;


// 	this->pump_displacement = (this->valve_count) * M_PI * pow(this->valve_bore/ 2.0, 2) * this->displacement_stroke;

// 	if(!RMP)
// 	{
// 		if(this->pump_displacement != 0.0)
// 		{
// 			this->hydraulic_Q = IN3_2_GPM((this->displacement_stroke * (hydroEngPumpRPM)) / 0.95);
// 		}
// 		else
// 		{
// 			this->hydraulic_Q = ((this->displacement_stroke * (hydroEngPumpRPM)) / cbM2Gal(1));
// 		}

// 		if(this->hydraulic_Q <= 0.0)
// 			this->hydraulic_Q = 0.0;
  
// 	}
// }

// void DC10HydraulicEnginePumpSystem::findDarcyFriction(bool RMP)
// {
// 	this->simulateEngPump(RMP);
// 	this->Re = (4.0 * this->hydraulic_Q) / (M_PI * this->D_pipe* this->fluidViscosity);
// 	if(this->Re != 0.0)
// 		this->f = 0.3164 / (pow(this->Re, 0.25));
// 	else
// 		this->f = 0.0;
// }

// void DC10HydraulicEnginePumpSystem::systemDeltaP(double rho, bool RMP)
// {
// 	double V = this->GPM2VEL(this->hydraulic_Q, this->D_pipe);
// 	this->dt = dt;

// 	this->findDarcyFriction(RMP);
// 	this->deltaP = this->f * (this->L_pipe/this->D_pipe) * (rho * pow(V, 2.0)) / 2.0;
// }

// double DC10HydraulicEnginePumpSystem::systemPressure(bool RMP)
// {
// 	this->systemDeltaP(0.69, RMP);
// 	if(this->f != 0.0)
// 	{
// 		this->flowFactor = this->hydraulic_Q / (sqrt(std::abs(this->deltaP)));
// 		this->hydraulic_pressure += ((this->hydraulic_Q * 9.27) / 50.27);
// 	}
// 	else{
// 		this->flowFactor = 0.0;
// 		this->hydraulic_pressure += 0.0;
// 	}


// 	return this->hydraulic_pressure;
// }

// double DC10HydraulicEnginePumpSystem::simulatePressureDisipate(double t, double k)
// {
// 	double acTime;
// 	acTime += t;

// 	this->hydraulic_pressure -= this->hydraulic_pressure * (std::pow(exp(1.0), -(0.265 * acTime)));
// 	return this->hydraulic_pressure;
// }


// void DC10HydraulicEnginePumpSystem::updateFlow(double flowRateDelta)
// {
// 	if(this->pump_id == 3)
// 		this->hydraulic_Q = this->hydraulic_Q - flowRateDelta;
// }

// void DC10HydraulicEnginePumpSystem::update_environmnet(double N2, double temperature, double dt)
// {
// 	this->N2 = N2;
// 	this->fluid_temperature = temperature;
// 	this->dt = dt;
// }


// double DC10HydraulicEnginePumpSystem::publishFlow()
// {
// 	return this->hydraulic_Q;
// }

// double DC10HydraulicEnginePumpSystem::publishPressure()
// {
// 	return this->hydraulic_pressure;
// }

// bool DC10HydraulicEnginePumpSystem::isRMP(int switch_pos)
// {
// 	if(switch_pos == -1)
// 	{
// 		this->is_RMP = true;
// 	}else if(switch_pos == 1)
// 	{
// 		if(this->N2 < (9000.0 * 0.45))
// 		{
// 			this->is_RMP = true;
// 		}
// 	}else if(switch_pos == 0){
		//this->is_RMP = false;
	//}

	//return this->is_RMP;
//}

//void DC10HydraulicEnginePumpSystem::check_pump_operation()
//{
    //double is_switch_toggled = fe_eng_1_hyd_pump_1_switch.current_value;
    //double is_switch_toggled = 0;
    //this->operating = (double)(is_switch_toggled != 0);
//}


// namespace MsfsAeroFramework {

// 	RungeKutta RK(0.0007);

	

	
	
// 	//double output = pid_test.


// 	void DC10ActuatorSystem::actuator_entry_fluid_velocity()
// 	{
// 		this->valve_cross_area = (M_PI * pow((this->actuator_valve_radius / 2.0), 2));
// 	}

// 	void DC10ActuatorSystem::actuator_entry_pressure()
// 	{
// 		this->actuator_entry_fluid_velocity();

// 		const double f = 0.00015;
// 		//density: TODO (more realistic simulation later)
// 		const double rho = 0.69;

// 		this->deltaP_actuator = f * (this->L / this->D) * (rho * pow(this->actuator_entry_flowrate, 2) / 2.0);
// 	}

// 	void DC10ActuatorSystem::simulate_actuator_fluid_velocity()
// 	{
// 		double ratio;
// 		double Q_rod;

// 		ratio = this->rod_area / this->bore_area;
// 		Q_rod = (GPS_2_M3S(this->ideal_flow)) * ratio;

// 		this->actuator_fluid_velocity = (GPS_2_M3S(this->ideal_flow)) / this->valve_cross_area;
// 		this->actuator_min_fluid_velocity = Q_rod / this->valve_cross_area;
// 	}

// 	void DC10ActuatorSystem::refresh_actuator_position()
// 	{
// 		this->delta_displacement = this->actuator_position - this->last_actuator_position;
// 		this->last_actuator_position = this->actuator_position;
// 	}

// 	void DC10ActuatorSystem::actuator_valve_displacement(double delta_time)
// 	{
// 		double volume_to_actuator = 0.0;
// 		double volume_to_reservoir = 0.0;

// 		if (this->delta_displacement > 0.0)
// 		{
// 			volume_to_actuator = this->delta_displacement * this->bore_area;
// 			volume_to_reservoir = volume_to_actuator / this->volume_extension_ratio;
// 		}
// 		else if (this->delta_displacement < 0.0)
// 		{
// 			volume_to_actuator = this->delta_displacement * this->rod_area;
// 			volume_to_reservoir = volume_to_actuator / this->volume_extension_ratio;
// 		}

// 		if (this->active_control)
// 		{
// 			this->total_volume_to_actuator += volume_to_actuator;
// 			this->total_volume_to_reservoir += volume_to_reservoir;
// 		}
// 		else {
// 			this->total_volume_to_reservoir += volume_to_reservoir - volume_to_actuator;
// 		}

// 		if (this->delta_displacement >= 0.0)
// 		{
// 			this->signed_flow = volume_to_actuator / delta_time;
// 		}
// 		else {
// 			this->signed_flow = -volume_to_actuator / delta_time;
// 		}
// 	}


// 	void DC10ActuatorSystem::update_actuator_position(double delta_time)
// 	{
// 		this->actuator_entry_pressure();
// 		this->simulate_actuator_fluid_velocity();

// 		double delta_distance;
// 		double direction = 1.0;

// 		double delta_request = this->actuator_position - this->requested_actuator_position;


// 		double last_pos = this->actuator_position;

// 		double ratio = this->rod_area / this->bore_area;
// 		double Q_rod = (this->ideal_flow / 264.2) * ratio;

// 		double test_v = (this->ideal_flow / 264.2) / this->valve_cross_area;
// 		double min_v = Q_rod / this->valve_cross_area;

// 		if (this->active_control)
// 		{
// 			if (std::abs(delta_request) < 0.019)
// 			{
// 				this->actuator_position = this->actuator_position;
// 			}
// 			else {
// 				if (delta_request < 0.0)
// 				{
// 					this->actuator_position = RK.RK4_actuator(this->actuator_position, this->actuator_fluid_velocity, 1.0, this->rod_area, this->actuator_load, delta_time, this->actuator_P);

// 				}
// 				else if (delta_request > 0.0)
// 				{
// 					this->actuator_position = RK.RK4_actuator(this->actuator_position, -this->actuator_fluid_velocity, 1.0, this->rod_area, this->actuator_load, delta_time, this->actuator_P);
// 				}

// 			}
// 		}
// 		else {
// 			if (!this->retract)
// 				this->actuator_position -= ((min_v * 1.0 * this->bore_area * delta_time)) / (LBS_2_N(actuator_load) / (PSI_2_PA(actuator_P) * this->bore_area));
// 			else
// 				this->actuator_position += ((test_v * 1.0 * this->rod_area * delta_time)) / (LBS_2_N(actuator_load) / (PSI_2_PA(actuator_P) * this->bore_area));
// 		}

// 		//limit actuator to physical boundaries
// 		if (this->actuator_position > 1.0)
// 			this->actuator_position = 1.0;
// 		if (this->actuator_position < -1.0)
// 			this->actuator_position = -1.0;




// 		delta_distance = this->actuator_position - last_pos;

// 		this->refresh_actuator_position();
// 		this->actuator_valve_displacement(delta_time);



// 	}

// 	void DC10ActuatorSystem::update_requirments(bool is_retract)
// 	{
// 		this->actuator_Q = fe_hydro_flow_1.current_value;
// 		this->actuator_P = fe_hydro_pressure_1.current_value;
// 		this->actuator_load = this->actuator_load_var.current_value;
// 		this->retract = is_retract;
// 		this->actuator_entry_flowrate = this->actuator_Q;
// 	}

// 	void DC10ActuatorSystem::update_request_control_surface(Control_Surface surface)
// 	{
// 		switch (surface)
// 		{
// 		case AILERON:
// 			this->requested_actuator_position = this->yoke_x_position.current_value;
// 			break;
// 		case ELEVATOR:
// 			this->requested_actuator_position = this->yoke_y_position.current_value;
// 			break;
// 		case RUDDER:
// 			this->requested_actuator_position = this->rudder_position.current_value;
// 			break;
// 		default:
// 			std::cout << "ERROR: Invalid Control Surface from update_class_position() function" << std::endl;
// 		}
// 	}


// 	void DC10ActuatorSystem::update_actuator_control_surface(Control_Surface surface)
// 	{
// 		switch (surface)
// 		{
// 		case AILERON:
// 			this->dc_left_out_aileron.current_value = this->actuator_position;
// 			this->dc_left_in_aileron.current_value = this->actuator_position;
// 			this->dc_right_out_aileron.current_value = this->actuator_position;
// 			this->dc_right_in_aileron.current_value = this->actuator_position;
// 			break;
// 		case ELEVATOR:
// 			break;
// 		case RUDDER:
// 			this->dc_upper_rudder.current_value = this->actuator_position;
// 			this->dc_lower_rudder.current_value = this->actuator_position;
// 			break;
// 		default:
// 			std::cout << "ERROR: Invalid actuator position from update_actuator_control_surface() function" << std::endl;
// 		}
// 	}
// }

// void DC10ReversibleMotorPump::simulate_pressure_exchange(DC10HydraulicEnginePumpSystem& attrib, double pump_3_hydraulic_pressure, int id, double dt)
// {
// 	//NO FLUID TRANSFER IN REVERSE MOTOR PUMP!!

// 	const double cylinder_area = 10.0; //in^2
// 	const double cylinder_diameter = 4.0; //in^2
// 	double pressure_change;

// 	double area = (cylinder_diameter * cylinder_diameter * M_PI) / 4.0;
// 	double delta_p = pump_3_hydraulic_pressure - attrib.hydraulic_pressure;
// 	double force = cylinder_area * delta_p;

// 	if(id == 3)
// 	{
// 		this->pressure_change = ((-force) / area) * dt - 0.01 * (attrib.hydraulic_pressure - 3000.0) / area;
// 	}else{
// 		this->pressure_change = ((force) / area) * dt - 0.01 * (attrib.hydraulic_pressure - 3000.0) / area;

// 	}

// 	attrib.hydraulic_pressure += this->pressure_change;

// 	//"pressure regulate" TODO: make this more realistic
// 	if(attrib.hydraulic_pressure > 3000.0)
// 	{
// 		attrib.hydraulic_pressure -= 50.0;
// 	}

// }


/*
AuxPump::AuxPump(DC10HydraulicEnginePumpSystem hydroPump) : auxPumpPSI(0.0), lastPumpPosition(1), pump(hydroPump) 
{
    // DEFAULT_INERTIA = 0.011;
    // DEFAULT_DYNAMIC_FRICTION_CONSTANT = 0.00004;
    // DEFAULT_RESISTANT_TORQUE_OFF = 2.8;
    // BACKPRESSURE_PRELOAD_PSI = 200.0;
    //
    // SPEED_DISPLACEMENT_FILTER_TIME_CONSTANT = 50;
    // MIN_FILTERING_RPM = 100.0;
    // ELECTRICAL_EFFICIENCY = 0.95;

    power_flow = true;
    pump_running = true;
    avail_potential = 140.0; //v
    consumed_power = 0.0; //watt
    acceleration = 0.0; //rad/s^2
    speed_raw = 0.0; //rad/s
    speed_filtered = SPEED_DISPLACEMENT_FILTER_TIME_CONSTANT; //angular_velocity
    inertia = DEFAULT_INERTIA;
    output_current = 0.0; //amp
    generated_torque = 0.0; //Nm
    resistant_torque = 0.0; //Nm
    current_controller;
    displacement_filter = SPEED_DISPLACEMENT_FILTER_TIME_CONSTANT;

}

void AuxPump::lastAuxPosition(int auxPumpSwitch)
{
	if(auxPumpSwitch == 2)
		this->lastPumpPosition = 2;
	if(auxPumpSwitch == 0)
		this->lastPumpPosition = 0;
}

void AuxPump::simulateAuxPump(DC10HydraulicEnginePumpSystem *pump, int auxPumpSwitch, double time)
{
	this->lastAuxPosition(auxPumpSwitch);
	//switch set to on
	if(auxPumpSwitch == 1)
	{
		//electrical check is not preformed 
		//driven electrically, so give a constant value that is only operating when ACGS power is on
		this->auxPumpPSI = pump->systemPressure(this->speed_filtered, 18.0, false, 0.0);

	}
	
}

double AuxPump::simulateAuxPumpDissipate(DC10HydraulicEnginePumpSystem *pump, double time)
{
		this->auxPumpPSI = pump->simulatePressureDisipate(time, 0.285);

		return this->auxPumpPSI;
}

double AuxPump::publishAuxPressure()
{
	return this->auxPumpPSI;
}

void AuxPump::update_pump_speed(double dt)
{
  double final_torque = this->generated_torque - this->resistant_torque;

  this->acceleration = final_torque / this->inertia;

  this->speed_raw += this->acceleration * dt;

  if(this->speed_raw > MIN_FILTERING_RPM)
  {
    this->speed_filtered = this->speed_raw;
  }else{
    this->speed_filtered = 0.0;
  }
  
}

void AuxPump::update_resistant_torque()
{
  double pumping_torque;
  double dynamic_friction_torque = DEFAULT_DYNAMIC_FRICTION_CONSTANT * this->speed_filtered;

  if(this->power_flow)
  {
    // pumping_torque = std::max(this->auxPumpPSI, BACKPRESSURE_PRELOAD_PSI) 
    //   * this->displacement_filter / (2.0 * M_PI);
    pumping_torque = 100
      * this->displacement_filter / (2.0 * M_PI);
  }else{
    pumping_torque = DEFAULT_RESISTANT_TORQUE_OFF;
  }

}

void AuxPump::update_current_control(double dt)
{
  if(this->power_flow)
  {
    // this->output_current = next_control_output(7600.0, dT);
     //this->output_current = 7600.0;
      //this->output_current = pid_aux.next_control_output(this->speed_raw, dt);
  }else{
    this->output_current= 0.0;
  }
}

void AuxPump::update_power_consumption()
{
  if(this->pump_running)
  {
    this->consumed_power = this->avail_potential * this->output_current * std::sqrt(3.0);
  }else{
    this->consumed_power = 0.0;
  }
}

void AuxPump::update_generated_torque(double dt)
{
  this->update_current_control(dt);
  this->update_power_consumption();

  if(this->pump_running)
  {
    if(this->speed_raw < 5.0 && this->output_current > 0.0)
    {
      this->generated_torque = 0.5 * this->output_current;
      //std::cout << this->generated_torque << std::endl;
    }else{
      //this->generated_torque = ELECTRICAL_EFFICIENCY * this->consumed_power / (this->speed_raw * 2 *M_PI)/60.0;
      this->generated_torque = ELECTRICAL_EFFICIENCY * this->consumed_power / this->speed_raw;
    }
  }else{
    this->generated_torque = 0.0;
  }
}

void AuxPump::update(double dt)
{
  this->update_resistant_torque();
  this->update_generated_torque(dt);
  this->update_pump_speed(dt);
}

double AuxPump::speed()
{
  return this->speed_filtered;
}

*/




//void fluidTransfer(DC10HydraulicEnginePumpSystem *givePSI, DC10HydraulicEnginePumpSystem *receivePSI, int onSwitch)
/*
void ReversibleMotorPump::fluidTransfer(DC10HydraulicEnginePumpSystem *systemGive,DC10HydraulicEnginePumpSystem *systemReceive, bool onSwitch, double first, double second)
{
	double dP;
	double dQ;
	double valvePosition;
	if(onSwitch)
	{
		valvePosition = lerp(systemReceive->hydraulic_pressure, 0.0, 1.0, 3000.0, 0.0);
		dP = systemGive->hydraulic_pressure - systemReceive->hydraulic_pressure;
		//dQ =  (1.89 * std::sqrt(dP / 9.00));
		dQ =  (first * std::sqrt(dP / second));
		if(dQ > 0.0)
		{
			//systemGive->Q -= 0.0;
			//systemReceive->Q += 0.10;
			//systemGive->hydraulic_pressure -= 2.2;
			//systemReceive->hydraulic_pressure = 2.2;
		}else{
			systemGive->hydraulic_Q-= 0.0;
			systemReceive->hydraulic_Q= 0.0;
		}
	}
}

*/


