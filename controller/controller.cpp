
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

#include <iostream>

#include "controller.hpp"
#include "base_flows.hpp"



void derailleur::Controller::message_callback(
    fluid_base::OFConnection *ofconn, uint8_t type, void *data, size_t len)
{
    switch (type){

    case 10: // packet_in
	
	break;

    case 6: // ofpt_features_reply
	std::cout << "switch up" << std::endl;
	derailleur::flows::normal(ofconn);
	break;

    default:
	break;
    }
}



void derailleur::Controller::connection_callback( fluid_base::OFConnection
    *ofconn, fluid_base::OFConnection::Event type)
{
    if (type == fluid_base::OFConnection::EVENT_STARTED)
    {
	
    }
    else if (type == fluid_base::OFConnection::EVENT_ESTABLISHED)
    {
        
    }
    else if (type == fluid_base::OFConnection::EVENT_FAILED_NEGOTIATION)
    {
	
    }
    else if (type == fluid_base::OFConnection::EVENT_CLOSED)
    {
	// printf("Connection id=%d closed by the user\n", ofconn->get_id());
	// dispatch_event(new SwitchDownEvent(ofconn));
    }
    else if (type == fluid_base::OFConnection::EVENT_DEAD)
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
