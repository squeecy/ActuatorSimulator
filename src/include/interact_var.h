#ifndef INTERACT_VAR_H
#define INTERACT_VAR_H
#include "render.h"

const int left_pressure_needle_index = 32;
const int center_pressure_needle_index = 36;
const int right_pressure_needle_index = 40;


void update_on_switch_click(SpritePositionData* data);
void rotate_needles(std::vector<sf::Sprite>& sprite);

#endif