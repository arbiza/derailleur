
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


#include <cstring>
#include <cstdlib>
#include <sstream>
#include <bitset>
# include <cstdint>

#include <fluid/OFServer.hh>

#include "switch.hpp"
#include "event.hpp"


std::string derailleur::Switch::convert_bits_to_mac_address (
     uint64_t* datapath_id )
{
     std::stringstream ss;
     ss << std::hex << std::setfill ( '0' );

     uint8_t array[8];
     std::memcpy ( array, datapath_id, 8 );

     for ( short i = 5; i >= 0; i-- ) {
          ss <<  std::setw ( 2 ) << ( int ) array[i];

          if ( i > 0 )
               ss << ":";
     }

     return ss.str();
}



// OpenFlow 1.0 Switch

short derailleur::Switch10::install_flow ( fluid_msg::FlowModCommon* flow,
          fluid_msg::Action* action )
{

     return EXIT_SUCCESS;
}


short int derailleur::Switch10::install_flow_table (  )
{

     return EXIT_SUCCESS;
}

void derailleur::Switch10::set_features_reply ( uint8_t* data )
{
     fluid_msg::of10::FeaturesReply reply;
     reply.unpack ( data );
     this->datapath_id_ = reply.datapath_id();
     this->n_buffers_ = reply.n_buffers();
     this->n_tables_ = reply.n_tables();


     this->mac_address_ =
          this->convert_bits_to_mac_address ( &this->datapath_id_ );


     std::bitset<32> capabilities ( reply.capabilities() );

     // Flow statistics
     capabilities_.OFPC_FLOW_STATS = ( capabilities[0] == 1 ? true : false );

     // Table statistics
     capabilities_.OFPC_TABLE_STATS = ( capabilities[1] ==  1 ? true : false );

     // Port statistics
     capabilities_.OFPC_PORT_STATS = ( capabilities[2] ==  1 ? true : false );

     // 802.1d spanning tree.
     capabilities_.OFPC_STP = ( capabilities[3] ==  1 ? true : false );

     // Reserved, must be zero.
     capabilities_.OFPC_RESERVED = ( capabilities[4] ==  1 ? true : false );

     // Can reassemble IP fragments
     capabilities_.OFPC_IP_REASM = ( capabilities[5] ==  1 ? true : false );

     // Queue statistics
     capabilities_.OFPC_QUEUE_STATS = ( capabilities[6] ==  1 ? true : false );

     // Block looping ports
     capabilities_.OFPC_ARP_MATCH_IP = ( capabilities[7] ==  1 ? true : false );
}



void derailleur::Switch10::install_flow_default()
{

}



void derailleur::Switch10::multipart_description_request()
{

}



void derailleur::Switch10::multipart_description_reply (
     const derailleur::InternalEvent* event )
{

}



// OpenFlow 1.3 Switch

short derailleur::Switch13::install_flow ( fluid_msg::FlowModCommon* flow,
          fluid_msg::Action* action )
{
     fluid_msg::of13::FlowMod* flow_mod =
          static_cast<fluid_msg::of13::FlowMod*> ( flow );

     fluid_msg::of13::OutputAction* output_action =
          static_cast<fluid_msg::of13::OutputAction*> ( action );


     fluid_msg::of13::ApplyActions *inst = new fluid_msg::of13::ApplyActions();
     inst->add_action ( output_action );
     flow_mod->add_instruction ( inst );
     uint8_t* buffer = flow_mod->pack();
     this->connection_->send ( buffer, flow_mod->length() );
     fluid_msg::OFMsg::free_buffer ( buffer );

     return EXIT_SUCCESS;
}


short int derailleur::Switch13::install_flow_table (  )
{

     return EXIT_SUCCESS;
}




void derailleur::Switch13::set_features_reply ( uint8_t* data )
{
     fluid_msg::of13::FeaturesReply reply;
     reply.unpack ( data );
     this->datapath_id_ = reply.datapath_id();
     this->n_buffers_ = reply.n_buffers();
     this->n_tables_ = reply.n_tables();
     this->auxiliary_id_ = reply.auxiliary_id();


     this->mac_address_ =
               this->convert_bits_to_mac_address ( &this->datapath_id_ );


     std::bitset<32> capabilities ( reply.capabilities() );

     // Flow statistics
     capabilities_.OFPC_FLOW_STATS = ( capabilities[0] == 1 ? true : false );

     // Table statistics
     capabilities_.OFPC_TABLE_STATS = ( capabilities[1] ==  1 ? true : false );

     // Port statistics
     capabilities_.OFPC_PORT_STATS = ( capabilities[2] ==  1 ? true : false );

     // Group statistics
     capabilities_.OFPC_GROUP_STATS = ( capabilities[3] ==  1 ? true : false );

     // Can reassemble IP fragments
     capabilities_.OFPC_IP_REASM = ( capabilities[5] ==  1 ? true : false );

     // Queue statistics
     capabilities_.OFPC_QUEUE_STATS = ( capabilities[6] ==  1 ? true : false );

     // Block looping ports
     capabilities_.OFPC_PORT_BLOCKED = ( capabilities[8] ==  1 ? true : false );
}


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

     fluid_msg::SwitchDesc desc = reply.desc();
     this->manufacturer_ = desc.mfr_desc();
     this->hardware_ = desc.hw_desc();
     this->software_ = desc.sw_desc();
     this->serial_number_ = desc.serial_num();
     this->datapath_ = desc.dp_desc();
}









