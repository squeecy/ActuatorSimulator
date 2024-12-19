#include <iostream>
#include <thread>
#include <sstream>
#include <SFML/Window.hpp>
#include "interact_var.h"
#include "simvar.h"
#include "render.h"
#include "interp.h"




//update simvars with respect to position of switches
void update_on_switch_click(SpritePositionData* data)
{
     fe_eng_1_hyd_pump_1_switch_master.current_value = data->spritePositions[0];
     fe_eng_1_hyd_pump_2_switch_master.current_value = data->spritePositions[1];
     fe_eng_2_hyd_pump_1_switch_master.current_value = data->spritePositions[2];
     fe_eng_2_hyd_pump_2_switch_master.current_value = data->spritePositions[3];
     fe_eng_3_hyd_pump_1_switch_master.current_value = data->spritePositions[4];
     fe_eng_3_hyd_pump_2_switch_master.current_value = data->spritePositions[5];
     fe_13_hydro_switch_master.current_value = data->spritePositions[6];
     fe_23_hydro_switch_master.current_value = data->spritePositions[7];
     fe_aux_hyd_pump_1_switch_master.current_value = data->spritePositions[8];
     fe_aux_hyd_pump_2_switch_master.current_value = data->spritePositions[9];
}

//rotate hydro needles based on hydro pressure
void rotate_needles(std::vector<sf::Sprite>& sprite)
{
    rotate_sprite(sprite, fe_hydro_pressure_1_master.current_value, left_pressure_needle_index);
    rotate_sprite(sprite, fe_hydro_pressure_2_master.current_value, center_pressure_needle_index);
    rotate_sprite(sprite, fe_hydro_pressure_3_master.current_value, right_pressure_needle_index);
}

void assign_game_axis_values()
{
  double yoke_x_actual = sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::X);
  double yoke_y_actual = sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::Y);
  double rudder_z_actual = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);

  yoke_x_position_master.current_value = yoke_x_actual * 0.01;
  yoke_y_position_master.current_value = yoke_y_actual * 0.01;
  rudder_position_master.current_value = rudder_z_actual * 0.01;
  
}


void connect_actuator_to_system()
{
  double normal_position = lerp(dc_left_out_aileron_master.current_value, -1.0, -155.0, 1.0, 150.0);

  std::string str = std::to_string(normal_position);
  updating_texture_library[1] = str;

}
