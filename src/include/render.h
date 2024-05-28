#ifndef RENDER__H
#define RENDER__H

#include <SFML/Graphics.hpp>
#include <utility>
#include "interp.h"

struct SpritePositionData 
{
    // int spritePositions[21] = {0, 0, 0, 
    //                            0, 0 ,0,
    //                            0 ,0 ,0,
    //                            -1, -1, -1
    //                            -1, -1 ,1
    //                            -1, -1, -1,
    //                            -1, -1, -1, 
    //                            -1, -1 };

    int spritePositions[26] = {0, 0, 0, 
                               0, 0 ,0,
                               0 ,0 ,0,
                               -1, -1, -1
                               -1, -1 ,1
                               -1, -1, -1,
                               -1, -1, -1, 
                               -1, -1, -1,
                               -1, -1, 0,
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
    "/hydro_panel/main_switch_off",//4
    "1350.0",//5
    "500.0",//6
    "FE_ENG2HYDPUMP1",//7
    "/hydro_panel/main_switch_off",//8
    "1625.0",//9
    "500.0",//10
    "FE_ENG3HYDPUMP1",//11
    "/hydro_panel/main_switch_off",//12
    "1160.0",//13
    "500.0",//14
    "FE_ENG1HYDPUMP2",//15
    "/hydro_panel/main_switch_off",//16
    "1440.0",//17
    "500.0",//18
    "FE_ENG2HYDPUMP2",//19
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
    "/hydro_panel/needle",//32
    "1150.0",//33
    "115.0",//34
    " ",//35
    "/hydro_panel/needle",//36
    "1420.0",//37
    "115.0",//38
    " ",//39
    "/hydro_panel/needle",//40
    "1705.0",//41
    "115.0",//42
    " ",//43
    "actuator", //44
    "0.0",//45
    "-200.0",//46
    "HYDRO_ACTUATOR",//47
    "actuator_rod", //48
    "0.0",//49
    "-200.0",//50
    " ",//51
    "/hydro_panel/hydro_panel", //52
    "940.0",//53
    "0.0",//54
    " ",//55

};

/*
static std::vector<std::string> texture_library = {
    "actuator", //0
    "0.0",
    "-200.0",
    "HYDRO_ACTUATOR",
    "actuator_rod", //3
    "0.0",
    "-200.0",
    " ",
    "/hydro_panel/hydro_panel", //6
    "940.0",
    "0.0",
    " ",
    //engine 1
    "/hydro_panel/main_switch_off", //9*
    "1070.0",
    "500.0",
    "FE_ENG1HYDPUMP1",
    //engine 2 
    "/hydro_panel/main_switch_off",//*
    "1350.0",
    "500.0",
    "FE_ENG2HYDPUMP1",
    //engine 3 
    "/hydro_panel/main_switch_off",//*
    "1625.0",
    "500.0",
    "FE_ENG3HYDPUMP1",
    // engine 1 switch 2
    "/hydro_panel/main_switch_off",
    "1160.0",
    "500.0",
    "FE_ENG1HYDPUMP2",
    // engine 2 switch 2
    "/hydro_panel/main_switch_off",
    "1440.0",
    "500.0",
    "FE_ENG2HYDPUMP2",
    // engine 3 switch 2
    "/hydro_panel/main_switch_off",
    "1715.0",
    "500.0",
    "FE_ENG3HYDPUMP2",
    "/hydro_panel/main_switch_on",
    "0.0",
    "0.0",
    " ",
    "/hydro_panel/main_switch_auto",
    "0.0",
    "0.0",
    " ",

};
*/


static std::vector<std::string> text_library = {
   "sys_2_p_text",
   "100.0",
   "750.0",
   "22"

};


static std::vector<sf::Text> text_class_vec(text_library.size()); 

static std::vector<sf::Texture> texture_class_vec(texture_library.size());
static std::vector<sf::Sprite> sprite_class_vec(texture_library.size());
static std::vector<int>spritePositions(30, 0);

static float valueToAngle(float value) {
    return lerp(value * 100, 0, 0, 3000, 180);
}
void rotate_sprite(std::vector<sf::Sprite>& sprite, double value, int index);


class Render
{
    public:
        
        static std::vector<sf::Drawable*> draw_textures;

        /*inits drawing of a sprite to the screen*/
        void init_sprite(std::vector<sf::Texture>& texture, std::vector<sf::Sprite>& sprite, const std::vector<std::string> name, float x = 0.0f, float y = 0.0f);
        void init_text(std::vector<sf::Text>& text, const std::vector<std::string> name);
        void handle_click(sf::RenderWindow& window);
        void handle_sprite_clickspots(SpritePositionData* data , std::vector<sf::Sprite>& sprite, sf::RenderWindow& window, int direction);
        void handle_texture_swap(SpritePositionData* data, std::vector<sf::Sprite>& sprite, std::vector<sf::Texture>& texture);

};


#endif