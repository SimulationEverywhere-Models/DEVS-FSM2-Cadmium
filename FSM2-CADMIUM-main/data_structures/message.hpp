#ifndef __MESSAGE__HPP
#define __MESSAGE__HPP

#include <iostream>

using namespace std;
struct Message_t
{
    Message_t(){}
    Message_t(int i_state)
   :msg_state(i_state){}
    int msg_state; // input state.
      
};

istream& operator>> (istream& is, Message_t& msg);  //overloading operators for cater for our model

ostream& operator<<(ostream& os, const Message_t& msg);

#endif