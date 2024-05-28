#include <iostream>
#include <fstream>
#include <string>
#include "user.h"

void add_render_data(std::string type, std::string name_data, 
std::string x_location_data, 
std::string y_location_data, 
std::string text_size_data)
{
    std::ofstream outputFile("res/data/"+type+"_data.txt", std::ios::app);

    if(!outputFile.is_open())
    {
        std::cerr << "Error opening render_data_file.txt" << std::endl;
    }

    if(text_size_data.empty())
    {
        outputFile << name_data << "\n"
                << x_location_data << "\n"
                << y_location_data << "\n";
    }else{

        outputFile << name_data << "\n"
                << x_location_data << "\n"
                << y_location_data << "\n"
                << text_size_data << "\n";
    }

    outputFile.close();
}

 void write_text_file(std::string type, unsigned int skipper, unsigned int denominator, 
 bool is_line_known, int known_value)
 {
    std::ifstream file("res/data/"+type+"_data.txt");
    std::string line;

    int line_count = 0;
    int place_count = 0;

    if(file.is_open())
    {
        while(getline(file, line))
        {
            line_count++;
            
            if(!is_line_known)
            {
                if(line_count % skipper == denominator)
                {
                    place_count++;
                    std::cout << place_count << " " << line << std::endl;
                }
            }else{
                if(line_count == known_value || line_count == known_value+1 || line_count == known_value+2 )
                {
                    std::cout << line << std::endl;
                }
            }
        }
        file.close();
    }else{
        std::cerr << "Unable to use " + type + " file" << std::endl;
    }


    
 }

void handle_render_config()
{
    int main_action, add_option, edit_option, edit_texture_source_option;
    int edit_line_number;
    std::string user_texture_name, user_text_name; //Name
    std::string user_texture_x_location, user_texture_y_location; // Texture location
    std::string user_text_x_location, user_text_y_location, user_text_size; //Text location




    std::cout << "1.Add" << std::endl;
    std::cout << "2.Edit" << std::endl;
    std::cout << "3.Delete" << std::endl;

    std::cin >> main_action;

    switch(main_action)
    {
        case 1:
            std::cout << "1. Add Texture" << std::endl;
            std::cout << "2. Add Text" << std::endl;
            std::cin >> add_option;
            switch(add_option)
            {
                case 1:
                    std::cout << "Enter Image File Path" << std::endl;
                    std::cin >> user_texture_name;
                    std::cout << "Enter X Location" << std::endl;
                    std::cin >> user_texture_x_location;
                    std::cout << "Enter Y Location" << std::endl;
                    std::cin >> user_texture_y_location;
                    add_render_data("render", user_texture_name, user_texture_x_location,
                    user_texture_y_location);
                    break;
                case 2:
                    std::cout << "Enter Text Name" << std::endl;
                    std::cin >> user_text_name;
                    std::cout << "Enter X Location" << std::endl;
                    std::cin >> user_text_x_location;
                    std::cout << "Enter Y Location" << std::endl;
                    std::cin >> user_text_y_location;
                    std::cout << "Enter Text Size" << std::endl;
                    std::cin >> user_text_size;
                    add_render_data("text", user_text_name, user_text_x_location,
                    user_text_y_location, user_text_size);
                    break;
                default:
                    break;
            }
            

            break;
        case 2:
            std::cout << "1. Edit Texture" << std::endl;
            std::cout << "2. Edit Text" << std::endl;
            std::cin >> edit_option;
            switch(edit_option)
            {
                case 1:
                    std::cout << "Select which source to edit" << std::endl;
                    write_text_file("render", 3, 1, false);
                    std::cin >> edit_texture_source_option;

                    edit_line_number = (edit_texture_source_option* 2) + (edit_texture_source_option -2);
                    std::cout << "under" << std::endl;
                    write_text_file("render", 3, 1, true, edit_line_number);


                case 2:
                    write_text_file("text", 3, 1, false);
            }
            break;
        case 3:
            std::cout << "init" << std::endl;
            break;
        default:
            std::cout << "init" << std::endl;
            break;

        
    }
}

