
#include "message.hpp"


istream& operator>> (istream& is, Message_t& msg)  //overloading operators for cater for our model
{
    is >> msg.msg_state;
    return is;
}

ostream& operator<<(ostream& os, const Message_t& msg)
{
    os << msg.msg_state;
    return os; 
}