#include <structure.h>
#include <iostream>
#include <cmath>

//linear interpolation
double inner_lerp(double x, double x1, double y1, double x2, double y2)
{
	return (y1 + (x - x1) * ((y2 - y1) / (x2 - x1)));
}

//draw objects to window in vector form
void draw_window_guage(sf::RenderWindow& window, 
                       const std::vector<sf::Drawable*>& drawables_static, 
                       const std::vector<sf::Drawable*>& drawables_dynamic)
{
	for(const auto& drawable: drawables_static)
	{
		window.draw(*drawable);
	}

	for(const auto& drawable: drawables_dynamic)
	{
		window.draw(*drawable);
	}
}

float set_needle_guage_interp(double set_point, double x1, double y1, double x2, double y2)
{
	return inner_lerp(set_point, x1, y1, x2, y2);
}

bool click_spot_circle(float mouseX, float mouseY, float centerX, float centerY, float radius)
{
	float distance = std::sqrt(std::pow(mouseX - centerX, 2) + std::pow(mouseY - centerY, 2));

	return distance <= radius;
}


void handle_switch_position(int delta, int& switch_pos, int max_pos, int min_pos)
{   
	if(delta < 0)
	{
		switch_pos -= 1;
	}else if(delta > 0)
	{
		switch_pos += 1;
	}

	if(switch_pos > max_pos)
		switch_pos = max_pos;
	if(switch_pos < min_pos)
		switch_pos = min_pos;
}

