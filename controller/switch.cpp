
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

#include <fluid/OFServer.hh>
#include <fluid/of13msg.hh>

#include "switch.hpp"


derailleur::Switch::Switch(fluid_base::OFConnection* const connection)
    : connection_(connection) {
    add_flow_default();
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
    this->connection_->send(buffer, fm.length());
    fluid_msg::OFMsg::free_buffer(buffer);
}
