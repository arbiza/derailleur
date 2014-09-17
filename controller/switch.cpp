
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


#include <bitset>

#include <fluid/OFServer.hh>

#include "switch.hpp"
#include "event.hpp"


derailleur::Switch::Switch ( fluid_base::OFConnection* connection,
                             derailleur::InternalEvent* event )
     : connection_ ( connection )
{
     // Stores features reply
     this->features_reply_->unpack ( event->get_data() );


     //  install flow default (connection with controller)
     install_flow_default();


     // Extract MAC address from datapath id. It requires convert uint64_t
     // datapath id to bitset, to extract the last 48 bits where MAC address is
     // and build a string with MAC address format (xx:xx:xx:xx:xx:xx).
     std::bitset<64> bits ( this->get_datapath_id() );
     this->mac_address_ =
          this->convert_bits_to_mac_address ( bits.to_string() );


     // Request switch description
     multipart_description_request();

     // Set capabilities to capabilities structure
     set_capabilities();
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



/** OpenFlow 1.3 Switch **/

void derailleur::Switch13::install_flow_default()
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



void derailleur::Switch13::multipart_description_request()
{
     fluid_msg::of13::MultipartRequestDesc request;
     uint8_t* buffer = request.pack();
     this->connection_->send ( buffer, request.length() );
     fluid_msg::OFMsg::free_buffer ( buffer );
}



void derailleur::Switch13::multipart_description_reply (
     const derailleur::InternalEvent* event )
{
     fluid_msg::of13::MultipartReplyDesc reply;
     reply.unpack ( event->get_data() );
     this->switch_description_ = reply.desc();
}


void derailleur::Switch13::set_capabilities()
{
     std::bitset<32> capabilities ( this->features_reply_.capabilities() );

     // Flow statistics
     this->capabilities_.OFPC_FLOW_STATS =
          ( capabilities[0] == 1 ? true : false );
     
     // Table statistics
     this->capabilities_.OFPC_TABLE_STATS =
          ( capabilities[1] ==  1  ? true : false);
          
     // Port statistics
     this->capabilities_.OFPC_PORT_STATS =
     ( capabilities[2] ==  1  ? true : false);
     
     // Group statistics
     this->capabilities_.OFPC_GROUP_STATS =
     ( capabilities[3] ==  1  ? true : false);
     
     // Can reassemble IP fragments
     this->capabilities_.OFPC_IP_REASM =
     ( capabilities[5] ==  1  ? true : false);
     
     // Queue statistics
     this->capabilities_.OFPC_QUEUE_STATS =
     ( capabilities[6] ==  1  ? true : false);
     
     // Block looping ports
     this->capabilities_.OFPC_PORT_BLOCKED =
     ( capabilities[8] ==  1  ? true : false);
}




/** OpenFlow 1.0 Switch **/

// derailleur::Switch10::Switch10 ( fluid_base::OFConnection* connection,
//                                  derailleur::InternalEvent* event )
//      : Switch ( connection, event ), of_version_ ( 1.0 )
// {
//
// }

