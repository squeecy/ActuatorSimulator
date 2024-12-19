// #include <RungeKutta.h>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "RungeKutta.h"
#include "hydraulics.h"
#include "interp.h"

using namespace MsfsAeroFramework;

#define IN3_2_GPM(x) ((x) * 0.0043)
#define LBS_2_N(x) ((x) * 4.4482)
#define PSI_2_PA(x) ((x) * 6894.0)
#define M2_2_in2(x) ((x) * 1550.0)
#define cbM2Gal(x) (x * 264.172)
#define M_sqr_2_in_sqr(x) (x * 1550.0)
#define GPS_2_M3S(x) (x / 264.2)
// #define lerp(x, x1, y1, x2, y2) (y1 + (x - x1) * ((y2 - y1) / (x2 - x1)))

namespace MsfsAeroFramework {
RungeKutta RK(0.0007);

double DC10HydraulicEnginePumpSystem::GPM2VEL(double x, double D) {
  return (x / D);
}

void DC10HydraulicEnginePumpSystem::hydraulicFluidViscosity() {
  this->fluidViscosity = lerp(this->fluid_temperature, -40.0, 2600, 100.0, 3.5);
}

void DC10HydraulicEnginePumpSystem::regulate_displacement() {


  const std::vector<DataPoint> displacement_data_points = {
      {0.0, 0.35},    {500.0, 0.35},  {750.0, 0.45},   {1000.0, 0.45},
      {1500.0, 0.45},  {2000.0, 0.50}, {2100.0, 0.45},  {2200.0, 0.37},
      {2300.0, 0.33}, {2500.0, 0.33}, {2700.0, 0.25}, {2900.0, 0.175},
      {3025.0, 0.0}};

  // double maximum_displacement = interpolate(displacement_data_points,
  // this->hydraulic_pressure);
  double maximum_displacement =
      interpolate(displacement_data_points,
                  100.0 * (*fe_hydraulic_pressure_member)->current_value);

  PID pid = PID(this->dt, maximum_displacement, 0, 0.1, 0.01, 0.5);

  // this->displacement_stroke = pid.calculate(3000, this->hydraulic_pressure);
  this->displacement_stroke = pid.calculate(
      3000, 100.0 * (*fe_hydraulic_pressure_member)->current_value);
  // std::cout << this->displacement_stroke << std::endl;

  // std::cout << this->hydraulic_pressure << "   " << this->displacement_stroke
  // << std::endl; std::cout << this->hydraulic_Q << "  " <<
  // this->hydraulic_pressure << std::endl;
}

double DC10HydraulicEnginePumpSystem::orificeEquation() {
  return 1.2 * (M_PI * std::pow(this->D_pipe / 2.0, 2)) *
         std::sqrt(std::abs(2.0 * (this->hydraulic_pressure / 0.69)));
}

double val = 0;
void DC10HydraulicEnginePumpSystem::simulateEngPump() {
  this->hydraulicFluidViscosity();
  this->regulate_displacement();

  // double rpmAddition = (this->N2) * (1.0 - (this->N2) / 9000.0);
  double rpmAddition = 9000.0 - (this->N2);
  double hydroEngPumpRPM =
      (this->N2) + (rpmAddition * lerp(this->N2, 475, 1, 0, 0, true));

  this->pump_displacement = (this->valve_count) * M_PI *
                            std::pow(this->valve_bore / 2.0, 2) *
                            this->displacement_stroke;

  if (!this->RMP) {
    if (this->pump_displacement != 0.0) {
      this->hydraulic_Q =
          IN3_2_GPM((this->displacement_stroke * (hydroEngPumpRPM)) / 0.95);
      // std::cout << "pump_displacement: " << this->pump_displacement << " : "
      // << this->hydraulic_Q << std::endl;
    } else {
      this->hydraulic_Q =
          ((this->displacement_stroke * (hydroEngPumpRPM)) / cbM2Gal(1));
      // std::cout << "pump_displacement: " << this->pump_displacement << " : "
      // << this->hydraulic_Q << std::endl;
    }

    if (this->hydraulic_Q <= 0.0)
      this->hydraulic_Q = 0.0;
  }
  (*fe_eng_primary_pump_flow_member)->current_value = this->hydraulic_Q;
}

// new function
// general hydraulic fluid loss in the system
void DC10HydraulicEnginePumpSystem::simulateFlowLoss() {
  // make random later
  const double FLOW_LOSS = 0.05; // GPH
  this->hydraulic_pressure -= (((FLOW_LOSS * 9.27) / 50.27));
}

void DC10HydraulicEnginePumpSystem::findDarcyFriction() {
  this->simulateEngPump();
  // this->Re = (4.0 * this->hydraulic_Q) / (M_PI * this->D_pipe *
  // this->fluidViscosity);
  this->Re = (4.0 * (*fe_system_hydro_flow_member)->current_value) /
             (M_PI * this->D_pipe * this->fluidViscosity);
  if (this->Re != 0.0)
    this->f = 0.3164 / (pow(this->Re, 0.25));
  else
    this->f = 0.0;
}

void DC10HydraulicEnginePumpSystem::systemDeltaP() {
  const double rho = 0.69;
  // double V = this->GPM2VEL(this->hydraulic_Q, this->D_pipe);
  double V = this->GPM2VEL((*fe_system_hydro_flow_member)->current_value,
                           this->D_pipe);
  this->dt = dt;

  this->findDarcyFriction();
  this->deltaP =
      this->f * (this->L_pipe / this->D_pipe) * (rho * pow(V, 2.0)) / 2.0;
}

void DC10HydraulicEnginePumpSystem::systemPressure() {
  this->systemDeltaP();
  this->simulateFlowLoss();
  if (this->f != 0.0) {
    // this->flowFactor = this->hydraulic_Q / (sqrt(abs(this->deltaP)));
    // this->hydraulic_pressure += (((this->hydraulic_Q * 9.27) / 50.27));

    // new
    this->flowFactor = (*fe_system_hydro_flow_member)->current_value /
                       (sqrt(abs(this->deltaP)));
    this->hydraulic_pressure +=
        ((((*fe_system_hydro_flow_member)->current_value * 9.27) / 50.27));
  } else {
    this->flowFactor = 0.0;
    this->hydraulic_pressure += 0.0;
  }
}

void DC10HydraulicEnginePumpSystem::simulatePressureDisipate(double pressure,
                                                             double t,
                                                             double k) {
  double pressure_disipate;
  double acTime;
  acTime += t;

  pressure_disipate = pressure * (pow(exp(1.0), (-0.265 * t)));

  this->hydraulic_pressure -= pressure_disipate;
}

void DC10HydraulicEnginePumpSystem::update_context() {

  switch (this->pump_id) {
  case 1:
    this->N2 = normal_n2_1->current_value;
    break;
  case 2:
    this->N2 = normal_n2_1->current_value;
    break;
  case 3:
    this->N2 = normal_n2_3->current_value;
    break;
  case 4:
    this->N2 = fe_aux_motor_1_rpm->current_value;
    break;
  case 5:
    this->N2 = fe_aux_motor_2_rpm->current_value;
    break;

    // default:
    // std::cout << "Error: Unknown pump id from update_context function
    // (HydraulicSystem.cpp)" << std::cout;
  }
}

void DC10HydraulicEnginePumpSystem::check_pump_operation() {
  const double FAILURE_PSI = 2400.0;

  switch (this->switch_id) {
  case 1:
    this->operating = (double)(this->switch_position != 0);
    break;
  case 2:
    // this->operating = (double)(this->switch_id == 2 &&
    // this->switch_position == SWITCH_AUTO &&
    // fe_eng_2_pump_pressure_member->current_value < FAILURE_PSI);
    if (this->switch_position == 1) {
      this->operating =
          (double)(this->switch_id == 1 && this->switch_position == 2 &&
                   (*fe_eng_2_pump_pressure_member)->current_value <
                       FAILURE_PSI);
    } else {
      this->operating = (double)(this->switch_position != -1);
    }
    break;
  default:
    break;
  }

  // std::cout << fe_eng_1_hyd_pump_1_switch.current_value << std::endl;
}

// FIXME in main
void DC10HydraulicEnginePumpSystem::check_pump_dissipate() {

  int rmp_operating;

  if (this->pump_id != 3) {
    this->dissipating =
        (double)(this->operating != 1 &&
                 (*fe_rmp_pump_operation_member)->current_value != 1 &&
                 this->hydraulic_pressure > 0);
  } else {
    rmp_operating =
        fe_rmp_13_operation->current_value + fe_rmp_23_operation->current_value;
    this->dissipating = (double)(this->operating != 1 && rmp_operating < 1 &&
                                 this->hydraulic_pressure > 0);
  }

  // switch (this->pump_id)
  // {
  // 	case 3:
  // 	rmp_operating = fe_rmp_13_operation->current_value +
  // fe_rmp_23_operation->current_value; 	this->dissipating =
  // (double)(this->operating != 1 && rmp_operating < 1 &&
  // this->hydraulic_pressure > 0); 	break;

  // default:
  // 	break;
  // }
}

void DC10HydraulicEnginePumpSystem::assign_member() {
  // write based on combination of selected engine pump and selected switch
  // hex corresponds with the combination of each
  // ex. 0x00010001: first system, first (or primary) switch
  // ex. 0x00030002: third system, second (or secondary) switch

  #define FIRST_FIRST 0x00010001
  #define FIRST_SECOND 0x00010002
  #define SECOND_FIRST 0x00020001
  #define SECOND_SECOND 0x00020002
  #define THIRD_FIRST 0x00030001
  #define THIRD_SECOND 0x00030002
  #define AUX_ONE 0x00040001
  //#define AUX_TWO 0x00050001

  int combined_id = (this->pump_id << 16) | this->switch_id;

  switch (combined_id) {
  case FIRST_FIRST:
    fe_hydraulic_pressure_member = &this->fe_hydro_pressure_1;
    fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_1_1;
    fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_1_2;
    fe_rmp_pump_operation_member = &this->fe_rmp_13_operation;
    fe_rmp_switch_position_member = &this->fe_13_hydro_switch;
    fe_hydro_line_operation_member = &this->line_1_1_operation;
    fe_hydro_line_opposite_operation_member = &this->line_1_2_operation;
    fe_eng_primary_pump_flow_member = &this->fe_eng_pump_flow_1_1;
    fe_eng_secondary_pump_flow_member = &this->fe_eng_pump_flow_1_2;
    fe_system_hydro_flow_member = &this->fe_hydro_flow_1;

    this->secondary_pump = fe_line_hydro_pressure_1_2->current_value;
    this->switch_position = fe_eng_1_hyd_pump_1_switch->current_value;

    break;
  case FIRST_SECOND:
    fe_hydraulic_pressure_member = &this->fe_hydro_pressure_1;
    fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_1_2;
    fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_1_1;
    fe_rmp_pump_operation_member = &this->fe_rmp_13_operation;
    fe_rmp_switch_position_member = &this->fe_13_hydro_switch;
    fe_hydro_line_operation_member = &this->line_1_2_operation;
    fe_hydro_line_opposite_operation_member = &this->line_1_1_operation;
    fe_eng_primary_pump_flow_member = &this->fe_eng_pump_flow_1_2;
    fe_eng_secondary_pump_flow_member = &this->fe_eng_pump_flow_1_1;
    fe_system_hydro_flow_member = &this->fe_hydro_flow_1;

    this->secondary_pump = fe_line_hydro_pressure_1_1->current_value;
    this->switch_position = fe_eng_1_hyd_pump_2_switch->current_value;
    break;
  case SECOND_FIRST:
    fe_hydraulic_pressure_member = &this->fe_hydro_pressure_2;
    fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_2_1;
    fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_2_2;
    fe_rmp_pump_operation_member = &this->fe_rmp_23_operation;
    fe_rmp_switch_position_member = &this->fe_23_hydro_switch;
    fe_hydro_line_operation_member = &this->line_2_1_operation;
    fe_hydro_line_opposite_operation_member = &this->line_2_2_operation;
    fe_eng_primary_pump_flow_member = &this->fe_eng_pump_flow_2_1;
    fe_eng_secondary_pump_flow_member = &this->fe_eng_pump_flow_2_2;
    fe_system_hydro_flow_member = &this->fe_hydro_flow_2;

    this->secondary_pump = fe_line_hydro_pressure_2_2->current_value;
    this->switch_position = fe_eng_2_hyd_pump_1_switch->current_value;
    break;
  case SECOND_SECOND:
    fe_hydraulic_pressure_member = &this->fe_hydro_pressure_2;
    fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_2_2;
    fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_2_1;
    fe_rmp_pump_operation_member = &this->fe_rmp_23_operation;
    fe_rmp_switch_position_member = &this->fe_23_hydro_switch;
    fe_hydro_line_operation_member = &this->line_2_2_operation;
    fe_hydro_line_opposite_operation_member = &this->line_2_1_operation;
    fe_eng_primary_pump_flow_member = &this->fe_eng_pump_flow_2_2;
    fe_eng_secondary_pump_flow_member = &this->fe_eng_pump_flow_2_1;
    fe_system_hydro_flow_member = &this->fe_hydro_flow_2;

    this->secondary_pump = fe_line_hydro_pressure_2_1->current_value;
    this->switch_position = fe_eng_2_hyd_pump_2_switch->current_value;
    break;
  case THIRD_FIRST:
    fe_hydraulic_pressure_member = &this->fe_hydro_pressure_3;
    fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_3_1;
    fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_3_2;
    fe_hydro_line_operation_member = &this->line_3_1_operation;
    fe_hydro_line_opposite_operation_member = &this->line_3_2_operation;
    fe_eng_primary_pump_flow_member = &this->fe_eng_pump_flow_3_1;
    fe_eng_secondary_pump_flow_member = &this->fe_eng_pump_flow_3_2;
    fe_system_hydro_flow_member = &this->fe_hydro_flow_3;

    this->secondary_pump = fe_line_hydro_pressure_3_2->current_value;
    this->switch_position = fe_eng_3_hyd_pump_1_switch->current_value;
    this->rmp_switch_position_combo =
        fe_rmp_13_operation->current_value + fe_rmp_23_operation->current_value;
    fe_rmp_partner_1_system_member = &this->fe_hydro_pressure_1;
    fe_rmp_partner_2_system_member = &this->fe_hydro_pressure_2;
    break;
  case THIRD_SECOND:
    fe_hydraulic_pressure_member = &this->fe_hydro_pressure_3;
    fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_3_2;
    fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_3_1;
    fe_hydro_line_operation_member = &this->line_3_2_operation;
    fe_hydro_line_opposite_operation_member = &this->line_3_1_operation;
    fe_eng_primary_pump_flow_member = &this->fe_eng_pump_flow_3_2;
    fe_eng_secondary_pump_flow_member = &this->fe_eng_pump_flow_3_1;
    fe_system_hydro_flow_member = &this->fe_hydro_flow_3;
    this->rmp_switch_position_combo =
        fe_rmp_13_operation->current_value + fe_rmp_23_operation->current_value;

    this->secondary_pump = fe_line_hydro_pressure_3_1->current_value;
    this->switch_position = fe_eng_3_hyd_pump_2_switch->current_value;
    fe_rmp_partner_1_system_member = &this->fe_hydro_pressure_1;
    fe_rmp_partner_2_system_member = &this->fe_hydro_pressure_2;
    break;
  case AUX_ONE:
    fe_hydraulic_pressure_member = &this->fe_hydro_pressure_3;
    fe_eng_1_pump_pressure_member = &this->fe_line_hydro_pressure_aux_1;
    fe_eng_2_pump_pressure_member = &this->fe_line_hydro_pressure_aux_2;
    fe_hydro_line_operation_member = &this->line_3_2_operation;
    fe_hydro_line_opposite_operation_member = &this->line_3_1_operation;
    fe_eng_primary_pump_flow_member = &this->fe_eng_pump_flow_3_2;
    fe_eng_secondary_pump_flow_member = &this->fe_eng_pump_flow_3_1;
    fe_system_hydro_flow_member = &this->fe_hydro_flow_3;
    break;

  default:
    std::cout << "combined: " << combined_id << std::endl;
    std::cout << "Error: Assigning nullptr in assign_member fucntion "
                 "(HydraulicSystem.cpp)"
              << std::endl;
    break;
  }
}

void DC10HydraulicEnginePumpSystem::make_members_current() {
  static bool addNew = true;
  int executed = 0;
  if (this->hydraulic_pressure < 0.0)
    this->hydraulic_pressure = 0.0;

  (*fe_eng_1_pump_pressure_member)->current_value = this->hydraulic_pressure;

  if ((*fe_eng_1_pump_pressure_member)->current_value < 0.0)
    (*fe_eng_1_pump_pressure_member)->current_value = 0.0;

  (*fe_system_hydro_flow_member)->current_value =
      (*fe_eng_primary_pump_flow_member)->current_value +
      (*fe_eng_secondary_pump_flow_member)->current_value;

  // system pressure will be the highest between the two hydro pump lines
  (*fe_hydraulic_pressure_member)->current_value =
      std::max((*fe_eng_1_pump_pressure_member)->current_value,
               secondary_pump) /
      100.0;

  //new
  if((*fe_hydro_line_opposite_operation_member)->current_value == false 
    && (*fe_eng_2_pump_pressure_member)->current_value > 0.0 
    && (*fe_eng_1_pump_pressure_member)->current_value == 0.0)
  {
    this->hydraulic_pressure = (*fe_eng_2_pump_pressure_member)->current_value;
  }
  

  (*fe_hydro_line_operation_member)->current_value = this->operating;
}

void DC10HydraulicEnginePumpSystem::update_state() {
  // sim_context->write(*this->fe_eng_1_pump_pressure_member);
  // sim_context->write(*this->fe_hydraulic_pressure_member);
}

void DC10HydraulicEnginePumpSystem::update(double delta_time) {
  this->dt = delta_time;
  this->update_context();
  this->assign_member();
  this->check_pump_operation();
  normal_n2_1->current_value = 7500.0;
  normal_n2_2->current_value = 7500.0;
  normal_n2_3->current_value = 7500.0;
  this->check_pump_dissipate();

  if (this->dissipating) {
    this->simulatePressureDisipate(
        (*fe_hydraulic_pressure_member)->current_value, delta_time, 0.266);
  }

  if (this->operating) {
    this->systemPressure();
  }

  this->make_members_current();
}

// actuator system
void DC10ActuatorSystem::actuator_entry_fluid_velocity() {
  this->valve_cross_area = (M_PI * pow((this->actuator_valve_radius / 2.0), 2));
}

void DC10ActuatorSystem::actuator_entry_pressure() {
  this->actuator_entry_fluid_velocity();

  const double f = 0.00015;
  // density: TODO (more realistic simulation later)
  const double rho = 0.69;

  this->deltaP_actuator = f * (this->L / this->D) *
                          (rho * pow(this->actuator_entry_flowrate, 2) / 2.0);
}

void DC10ActuatorSystem::simulate_actuator_fluid_velocity() {
  double ratio;
  double Q_rod;

  ratio = this->rod_area / this->bore_area;
  Q_rod = (GPS_2_M3S(this->ideal_flow)) * ratio;

  this->actuator_fluid_velocity =
      (GPS_2_M3S(this->ideal_flow)) / this->valve_cross_area;
  this->actuator_min_fluid_velocity = Q_rod / this->valve_cross_area;
}

void DC10ActuatorSystem::refresh_actuator_position() {
  this->delta_displacement =
      this->actuator_position - this->last_actuator_position;
  this->last_actuator_position = this->actuator_position;
}

void DC10ActuatorSystem::actuator_valve_displacement(double delta_time) {
  double volume_to_actuator = 0.0;
  double volume_to_reservoir = 0.0;

  if (this->delta_displacement > 0.0) {
    volume_to_actuator = this->delta_displacement * this->bore_area;
    volume_to_reservoir = volume_to_actuator / this->volume_extension_ratio;
  } else if (this->delta_displacement < 0.0) {
    volume_to_actuator = this->delta_displacement * this->rod_area;
    volume_to_reservoir = volume_to_actuator / this->volume_extension_ratio;
  }

  if (this->active_control) {
    this->total_volume_to_actuator += volume_to_actuator;
    this->total_volume_to_reservoir += volume_to_reservoir;
  } else {
    this->total_volume_to_reservoir += volume_to_reservoir - volume_to_actuator;
  }

  if (this->delta_displacement >= 0.0) {
    this->signed_flow = volume_to_actuator / delta_time;
  } else {
    this->signed_flow = -volume_to_actuator / delta_time;
  }
  system_1.fe_hydro_pressure_1->current_value -= ((((total_volume_to_reservoir * 100.0) * 9.27) / 50.27));
}

void DC10ActuatorSystem::update_actuator_position(double delta_time) {
  this->actuator_entry_pressure();
  this->simulate_actuator_fluid_velocity();

  double delta_distance;
  double direction = 1.0;

  double delta_request =
      this->actuator_position - this->requested_actuator_position;

  double last_pos = this->actuator_position;

  double ratio = this->rod_area / this->bore_area;
  double Q_rod = (this->ideal_flow / 264.2) * ratio;

  double test_v = (this->ideal_flow / 264.2) / this->valve_cross_area;
  double min_v = Q_rod / this->valve_cross_area;

  if (this->active_control) {
    if (std::abs(delta_request) < 0.019) {
      this->actuator_position = this->actuator_position;
    } else {
      if (delta_request < 0.0) {
        this->actuator_position = RK.RK4_actuator(
            this->actuator_position, this->actuator_fluid_velocity, 1.0,
            this->rod_area, 500.0, delta_time, this->actuator_P);

      } else if (delta_request > 0.0) {
        //this->actuator_position = RK.RK4_actuator(
 //this->actuator_position, -this->actuator_fluid_velocity, 1.0,
            //this->rod_area, this->actuator_load, delta_time, this->actuator_P);
        this->actuator_position = RK.RK4_actuator(
            this->actuator_position, -this->actuator_fluid_velocity, 1.0,
            this->rod_area, 500.0, delta_time, this->actuator_P);
      }
    }
  } else {
    if (!this->retract)
      this->actuator_position -=
          ((min_v * 1.0 * this->bore_area * this->dt)) /
          (LBS_2_N(actuator_load) / (PSI_2_PA(actuator_P) * this->bore_area));
    else
      this->actuator_position +=
          ((test_v * 1.0 * this->rod_area * this->dt)) /
          (LBS_2_N(actuator_load) / (PSI_2_PA(actuator_P) * this->bore_area));
  }

  // limit actuator to physical boundaries
  this->actuator_position = clamp(this->actuator_position, 1.0, -1.0);

  delta_distance = this->actuator_position - last_pos;

  this->refresh_actuator_position();
  this->actuator_valve_displacement(delta_time);
}

 void DC10ActuatorSystem::update_requirments(bool is_retract)
 {
 	this->actuator_Q = fe_hydro_flow_1->current_value;
 	this->actuator_P = fe_hydro_pressure_1->current_value;
 	this->actuator_load = actuator_load_var->current_value;
 	this->retract = is_retract;
 	this->actuator_entry_flowrate = this->actuator_Q;
  this->actuator_P = std::max(system_1.fe_hydro_pressure_1->current_value, system_2.fe_hydro_pressure_2->current_value) * 100.0;
  

 }

 void DC10ActuatorSystem::update_request_control_surface(Control_Surface surface)
 {
 	switch (surface)
 	{
 	case AILERON:
 		this->requested_actuator_position = yoke_x_position->current_value;
 		  break; 	
  case ELEVATOR:
 		this->requested_actuator_position = yoke_y_position->current_value;
  		break; 	
  case RUDDER:
 		this->requested_actuator_position = rudder_position->current_value;
  		break; 	
  default:
    std::cout << "ERROR: Invalid Control Surface from update_class_position() function (HydraulicSystem.cpp)" << std::endl; 
 	}
 }

 void DC10ActuatorSystem::update_actuator_control_surface(Control_Surface surface)
 {

 	switch (surface)
	{
    case AILERON:
        dc_left_out_aileron->current_value = this->actuator_position;
        dc_left_in_aileron->current_value = this->actuator_position;
        dc_right_out_aileron->current_value = this->actuator_position;
        dc_right_in_aileron->current_value = this->actuator_position;
  		break; 	
    case ELEVATOR: 		
      break;
    case RUDDER: 		
        dc_upper_rudder->current_value = this->actuator_position;
        dc_lower_rudder->current_value = this->actuator_position;
        break; 	
  default: 
    std::cout << "Error: Invalid actuator position from update_actuator_control_surface function" << std::endl;

 	}

 }

void DC10ActuatorSystem::update(double delta_time) {
  this->update_requirments(false);
  this->update_request_control_surface(AILERON);
  this->update_actuator_position(delta_time);
  this->update_actuator_control_surface(AILERON);
}


#define SWITCH_OFF 1
#define SWITCH_ON 0
#define SWITCH_AUTO 2

void DC10ReversibleMotorPump::assign_member() {
  this->eng_switch_3_pump = fe_eng_3_hyd_pump_1_switch->current_value;
  this->eng_switch_positions = fe_eng_1_hyd_pump_1_switch->current_value +
                               fe_eng_1_hyd_pump_2_switch->current_value +
                               fe_eng_2_hyd_pump_1_switch->current_value +
                               fe_eng_2_hyd_pump_2_switch->current_value +
                               fe_eng_3_hyd_pump_1_switch->current_value +
                               fe_eng_3_hyd_pump_2_switch->current_value;
  switch (this->switch_id) {
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

void DC10ReversibleMotorPump::check_rmp_state(Pump_State &state) {
  int support_operating =
      this->slave_hydro_line_1.operating + this->slave_hydro_line_2.operating;
  int sys_3_operating =
      this->system_3_left_line.operating + this->system_3_right_line.operating;
  int hydro_operation =
      line_1_1_operation->current_value + line_1_2_operation->current_value +
      line_2_1_operation->current_value + line_2_2_operation->current_value +
      line_3_1_operation->current_value + line_3_2_operation->current_value;

  if (hydro_operation <= 0) {
    this->operating = 0;
    if (fe_rmp_23_operation->current_value != 0 &&
        fe_rmp_13_operation->current_value != 1 && sys_3_operating < 1) {
      this->operating = 0;
    }

    if (fe_rmp_13_operation->current_value != 0 &&
        fe_rmp_23_operation->current_value != 1 && sys_3_operating < 1) {
      this->operating = 0;
    }
  }

  if (sys_3_operating > 0 && support_operating <= 0) {
    this->sys_3_rmp_state = GIVER;
  } else {
    this->sys_3_rmp_state = RECEIVER;
  }
}

void DC10ReversibleMotorPump::handle_rmp_state(Pump_State &state) {
  switch (this->sys_3_rmp_state) {
  case GIVER:
    this->operating = 1;
    break;
  case RECEIVER:
    this->operating = 1;
    break;
  default:
    break;
  }
}

void DC10ReversibleMotorPump::check_RMP_operation() {
#define SWITCH_OFF 0
#define SWITCH_ON -1
#define SWITCH_AUTO 1

  auto startTime = std::chrono::steady_clock::now();
  bool timerRunning = true;

  const double MIN_N2 = 45.0;

  switch (this->switch_position) {
  case SWITCH_OFF:
    this->operating = 0;

    break;
  case SWITCH_ON:
    this->handle_rmp_state(state);

    if (switch_id == 2) {
      if (this->slave_hydro_line_1.operating +
                  this->slave_hydro_line_2.operating ==
              0 &&
          slave_hydro_line_1.hydraulic_pressure +
                  slave_hydro_line_2.hydraulic_pressure +
                  system_3_left_line.hydraulic_pressure +
                  system_3_right_line.hydraulic_pressure >
              0.0 &&
          fe_rmp_13_operation->current_value != 1) {
        this->operating = 0;
      }

    } else if (switch_id == 1) {
      if (this->slave_hydro_line_1.operating +
                  this->slave_hydro_line_2.operating ==
              0 &&
          slave_hydro_line_1.hydraulic_pressure +
                  slave_hydro_line_2.hydraulic_pressure +
                  system_3_left_line.hydraulic_pressure +
                  system_3_right_line.hydraulic_pressure >
              0.0 &&
          fe_rmp_23_operation->current_value != 1) {
        this->operating = 0;
      }
    }
    if (sys_3_rmp_state == GIVER) {
      this->operating = (double)(this->switch_position != SWITCH_OFF &&
                                 this->eng_switch_positions != -3);
    }

    this->check_rmp_state(state);
    break;
  case SWITCH_AUTO:
    this->operating = (double)((*fe_engine_n2_member)->current_value < MIN_N2);
    break;

  default:
    break;
  }
}

void DC10ReversibleMotorPump::simulate_pressure_exchange(double delta_time) {
  const double cylinder_area = 40.75;
  const double pump_diameter = 2.0;
  double pressure_change;
  double slave_pressure = std::max(slave_hydro_line_1.hydraulic_pressure,
                                   slave_hydro_line_2.hydraulic_pressure);

  double area = (pump_diameter * pump_diameter * M_PI) / 4.0;

  double delta_p =
      (fe_hydro_pressure_3->current_value * 100.0) - slave_pressure;

  double force = area * delta_p;

  this->pressure_change =
      ((force) / cylinder_area) * delta_time -
      0.01 * (slave_pressure - (fe_hydro_pressure_3->current_value * 100.0)) /
          area;

  double pressure_delta =
      std::abs((std::max(this->system_3_left_line.hydraulic_pressure,
                         this->system_3_right_line.hydraulic_pressure)) -
               slave_pressure);

  if (this->operating) {
    (*fe_rmp_pump_operation_member)->current_value = true;

    PID pid = PID(delta_time, 1.0, 0, 0.1, 0.01, 0.5);

    double rmp_valve = pid.calculate(3000, pressure_delta);
    if (std::abs(pressure_delta) >= 3000.0) {
      rmp_valve = 1.0;
    }

    slave_hydro_line_1.hydraulic_pressure += this->pressure_change * rmp_valve;
    slave_hydro_line_2.hydraulic_pressure += this->pressure_change * rmp_valve;
    system_3_left_line.hydraulic_pressure -=
        (this->pressure_change) * rmp_valve;
    system_3_right_line.hydraulic_pressure -=
        (this->pressure_change) * rmp_valve;

    //"pressure regulate" TODO: make this more realistic
    if (slave_hydro_line_1.hydraulic_pressure > 3000.0) {
      slave_hydro_line_1.hydraulic_pressure -= 3.0;
    }

    if (slave_hydro_line_2.hydraulic_pressure > 3000.0) {
      slave_hydro_line_2.hydraulic_pressure -= 3.0;
    }

  } else {
    (*fe_rmp_pump_operation_member)->current_value = false;
  }
}

void DC10ReversibleMotorPump::update_state(SimContext *sim_context) {
  // sim_context->write(*this->fe_rmp_pump_operation_member);
}


void DC10ReversibleMotorPump::update(double delta_time) {
  this->assign_member();
  this->check_RMP_operation();
  this->simulate_pressure_exchange(delta_time);
}


void DC10HydraulicAuxPumpSystem::elec_motor(double delta_time)
{

  double temperature = 25.0;
  double voltage = 28.0;

  double eb = ke * omega;

  double di_dt = (voltage - eb - R * current) / L;
  
  current += di_dt * delta_time;

  double torque = kt * current; 
  double loadTorque = 24.0;
  double omega_dt = (torque - loadTorque - B * omega) / J;
  omega += omega_dt;

  double heatGenerated = current * current * R;
  double heatDissipated = (temperature - 30.0) / thermalRes;
  double Dt_dt = (heatGenerated - heatDissipated) / thermalCap;
  temperature += Dt_dt * delta_time;

  P_in = voltage * current;
  P_mech_work = P_in * eff;
  e_rpm = 60.0 * P_mech_work / ((2*M_PI) * 1.2);

}

void DC10HydraulicAuxPumpSystem::elec_pump(double delta_time)
{
  enginePumpSystem->systemPressure();
}

void DC10HydraulicAuxPumpSystem::check_motor_operation()
{
  const int START_AUX = 1;
  const int OPERATING_CIR = 1;
  const int OFF_ADG = 0;

  switch (enginePumpSystem->switch_id) {
    case 1:
      this->operating = (double)(this->switch_position == START_AUX && 
        cir_hydro_aux_pump_1->current_value == OPERATING_CIR && o_adg_sw_master.current_value == OFF_ADG);
      std::cout << this->operating << std::endl;
    case 2:
      this->operating = (double)(this->switch_position == START_AUX && 
        cir_hydro_aux_pump_2->current_value == OPERATING_CIR);

    default:
      break;
  }
}

void DC10HydraulicAuxPumpSystem::assign_member()
{
  switch (enginePumpSystem->switch_id) {

    case 1:
      fe_aux_motor_rpm_member = &this->fe_aux_motor_1_rpm;
      this->switch_position = fe_aux_hyd_pump_1_switch->current_value;
      break;
    case 2:
      fe_aux_motor_rpm_member = &this->fe_aux_motor_2_rpm;
      this->switch_position = fe_aux_hyd_pump_2_switch->current_value;
      break;

    default:
      break;
  }
}

//testbed specific
void DC10HydraulicAuxPumpSystem::operate_circuts()
{
  cir_hydro_aux_pump_1->current_value = 1;
  cir_hydro_aux_pump_2->current_value = 1;
  o_adg_sw->current_value = 0;
}

void DC10HydraulicAuxPumpSystem::update(double delta_time)
{
  this->operate_circuts();
  this->assign_member();
  this->check_motor_operation();
  //this->elec_motor(delta_time);
}



void DC10Accumulator::assign_member() {}

void DC10Accumulator::simulate_accumulator() {
  // precharge pressure
  const double P0 = 1000.0;
  // final pressure
  const double P2 = 3000.0;
  // gal
  const double V0 = 4.5;
  // p1 * v1 = p2 * v2 .. gal
  const double V2 = 1.5;
}



void DC10Accumulator::update(double delta_time) {}

void DC10HydraulicEnginePump::update(double delta_time) {
  this->engine_pump_1_1.update(delta_time);
  this->engine_pump_1_2.update(delta_time);
  this->engine_pump_2_1.update(delta_time);
  this->engine_pump_2_2.update(delta_time);
  this->engine_pump_3_1.update(delta_time);
  this->engine_pump_3_2.update(delta_time);

  this->reverse_motor_pump_1_3.update(delta_time);
  this->reverse_motor_pump_2_3.update(delta_time);
  //this->test_actuator.update(delta_time);
  this->test_aux.update(delta_time);
}

}


