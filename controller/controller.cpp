
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

// TODO: remove when log class in use
#include <iostream>


#include <memory>
#include <map>

#include "controller.hpp"
#include "switch.hpp"



derailleur::Controller::Controller(const char* address,
				   const int port,
				   const int n_workers,
				   const bool secure)
    : fluid_base::OFServer(address,
			   port,
			   n_workers,
			   secure,
			   fluid_base::OFServerSettings().
			   supported_version(1).
			   supported_version(4).
			   keep_data_ownership(false)) {
    
    switch_stack_ = std::make_shared< std::map<int, derailleur::Switch> >;
}



void derailleur::Controller::message_callback(
    fluid_base::OFConnection *ofconn, uint8_t type, void *data, size_t len)
{
    if(type == 6) {
	derailleur::Switch s(ofconn);
    }
    else {
	std::cout << "lost message arrived." << std::endl;
    }
    // switch (type){

    // case 10: // packet_in
	
    // 	break;

    // case 6: // ofpt_features_reply
    // 	std::cout << "switch up" << std::endl;
	
    // 	break;

    // default:
    // 	break;
    // }
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
	std::cout << "id: " << ofconn->get_id() << std::endl;
	// printf("Connection id=%d closed by the user\n", ofconn->get_id());
	// dispatch_event(new SwitchDownEvent(ofconn));
    }
    else if (type == fluid_base::OFConnection::EVENT_DEAD)
    {
	std::cout << "id: " << ofconn->get_id() << std::endl;
	// printf("Connection id=%d closed due to inactivity\n", ofconn->get_id());
	// dispatch_event(new SwitchDownEvent(ofconn));
    }
}
