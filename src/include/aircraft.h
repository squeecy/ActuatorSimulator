#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#ifndef KEY_ID_MIN
    #define KEY_ID_MIN 0x00010000
#endif

namespace MsfsAeroFramework {
    enum SimVarType {
        NORMAL = 0,
        LOCAL = 1
    };

    class SimVar {
    private:
        double read_value = -6.42302; //some random value
    public:
        SimVar() : name(""), type(SimVarType::LOCAL), unit("") {} // Default constructor
        std::string name;
        SimVarType type;
        std::string unit;
        int index;
        double current_value;
        double initialization_value;
        std::string var_str;
        std::string rpl_str;
        SimVar(std::string name, SimVarType type, std::string unit, int index, double initialization_value) :
            name(name),
            type(type),
            unit(unit),
            index(index),
            initialization_value(initialization_value),
            current_value(initialization_value)
        {
            var_str = (this->type == SimVarType::NORMAL ? "A:" : "L:") + this->name + (this->index == -1 ? "" : (":" + std::to_string(this->index)));
            rpl_str = "(" + var_str + ", " + this->unit + ")";
        }
        SimVar(std::string name, SimVarType type, double initialization_value) :
            SimVar(name, type, "number", -1, initialization_value) {}
        SimVar(std::string name, SimVarType type, int index, double initialization_value) :
            SimVar(name, type, "number", index, initialization_value) {}
        SimVar(std::string name, SimVarType type) :
            SimVar(name, type, "number", -1, 0.0) {}
        SimVar(std::string name, SimVarType type, std::string unit) :
            SimVar(name, type, unit, -1, 0.0) {}
        SimVar(std::string name, SimVarType type, std::string unit, int index) :
            SimVar(name, type, unit, index, 0.0) {}
        SimVar(std::string name, SimVarType type, int index) :
            SimVar(name, type, "number", index, 0.0) {}
        std::string to_var_string();
        //double tester();
        //std::string to_rpl_calculator_string();
        double operator+(SimVar const& obj) {
            return this->current_value + obj.current_value;
        }
        double operator-(SimVar const& obj) {
            return this->current_value - obj.current_value;
        }
        double operator*(SimVar const& obj) {
            return this->current_value * obj.current_value;
        }
        double operator/(SimVar const& obj) {
            return this->current_value / obj.current_value;
        }
        double get_read_value() {
            return this->read_value;
        }

        void set_read_value(double val) {
            this->read_value = val;
        }
    };

    class KeyEvent {
        public:
            int key_id;
            int p1;
            int p2;
            int p3;
            int p4;
            int p5;
            int prev_p1;
            int prev_p2;
            int prev_p3;
            int prev_p4;
            int prev_p5;
            int first_call = 0;
            KeyEvent(int key_id) {
                this->key_id = key_id;
                this->p1 = 0;
                this->p2 = 0;
                this->p3 = 0;
                this->p4 = 0;
                this->p5 = 0;
            }
    };

    class SimContext {
    public:
        const std::string prefix;
        std::unordered_map<std::string, double> local_context;
        SimContext(const std::string prefix) : prefix(prefix) {}
        virtual double read(SimVar& var) = 0;
        double read(std::string name, SimVarType type, std::string unit) {
            SimVar sv = SimVar(
                name,
                type,
                unit
            );
            double return_val = this->read(sv);
            sv.current_value = return_val;
            return return_val;
        }
        std::pair<bool, double> local_context_read(std::string var);
        virtual void write(SimVar& var, double value) = 0;
        void write(SimVar& var) {
            this->write(var, var.current_value);
        }
        void local_context_write(std::string var, double val);
        virtual void trigger_key_event(KeyEvent& key_event) = 0;
    };

    class System {
    public:
        std::string name;
        bool perform_post_update = false;
        virtual void update(SimContext* sim_context, double delta_time) = 0;
        void post_update(SimContext* sim_context, double delta_time) {
            //Implementation of a post_update() function is optional
            return;
        }
        System(std::string system_name) : name(system_name) {}
    };

    class Aircraft {
    public:
        std::string aircraft_type_icao;
        std::vector<System*> aircraft_systems;
        Aircraft(std::string aircraft_type_icao) : aircraft_type_icao(aircraft_type_icao), aircraft_systems({}) {};
        void update(SimContext* sim_context, double delta_time);
        void post_update(SimContext* sim_context, double delta_time);
        void add_system(System* system);
    };

    class KC10 : public Aircraft {
    public:
        KC10() : Aircraft("KC10") {};
    };

    class DummySystem : public System {
    public:
        DummySystem(std::string name) : System(name) {}
        void update(SimContext* sim_context, double delta_time) override { return; }
    };

}  // namespace

#endif