
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
#include <utility>

#include "controller.hpp"
#include "switch.hpp"


void echo(const char* text) {
    std::cout << text << std::endl;
}


void teste(fluid_base::OFConnection* ofconn,
    fluid_base::OFHandler* handler) {
    derailleur::Switch s(ofconn, handler);
    // std::cout << "ok" << std::endl;
}


void derailleur::Controller::message_callback(
    fluid_base::OFConnection *ofconn, uint8_t type, void *data, size_t len)
{
    if(type == 6) {
	// this->switches_.push_back(std::thread(
	// 			      &derailleur::Controller::new_switch,
	// 			      ofconn,
	//  			      this));
	// this->switches_.push_back(std::thread(
	// 			      &derailleur::Controller::new_switch,
	// 			      this));
	//derailleur::Switch s(ofconn, this);
	//std::thread t(std::move(s));

	this->switches_.push_back(std::thread(teste,ofconn, this));

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
	// printf("Connection id=%d closed by the user\n", ofconn->get_id());
	// dispatch_event(new SwitchDownEvent(ofconn));
    }
    else if (type == fluid_base::OFConnection::EVENT_DEAD)
    {
	// printf("Connection id=%d closed due to inactivity\n", ofconn->get_id());
	// dispatch_event(new SwitchDownEvent(ofconn));
    }
}



void derailleur::Controller::new_switch(fluid_base::OFConnection* ofconn,
    fluid_base::OFHandler* handler) {
//    derailleur::Switch s(ofconn, handler);
    // std::cout << "ok" << std::endl;
}



















