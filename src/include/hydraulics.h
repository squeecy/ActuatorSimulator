#ifndef HYDRAULICS_H
#define HYDRAULICS_H

#include <aircraft.h>
#include <helper.h>
#include <pid.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include "simvar.h"
#include <SFML/Window.hpp>

#define TIME_STEP_HYDRO 0.05


#define SWITCH_OFF 0 
#define SWITCH_ON 1 
#define SWITCH_AUTO 2 



namespace MsfsAeroFramework {

	struct EnginePumpParams
	{
		int _valve_count;
		double _L_pipe;
		double _D_pipe;
		double _valve_bore;
		double _engine_pump_ID;
		double _switch_iD;
	};


	const struct EnginePumpParams params_1_1 = {
	7,
	3.048,
	0.5144,
	0.0508,
	1,
	1,
	};

	const struct EnginePumpParams params_1_2 = {
		7,
		3.048,
		0.5144,
		0.0508,
		1,
		2
	};

	const struct EnginePumpParams params_2_1 = {
		7,
		3.048,
		0.5144,
		0.0508,
		2,
		1
	};

	const struct EnginePumpParams params_2_2 = {
		7,
		3.048,
		0.5144,
		0.0508,
		2,
		2
	};

	const struct EnginePumpParams params_3_1 = {
		7,
		3.048,
		0.5144,
		0.0508,
		3,
		1
	};

	const struct EnginePumpParams params_3_2 = {
		7,
		3.048,
		0.5144,
		0.0508,
		3,
	  2	
	};

	const struct EnginePumpParams params_aux_1= {
		7,
		3.048,
		0.5144,
		0.0508,
		4,
	  1	
	};

	const struct EnginePumpParams params_aux_2= {
		7,
		3.048,
		0.5144,
		0.0508,
		5,
	  2	
	};


	struct HydraulicSystem
	{
		//double pressure;
		double hydraulic_pressure;
		double flow_rate;
	};

	enum Pump_State
	{
		GIVER,
		RECEIVER,
	};

	enum Control_Surface
	{
		AILERON,
		ELEVATOR,
		RUDDER
	};

	struct ActuatorParams {
		double bore_area_init;
		double rod_area_init;
		double actuator_value_radius_init;
		double actuator_position_init;
		double ideal_flow_init;
		bool active_control_init;
	};

  const struct ActuatorParams actuator_param_test = {
  0.145277,
  0.104682, 
  0.05908,
  0.0,
  0.08,
  true 
};



	struct ActuatorPrelims {
		double primary_pressure;
		double secondary_pressure;
		double primary_flow;
		double secondary_flow;
		double load;
		double axis;
		double delta_time;
	};

	class DC10HydraulicEnginePumpSystem {
	private:
		int valve_count;
		double L_pipe;
		double D_pipe;
		double valve_bore;
		double pump_displacement;
    double aux_line_1_pressure;
    double aux_line_2_pressure;

	public:
		double Re;
		double hydraulic_Q;
		double f;
		double N2;
		double dt;
		double flowFactor;
		double deltaP;
		double fluidViscosity;
		double FluidEnternalEnergy;
		double hydraulic_pressure;
		double displacement_stroke;
		double rmp_pressure;
		double fluid_temperature;
		double switch_position;
		double second_switch_position;
		double pump_2_pressure_test;
		bool RMP = false;
		bool operating;
		bool dissipating;
		double secondary_pump;
		double rmp_switch_position_combo;
		int pump_state;
		int pump_id;
		int switch_id;
		DC10HydraulicEnginePumpSystem(const EnginePumpParams& pump_parms)
			:   valve_count(pump_parms._valve_count), L_pipe(pump_parms._L_pipe), D_pipe(pump_parms._D_pipe), valve_bore(pump_parms._valve_bore),
					switch_id(pump_parms._switch_iD), pump_id(pump_parms._engine_pump_ID) {}

			

		  SimVar* normal_n2_1 = &normal_n2_1_master;
		  SimVar* normal_n2_2 = &normal_n2_1_master;
      SimVar* normal_n2_3 = &normal_n2_3_master;
      SimVar* fe_aux_motor_1_rpm = &fe_aux_motor_1_rpm_master;
      SimVar* fe_aux_motor_2_rpm = &fe_aux_motor_2_rpm_master;

      SimVar* fe_hydro_pressure_1 = &fe_hydro_pressure_1_master;
      SimVar* fe_line_hydro_pressure_1_1 = &fe_line_hydro_pressure_1_1_master;
      SimVar* fe_line_hydro_pressure_1_2 = &fe_line_hydro_pressure_1_2_master;
      SimVar* fe_rmp_13_operation = &fe_rmp_13_operation_master;
      SimVar* fe_eng_1_hyd_pump_1_switch = &fe_eng_1_hyd_pump_1_switch_master;
      SimVar* fe_eng_1_hyd_pump_2_switch = &fe_eng_1_hyd_pump_2_switch_master;
      SimVar* fe_hydro_pressure_2 = &fe_hydro_pressure_2_master;
      SimVar* fe_line_hydro_pressure_2_1 = &fe_line_hydro_pressure_2_1_master;
      SimVar* fe_line_hydro_pressure_2_2 = &fe_line_hydro_pressure_2_2_master;
      SimVar* fe_rmp_23_operation = &fe_rmp_23_operation_master;
      SimVar* fe_eng_2_hyd_pump_1_switch = &fe_eng_2_hyd_pump_1_switch_master;
      SimVar* fe_eng_2_hyd_pump_2_switch = &fe_eng_2_hyd_pump_2_switch_master;
      SimVar* fe_hydro_pressure_3 = &fe_hydro_pressure_3_master;
      SimVar* fe_hydro_pressure_aux_1 = &fe_hydro_pressure_aux_1_master;
      SimVar* fe_line_hydro_pressure_3_1 = &fe_line_hydro_pressure_3_1_master;
      SimVar* fe_line_hydro_pressure_3_2 = &fe_line_hydro_pressure_3_2_master;
      SimVar* fe_line_hydro_pressure_aux_1 = &fe_line_hydro_pressure_aux_1_master; //add new simvar
      SimVar* fe_line_hydro_pressure_aux_2 = &fe_line_hydro_pressure_aux_2_master; //add new simvar
      SimVar* fe_eng_3_hyd_pump_1_switch = &fe_eng_3_hyd_pump_1_switch_master;
      SimVar* fe_eng_3_hyd_pump_2_switch = &fe_eng_3_hyd_pump_2_switch_master;
		  SimVar* fe_13_hydro_switch = &fe_13_hydro_switch_master;
		  SimVar* fe_23_hydro_switch = &fe_23_hydro_switch_master;
		  SimVar* line_1_1_operation = &hydro_line_1_1_operation_master;
		  SimVar* line_1_2_operation = &hydro_line_1_2_operation_master;
		  SimVar* line_2_1_operation = &hydro_line_2_1_operation_master;
		  SimVar* line_2_2_operation = &hydro_line_2_2_operation_master;
		  SimVar* line_3_1_operation = &hydro_line_3_1_operation_master;
		  SimVar* line_3_2_operation = &hydro_line_3_2_operation_master;
		  SimVar* fe_eng_pump_flow_1_1 = &fe_eng_pump_flow_1_1_master;
		  SimVar* fe_eng_pump_flow_1_2 = &fe_eng_pump_flow_1_2_master;
		  SimVar* fe_eng_pump_flow_2_1 = &fe_eng_pump_flow_2_1_master;
		  SimVar* fe_eng_pump_flow_2_2 = &fe_eng_pump_flow_2_2_master;
		  SimVar* fe_eng_pump_flow_3_1 = &fe_eng_pump_flow_3_1_master;
		  SimVar* fe_eng_pump_flow_3_2 = &fe_eng_pump_flow_3_2_master;
      //new
		  SimVar* fe_aux_pump_flow_1 = &fe_aux_pump_flow_1_master;
		  SimVar* fe_aux_pump_flow_2 = &fe_aux_pump_flow_2_master;
		  SimVar* fe_hydro_flow_1 = &fe_hydro_flow_1_master;
		  SimVar* fe_hydro_flow_2 = &fe_hydro_flow_2_master;
		  SimVar* fe_hydro_flow_3 = &fe_hydro_flow_3_master;
      SimVar* fe_aux_1_operation = &fe_aux_1_operation_master;
      SimVar* fe_aux_2_operation = &fe_aux_2_operation_master;
		  

		

		//SimVar* fe_eng_pump_pressure_member = nullptr;
		SimVar** fe_eng_1_pump_pressure_member = nullptr;
		SimVar** fe_eng_2_pump_pressure_member = nullptr;
		SimVar** fe_hydraulic_pressure_member = nullptr;
		SimVar** fe_rmp_pump_operation_member = nullptr;
		SimVar** fe_rmp_partner_1_system_member = nullptr;
		SimVar** fe_rmp_partner_2_system_member = nullptr;
		SimVar** fe_rmp_switch_position_member = nullptr;
		SimVar** fe_hydro_line_operation_member = nullptr;
    //new
		SimVar** fe_hydro_line_opposite_operation_member = nullptr;
		SimVar** fe_hydro_line_1_flow_member = nullptr;
		SimVar** fe_eng_primary_pump_flow_member = nullptr;
		SimVar** fe_eng_secondary_pump_flow_member = nullptr;
		SimVar** fe_system_hydro_flow_member = nullptr;
		SimVar** fe_aux_operating_member = nullptr;


		double GPM2VEL(double x, double D);
		void hydraulicFluidViscosity();
		void regulate_displacement();
		double orificeEquation();
		void simulateEngPump();
		void simulateFlowLoss();
		void findDarcyFriction();
		void systemDeltaP();
		void systemPressure();
		void simulatePressureDisipate(double pressure, double t, double k);
		void assign_member();
		void make_members_current();
		void update_state();
		void update_context();
		void check_pump_operation();
		void check_pump_dissipate();
		void update(double delta_time) ;



	};

	class DC10ActuatorSystem {
		public:
			double dt;
			double requested_actuator_position;
			double actuator_Q;
			double actuator_P;
			double actuator_load = 500.0;
			double delta_time;
			bool retract;

			DC10ActuatorSystem(const ActuatorParams& params, 
                      DC10HydraulicEnginePumpSystem hydraulic_system_1, 
                      DC10HydraulicEnginePumpSystem hydraulic_system_2)
				: bore_area(params.bore_area_init), rod_area(params.rod_area_init),
				actuator_valve_radius(params.actuator_value_radius_init), actuator_position(params.actuator_position_init),
				active_control(params.active_control_init), ideal_flow(params.ideal_flow_init), system_1(hydraulic_system_1),
					system_2(hydraulic_system_2) {}


			SimVar* normal_n2_1 = &normal_n2_1_master;
			SimVar* fe_hydro_pressure_1 = &fe_hydro_pressure_1_master;
			SimVar* fe_hydro_pressure_2 = &fe_hydro_pressure_2_master;
			SimVar* fe_hydro_pressure_3 = &fe_hydro_pressure_3_master;


		  SimVar* fe_hydro_flow_1 = &fe_hydro_flow_1_master;
		  SimVar* fe_hydro_flow_2 = &fe_hydro_flow_2_master;
		  SimVar* fe_hydro_flow_3 = &fe_hydro_flow_3_master;

		  SimVar* dc_left_out_aileron = &dc_left_out_aileron_master;
		  SimVar* dc_left_in_aileron = &dc_left_in_aileron_master;
		  SimVar* dc_right_out_aileron = &dc_right_out_aileron_master;
		  SimVar* dc_right_in_aileron = &dc_right_in_aileron_master;
		  SimVar* dc_upper_rudder = &dc_upper_rudder_master;
		  SimVar* dc_lower_rudder = &dc_lower_rudder_master;
		  SimVar* actuator_load_var = &actuator_load_var_master;
		  SimVar* yoke_x_position = &yoke_x_position_master;
		  SimVar* yoke_y_position = &yoke_y_position_master;
		  SimVar* rudder_position = &rudder_position_master;

			//flight surfaces
			// SimVar dc_left_out_aileron = SimVar(
			// 	"DC_LeftOutAileron",
			// 	SimVarType::LOCAL,
			// 	"Number"
			// );

			// SimVar dc_left_in_aileron = SimVar(
			// 	"DC_LeftInAileron",
			// 	SimVarType::LOCAL,
			// 	"Number"
			// );

			// SimVar dc_right_out_aileron = SimVar(
			// 	"DC_RightOutAileron",
			// 	SimVarType::LOCAL,
			// 	"Number"
			// );

			// SimVar dc_right_in_aileron = SimVar(
			// 	"DC_RightInAileron",
			// 	SimVarType::LOCAL,
			// 	"Number"
			// );

			// SimVar dc_upper_rudder = SimVar(
			// 	"DC_RightInAileron",
			// 	SimVarType::LOCAL,
			// 	"Number"
			// );

			// SimVar dc_lower_rudder = SimVar(
			// 	"DC_RightInAileron",
			// 	SimVarType::LOCAL,
			// 	"Number"
			// );

			// SimVar actuator_load_var = SimVar(
			// 	"ACTUAOTR LOAD",
			// 	SimVarType::NORMAL,
			// 	"Number"
			// );

			// SimVar yoke_x_position = SimVar(
			// 	"YOKE X POSITION",
			// 	SimVarType::NORMAL,
			// 	"Number"
			// );

			// SimVar yoke_y_position = SimVar(
			// 	"YOKE Y POSITION",
			// 	SimVarType::NORMAL,
			// 	"Number"
			// );

			// SimVar rudder_position = SimVar(
			// 	"RUDDER POSITION",
			// 	SimVarType::NORMAL,
			// 	"Number"
			// );





	#define M_PI 3.14159265358979323846
	#define GPS_2_M3S(x) (x/264.2)
	#define LBS_2_N(x) ((x) * 4.4482)
	#define PSI_2_PA(x) ((x) * 6894.0)
			//TODO change later
			DC10HydraulicEnginePumpSystem system_1;
			DC10HydraulicEnginePumpSystem system_2;
			double actuator_valve_radius = 0.0;
			double actuator_entry_flowrate = 0.0;
			double valve_cross_area;
			double deltaP_actuator = 0.0;
			double actuator_position = 0.0;
			double actuator_fluid_velocity = 0.0;
			double actuator_min_fluid_velocity = 0.0;
			double total_volume_to_actuator = 0.0;
			double total_volume_to_reservoir = 0.0;
			double signed_flow = 0.0;
			//delta displacement of actuator
			double delta_displacement;
			//last position of actuator
			double last_actuator_position;
			//Diameter of pipe (m)
			const double D = 0.1016;
			//Simulation of length of pipe (m)
			const double L = 0.0762;
			//rod area of actuator piston TODO: class argument later
			double rod_area = 0.72;
			//bore area of actuator piston TODO: class argument later
			double bore_area = 0.72;
			//ideal flow within actuator piston TODO: class argument later
			double ideal_flow = 1.32;
			const double volume_extension_ratio = 0.1;
			//Allows active(axis) control of actuatora TODO: class argument later
			bool active_control = false;




      void assign_member();
			void actuator_entry_fluid_velocity();
			void actuator_entry_pressure();
			void simulate_actuator_fluid_velocity();
			void refresh_actuator_position();
			void actuator_valve_displacement(double delta_time);
			void update_requirments(bool is_retract);
			void update_request_control_surface(Control_Surface surface);
			void update_actuator_control_surface(Control_Surface surface);
			void update_actuator_position(double delta_time);
			void update(double delta_time);
		};

  class DC10ReversibleMotorPump {
	private:
		int switch_id;
		int switch_position;
		int eng_switch_3_pump;
		int eng_switch_positions;
		int eng_secondary_pump;
		MsfsAeroFramework::DC10HydraulicEnginePumpSystem& slave_hydro_line_1;
		MsfsAeroFramework::DC10HydraulicEnginePumpSystem& slave_hydro_line_2;
		MsfsAeroFramework::DC10HydraulicEnginePumpSystem& system_3_left_line;
		MsfsAeroFramework::DC10HydraulicEnginePumpSystem& system_3_right_line;
		MsfsAeroFramework::DC10HydraulicEnginePumpSystem& system_aux_line_1;
		MsfsAeroFramework::DC10HydraulicEnginePumpSystem& system_aux_line_2;
	public:
		double pressure_change;
		int operating;
		int sys_suppoprt_rmp_state;
		int sys_3_rmp_state;
		Pump_State state;

		SimVar** fe_eng_1_pump_pressure_member = nullptr;
		SimVar** fe_eng_2_pump_pressure_member = nullptr;
		SimVar** fe_engine_n2_member = nullptr;
		SimVar** fe_hydro_slave_member = nullptr;
		SimVar** fe_rmp_pump_operation_member = nullptr;

		SimVar* fe_hydro_pressure_1 = &fe_hydro_pressure_1_master;
		SimVar* fe_hydro_pressure_2 = &fe_hydro_pressure_2_master;
		SimVar* fe_hydro_pressure_3 = &fe_hydro_pressure_3_master;
		SimVar* fe_13_hydro_switch = &fe_13_hydro_switch_master;
		SimVar* fe_23_hydro_switch = &fe_23_hydro_switch_master;
		SimVar* fe_eng_1_hyd_pump_1_switch = &fe_eng_1_hyd_pump_1_switch_master;
		SimVar* fe_eng_1_hyd_pump_2_switch = &fe_eng_1_hyd_pump_2_switch_master;
		SimVar* fe_eng_2_hyd_pump_1_switch = &fe_eng_2_hyd_pump_1_switch_master;
		SimVar* fe_eng_2_hyd_pump_2_switch = &fe_eng_2_hyd_pump_2_switch_master;
		SimVar* fe_eng_3_hyd_pump_1_switch = &fe_eng_3_hyd_pump_1_switch_master;
		SimVar* fe_eng_3_hyd_pump_2_switch = &fe_eng_3_hyd_pump_2_switch_master;
    SimVar* fe_aux_hyd_pump_1_switch = &fe_aux_hyd_pump_1_switch_master;
    SimVar* fe_aux_hyd_pump_2_switch = &fe_aux_hyd_pump_2_switch_master;
		SimVar* fe_rmp_13_operation = &fe_rmp_13_operation_master;
		SimVar* fe_rmp_23_operation = &fe_rmp_23_operation_master;
		SimVar* eng_1_n2_percent = &eng_1_n2_percent_master;
		SimVar* eng_2_n2_percent = &eng_2_n2_percent_master;
		SimVar* line_1_1_operation = &hydro_line_1_1_operation_master;
		SimVar* line_1_2_operation = &hydro_line_1_2_operation_master;
		SimVar* line_2_1_operation = &hydro_line_2_1_operation_master;
		SimVar* line_2_2_operation = &hydro_line_2_2_operation_master;
		SimVar* line_3_1_operation = &hydro_line_3_1_operation_master;
		SimVar* line_3_2_operation = &hydro_line_3_2_operation_master;
    SimVar* line_aux_1_operation = &fe_aux_1_operation_master;
    SimVar* line_aux_2_operation = &fe_aux_2_operation_master;






		DC10ReversibleMotorPump(int _switch_iD, DC10HydraulicEnginePumpSystem& slave_line_1, DC10HydraulicEnginePumpSystem& slave_line_2, 
			DC10HydraulicEnginePumpSystem& master_system, DC10HydraulicEnginePumpSystem& master_system_2,
      DC10HydraulicEnginePumpSystem& master_aux_1, DC10HydraulicEnginePumpSystem& master_aux_2)
			: switch_id(_switch_iD), slave_hydro_line_1(slave_line_1), slave_hydro_line_2(slave_line_2),
				system_3_left_line(master_system), system_3_right_line(master_system_2), system_aux_line_1(master_aux_1),
        system_aux_line_2(master_aux_2){}

		void assign_member();
		void check_rmp_state(Pump_State& state);
		void handle_rmp_state(Pump_State& state);
		void check_RMP_operation();
		void simulate_pressure_exchange(double delta_time);
		void update_state(SimContext* sim_context);
		void update(double delta_time);

	};


  class DC10HydraulicAuxPumpSystem {

  private:


    DC10HydraulicEnginePumpSystem* enginePumpSystem;


  public:
   
   DC10HydraulicAuxPumpSystem(DC10HydraulicEnginePumpSystem* pumpSystem)
      : enginePumpSystem(pumpSystem) {}

    int switch_position;
    int switch_id;
    bool operating;
    double e_rpm = 0;

    void elec_motor(double delta_time);
    void elec_pump(double delta_time);
    void check_motor_operation();
    void assign_member();
    void simulate_aux_pump();
    void operate_circuts();
    void update(double delta_time);

    //fe
    SimVar* fe_aux_motor_1_rpm = &fe_aux_motor_1_rpm_master;
    SimVar* fe_aux_motor_2_rpm = &fe_aux_motor_2_rpm_master;
    SimVar* fe_aux_hyd_pump_1_switch = &fe_aux_hyd_pump_1_switch_master;
    SimVar* fe_aux_hyd_pump_2_switch = &fe_aux_hyd_pump_2_switch_master;
		SimVar* fe_aux_1_operation = &fe_aux_1_operation_master;
		SimVar* fe_aux_2_operation = &fe_aux_2_operation_master;

    //circuit 
    SimVar* cir_hydro_aux_pump_1 = &cir_hydro_aux_pump_1_master;
    SimVar* cir_hydro_aux_pump_2 = &cir_hydro_aux_pump_2_master;
    SimVar* o_adg_sw = &o_adg_sw_master;

		SimVar** fe_aux_motor_rpm_member = nullptr;
		SimVar** fe_aux_operating_member = nullptr;


};

	
	class DC10Accumulator {
	private:

	public:
		SimVar* fe_hydro_pressure_1 = &fe_hydro_pressure_1_master;
		SimVar* fe_hydro_pressure_2 = &fe_hydro_pressure_2_master;
		SimVar* fe_hydro_pressure_3 = &fe_hydro_pressure_3_master;

		void assign_member();
		void simulate_accumulator();
		void update(double delta_time); //FIXME --testbed exempt



	};

	class DC10HydraulicEnginePump {

			DC10HydraulicEnginePumpSystem engine_pump_1_1 = DC10HydraulicEnginePumpSystem(params_1_1);
			DC10HydraulicEnginePumpSystem engine_pump_1_2 = DC10HydraulicEnginePumpSystem(params_1_2);
			DC10HydraulicEnginePumpSystem engine_pump_2_1 = DC10HydraulicEnginePumpSystem(params_2_1);
			DC10HydraulicEnginePumpSystem engine_pump_2_2 = DC10HydraulicEnginePumpSystem(params_2_2);
			DC10HydraulicEnginePumpSystem engine_pump_3_1 = DC10HydraulicEnginePumpSystem(params_3_1);
			DC10HydraulicEnginePumpSystem engine_pump_3_2 = DC10HydraulicEnginePumpSystem(params_3_2);
      DC10HydraulicEnginePumpSystem aux_pump_1 = DC10HydraulicEnginePumpSystem(params_aux_1);
      DC10HydraulicEnginePumpSystem aux_pump_2 = DC10HydraulicEnginePumpSystem(params_aux_2);

			DC10ReversibleMotorPump reverse_motor_pump_1_3 = DC10ReversibleMotorPump(1, engine_pump_1_1, 
                                                                                  engine_pump_1_2, 
                                                                                  engine_pump_3_1, 
                                                                                  engine_pump_3_2,
                                                                                  aux_pump_1,
                                                                                  aux_pump_2);

			DC10ReversibleMotorPump reverse_motor_pump_2_3 = DC10ReversibleMotorPump(2, engine_pump_2_1, 
                                                                                  engine_pump_2_2, 
                                                                                  engine_pump_3_1, 
                                                                                  engine_pump_3_2,
                                                                                  aux_pump_1,
                                                                                  aux_pump_2);

      DC10ActuatorSystem test_actuator = DC10ActuatorSystem(actuator_param_test, engine_pump_1_1, engine_pump_1_2);

      DC10HydraulicAuxPumpSystem auxiliary_pump_1= DC10HydraulicAuxPumpSystem(&aux_pump_1);
      DC10HydraulicAuxPumpSystem auxiliary_pump_2= DC10HydraulicAuxPumpSystem(&aux_pump_2);


	public:
		DC10HydraulicEnginePump() {};
		void update(double delta_time);
	};
}



















#endif

