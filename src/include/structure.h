#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <SFML/Graphics.hpp>

static sf::RectangleShape system_1_needle(sf::Vector2f(5.f, 65.f));
static sf::RectangleShape system_2_needle(sf::Vector2f(5.f, 65.f));
static sf::RectangleShape system_3_needle(sf::Vector2f(5.f, 65.f));

static float system_1_needle_angle = 0.f;
static float system_2_needle_angle = 0.f;
static float system_3_needle_angle = 0.f;

//void draw_window_guage(sf::RenderWindow& window, const sf::Drawable& write);
//void draw_window_guage(sf::RenderWindow& window, const std::vector<sf::Drawable*>& drawables);
void draw_window_guage(sf::RenderWindow& window, 
                       const std::vector<sf::Drawable*>& drawables_static, 
                       const std::vector<sf::Drawable*>& drawables_dynamic);
double inner_lerp(double x, double x1, double y1, double x2, double y2);
float set_needle_guage_interp(double set_point, double x1, double y1, double x2, double y2);
bool click_spot_circle(float mouseX, float mouseY, float centerX, float centerY, float radius);
void handle_switch_position(int delta, int& switch_pos, int max_pos, int min_pos);

#endif
