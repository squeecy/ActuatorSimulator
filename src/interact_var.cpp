#include <iostream>
#include <thread>
#include "interact_var.h"
#include "simvar.h"



//update simvars with respect to position of switches
void update_on_switch_click(SpritePositionData* data)
{

     fe_eng_1_hyd_pump_1_switch_master.current_value = data->spritePositions[0];
     fe_eng_1_hyd_pump_2_switch_master.current_value = data->spritePositions[12];
     fe_eng_2_hyd_pump_1_switch_master.current_value = data->spritePositions[4];
     fe_eng_2_hyd_pump_2_switch_master.current_value = data->spritePositions[16];
     fe_eng_3_hyd_pump_1_switch_master.current_value = data->spritePositions[8];
     fe_eng_3_hyd_pump_2_switch_master.current_value = data->spritePositions[20];
     fe_13_hydro_switch_master.current_value = data->spritePositions[24];
     fe_23_hydro_switch_master.current_value = data->spritePositions[28];

}

//rotate hydro needles based on hydro pressure
void rotate_needles(std::vector<sf::Sprite>& sprite)
{
    rotate_sprite(sprite, fe_hydro_pressure_1_master.current_value, left_pressure_needle_index);
    rotate_sprite(sprite, fe_hydro_pressure_2_master.current_value, center_pressure_needle_index);
    rotate_sprite(sprite, fe_hydro_pressure_3_master.current_value, right_pressure_needle_index);
}