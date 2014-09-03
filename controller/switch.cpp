
/**
 *  File: switch.cpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  This file is an implementation of the class(es) defined in
 *  switch.hpp header file.
 *
 *  See the header file for documentation and license information.
 *
 **/

#include <fluid/of13msg.hh>

#include "switch.hpp"



void derailleur::Switch::message_callback(fluid_base::OFConnection *ofconn,
					  uint8_t type,
					  void *data,
					  size_t len) {
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



void derailleur::Switch::connection_callback(
    fluid_base::OFConnection *ofconn,
    fluid_base::OFConnection::Event type)
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




void derailleur::Switch::add_flow_normal() {

}



void derailleur::Switch::add_flow_default() {
    uint8_t* buffer;
    fluid_msg::of13::FlowMod fm(42, 0, 0xffffffffffffffff, 0,
				fluid_msg::of13::OFPFC_ADD, 0, 0, 0,
				0xffffffff, 0, 0, 0);
    fluid_msg::of13::OutputAction *act =
	new fluid_msg::of13::OutputAction(fluid_msg::of13::OFPP_CONTROLLER,
					  fluid_msg::of13::OFPCML_NO_BUFFER);
    fluid_msg::of13::ApplyActions *inst = new fluid_msg::of13::ApplyActions();
    inst->add_action(act);
    fm.add_instruction(inst);
    buffer = fm.pack();
    connection_->send(buffer, fm.length());
    fluid_msg::OFMsg::free_buffer(buffer);
}




















