
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
#include "log.hpp"
#include "util.hpp"



derailleur::Switch::Switch ( const derailleur::Switch& source )
{
     /* sets connectio to null to prevent access attempets to a pointer that may
        do not exist anymore.*/
     this->connection_ = nullptr;
     this->switch_id_ = source.get_switch_id();
     this->name_ = source.get_name();
     this->mac_address_ = source.get_mac_address();
     this->of_version_ = source.get_of_version();
     this->datapath_id_ = source.get_datapath_id();
     this->n_buffers_ = source.get_n_buffers();
     this->n_tables_ = source.get_n_tables();
     this->manufacturer_ = source.get_manufacturer();
     this->hardware_ = source.get_hardware();
     this->software_ = source.get_software();
     this->serial_number_ = source.get_serial_number();
     this->datapath_ = source.get_datapath();

     /* ARP-like tables and mutex are not copied in the copy. */
}



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




bool derailleur::Switch::set_IPv4_neighbor ( derailleur::Arp4* entry )
{

     bool matched = false;
     bool new_device = false;

//      this->mutex_.lock();
//
//      for ( std::vector<Arp4>::iterator it = arp_table_v4_.begin();
//                it != arp_table_v4_.end(); ++it ) {
//
//           /* If entry already exists. */
//           if ( derailleur::util::compare_byte_arrays ( it->mac, entry->mac, 6 )
//                     &&
//                     derailleur::util::compare_byte_arrays ( it->ip, entry->ip, 6 ) ) {
//
//                matched = true;
//           } else {
//
//                /* Check if MAC, IP or port are already in the table; if yes table
//                 * entry will be updated. */
//                if ( derailleur::util::compare_byte_arrays ( it->mac, entry->mac, 6 )
//                          ||
//                          derailleur::util::compare_byte_arrays ( it->ip, entry->ip, 6 ) ) {
//                     if ( !matched ) {
//                          memcpy ( it->ip, entry->ip, 4 );
//                          memcpy ( it->mac, entry->mac, 6 );
//                          it->port = entry->port;
//                          matched = true;
//                          new_device = true;
//                     } else {
//                          arp_table_v4_.erase ( it );
//                     }
//                }
//           }
//      }
//
//      /* If entry did not match, it is a new device. */
//      if ( !matched ) {
//           arp_table_v4_.push_back ( *entry );
//           new_device = true;
//      }
//
//      this->mutex_.unlock();

     return new_device;
}


void derailleur::Switch::install_flow ( fluid_msg::FlowModCommon* flow )
{
     uint8_t* buffer = flow->pack();
     this->connection_->send ( buffer, flow->length() );
     fluid_msg::OFMsg::free_buffer ( buffer );
}



// OpenFlow 1.0 Switch

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
     uint8_t* buffer;

     fluid_msg::of10::FlowMod fm (
          0,                                                // xid
          0,                                                // cookie
          fluid_msg::of10::OFPFC_ADD,                       // command
          0,                                                // idle timeout
          0,                                                // hard timeout
          0,                                                // priority
          0xffffffff,                                       // buffer id
          0,                                                // out_port
          0                                                 // flags
     );

     fluid_msg::of10::OutputAction act ( fluid_msg::of10::OFPP_CONTROLLER,
                                         1024 );
     fm.add_action ( act );

     buffer = fm.pack();
     this->connection_->send ( buffer, fm.length() );
     fluid_msg::OFMsg::free_buffer ( buffer );
}


void derailleur::Switch10::flood ( fluid_msg::PacketInCommon* packet_in, uint32_t port )
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

// short derailleur::Switch13::install_flow ( fluid_msg::FlowModCommon* flow,
//           fluid_msg::Action* action )
// {
//      fluid_msg::of13::FlowMod* flow_mod =
//           static_cast<fluid_msg::of13::FlowMod*> ( flow );
//
//      fluid_msg::of13::OutputAction* output_action =
//           static_cast<fluid_msg::of13::OutputAction*> ( action );
//
//
//      fluid_msg::of13::ApplyActions *inst = new fluid_msg::of13::ApplyActions();
//      inst->add_action ( output_action );
//      flow_mod->add_instruction ( inst );
//      uint8_t* buffer = flow_mod->pack();
//      this->connection_->send ( buffer, flow_mod->length() );
//      fluid_msg::OFMsg::free_buffer ( buffer );
//
//      return EXIT_SUCCESS;
// }




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

     fluid_msg::of13::FlowMod fm;
     //fm.xid(pi.xid());
     fm.cookie ( 0 );
     fm.cookie_mask ( 0xffffffffffffffff );
     fm.table_id ( 0 );
     fm.command ( fluid_msg::of13::OFPFC_ADD );
     fm.idle_timeout ( 0 );
     fm.hard_timeout ( 0 );
     fm.priority ( 0 );
     fm.buffer_id ( 0xffffffff );
     fm.out_port ( 0 );
     fm.out_group ( 0 );
     fm.flags ( 0 );

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


void derailleur::Switch13::flood ( fluid_msg::PacketInCommon* packet_in,
                                   uint32_t port )
{
     uint8_t* buffer;

     fluid_msg::of13::PacketOut packet_out ( packet_in->xid(),
                                             packet_in->buffer_id(), port );
                                             
     /* if packet-in data was not buffered its data is added to the packet-out */
     if (packet_in->buffer_id() == -1 )
          packet_out.data( packet_in->data(), packet_in->data_len() );
          
     fluid_msg::of13::OutputAction action ( fluid_msg::of13::OFPP_FLOOD,  1024);
     packet_out.add_action( action );
     
     buffer = packet_out.pack();
     
     this->connection_->send( buffer, packet_out.length() );
     
     fluid_msg::OFMsg::free_buffer( buffer );
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









