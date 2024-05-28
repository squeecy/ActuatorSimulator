
PIDController  PID(0.2, 0.4, 0.6, 3000.0, false);
PIDController  PID2(0.2, 0.4, 0.6, 3000.0, false);
PIDController  PID3(1600.0, 300.4, 300.6, 0.0, true);
PIDController  PID4(100.2, 2000.4, 300.6, 0.0, true);
PIDController  PID69(0.2, 0.4, 0.6, 3000.0, false);
//0.0708 for gear
//LinearActuator actuator(0.145277, 0.104682, 0.127, 0.05908, true, 0.0, PID3, PID4);
LinearActuator actuator(0.0165, 0.0086, 0.364, 0.05908, true, 1.0, 0.17, PID3, PID4);
//LinearActuator actuator(0.0002953, 0.0010, 0.043, 0.0889, true, 0.0, PID3, PID4);
LinearActuator actuator2(0.145277, 0.104682, 0.127, 0.1908, true, 0.0, 0.00063, PID3, PID4);
HydraulicEngPump system1(7.0, 3.048, 0.5144, 0.0508, PID2, 1.0);
HydraulicEngPump system2(7.0, 3.048, 0.5144, 0.0508, PID2, 1.0);
HydraulicEngPump system3(6.0, 3.048, 0.5144, 0.0508, PID2, 1.0);
HydraulicEngPump auxhydro(6.0, 3.048, 0.5144, 0.0508, PID, 1.0);
AuxPump auxP1(system3);
AuxPump auxP2(system3);
ReversibleMotorPump Rev_MP;


double ENG_1_N2_RPM = 0.0;
double ENG_2_N2_RPM = 0.0;
double AUX_1_RPM = 0.0;
double aileron_actuator(double position, float time, double pressure, double Q, bool retract)
{
	
	//actuator.simulatePressureLoss(system1);
	//actuator2.simulatePressureLoss(system2);
	return actuator.position(time, position, retract, Q, pressure, 3000.0);
}

double test_actuator(double position, float time, double pressure, double Q, bool retract)
{
	//actuator2.simulatePressureLoss(system1);
	return actuator2.position(time, position, retract, Q, pressure, 200.0);
}

double delta_position()
{
	return actuator.publish_position();
}

double actuator_vol()
{
	return actuator.publish_actuator_vol();
}

double flow_rate()
{
	return system1.publishFlow();
}

double flow_2_rate()
{
	return system2.publishFlow();
}

double flow_3_rate()
{
	return system3.publishFlow();
}

double pressure_system_1(double N2, bool RMP)
{
	system1.systemPressure(N2, 18.0, RMP);
	system1.fluidLoss(3.7);

	return system1.publishPressure();
}

double pressure_system_2(double N2, bool RMP)
{
	//bool RMP = system2.isRMP(RMP_switch_pos);
	//std::cout << RMP << std::endl;
	system2.systemPressure(N2, 18.0, RMP);
	system2.fluidLoss(3.7);

	return system2.publishPressure();
}

double pressure_system_3()
{
	system2.systemPressure(0.0, 18.0, false);

	return system3.publishPressure();
}

double auxPressure1(int switchPosition, double time)
{
	auxP1.simulateAuxPump(&system3, switchPosition, time);
	return auxP1.publishAuxPressure();
}

double auxPressure2(int switchPosition, double time)
{
	auxP2.simulateAuxPump(&system3, switchPosition, time);
	return auxP2.publishAuxPressure();
}

double actuator_flow()
{
	return actuator.publish_actuator_fluid_flow();
}



void hydro_sprites()
{
	sf::Texture pActuator;
	sf::Texture pActuator_rod;
	sf::Texture pHydro_panel;
	sf::Texture pEng1HydSwitch_off;
	sf::Texture pEng1HydSwitch_auto;
	sf::Texture pEng1HydSwitch_on;

	sf::Texture pAuxSwitch_norm;
	sf::Texture pAuxSwitch_start;
	sf::Texture pAuxSwitch_stop;

	sf::Sprite sActuator;
	sf::Sprite sActuator_rod;
	sf::Sprite sHydro_panel;
	sf::Sprite sEng1HydSwitch;
	sf::Sprite sEng2HydSwitch;
	sf::Sprite sEng3HydSwitch;
	sf::Sprite sEng1HydSwitch_2;
	sf::Sprite sEng2HydSwitch_2;
	sf::Sprite sEng3HydSwitch_2;
	sf::Sprite sEng1RMPSwitch;
	sf::Sprite sEng2RMPSwitch;
	sf::Sprite sAuxSwitch_1;
	sf::Sprite sAuxSwitch_2;
}
