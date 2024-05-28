#ifndef USER__H
#define USER__H

#include <string>

//Prompts user for configuring texutre/text/sprite information
void handle_render_config();

//adds texture/text/sprite user input data to text file
void add_render_data(std::string type, std::string name_data, 
std::string x_location_data, 
std::string y_location_data, 
std::string text_size_data = "");

//prints contants of text file 
void write_text_file(std::string type, unsigned int skipper, unsigned int denominator,
bool is_line_known, int known_value = 0);

#endif