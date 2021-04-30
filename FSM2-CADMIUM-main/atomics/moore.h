

#ifndef __MOORE__H__
#define __MOORE__H__

// including the relevant cadmium headers
#include "cadmium/modeling/ports.hpp"
#include "cadmium/modeling/message_bag.hpp"

#include "../data_structures/message.hpp"

#include <vector>
#include <assert.h>
#include <limits>
#include <string>

using namespace cadmium;
using namespace std;

// Port Definition

struct Moore_defs
{
    struct state_out : public out_port<Message_t>
    {
    };
    struct transition_out : public out_port<Message_t>
    {
    }; // transition out of port
    struct error_out : public out_port<Message_t>
    {
    };

    struct event_in : public in_port<Message_t>
    {
    };
    struct transition_in : public in_port<Message_t>
    {
    };
};

template <typename TIME>

class Moore
{
    public:
    TIME preparationTime;
    int number_of_transitions; // defines number of transition of this state
    bool has_state_value;      //whether the module have its own state value;
    bool error;                // was there error in the input

    //default constructor required by CADMIUM
    Moore() noexcept
    {
        preparationTime = numeric_limits<TIME>::infinity(); //passive until input
        state.phase = false;                                //inactive
        state.state_code = 0;                               //will become active is value 0 is given
        has_state_value = true;
        state.state_value = 1;
        state.error_code = 0; //no error
        state.is_Event = false;
        state.transition.push_back(make_pair(0, 1)); //transition from zero to one.
    }

    //Constructor to allow diffrent settings of states;

    Moore(bool phase, int state_code, int state_value, vector<pair<int, int>> transition)
    //: state.transition{move(transition)} {}//copy to transition
    {
        preparationTime = numeric_limits<TIME>::infinity(); //passive until input
        state.phase = phase;                                //inactive
        state.state_code = state_code;                      //will become active is value 0 is given
        has_state_value = true;
        state.state_value = state_value;
        state.error_code = 0; //no error
        state.is_Event = false;
        state.transition = transition;
        number_of_transitions = state.transition.size(); //transition number of transitions.
    }

    struct state_type
    {
        int state_code;  // global_state_code.
        int state_value; // value of the state
        bool phase;      // is this moore state active or passive
        int temp_state_value;
        int error_code;
        bool is_Event;
        vector<pair<int, int>> transition; //transitions from one to another state;
    };
    state_type state;

    //defining ports
    using input_ports = tuple<typename Moore_defs::event_in, typename Moore_defs::transition_in>;    //input transition from self
    using output_ports = tuple<typename Moore_defs::state_out, typename Moore_defs::transition_out, typename Moore_defs::error_out>; // output transitiong of moore

    /// Internal Transision
    /// Called after the output function

    void internal_transition()
    {   
        if (error){
            
    }
        if (!error)
        {
            if (state.is_Event)
            {
                state.phase = false;
                preparationTime = numeric_limits<TIME>::infinity();
            }
            else
            {
                state.phase = true;
                preparationTime = numeric_limits<TIME>::infinity();
            }
        }
        else
        {
            if (state.phase)
            {
                preparationTime = numeric_limits<TIME>::infinity();
            }
            else
            {
                //state.phase = true;
                preparationTime = numeric_limits<TIME>::infinity();
            }
        }
    }

    //// External Transition Function ////
    //// Called when input is recieved ////
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
    {
        error = false;

        // check for erroneous inputs
        if (get_messages<typename Moore_defs::event_in>(mbs).size() > 1)
        {
            assert(false && "Multiple messages recieved in port event_in");
        }
        if (get_messages<typename Moore_defs::transition_in>(mbs).size() > 1)
        {
            assert(false && "Multiple messages recieved in port transition_in");
        }

        switch (state.phase)
        { // check if state.phase is active or not
        case true:

            if (get_messages<typename Moore_defs::event_in>(mbs).size() > 0) //we have an event input
            {
                int state_msg; //to store the state msg
                vector<Message_t> event_in = get_messages<typename Moore_defs::event_in>(mbs);
                state_msg = event_in[0].msg_state;
                bool transition_found = false;
                for (int i = 0; i < number_of_transitions; i++)
                {
                    if (state_msg == state.transition[i].first)
                    {
                        state.temp_state_value = state.transition[i].second; //for this transition fill second value
                        transition_found = true;
                        break;
                    }
                }
                if (!transition_found)
                {
                    error = true;
                    state.error_code = -999; // This state should not be active as it does not have any relevant transitions.
                }
                state.phase = true;
                preparationTime = TIME("00:00:00");
                state.is_Event = true;
            }

            break;
        case false:
            if (get_messages<typename Moore_defs::transition_in>(mbs).size() > 0) //did we get a transition_input?
            {
                int state_msg;
                vector<Message_t> event_in = get_messages<typename Moore_defs::transition_in>(mbs);  
                state_msg = event_in[0].msg_state;

                if (state_msg == state.state_code)
                {
                    state.is_Event = false;
                    state.phase = true;
                    preparationTime = TIME("00:00:00");
                }
            }
            break;
        }
    }

    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }

    /// Output Function

    typename make_message_bags<output_ports>::type output() const
    { //takes no inputs
        typename make_message_bags<output_ports>::type bags;

        Message_t out_state;
        if (error)
        {
           out_state.msg_state = state.error_code;
           get_messages<typename Moore_defs::error_out>(bags).push_back(out_state);
        }
        else
        {
            if (state.is_Event)
            {
                out_state.msg_state = state.temp_state_value;
                get_messages<typename Moore_defs::transition_out>(bags).push_back(out_state);
            }
          else if (has_state_value)
          {
                out_state.msg_state = state.state_value;
                get_messages<typename Moore_defs::state_out>(bags).push_back(out_state);
          }
        }
        return bags;
    }

    /// Time Advance Function
    TIME time_advance() const
    {
        TIME next_interval;
        next_interval = preparationTime;
        return next_interval;
    }
    // log output
    friend std::ostringstream& operator<<(std::ostringstream& os, const typename Moore<TIME>::state_type& i)
    {
        //os << " StateCode: "; ("StateCode: ") 
        os << (i.state_code) << (" phase:" ) << (i.phase) << (" StateValue:") << (i.state_value) << (" IsEvent:") << (i.is_Event) << (" TempStateValue: ") << (i.temp_state_value);
        return os;
    }

};


#endif