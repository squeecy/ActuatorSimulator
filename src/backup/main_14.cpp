#include <iostream>
#include <hydraulics.h>
//#include <main.h>
#include <structure.h>
#include <thread>
#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <chrono>
using namespace std::chrono;




double test = 0.0;
int main()
{
	double seconds = 0.0;
	double previous_seconds = 0.0;
	double local_elapsed_time = 0.0;

	sf::RenderWindow simulation_window(sf::VideoMode(1820, 960), "Value Window");

	sf::Clock clock;
	sf::RectangleShape rect;


	sf::Font font;

	sf::Text system_pressure_text;
	sf::Text sys_2_p_text;
	sf::Text sys_3_p_text;
	sf::Text act_flow_text;
	sf::Text system_flow_text;
	sf::Text system_2_flow_text;
	sf::Text system_3_flow_text;
	sf::Text aux_pressure_text;

	auto start = std::chrono::high_resolution_clock::now();



	double system_1_pressure_var = 0.0;
	double system_2_pressure_var = 0.0;
	double system_3_pressure_var = 0.0;
	double actuator_volume_var = 0.0;
	double actuator_fluid_flow_var = 0.0;
	double system_flow_rate_var = 0.0;
	double system_2_flow_rate_var = 0.0;
	double system_3_flow_rate_var = 0.0;
	double aux_1_pressure_var = 0.0;
	double aux_2_pressure_var = 0.0;
	double currentPos = 0.0;
	double testPos = 0.0;
	double currentPos2 = 0.0;

	int eng1L_switch_position = 0;
	int eng2L_switch_position = 0;
	int eng1RMP_switch_position = 0;
	int eng2RMP_switch_position = 0;
	int auxPump_1_switch_position = 0;
	int auxPump_2_switch_position = 0;

	if(!pActuator.loadFromFile("res/actuator.png"))
		std::cout << "Error couldnt load actuator image" << std::endl;
	if(!pActuator_rod.loadFromFile("res/actuator_rod.png"))
		std::cout << "Error couldnt load actuator_rod image" << std::endl;
	if(!pHydro_panel.loadFromFile("res/hydro_panel/hydro_panel.png"))
		std::cout << "Error couldnt load hydro panel image" << std::endl;
	if(!font.loadFromFile("res/fonts/RobotoMono-Thin.ttf"))
		std::cout << "Error loading font from file" << std::endl;
	if(!pEng1HydSwitch_off.loadFromFile("res/hydro_panel/main_switch_off.png"))
		std::cout << "Error couldnt load main switch off image" << std::endl;
	if(!pEng1HydSwitch_on.loadFromFile("res/hydro_panel/main_switch_on.png"))
		std::cout << "Error couldnt load main switch off image" << std::endl;
	if(!pEng1HydSwitch_auto.loadFromFile("res/hydro_panel/main_switch_auto.png"))
		std::cout << "Error couldnt load main switch off image" << std::endl;
	if(!pAuxSwitch_norm.loadFromFile("res/hydro_panel/aux_switch_norm.png"))
		std::cout << "Error couldnt load main switch off image" << std::endl;
	if(!pAuxSwitch_start.loadFromFile("res/hydro_panel/aux_switch_start.png"))
		std::cout << "Error couldnt load main switch off image" << std::endl;
	if(!pAuxSwitch_stop.loadFromFile("res/hydro_panel/aux_switch_stop.png"))
		std::cout << "Error couldnt load main switch off image" << std::endl;

	sys_2_p_text.setFont(font);
	sys_3_p_text.setFont(font);
	act_flow_text.setFont(font);
	system_pressure_text.setFont(font);
	system_flow_text.setFont(font);
	system_2_flow_text.setFont(font);
	system_3_flow_text.setFont(font);
	aux_pressure_text.setFont(font);

	system_pressure_text.setPosition(86, 653);
	sys_2_p_text.setPosition(100,750); 
	sys_3_p_text.setPosition(100,850); 
	act_flow_text.setPosition(570, 500);
	system_flow_text.setPosition(570, 653);
	system_2_flow_text.setPosition(570, 750);
	system_3_flow_text.setPosition(570, 850);
	aux_pressure_text.setPosition(100, 800);

	system_pressure_text.setCharacterSize(22);
	sys_2_p_text.setCharacterSize(22);
	sys_3_p_text.setCharacterSize(22);
	act_flow_text.setCharacterSize(22);
	system_flow_text.setCharacterSize(22);
	system_2_flow_text.setCharacterSize(22);
	system_3_flow_text.setCharacterSize(22);
	aux_pressure_text.setCharacterSize(22);

	sActuator.setTexture(pActuator);
	sActuator_rod.setTexture(pActuator_rod);
	sHydro_panel.setTexture(pHydro_panel);
	sEng2HydSwitch.setTexture(pEng1HydSwitch_off);
	sEng3HydSwitch.setTexture(pEng1HydSwitch_off);

	sEng1HydSwitch_2.setTexture(pEng1HydSwitch_off);
	sEng2HydSwitch_2.setTexture(pEng1HydSwitch_off);
	sEng3HydSwitch_2.setTexture(pEng1HydSwitch_off);



	sHydro_panel.setPosition(940.f, 0.f);
	sActuator.setPosition(0.f, -200.0f);


	bool button_count = false;
	bool aux_count = true;

	rect.setSize(sf::Vector2f(100, 100));
	simulation_window.setFramerateLimit(60);


	sf::Vector2f anchoredEndPosition_system_1_p(1150.f, 120.f);
	sf::Vector2f anchoredEndPosition_system_2_p(1420.f, 120.f);
	sf::Vector2f anchoredEndPosition_system_3_p(1705.f, 120.f);

	system_1_needle.setPosition(anchoredEndPosition_system_1_p);
	system_2_needle.setPosition(anchoredEndPosition_system_2_p);
	system_3_needle.setPosition(anchoredEndPosition_system_3_p);


	
	sEng1HydSwitch.setPosition(1070.f, 500.f);
	sEng2HydSwitch.setPosition(1350.f, 500.f);
	sEng3HydSwitch.setPosition(1625.f, 500.f);

	sEng1HydSwitch_2.setPosition(1160.f, 500.f);
	sEng2HydSwitch_2.setPosition(1440.f, 500.f);
	sEng3HydSwitch_2.setPosition(1715.f, 500.f);

	sEng1RMPSwitch.setPosition(1240.f, 200.f);
	sEng2RMPSwitch.setPosition(1480.f, 250.f);
	sAuxSwitch_1.setPosition(1473.f, 500.f);
	//sf::Sprite sEng1RMPSwitch;


	//push to vector to draw to window
	std::vector<sf::Drawable*> drawables;
	drawables.push_back(&sHydro_panel);
	drawables.push_back(&sActuator);
	drawables.push_back(&sActuator_rod);
	drawables.push_back(&system_1_needle);
	drawables.push_back(&system_2_needle);
	drawables.push_back(&system_3_needle);
	drawables.push_back(&system_pressure_text);
	drawables.push_back(&sys_2_p_text);
	drawables.push_back(&sys_3_p_text);
	drawables.push_back(&system_flow_text);
	drawables.push_back(&system_2_flow_text);
	drawables.push_back(&system_3_flow_text);
	drawables.push_back(&sEng1HydSwitch);
	drawables.push_back(&sEng2HydSwitch);
	drawables.push_back(&sEng3HydSwitch);
	drawables.push_back(&sEng1HydSwitch_2);
	drawables.push_back(&sEng2HydSwitch_2);
	drawables.push_back(&sEng3HydSwitch_2);
	drawables.push_back(&sEng1RMPSwitch);
	drawables.push_back(&sEng2RMPSwitch);
	drawables.push_back(&sAuxSwitch_1);
	drawables.push_back(&sAuxSwitch_2);

	while (simulation_window.isOpen())
	{

		auto end = high_resolution_clock::now();
		duration<double> elapsedTime = end - start;

		sf::Event event;
		sf::Event text_event;
		sf::Event hydro_panel_event;
		sf::Event simulation_window_event;


		while(simulation_window.pollEvent(simulation_window_event))
		{
			if (simulation_window_event.type == sf::Event::Closed)
				simulation_window.close();
			else if(simulation_window_event.type == sf::Event::MouseWheelScrolled)
			{
				//std::cout << simulation_window_event.mouseWheelScroll.delta << std::endl;
			}
		}

		system_1_needle_angle = set_needle_guage_interp(system_1_pressure_var, 0, 0, 3000, 180);
		system_1_needle.setRotation(system_1_needle_angle);

		system_2_needle_angle = set_needle_guage_interp(system_2_pressure_var, 0, 0, 3000, 180);
		system_2_needle.setRotation(system_2_needle_angle);

		system_3_needle_angle = set_needle_guage_interp(system_3_pressure_var, 0, 0, 3000, 180);
		system_3_needle.setRotation(system_3_needle_angle);


		float elapsed = clock.getElapsedTime().asSeconds();
		sf::Time deltaTime = clock.restart();
		float elapsedDt = deltaTime.asSeconds();
		double yoke_x = sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::X);
		double rod_yoke_to_screen = lerp(currentPos, -1.0, -150.0, 1.0, 150.0);
		bool RMP_SWITCH = false;
		int gear_position = 0;
		sf::Vector2i mousePosition = sf::Mouse::getPosition(simulation_window);
		//std::cout << mousePosition.x << ", " << mousePosition.y << std::endl;



		if(sf::Joystick::isConnected(1))
		{

			sf::Vector2f actuator_rod_position(rod_yoke_to_screen, -200.0);
			testPos;

			sActuator_rod.setPosition(actuator_rod_position);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			button_count = true;
		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			button_count = false;
		}


		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			aux_count = true;
		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			aux_count = false;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			RMP_SWITCH = true;
		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			RMP_SWITCH = false;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
				gear_position  +=1;
		}

		if(click_spot_circle(mousePosition.x, mousePosition.y, 1125.f, 555.f, 20.f))
		{
			handle_switch_position(simulation_window_event.mouseWheelScroll.delta, eng1L_switch_position, 1, -1);
		}else if(click_spot_circle(mousePosition.x, mousePosition.y, 1405.f, 555.f, 20.f))
		{
			handle_switch_position(simulation_window_event.mouseWheelScroll.delta, eng2L_switch_position, 1, -1);
		}

		if(click_spot_circle(mousePosition.x, mousePosition.y, 1295.f, 235.f, 20.f))
		{
			handle_switch_position(simulation_window_event.mouseWheelScroll.delta, eng1RMP_switch_position, 1, -1);
		}

		if(click_spot_circle(mousePosition.x, mousePosition.y, 1530.f, 300.f, 20.f))
		{
			handle_switch_position(simulation_window_event.mouseWheelScroll.delta, eng2RMP_switch_position, 1, -1);
		}

		if(click_spot_circle(mousePosition.x, mousePosition.y, 1560.f, 590.f, 20.f))
		{
			handle_switch_position(simulation_window_event.mouseWheelScroll.delta, auxPump_1_switch_position, 1, -1);
		}

		//std::cout << eng1RMP_switch_position << std::endl;
		bool RMP_1;
		bool RMP_2;

		previous_seconds = seconds;
		seconds = elapsedTime.count();

		//engine 1 left hyd switch
		if(eng1L_switch_position == 1)
		{
			actuator_fluid_flow_var = actuator_flow();
			actuator_volume_var = actuator_vol();
			system_flow_rate_var = flow_rate();
			sEng1HydSwitch.setTexture(pEng1HydSwitch_on);
			ENG_1_N2_RPM = 9000.0;

		}
		else{
			sEng1HydSwitch.setTexture(pEng1HydSwitch_off);
			//system_1_pressure_var = system1.simulatePressureDisipate(0.0);

		}
		system_1_pressure_var = pressure_system_1(ENG_1_N2_RPM, RMP_1);

		RMP_1 = system2.isRMP(eng1RMP_switch_position);
		RMP_2 = system2.isRMP(eng2RMP_switch_position);

		//engine 2 left hyd switch
		if(eng2L_switch_position == 1)
		{
			system_2_flow_rate_var = flow_2_rate();
			sEng2HydSwitch.setTexture(pEng1HydSwitch_on);
			ENG_2_N2_RPM = 9000.0;
		}
		else{
			sEng2HydSwitch.setTexture(pEng1HydSwitch_off);
			ENG_2_N2_RPM = 0.0;
			double test;
			if(system_2_pressure_var > 0.0)
			{
				double difference = seconds - previous_seconds;

				local_elapsed_time += difference;
				test += difference;

				//std::cout << test << std::endl;
				//system_2_pressure_var = system2.simulatePressureDisipate(local_elapsed_time, 0.265);
			}else{
				local_elapsed_time = 0.0;
			}
		}

		system_2_pressure_var = pressure_system_2(ENG_2_N2_RPM, RMP_2);

		//eng 1 RMP switch
		if(eng1RMP_switch_position == 0)
		{
			sEng1RMPSwitch.setTexture(pEng1HydSwitch_on);

		}else if(eng1RMP_switch_position == -1)
		{
			sEng1RMPSwitch.setTexture(pEng1HydSwitch_off);
		}

		//eng 2 RMP switch
		if(eng2RMP_switch_position == 0)
		{
			sEng2RMPSwitch.setTexture(pEng1HydSwitch_on);

		}else if(eng2RMP_switch_position == -1)
		{
			sEng2RMPSwitch.setTexture(pEng1HydSwitch_off);
		}

		//aux pumps
		if(auxPump_1_switch_position == 1)
		{

			sAuxSwitch_1.setTexture(pAuxSwitch_start);
			aux_1_pressure_var = auxPressure1(aux_count, 0.0);
			system_3_flow_rate_var = flow_3_rate();
			system_3_pressure_var = aux_1_pressure_var;
			AUX_1_RPM = 9000.0;
		}else{
			sAuxSwitch_1.setTexture(pAuxSwitch_norm);
			AUX_1_RPM = 0.0;
			if(system_3_pressure_var > 0.0)
			{
				double difference = seconds - previous_seconds;
				double local_time;
				local_time += difference;
				//system_3_pressure_var = system3.simulatePressureDisipate(local_elapsed_time, 0.208);
				system_3_flow_rate_var = auxP1.simulateAuxPumpDissipate(&system3, local_time);
				std::cout << local_time << std::endl;
			}else{
			}
		}


		Rev_MP.fluidTransfer(&system3, &system2, std::abs(RMP_2),1.89, 9.00 );
		Rev_MP.fluidTransfer(&system3, &system1, std::abs(RMP_1), 0.89, 11.96);
		//std::cout << std::abs(RMP_2) << std::endl;
		//std::cout << RMP << std::endl;
		//std::cout << std::abs(eng1RMP_switch_position) << std::endl;

		currentPos =  aileron_actuator((yoke_x / 100.0), elapsed, pressure_system_1(ENG_1_N2_RPM, RMP_1), system_flow_rate_var, gear_position);
		//testPos =  test_actuator((yoke_x / 100.0), elapsed, pressure_system_1(), system_flow_rate_var, gear_position);
		//currentPos2 =  aileron_actuator((yoke_x / 100.0), elapsed, pressure_system_2(), system_2_flow_rate_var, gear_position);
		//std::cout << aileron_actuator((yoke_x / 100.0), elapsed, system_pressure, 1.1) << std::endl;
		//std::cout << elapsed << " : " << currentPos << std::endl;
		//std::cout << delta_position() << std::endl;
		//std::cout << elapsed << std::endl;
		//std::cout << velocity(yoke_x / 100.0, 1.1) << std::endl;
		//std::cout << "Flow Rate: " << system_pressure << std::endl;
		//std::cout << aux_count << std::endl;
		//std::cout << auxPressure(aux_count, elapsed) << std::endl;
		//std::cout << gear_position << std::endl;
		//std::cout << actuator_flow() << std::endl;
		//std::cout << mousePosition.x << " " << mousePosition.y << std::endl;
		//std::cout << RMP_SWITCH << std::endl;
		//std::cout << system2.publishPressure() << std::endl;
		//std::cout << ((yoke_x/100.0)) << std::endl;




		/* TODO assign units */
		std::string actuator_flow_rate = std::to_string(actuator_fluid_flow_var);
		std::string actuator_volume = std::to_string(actuator_volume_var);
		std::string system_1_pressure = std::to_string(system_1_pressure_var);
		std::string system_2_pressure = std::to_string(system_2_pressure_var);
		std::string system_3_pressure = std::to_string(system_3_pressure_var);
		std::string system_flow_rate = std::to_string(system_flow_rate_var);
		std::string system_2_flow_rate = std::to_string(system_2_flow_rate_var);
		std::string system_3_flow_rate = std::to_string(system_3_flow_rate_var);
		std::string aux_pressure_write = std::to_string(aux_1_pressure_var);

		system_pressure_text.setString(" System 1 Pressure: " + system_1_pressure + " PSI ");
		sys_2_p_text.setString("System 2 Pressure: " +  system_2_pressure + " PSI ");
		sys_3_p_text.setString("System 3 Pressure: " +  system_3_pressure + " PSI ");
		system_flow_text.setString(" System 1 Flow: " + system_flow_rate + " GPM ");
		system_2_flow_text.setString(" System 2 Flow: " + system_2_flow_rate + " GPM ");
		system_3_flow_text.setString(" System 3 Flow: " + system_3_flow_rate + " GPM ");
		//act_flow_text.setString("Actuator Flow Rate: " + actuator_flow_rate + " GPM ");
		//aux_pressure_text.setString(" Aux Pressure : " + aux_pressure_write);

		/* simulation window */
		draw_window_guage(simulation_window, drawables);
		simulation_window.display();
		simulation_window.clear();

		//std::cout << sec << std::endl;
	}
	return 0;
}
