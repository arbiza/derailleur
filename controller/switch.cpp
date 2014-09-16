
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

#include <stdio.h>

#include <bitset>

#include <fluid/OFServer.hh>

#include "switch.hpp"
#include "event.hpp"


derailleur::Switch::Switch ( fluid_base::OFConnection* connection,
                             derailleur::InternalEvent* event )
     : connection_ ( connection )
{
     // Stores features reply
     this->features_reply_.unpack ( event->get_data() );

     // Extract MAC address from datapath id. It requires convert uint64_t
     // datapath id to bitset, to extract the last 48 bits where MAC address is
     // and build a string with MAC address format (xx:xx:xx:xx:xx:xx).
     std::bitset<64> bits ( this->get_datapath_id() );

     this->mac_address_ = this->convert_bits_to_mac_address ( bits.to_string() );


     //  install flow default (connection with controller)
     install_flow_default();

     // When a switch connects, the controller will request all information
     //available about it.
     fluid_msg::of13::MultipartRequestDesc request;
     uint8_t* buffer = request.pack();
     this->connection_->send ( buffer, request.length() );
     fluid_msg::OFMsg::free_buffer ( buffer );
}



void derailleur::Switch::handle_multipart_description_reply (
     const derailleur::InternalEvent* event )
{
     fluid_msg::of13::MultipartReplyDesc reply;
     reply.unpack ( event->get_data() );
     this->switch_description_ = reply.desc();
}



void derailleur::Switch::install_flow_default()
{
     uint8_t* buffer;
     fluid_msg::of13::FlowMod fm ( 42, 0, 0xffffffffffffffff, 0,
                                   fluid_msg::of13::OFPFC_ADD, 0, 0, 0,
                                   0xffffffff, 0, 0, 0 );
     fluid_msg::of13::OutputAction *act = new fluid_msg::of13::OutputAction (
          fluid_msg::of13::OFPP_CONTROLLER,
          fluid_msg::of13::OFPCML_NO_BUFFER );
     fluid_msg::of13::ApplyActions *inst = new fluid_msg::of13::ApplyActions();
     inst->add_action ( act );
     fm.add_instruction ( inst );
     buffer = fm.pack();
     this->connection_->send ( buffer, fm.length() );
     fluid_msg::OFMsg::free_buffer ( buffer );
}



std::string derailleur::Switch::convert_bits_to_mac_address (
     std::string datapath_id )
{
     short position = 16; // MAC address starts at position 16 in datapath_id
     size_t length = 4;
     std::string mac;

     for ( short i = 0; i < 12; i++ ) {
          std::string mac_in_bits = datapath_id.substr ( position, length );

          if ( mac_in_bits == "0000" )
               mac += "0";
          else if ( mac_in_bits == "0001" )
               mac += "1";
          else if ( mac_in_bits == "0010" )
               mac += "2";
          else if ( mac_in_bits == "0011" )
               mac += "3";
          else if ( mac_in_bits == "0100" )
               mac += "4";
          else if ( mac_in_bits == "0101" )
               mac += "5";
          else if ( mac_in_bits == "0110" )
               mac += "6";
          else if ( mac_in_bits == "0111" )
               mac += "7";
          else if ( mac_in_bits == "1000" )
               mac += "8";
          else if ( mac_in_bits == "1001" )
               mac += "9";
          else if ( mac_in_bits == "1010" )
               mac += "a";
          else if ( mac_in_bits == "1011" )
               mac += "b";
          else if ( mac_in_bits == "1100" )
               mac += "c";
          else if ( mac_in_bits == "1101" )
               mac += "d";
          else if ( mac_in_bits == "1110" )
               mac += "e";
          else if ( mac_in_bits == "1111" )
               mac += "f";

          position += length;
     }

     return mac;
}

