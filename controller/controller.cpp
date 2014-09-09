
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


#include <map>
#include <utility>

#include "controller.hpp"
#include "message.hpp"




derailleur::Controller::Controller(
    const char* address,
    const int port,
    const int n_workers,
    const bool secure,
    std::unique_ptr<derailleur::Application> application) 
    : fluid_base::OFServer(address,
			   port,
			   n_workers,
			   secure,
			   fluid_base::OFServerSettings().
			   supported_version(1).
			   supported_version(4).
			   keep_data_ownership(false)) {

    this->application_ = std::move(application);
    this->application_->set_rack_pointer();
}



void derailleur::Controller::message_callback(fluid_base::OFConnection *ofconn,
					      uint8_t type,
					      void *data,
					      size_t len)
{
    if(type == 6) {

	//TODO: lock here
    switches_rack_.emplace(std::make_pair(int (ofconn->get_id()),
                                          derailleur::Switch(ofconn)));
	//TODO: unlock here	
    }
    else {
        // switches_rack_.at(ofconn->get_id()).message_handler(
	//     new Message(this, type, data, len));
    }
}



void derailleur::Controller::connection_callback( fluid_base::OFConnection
    *ofconn, fluid_base::OFConnection::Event type)
{
    if (type == fluid_base::OFConnection::EVENT_STARTED)
    {
	// TODO: log
    }
    else if (type == fluid_base::OFConnection::EVENT_ESTABLISHED)
    {
        // TODO: log
    }
    else if (type == fluid_base::OFConnection::EVENT_FAILED_NEGOTIATION)
    {
	// TODO: log
    }
    else if (type == fluid_base::OFConnection::EVENT_CLOSED)
    {
	std::cout << "closed id: " << ofconn->get_id() << std::endl;
        // TODO: Delete switch from rack
    }
    else if (type == fluid_base::OFConnection::EVENT_DEAD)
    {
	std::cout << "closed id: " << ofconn->get_id() << std::endl;
        // TODO: Delete switch from rack
    }
}
