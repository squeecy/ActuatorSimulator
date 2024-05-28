#ifndef GUI__H
#define GUI__H

// #include "imgui/imgui.h"
// #include "imgui/imgui-SFML.h"
#include <vector>
#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

static void floatToString(double value, char* buffer, size_t buffer_size) {
    std::stringstream ss;
    ss << value;
    std::string str = ss.str();
    strncpy(buffer, str.c_str(), buffer_size - 1);
    buffer[buffer_size - 1] = '\0'; // Ensure null-termination
}

const std::vector<std::string> switch_vars = {
    "eng_1_hyd_pump_1_switch",
    "eng_1_hyd_pump_2_switch",
    "eng_2_hyd_pump_1_switch",
    "eng_2_hyd_pump_2_switch",
    "eng_3_hyd_pump_1_switch",
    "eng_3_hyd_pump_2_switch",
    "fe_13_hyd_rmp_switch",
    "fe_23_hyd_rmp_switch",

};

const std::vector<std::string> hydro_vars = {
    "system_1_pressure",
    "system_2_pressure",
    "system_3_pressure",

    "fe_eng_pump_1_1_pressure",
    "fe_eng_pump_1_2_pressure",
    "fe_eng_pump_2_1_pressure",
    "fe_eng_pump_2_2_pressure",
    "fe_eng_pump_3_1_pressure",
    "fe_eng_pump_3_2_pressure",

    "fe_hydro_line_1_1_pressure",
    "fe_hydro_line_1_2_pressure",
    "fe_hydro_line_2_1_pressure",
    "fe_hydro_line_2_2_pressure",
    "fe_hydro_line_3_1_pressure",
    "fe_hydro_line_3_2_pressure",

    "fe_hydro_1_flow",
    "fe_hydro_2_flow",
    "fe_hydro_3_flow",

    "fe_eng_pump_1_1_flow",
    "fe_eng_pump_1_2_flow",
    "fe_eng_pump_2_1_flow",
    "fe_eng_pump_2_2_flow",
    "fe_eng_pump_3_1_flow",
    "fe_eng_pump_3_2_flow",
    "fe_rmp_13_operation",
    "fe_rmp_23_operation",

};

const std::vector<std::string> engine_vars = {
    "engine_1_n2",
    "engine_2_n2",
    "engine_3_n2",
};


class GUI
{
    public:
        
        void update_sim_var_values();
        void list_with_text(const std::vector<std::string> list_names, std::vector<double> sim_var_value);
        void gui_var_values();
    private:
        std::vector<double> switch_var_values;
        std::vector<double> hydro_var_values;
        std::vector<double> engine_var_values;


};

#endif