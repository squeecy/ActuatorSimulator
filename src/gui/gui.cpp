#include <iostream>
#include <string>
#include <sstream>
#include "gui/gui.h"
#include "simvar.h"

void GUI::update_sim_var_values()
{
    switch_var_values = {
        fe_eng_1_hyd_pump_1_switch_master.current_value,
        fe_eng_1_hyd_pump_2_switch_master.current_value,
        fe_eng_2_hyd_pump_1_switch_master.current_value,
        fe_eng_2_hyd_pump_2_switch_master.current_value,
        fe_eng_3_hyd_pump_1_switch_master.current_value,
        fe_eng_3_hyd_pump_2_switch_master.current_value,
        fe_13_hydro_switch_master.current_value,
        fe_23_hydro_switch_master.current_value 
    };

    hydro_var_values = {
        fe_hydro_pressure_1_master.current_value,
        fe_hydro_pressure_2_master.current_value,
        fe_hydro_pressure_3_master.current_value,

        fe_eng_pump_pressure_1_1_master.current_value,
        fe_eng_pump_pressure_1_2_master.current_value,
        fe_eng_pump_pressure_2_1_master.current_value,
        fe_eng_pump_pressure_2_2_master.current_value,
        fe_eng_pump_pressure_3_1_master.current_value,
        fe_eng_pump_pressure_3_2_master.current_value,

        fe_line_hydro_pressure_1_1_master.current_value,
        fe_line_hydro_pressure_1_2_master.current_value,
        fe_line_hydro_pressure_2_1_master.current_value,
        fe_line_hydro_pressure_2_2_master.current_value,
        fe_line_hydro_pressure_3_1_master.current_value,
        fe_line_hydro_pressure_3_2_master.current_value,

        fe_hydro_flow_1_master.current_value,
        fe_hydro_flow_2_master.current_value,
        fe_hydro_flow_3_master.current_value,

        fe_eng_pump_flow_1_1_master.current_value,
        fe_eng_pump_flow_1_2_master.current_value,
        fe_eng_pump_flow_2_1_master.current_value,
        fe_eng_pump_flow_2_2_master.current_value,
        fe_eng_pump_flow_3_1_master.current_value,
        fe_eng_pump_flow_3_2_master.current_value,

        fe_rmp_13_operation_master.current_value,
        fe_rmp_23_operation_master.current_value,
    };

    engine_var_values = {
        normal_n2_1_master.current_value,
        normal_n2_2_master.current_value,
        normal_n2_3_master.current_value,
    };


}


const char* dropdown_items[] = { "Switches", "Hydro", "Engines" };
int dropdown_items_count = sizeof(dropdown_items) / sizeof(dropdown_items[0]);


void GUI::list_with_text(std::vector<std::string> list_names, std::vector<double> sim_var_value)
{
    char sim_var_str[32];
    for(int i = 0; i < list_names.size(); i++)
    {
        floatToString(sim_var_value[i], sim_var_str, sizeof(sim_var_str));
        if(i % 2 == 0)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Lighter shade
        }else{
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f)); // Darker shade
        }

        ImGui::Text("%s", list_names[i].c_str());
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::InputText("##text1", sim_var_str, sizeof(sim_var_str));


    }

}

void GUI::gui_var_values()
{
    update_sim_var_values();
    ImGui::Begin("Sim Var Watcher");
    static int selected_index = 0;


    ImGui::Combo(" ", &selected_index, dropdown_items, dropdown_items_count);

    switch (selected_index)
    {
    case 0:
        GUI::list_with_text(switch_vars, switch_var_values);
        break;
    case 1:
        GUI::list_with_text(hydro_vars, hydro_var_values);
        break;
    case 2:
        GUI::list_with_text(engine_vars, engine_var_values);
        break;
    
    default:
        break;
    }



    ImGui::End();
}