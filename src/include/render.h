#ifndef RENDER__H
#define RENDER__H

#include <SFML/Graphics.hpp>
#include <utility>
#include "interp.h"

struct SpritePositionData 
{

    int spritePositions[10] = {0, 
                             -1, 
                              0, 
                              -1, 
                              0 ,
                             -1,
                              0, 
                              0, 
                              0,
                              0,
                               };
};

static sf::Text test_text;
static sf::Font font;

/*texture library format
    0: name of texture .png
    1: x pos of sprite
    2: y pos of sprite
*/
static std::vector<std::string> texture_library = {
    "/hydro_panel/main_switch_off", //0
    "1070.0",//1
    "500.0",//2
    "FE_ENG1HYDPUMP1",//3
    "/hydro_panel/main_switch_off",//12
    "1160.0",//13
    "500.0",//14
    "FE_ENG1HYDPUMP2",//15
    "/hydro_panel/main_switch_off",//4
    "1350.0",//5
    "500.0",//6
    "FE_ENG2HYDPUMP1",//7
    "/hydro_panel/main_switch_off",//16
    "1440.0",//17
    "500.0",//18
    "FE_ENG2HYDPUMP2",//19
    "/hydro_panel/main_switch_off",//8
    "1625.0",//9
    "500.0",//10
    "FE_ENG3HYDPUMP1",//11
    "/hydro_panel/main_switch_off",//20
    "1715.0",//21
    "500.0",//22
    "FE_ENG3HYDPUMP2",//23
    "/hydro_panel/main_switch_off",//24
    "1240.0",//25
    "185.0",//26
    " ",//27
    "/hydro_panel/main_switch_off",//28
    "1490.0",//29
    "260.0",//30
    " ",//31
    "/hydro_panel/aux_switch_norm",//36
    "1471.0",//37
    "675.0",//38
    " ",//39
    "/hydro_panel/aux_switch_norm",//32
    "1471.0",//33
    "518.0",//34
    " ",//35
    "/hydro_panel/needle",//40
    "1150.0",//41
    "115.0",//42
    " ",//43
    "/hydro_panel/needle",//44
    "1420.0",//45
    "115.0",//46
    " ",//47
    "/hydro_panel/needle",//48
    "1705.0",//49
    "115.0",//50
    " ",//51
    "actuator", //52
    "0.0",//53
    "-200.0",//54
    "HYDRO_ACTUATOR",//55
    "/hydro_panel/hydro_panel", //56
    "940.0",//57
    "0.0",//58
    " ",//59

};
static int texture_swap_difference = texture_library.size() - 40;

extern std::vector<std::string> updating_texture_library;



static std::vector<std::string> text_library = {
   "sys_2_p_text",
   "100.0",
   "750.0",
   "22"

};


static std::vector<sf::Text> text_class_vec(text_library.size()); 

static std::vector<sf::Texture> texture_class_vec(texture_library.size());
static std::vector<sf::Sprite> sprite_class_vec(texture_library.size());

static std::vector<sf::Texture> update_texture_class_vec(updating_texture_library.size());
static std::vector<sf::Sprite> update_sprite_class_vec(updating_texture_library.size());
static std::vector<int>spritePositions(30, 0);

static float valueToAngle(float value) {
    return lerp(value * 100, 0, 0, 3000, 180);
}
void rotate_sprite(std::vector<sf::Sprite>& sprite, double value, int index);


class Render
{
    public:
        
        static std::vector<sf::Drawable*> draw_textures;
        static std::vector<sf::Drawable*> draw_textures_u;

        /*inits drawing of a sprite to the screen*/
        void init_sprite(std::vector<sf::Texture>& texture, 
                         std::vector<sf::Sprite>& sprite, 
                         const std::vector<std::string> name, float x = 0.0f, float y = 0.0f);
  
        void update_sprite(std::vector<sf::Texture>& texture, 
                           std::vector<sf::Sprite>& sprite, 
                           const std::vector<std::string> name, float x = 0.0f, float y = 0.0f);
  
        void init_text(std::vector<sf::Text>& text, const std::vector<std::string> name);
        void handle_click(sf::RenderWindow& window);
        void handle_sprite_clickspots(SpritePositionData* data , std::vector<sf::Sprite>& sprite, sf::RenderWindow& window, int direction);
        void handle_texture_swap(SpritePositionData* data, std::vector<sf::Sprite>& sprite, std::vector<sf::Texture>& texture);

};


#endif
