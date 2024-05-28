#include <iostream>
#include <string>
#include "render.h"

std::vector<sf::Drawable*> Render::draw_textures;

//rotate needle sprite
void rotate_sprite(std::vector<sf::Sprite>& sprite, double value, int index)
{
    double angle = valueToAngle(value);
    sprite[index].setRotation(angle);
}

//reads sprite/texture vector and initializes to for draw
void Render::init_sprite(std::vector<sf::Texture>& texture, std::vector<sf::Sprite>& sprite, const std::vector<std::string> name, float x, float y)
{
    //for(int i = 0; i < texture_library.size(); i+=4)
    for(int i = texture_library.size() - 4; i >= 0; i-=4)
    {
        if(!texture[i].loadFromFile("res/" + name[i] + ".png"))
            std::cout << "Error couldnt load" << name[i] << "image" << std::endl;

        sprite[i].setTexture(texture[i]);
        sf::Vector2u textureSize = texture[i].getSize();
        if(i == 32 || i == 36 || i == 40)
            sprite[i].setOrigin(5, 0);
            //sprite[i].setOrigin(textureSize.x + 20 , 0);
        sprite[i].setPosition(std::stof(name[i+1]), std::stof(name[i+2]));


        Render::draw_textures.push_back(&sprite[i]);

        if(i == 0 || i == 4 || i == 8)
            continue;
        //else
            //spritePositions[i] = -1;
        }



    
    texture_library.clear();

}

void Render::init_text(std::vector<sf::Text>& text, const std::vector<std::string> name)
{
    std::string font_name;
    //const std::string font_name = "res/fonts/RobotoMono-Thin.ttf";

    font_name = "res/fonts/RobotoMono-Thin.ttf";

    if(!font.loadFromFile(font_name))
        std::cout << "Error loading " << font_name << " file" << std::endl;

    for(int i = 0; i < text_library.size(); i+=4)
    {
        //if(text_library[i+1] != "default")
            //font_name = text_library[i+1];

        text[i].setFont(font);
        //text[i].setPosition(std::stof(name[i+1]), std::stof(name[i+2]));
        text[i].setPosition(250.0f, 250.0f);
        text[i].setCharacterSize(std::stoi(name[i+3]));
        std::cout << name[i+3] << std::endl;

        
    }

    test_text.setFont(font);
    test_text.setCharacterSize(22);
}

//check if a sprite has been clicked
void Render::handle_click(sf::RenderWindow& window)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                std::cout << "Left Click At" << mousePos.x << ", " << mousePos.y << std::endl;
            }
            else if(event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                std::cout << "Right Click At" << mousePos.x << ", " << mousePos.y << std::endl;
            }
        }
    }
}

//do something about the sprites being clicked
void Render::handle_sprite_clickspots(SpritePositionData* data , std::vector<sf::Sprite>& sprite, sf::RenderWindow& window, int direction)
{
    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

    for (int i = 0; i < sprite.size(); i+=4)
    {
        // Get global bounds of sprite
        sf::FloatRect bounds = sprite[i].getGlobalBounds();

        // Check if mouse position is inside sprite bounds
        if (bounds.contains(mousePos) )
        {
            //if(i == 0 || i == 4 || i == 8)
            //if(i == 32 || i == 36 || i == 40)
                //continue;

            //spritePositions[i] += direction;
            data->spritePositions[i] += direction;

            //if(spritePositions[i] >= 1)
                //spritePositions[i] = 1;
            if(data->spritePositions[i] >= 1)
                data->spritePositions[i] = 1;

            if(i == 0 || i == 4 || i == 8)
            {
                //if(spritePositions[i] < 0)
                    //spritePositions[i] = 0;
                if(data->spritePositions[i] < 0)
                    data->spritePositions[i] = 0;
            }else{
               //if(spritePositions[i] < -1)
                    //spritePositions[i] = -1;
                if(data->spritePositions[i] < -1)
                    data->spritePositions[i] = -1;

            }


            // Handle click inside sprite bounds
            //std::cout << "Clicked on sprite " << i << std::endl;
            //std::cout << spritePositions[i] << std::endl;
            //std::cout << data->spritePositions[i] << std::endl;
            //std::cout << i << std::endl;
        }
    }
}

//swap textures after checking switch position
void Render::handle_texture_swap(SpritePositionData* data, std::vector<sf::Sprite>& sprite, std::vector<sf::Texture>& texture)
{
    for (int i = 0; i < sprite.size()- 15; ++i)
    {
        //if(i == 32 || i == 36 || i == 40)
            //continue;

        //switch (spritePositions[i])
        switch (data->spritePositions[i])
        {
        case 1:
            texture[i].loadFromFile("res/hydro_panel/main_switch_auto.png");
            
            sprite[i].setTexture(texture[i]);
            break;
        case 0:
            if(i == 0 || i == 4 || i == 8)
                texture[i].loadFromFile("res/hydro_panel/main_switch_off.png");
            else
                texture[i].loadFromFile("res/hydro_panel/main_switch_on.png");
            sprite[i].setTexture(texture[i]);
            break;
        case -1:
            texture[i].loadFromFile("res/hydro_panel/main_switch_off.png");
            
            sprite[i].setTexture(texture[i]);
            break;

        
        default:
            break;
        }
    }

}











