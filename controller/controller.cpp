
/**
 *  File: controller.cpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  This file is an implementation of the class(es) defined in
 *  controller.hpp header file.
 *
 *  See the header file for documentation and license information.
 *
 **/


#include "controller.hpp"



void derailleur::Controller::message_callback(
    fluid_base::OFConnection *ofconn, uint8_t type, void *data, size_t len)
{
    if (type == 10) { // OFPT_PACKET_IN
	dispatch_event(new PacketInEvent(ofconn, this, data, len));
    }
    else if (type == 6) { // OFPT_FEATURES_REPLY
	dispatch_event(new SwitchUpEvent(ofconn, this, data, len));
    }   
}



void derailleur::Controller::connection_callback( fluid_base::OFConnection
    *ofconn, fluid_base::OFConnection::Event type)
{
    if (type == OFConnection::EVENT_STARTED)
    {
	
    }
    else if (type == OFConnection::EVENT_ESTABLISHED)
    {
        
    }
    else if (type == OFConnection::EVENT_FAILED_NEGOTIATION)
    {
	
    }
    else if (type == OFConnection::EVENT_CLOSED)
    {
	// printf("Connection id=%d closed by the user\n", ofconn->get_id());
	// dispatch_event(new SwitchDownEvent(ofconn));
    }
    else if (type == OFConnection::EVENT_DEAD)
    {
	// printf("Connection id=%d closed due to inactivity\n", ofconn->get_id());
	// dispatch_event(new SwitchDownEvent(ofconn));
    }
}



// TODO: remove it!    
void derailleur::Controller::teste()
{ 
    std::cout << "OK!" << std::endl; 
}
