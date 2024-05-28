#include <iostream>
#include "hydraulics.h"
#include "user.h"
//#include <main.h>
#include "structure.h"
#include <thread>
#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <chrono>
#include <ctime>

#include "render.h"
#include "interact_var.h"
#include "gui/gui.h"

using namespace std::chrono;

const double Kp = 4.01;
const double Ki = 7.05;
const double Kd = 10.07;
const double target_pressure = 3000.0;
//HydraulicEngPump system1(7.0, 3.048, 0.5144, 0.0508, 1.0);

double ENG_1_N2_RPM = 0.0;
double ENG_2_N2_RPM = 0.0;
double AUX_1_RPM = 0.0;


//MsfsAeroFramework::DC10HydraulicEnginePumpSystem hydro_pump = MsfsAeroFramework::DC10HydraulicEnginePumpSystem();
MsfsAeroFramework::DC10HydraulicEnginePump hydro_pump; 

int main()
{

	struct SpritePositionData sprite_data;
	double seconds = 0.0;
	double previous_seconds = 0.0;
	double local_elapsed_time = 0.0;

	sf::RenderWindow simulation_window(sf::VideoMode(1820, 960), "Value Window");
	ImGui::SFML::Init(simulation_window);

	sf::Clock clock;
	sf::RectangleShape rect;


	auto start = std::chrono::high_resolution_clock::now();



	Render render;
	GUI gui;


	render.init_sprite(texture_class_vec, sprite_class_vec, texture_library);
	render.init_text(text_class_vec, text_library);


	rect.setSize(sf::Vector2f(100, 100));
	simulation_window.setFramerateLimit(60);


	sf::Vector2f anchoredEndPosition_system_1_p(1150.f, 120.f);
	sf::Vector2f anchoredEndPosition_system_2_p(1420.f, 120.f);
	sf::Vector2f anchoredEndPosition_system_3_p(1705.f, 120.f);

	system_1_needle.setPosition(anchoredEndPosition_system_1_p);
	system_2_needle.setPosition(anchoredEndPosition_system_2_p);
	system_3_needle.setPosition(anchoredEndPosition_system_3_p);




	sf::Clock deltaClock;

	while (simulation_window.isOpen())
	{


    std::clock_t c_start = std::clock();
		auto end = high_resolution_clock::now();
		duration<double> elapsedTime = end - start;

		sf::Event event;
		sf::Event text_event;
		sf::Event hydro_panel_event;
		sf::Event simulation_window_event;


		while(simulation_window.pollEvent(simulation_window_event))
		{
			ImGui::SFML::ProcessEvent(simulation_window, simulation_window_event);
			if(simulation_window_event.type == sf::Event::MouseWheelScrolled)
			{
				if(simulation_window_event.mouseWheelScroll.delta > 0)
				{
					render.handle_sprite_clickspots(&sprite_data, sprite_class_vec, simulation_window, 1);
				}
				if(simulation_window_event.mouseWheelScroll.delta < 0)
				{
					render.handle_sprite_clickspots(&sprite_data, sprite_class_vec, simulation_window, -1);
				}
			}

			if(simulation_window_event.type == sf::Event::Closed)
				simulation_window.close();


		}
		render.handle_texture_swap(&sprite_data, sprite_class_vec, texture_class_vec);
		update_on_switch_click(&sprite_data);
		rotate_needles(sprite_class_vec);


		float elapsed = clock.getElapsedTime().asSeconds();
		sf::Time deltaTime = clock.restart();
		float elapsedDt = deltaTime.asSeconds();
		double yoke_x = sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::X);
		double right_brake = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
		double left_brake = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
		double rudder = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);

		hydro_pump.update(elapsedDt);

		ImGui::SFML::Update(simulation_window, deltaClock.restart());
		gui.gui_var_values();

		sf::Vector2i mousePosition = sf::Mouse::getPosition(simulation_window);
		//std::cout << yoke_x << std::endl;
		//std::cout << mousePosition.x << ", " << mousePosition.y << std::endl;




		if(sf::Joystick::isConnected(1))
		{

		}

		previous_seconds = seconds;
		seconds = elapsedTime.count();
		
		draw_window_guage(simulation_window, render.draw_textures);
		ImGui::SFML::Render(simulation_window);
		simulation_window.display();
		simulation_window.clear();

	}
	ImGui::SFML::Shutdown();
	return 0;
}
