#ifndef INTERACT_VAR_H
#define INTERACT_VAR_H
#include "render.h"

const int left_pressure_needle_index = 40;
const int center_pressure_needle_index = 44;
const int right_pressure_needle_index = 48;


void update_on_switch_click(SpritePositionData* data);
void rotate_needles(std::vector<sf::Sprite>& sprite);
void assign_game_axis_values();
void connect_actuator_to_system();

#endif
