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
    const char * i_input_data_control = "../input_data/main_ATM.txt";
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
  shared_ptr<dynamic::modeling::model> idle = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("idle", true,0,0,vector<pair<int,int>>{make_pair(0,1)});
  shared_ptr<dynamic::modeling::model> checkingpin = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("checkingpin", false,1,1,vector<pair<int,int>>{make_pair(1,0),make_pair(2,2)});
  shared_ptr<dynamic::modeling::model> selectingtransactions = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("selectingtransactions", false,2,2,vector<pair<int,int>>{make_pair(3,3),make_pair(9,7)});
  shared_ptr<dynamic::modeling::model> withdrawing = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("withdrawing", false,3,3,vector<pair<int,int>>{make_pair(4,4)});
  shared_ptr<dynamic::modeling::model> inputingamount = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("inputingamount", false,4,4,vector<pair<int,int>>{make_pair(5,5)});
  shared_ptr<dynamic::modeling::model> confirming = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("confirming", false,5,5,vector<pair<int,int>>{make_pair(7,6), make_pair<int,int>(6,4)});
  shared_ptr<dynamic::modeling::model> withdrawaldone = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("withdrawaldone", false,6,6,vector<pair<int,int>>{make_pair(8,0)});
  shared_ptr<dynamic::modeling::model> depositing = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("depositing", false,7,7,vector<pair<int,int>>{make_pair(4,8)});
  shared_ptr<dynamic::modeling::model> insertingmoney = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("insertingmoney", false,8,8,vector<pair<int,int>>{make_pair(10,9)});
  shared_ptr<dynamic::modeling::model> depositdone = dynamic::translate::make_dynamic_atomic_model<Moore, TIME>("depositdone", false,9,9,vector<pair<int,int>>{make_pair(8,0)});


/*******ATM MODEL********/
    dynamic::modeling::Ports iports_ATM = {typeid(input_port)};
    dynamic::modeling::Ports oports_ATM = {typeid(output_port)};
    dynamic::modeling::Models submodels_ATM = { idle ,checkingpin, selectingtransactions, withdrawing , inputingamount 
                                              ,confirming, withdrawaldone, depositing , insertingmoney ,depositdone};
    dynamic::modeling::EICs eics_ATM = { 
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("idle"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("checkingpin"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("selectingtransactions"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("withdrawing"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("inputingamount"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("confirming"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("withdrawaldone"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("depositing"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("insertingmoney"),
        dynamic::translate::make_EIC<input_port, Moore_defs::event_in>("depositdone")
};
    dynamic::modeling::EOCs eocs_ATM = {
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("idle"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("idle"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("checkingpin"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("checkingpin"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("selectingtransactions"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("selectingtransactions"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("withdrawing"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("withdrawing"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("inputingamount"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("inputingamount"),                
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("confirming"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("confirming"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("withdrawaldone"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("withdrawaldone"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("depositing"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("depositing"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("insertingmoney"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("insertingmoney"),
        dynamic::translate::make_EOC<Moore_defs::state_out,output_port>("depositdone"),
        dynamic::translate::make_EOC<Moore_defs::error_out,output_port>("depositdone")
    };
    dynamic::modeling::ICs ics_ATM = {
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("idle","idle"), // idle to idle 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("idle","checkingpin"), // idle to check pin
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("checkingpin","idle"),  // incorrect pin back to idle
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("checkingpin","selectingtransactions"), 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("selectingtransactions","withdrawing"), // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("selectingtransactions","depositing"),
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("withdrawing","inputingamount"),  // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("inputingamount","confirming"), //
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("confirming","inputingamount"),  // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("confirming","withdrawaldone"),  // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("withdrawaldone","idle"),  // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("depositing","insertingmoney"),  // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("insertingmoney","depositdone"),  // 
        dynamic::translate::make_IC<Moore_defs::transition_out,Moore_defs::transition_in>("depositdone","idle"),  // 
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> ATM;
    ATM = make_shared<dynamic::modeling::coupled<TIME>>(
        "ATM", submodels_ATM, iports_ATM, oports_ATM, eics_ATM, eocs_ATM, ics_ATM 
    );

    /*************** TOP *******************/

dynamic::modeling::Ports iports_TOP = {};
dynamic::modeling::Ports oports_TOP = {typeid(TOP_output_port)};
dynamic::modeling::Models submodels_TOP = {reader, ATM};
dynamic::modeling::EICs eics_TOP = {}; // no external inputs we get inputs from reader
dynamic::modeling::EOCs eocs_TOP = {
dynamic::translate::make_EOC<output_port,TOP_output_port>("ATM")};
dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<reader_output,input_port>("reader","ATM"), // reader to ATM interconnect        
};

shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );


    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/COUPLED_ATM_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/COUPLED_ATM_state.txt");
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
    r.run_until(NDTime("00:00:00:210"));
    return 0;

}