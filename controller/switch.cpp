
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

#include "switch.hpp"


derailleur::Switch::Switch ( fluid_base::OFConnection* connection )
     : connection_ ( connection ),  ready_(false)
{
     //  install flow default (connection with controller)
     install_flow_default();
     
     // When a switch connects, the controller will request all information
     //available about it.
//      fluid_msg::of13::MultipartRequestDesc request;
//      uint8_t* buffer = request.pack();
//      this->connection_->send ( buffer, request.length() );
//      fluid_msg::OFMsg::free_buffer ( buffer );
}



bool derailleur::Switch::handle_multipart_description_reply (
     const derailleur::Message* message )
{
     fluid_msg::of13::MultipartReplyDesc reply;
     reply.unpack ( message->get_data() );
     this->switch_description_ = reply.desc();
     
     this->ready_ = true;

     return true;
}



void derailleur::Switch::install_flow_default()
{
     uint8_t* buffer;
     fluid_msg::of13::FlowMod fm ( 42, 0, 0xffffffffffffffff, 0,
                                   fluid_msg::of13::OFPFC_ADD, 0, 0, 0,
                                   0xffffffff, 0, 0, 0 );
     fluid_msg::of13::OutputAction *act =
          new fluid_msg::of13::OutputAction ( fluid_msg::of13::OFPP_CONTROLLER,
                    fluid_msg::of13::OFPCML_NO_BUFFER );
     fluid_msg::of13::ApplyActions *inst = new fluid_msg::of13::ApplyActions();
     inst->add_action ( act );
     fm.add_instruction ( inst );
     buffer = fm.pack();
     this->connection_->send ( buffer, fm.length() );
     fluid_msg::OFMsg::free_buffer ( buffer );
}
