
/**
 *  File: controller.cpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  This file is an implementation of the class(es) defined in
 *  controller.h header file.
 *
 *  See the header file for documentation and license information.
 *
 **/


#include "controller.h"



void derailleur::Controller::message_callback(
    fluid_base::OFConnection *ofconn, 
    uint8_t type, void *data, size_t len)
{
     std::cout << "message" << std::endl;
}



void derailleur::Controller::connection_callback(
    fluid_base::OFConnection *ofconn, 
    fluid_base::OFConnection::Event type)
{
    // what?
}



// TODO: remove it!    
void derailleur::Controller::teste()
{ 
    std::cout << "OK!" << std::endl; 
}
