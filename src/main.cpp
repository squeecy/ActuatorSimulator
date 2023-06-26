#include <iostream>
#include <SFML/Graphics.hpp>
#include <hydraulics.h>
#include <thread>
#include <chrono>
#include <iomanip>
//#include <string.h>
#include <string>
#include <sstream>
#include <windows.h>

PIDController  PID(0.2, 0.4, 0.6, 3000.0, false);
PIDController  PID2(0.2, 0.4, 0.6, 3000.0, false);
PIDController  PID3(1600.0, 300.4, 300.6, 0.0, true);
//0.0708 for gear
LinearActuator actuator(0.145277, 0.104682, 0.127, 0.1908, true, 0.0, PID3);
HydraulicEngPump system1(6.0, 3.048, 0.5144, 0.0508, PID2, 1.0);
HydraulicEngPump system2(6.0, 3.048, 0.5144, 0.0508, PID2, 1.0);
HydraulicEngPump auxhydro(6.0, 3.048, 0.5144, 0.0508, PID, 1.0);
AuxPump auxP(auxhydro);
ReversibleMotorPump RMP;

double aileron_actuator(double position, float time, double pressure, double Q, bool retract)
{
	
	actuator.simulatePressureLoss(system1);
	return actuator.position(time, position, retract, Q, pressure);
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

double pressure_system_1()
{
	system1.systemPressure(9000.0, 18.0);

	return system1.publishPressure();
}

double pressure_system_2()
{
	system2.systemPressure(0.0, 18.0);

	return system2.publishPressure();
}

double auxPressure(int switchPosition, double time)
{
	auxP.simulateAuxPump(switchPosition, time);
	return auxP.publishAuxPressure();
}

double actuator_flow()
{
	return actuator.publish_actuator_fluid_flow();
}


double test = 0.0;
int main()
{
	sf::RenderWindow window(sf::VideoMode(1400, 800), "Actuator Window");
	sf::RenderWindow wing_window(sf::VideoMode(1400, 800), "Wing Window");
	sf::RenderWindow text_window(sf::VideoMode(1100, 800), "Value Window");


	sf::Clock clock;
	sf::RectangleShape rect;

	sf::Texture pActuator;
	sf::Texture pActuator_rod;
	sf::Texture pWing_base;

	sf::Sprite sActuator;
	sf::Sprite sActuator_rod;
	sf::Sprite sWing_base;

	sf::Font font;
	sf::Text text;
	sf::Text act_flow_text;
	sf::Text system_pressure_text;
	sf::Text system_flow_text;
	sf::Text aux_pressure_text;

	std::string userInput;

	double actuator_volume_var = 0.0;
	double actuator_fluid_flow_var = 0.0;
	double system_pressure_var = 0.0;
	double system_flow_rate_var = 0.0;
	double aux_pressure_var = 0.0;
	double currentPos = 0.0;


	if(!pActuator.loadFromFile("res/actuator.png"))
		std::cout << "Error couldnt load actuator image" << std::endl;
	if(!pActuator_rod.loadFromFile("res/actuator_rod.png"))
		std::cout << "Error couldnt load actuator_rod image" << std::endl;
	if(!pWing_base.loadFromFile("res/wing_base.png"))
		std::cout << "Error couldnt load wing_base image" << std::endl;
	if(!font.loadFromFile("res/fonts/RobotoMono-Thin.ttf"))
		std::cout << "Error loading font from file" << std::endl;

	text.setFont(font);
	act_flow_text.setFont(font);
	system_pressure_text.setFont(font);
	system_flow_text.setFont(font);
	aux_pressure_text.setFont(font);

	text.setPosition(100, 0);
	act_flow_text.setPosition(570, 0);
	system_pressure_text.setPosition(100, 150);
	system_flow_text.setPosition(570, 150);
	aux_pressure_text.setPosition(100, 300);

	text.setCharacterSize(22);
	act_flow_text.setCharacterSize(22);
	system_pressure_text.setCharacterSize(22);
	system_flow_text.setCharacterSize(22);
	aux_pressure_text.setCharacterSize(22);

	sActuator.setTexture(pActuator);
	sActuator_rod.setTexture(pActuator_rod);
	sWing_base.setTexture(pWing_base);


	bool button_count = false;
	bool aux_count = true;

	rect.setSize(sf::Vector2f(100, 100));
	window.setFramerateLimit(60);

	sf::Vertex aileron_pivot[]
	{
		sf::Vertex(sf::Vector2f(386, 548)),
		sf::Vertex(sf::Vector2f(529, 460))
	};

	sf::Vertex aileron_right_face[]
	{
		/* base */
		sf::Vertex((aileron_pivot[1].position)),

		/* level */
		sf::Vertex(sf::Vector2f(510,442))

		/* up */
		//sf::Vertex(sf::Vector2f(518,433))
		
		/* down */
		//sf::Vertex(sf::Vector2f(503,460))

	};

	sf::Vertex aileron_left_face[]
	{
		/* base */
		sf::Vertex((aileron_pivot[0].position)),

		/* level */
		sf::Vertex(sf::Vector2f(360, 536))

		/* up */
		//sf::Vertex(sf::Vector2f(380, 520))

		/* down */
		//sf::Vertex(sf::Vector2f(369, 544))
	};

	sf::Vertex aileron_trailing_face[]
	{
		sf::Vertex(aileron_left_face[1].position),

		sf::Vertex(aileron_right_face[1].position)
	};
	
	while (window.isOpen())
	{


		sf::Event event;
		sf::Event text_event;
		sf::Event wing_event;


		while(window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		while(text_window.pollEvent(text_event))
		{
			if (text_event.type == sf::Event::Closed)
				text_window.close();
		}

		while(wing_window.pollEvent(wing_event))
		{
			if (wing_event.type == sf::Event::Closed)
				wing_window.close();
		}



		float elapsed = clock.getElapsedTime().asSeconds();
		double yoke_x = sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::X);
		double rectangle_yoke_to_screen = lerp(yoke_x, -100.0, 0.0, 100.0, 1300.0);
		double rod_yoke_to_screen = lerp(currentPos, -1.0, -150.0, 1.0, 150.0);
		bool RMP_SWITCH = false;
		int gear_position = 0;
		sf::Vector2i mousePosition = sf::Mouse::getPosition(wing_window);

		//aileron_right_face[0].position += sf::Vector2f(1.0f, 1.0f);
		//aileron_left_face[1].position += sf::Vector2f(1.0f, 1.0f);
			aileron_right_face[0].position += sf::Vector2f(1.0f, 1.0f);
			aileron_left_face[0].position += sf::Vector2f(1.0f, 1.0f);
			
			aileron_right_face[1].position += sf::Vector2f(1.0f, 1.0f);
			aileron_left_face[1].position += sf::Vector2f(1.0f, 1.0f);

			aileron_trailing_face[0].position = aileron_left_face[1].position;
			aileron_trailing_face[1].position = aileron_right_face[1].position;

		
		// left face up sf::Vertex(sf::Vector2f(380, 520))
		//right face up sf::Vertex(sf::Vector2f(518,433))


		if(sf::Joystick::isConnected(1))
		{

			sf::Vector2f rectanglePosition(rectangle_yoke_to_screen, 600);
			sf::Vector2f actuator_rod_position(rod_yoke_to_screen, 0.0);
			rect.setPosition(rectanglePosition);
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

		if(aux_count == true)
		{
			aux_pressure_var = auxPressure(aux_count, elapsed);
		}else{
			aux_pressure_var = auxhydro.simulatePressureDisipate(elapsed);
		}

		RMP.fluidTransfer(&system1, &system2,  RMP_SWITCH);


		if(button_count == true)
		{
			actuator_fluid_flow_var = actuator_flow();
			actuator_volume_var = actuator_vol();
			system_pressure_var = pressure_system_1();
			pressure_system_2();
			system_flow_rate_var = flow_rate();
		}
		else{
			system_pressure_var = system1.simulatePressureDisipate(elapsed);
		}

		currentPos =  aileron_actuator((yoke_x / 100.0), elapsed, pressure_system_1(), system_flow_rate_var, gear_position);

		//std::cout << aileron_actuator((yoke_x / 100.0), elapsed, system_pressure, 1.1) << std::endl;
		//std::cout << elapsed << " : " << currentPos << std::endl;
		//std::cout << delta_position() << std::endl;
		//std::cout << elapsed << std::endl;
		//std::cout << velocity(yoke_x / 100.0, 1.1) << std::endl;
		//std::cout << "Flow Rate: " << system_pressure << std::endl;
		//std::cout << currentPos  << std::endl;
		//std::cout << aux_count << std::endl;
		//std::cout << auxPressure(aux_count, elapsed) << std::endl;
		//std::cout << gear_position << std::endl;
		//std::cout << actuator_flow() << std::endl;
		//std::cout << mousePosition.x << " " << mousePosition.y << std::endl;
		//std::cout << RMP_SWITCH << std::endl;
		//std::cout << system2.publishPressure() << std::endl;
		//std::cout << ((yoke_x/100.0)) << std::endl;
		float dot_x = 200.0f;
		float dot_y = 200.0f;





		window.draw(sActuator);
		window.draw(sActuator_rod);
		window.display();
		window.clear();

		/* TODO assign units */
		std::string actuator_flow_rate = std::to_string(actuator_fluid_flow_var);
		std::string actuator_volume = std::to_string(actuator_volume_var);
		std::string system_pressure = std::to_string(system_pressure_var);
		std::string system_flow_rate = std::to_string(system_flow_rate_var);
		std::string aux_pressure_write = std::to_string(aux_pressure_var);

		text.setString("Actuator Volume: " +  actuator_volume + " cbm^3 " " | ");
		act_flow_text.setString("Actuator Flow Rate: " + actuator_flow_rate);
		system_pressure_text.setString(" System Pressure: " + system_pressure);
		system_flow_text.setString(" System Flow: " + system_flow_rate);
		aux_pressure_text.setString(" Aux Pressure : " + aux_pressure_write);


		/* text window */
		text_window.draw(text);
		text_window.draw(act_flow_text);
		text_window.draw(system_pressure_text);
		text_window.draw(system_flow_text);
		text_window.draw(aux_pressure_text);
		text_window.display();
		text_window.clear();

		/* wing window */ 
		wing_window.draw(sWing_base);
		wing_window.draw(aileron_pivot, 2, sf::Lines);
		wing_window.draw(aileron_left_face, 2, sf::Lines);
		wing_window.draw(aileron_right_face, 2, sf::Lines);
		wing_window.draw(aileron_trailing_face, 2, sf::Lines);
		wing_window.display();
		wing_window.clear();
	}
	return 0;
}
