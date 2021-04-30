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
    const char * i_input_data_control = "../input_data/main_telephone.txt";
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
  shared_ptr<dynamic::modeling::model> idle = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("idle", true,0,0,vector<pair<int,int>>{make_pair(0,1),make_pair(5,3)});
  shared_ptr<dynamic::modeling::model> gettingnumber = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("gettingnumber", false,1,1,vector<pair<int,int>>{make_pair(1,2),make_pair(4,0),make_pair(6,5)});
  shared_ptr<dynamic::modeling::model> ringingaside = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("ringingaside", false,2,2,vector<pair<int,int>>{make_pair(4,0),make_pair(2,4)});
  shared_ptr<dynamic::modeling::model> ringingbside = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("ringingbside", false,3,3,vector<pair<int,int>>{make_pair(4,0),make_pair(2,4)});
  shared_ptr<dynamic::modeling::model> speech = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("speech", false,4,4,vector<pair<int,int>>{make_pair(4,0),make_pair(3,5)});
  shared_ptr<dynamic::modeling::model> waitonhook = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("waitonhook", false,5,5,vector<pair<int,int>>{make_pair(4,0)});
   

/*******TELEPHONE MODEL********/
    dynamic::modeling::Ports iports_TELEPHONE = {typeid(input_port)};
    dynamic::modeling::Ports oports_TELEPHONE = {typeid(output_port)};
    dynamic::modeling::Models submodels_TELEPHONE = { idle ,gettingnumber, ringingaside, ringingbside , speech, waitonhook};
    dynamic::modeling::EICs eics_TELEPHONE = { 
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("idle"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("gettingnumber"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("ringingaside"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("ringingbside"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("speech"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("waitonhook"),
};
    dynamic::modeling::EOCs eocs_TELEPHONE = {
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("idle"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("idle"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("gettingnumber"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("gettingnumber"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("ringingaside"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("ringingaside"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("ringingbside"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("ringingbside"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("speech"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("speech"),                
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("waitonhook"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("waitonhook"),
    };
    dynamic::modeling::ICs ics_TELEPHONE = {
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("idle","gettingnumber"), // idle to getting/dialing number
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("idle","ringingbside"), // idle to ringing other side(b)
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("gettingnumber","idle"),  // gettingnumber to disconnecting or idle
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("gettingnumber","ringingaside"), 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("gettingnumber","waitonhook"), // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("ringingaside","idle"),
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("ringingaside","speech"),  // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("ringingbside","idle"), //
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("ringingbside","speech"),  // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("speech","idle"),// speech to disconnect
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("speech","waitonhook"),  // place on hool
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("waitonhook","idle"),  //  disconnect from waitonhook
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TELEPHONE;
    TELEPHONE = make_shared<dynamic::modeling::coupled<TIME>>(
        "TELEPHONE", submodels_TELEPHONE, iports_TELEPHONE, oports_TELEPHONE, eics_TELEPHONE, eocs_TELEPHONE, ics_TELEPHONE 
    );

    /*************** TOP *******************/

dynamic::modeling::Ports iports_TOP = {};
dynamic::modeling::Ports oports_TOP = {typeid(TOP_output_port)};
dynamic::modeling::Models submodels_TOP = {reader, TELEPHONE};
dynamic::modeling::EICs eics_TOP = {}; // no external inputs we get inputs from reader
dynamic::modeling::EOCs eocs_TOP = {
dynamic::translate::make_EOC<output_port,TOP_output_port>("TELEPHONE")};
dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<reader_output,input_port>("reader","TELEPHONE"), // reader to TELEPHONE interconnect        
};

shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );


    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/COUPLED_TELEPHONE_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/COUPLED_TELEPHONE_state.txt");
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