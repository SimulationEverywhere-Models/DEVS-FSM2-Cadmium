//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

#include "../atomics/moore.h"
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs


#include <iostream>
#include <NDTime.hpp>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// Input ports
struct input_port : public cadmium::in_port<Message_t>{};

// output ports
struct reader_output : public iestream_input_defs<Message_t>::out{};
struct output_port : public cadmium::out_port<Message_t>{};
struct TOP_output_port : public cadmium::out_port<Message_t>{};


/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_Message_t : public iestream_input<Message_t,T> {
    public:
        InputReader_Message_t () = default;
        InputReader_Message_t (const char* file_path) : iestream_input<Message_t,T>(file_path) {}
};


int main(){

 /****** Input Readers atomic model instantiation *******************/
    const char * i_input_data_control = "../input_data/main_serial_adder.txt";
    shared_ptr<dynamic::modeling::model> input_reader = dynamic::translate::make_dynamic_atomic_model<InputReader_Message_t, TIME, const char* >("input_reader" , std::move(i_input_data_control));

/****** Input reader Model ********/
dynamic::modeling::Ports iports_reader = {};
dynamic::modeling::Ports oports_reader = {typeid(reader_output)};
dynamic::modeling::Models submodels_reader = {input_reader};
dynamic::modeling::EICs eics_reader = {};
dynamic::modeling::EOCs eocs_reader = {
dynamic::translate::make_EOC<iestream_input_defs<Message_t>::out,reader_output>("input_reader")};
dynamic::modeling::ICs ics_reader = {};

shared_ptr<dynamic::modeling::coupled<TIME>> reader;
    reader = make_shared<dynamic::modeling::coupled<TIME>>(
        "reader", submodels_reader, iports_reader, oports_reader, eics_reader, eocs_reader, ics_reader 
    );





 /****** Moore atomic model instantiation *******************/
  shared_ptr<dynamic::modeling::model> state0 = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("state0", true,0,0,vector<pair<int,int>>{make_pair(0,0),make_pair(1,1),make_pair(2,2)});
  shared_ptr<dynamic::modeling::model> state1 = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("state1", false,1,1,vector<pair<int,int>>{make_pair(0,0),make_pair(1,1),make_pair(2,2)});
  shared_ptr<dynamic::modeling::model> state2 = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("state2", false,2,0,vector<pair<int,int>>{make_pair(0,1),make_pair(1,2),make_pair(2,3)}); // 0 with carry bit
  shared_ptr<dynamic::modeling::model> state3 = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("state3", false,3,1,vector<pair<int,int>>{make_pair(0,1),make_pair(1,2),make_pair(2,3)}); // 1 with carry bit
   

/*******serial_adder MODEL********/
    dynamic::modeling::Ports iports_serial_adder = {typeid(input_port)};
    dynamic::modeling::Ports oports_serial_adder = {typeid(output_port)};
    dynamic::modeling::Models submodels_serial_adder = { state0 ,state1, state2, state3};
    dynamic::modeling::EICs eics_serial_adder = { 
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("state0"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("state1"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("state2"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("state3"),
};
    dynamic::modeling::EOCs eocs_serial_adder = {
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("state0"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("state0"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("state1"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("state1"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("state2"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("state2"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("state3"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("state3"),
    };
    dynamic::modeling::ICs ics_serial_adder = {
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state0","state0"), 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state0","state1"),  
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state0","state2"),  
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state1","state1"), 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state1","state0"), 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state1","state2"),
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state2","state2"),   
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state2","state1"), 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state2","state3"),  
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state3","state3"),
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state3","state1"),  
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("state3","state2"),  
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> serial_adder;
    serial_adder = make_shared<dynamic::modeling::coupled<TIME>>(
        "serial_adder", submodels_serial_adder, iports_serial_adder, oports_serial_adder, eics_serial_adder, eocs_serial_adder, ics_serial_adder 
    );

    /*************** TOP *******************/

dynamic::modeling::Ports iports_TOP = {};
dynamic::modeling::Ports oports_TOP = {typeid(TOP_output_port)};
dynamic::modeling::Models submodels_TOP = {reader, serial_adder};
dynamic::modeling::EICs eics_TOP = {}; // no external inputs we get inputs from reader
dynamic::modeling::EOCs eocs_TOP = {
dynamic::translate::make_EOC<output_port,TOP_output_port>("serial_adder")};
dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<reader_output,input_port>("reader","serial_adder"), // reader to serial_adder interconnect        
};

shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );


    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/COUPLED_serial_adder_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/COUPLED_serial_adder_state.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };

    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/ 
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("00:00:00:150"));
    return 0;

}