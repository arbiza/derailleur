
// TODO: remove
#include <iostream>

#include <fluid/OFServer.hh>
#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>

#include "application.hpp"
#include "event.hpp"
#include "log.hpp"


std::vector< int > derailleur::Application::get_switches_IDs()
{

     std::vector<int> ids;

     this->mutex_->lock();
     for ( auto& s : *this->stack_ptr_ )
          ids.push_back ( s.first );
     this->mutex_->unlock();
     return ids;
}


uint8_t derailleur::Application::get_auxiliary_id ( short int switch_id )
{
     uint8_t auxiliary_id = 0;

     if ( get_switch_version ( switch_id ) ==
               ( int ) fluid_msg::of10::OFP_VERSION ) {

          this->mutex_->lock();

          derailleur::Switch13* s =
               static_cast<derailleur::Switch13*> (
                    this->stack_ptr_->at ( switch_id ) );
          return s->get_auxiliary_id();

          this->mutex_->unlock();
     }

     return auxiliary_id;
}


bool derailleur::Application::get_switch_copy ( short int switch_id,
          derailleur::Switch& other )
{
     bool found = false;

     this->mutex_->lock();

     /* Ensure that element exists in the container,  otherwise returns false. */
     if ( this->stack_ptr_->find ( switch_id ) != this->stack_ptr_->end() ) {
          derailleur::Switch* source = this->stack_ptr_->at ( switch_id );

          other.connection_ = nullptr;
          other.switch_id_ = source->switch_id_;
          other.name_ = source->name_;
          other.mac_address_ = source->mac_address_;
          other.of_version_ = source->of_version_;
          other.datapath_id_ = source->datapath_id_;
          other.n_buffers_ = source->n_buffers_;
          other.n_tables_ = source->n_tables_;
          other.manufacturer_ = source->manufacturer_;
          other.hardware_ = source->hardware_;
          other.software_ = source->software_;
          other.serial_number_ = source->serial_number_;
          other.datapath_ = source->datapath_;
          other.arp_table_v4_ = source->arp_table_v4_;
          other.arp_table_v6_ = source->arp_table_v6_;

          found = true;
     }

     this->mutex_->unlock();

     return found;
}


bool derailleur::Application::get_switch_copy ( short int switch_id,
          derailleur::Switch* other )
{
     bool found = false;

     this->mutex_->lock();

     /* Ensure that element exists in the container,  otherwise returns false. */
     if ( this->stack_ptr_->find ( switch_id ) != this->stack_ptr_->end() ) {
          derailleur::Switch* source = this->stack_ptr_->at ( switch_id );

          other->connection_ = nullptr;
          other->switch_id_ = source->switch_id_;
          other->name_ = source->name_;
          other->mac_address_ = source->mac_address_;
          other->of_version_ = source->of_version_;
          other->datapath_id_ = source->datapath_id_;
          other->n_buffers_ = source->n_buffers_;
          other->n_tables_ = source->n_tables_;
          other->manufacturer_ = source->manufacturer_;
          other->hardware_ = source->hardware_;
          other->software_ = source->software_;
          other->serial_number_ = source->serial_number_;
          other->datapath_ = source->datapath_;
          other->arp_table_v4_ = source->arp_table_v4_;
          other->arp_table_v6_ = source->arp_table_v6_;

          found = true;
     }

     this->mutex_->unlock();

     return found;
}


void derailleur::Application::get_switches_copies (
     std::map< int, derailleur::Switch* >* copies )
{
     /* remove all switches from copies container */
     copies->clear();

     std::vector<int> ids = get_switches_IDs();


     /* In the following loop every switch connected will be copied in copies
      * container. */
     for ( int& switch_id : ids ) {

          derailleur::Switch* s;

          if ( this->stack_ptr_->at ( switch_id )->get_of_version() ==
                    fluid_msg::of13::OFP_VERSION )
               s = new derailleur::Switch13;
          else
               s = new derailleur::Switch10;

          get_switch_copy ( switch_id, s );
          copies->emplace ( std::make_pair ( switch_id, s ) );
     }
}



void derailleur::Application::update_switch_ARP_tables (
     std::map< int, derailleur::Switch* >* copies, int id )
{
     this->mutex_->lock();

     /* Ensure that element exists in the container,  otherwise returns false. */
     if ( this->stack_ptr_->find ( id ) != this->stack_ptr_->end() ) {

          copies->at ( id )->arp_table_v4_ =
               this->stack_ptr_->at ( id )->arp_table_v4_;
          copies->at ( id )->arp_table_v6_ =
               this->stack_ptr_->at ( id )->arp_table_v6_;
     }

     this->mutex_->unlock();
}




bool derailleur::Application::learning_switch (
     const derailleur::Event* const event,
     uint16_t hard_timeout, uint16_t idle_timeout )
{

     bool flow_installed = false; /* value returned if no flow is installed.*/

     fluid_msg::PacketInCommon* packet_in = nullptr;
     uint32_t in_port;
     uint32_t out_port;


     /* Set the proper version of packet-in. */
     if ( event->get_version() ==  fluid_msg::of13::OFP_VERSION ) {

          fluid_msg::of13::PacketIn* p_in13 = new fluid_msg::of13::PacketIn();
          p_in13->unpack ( event->get_data() );
          in_port = p_in13->match().in_port()->value();
          packet_in = p_in13;

     } else {
          fluid_msg::of10::PacketIn* p_in10 = new fluid_msg::of10::PacketIn();
          in_port = p_in10->in_port();
          p_in10->unpack ( event->get_data() );

          packet_in = p_in10;
     }



     /* If destination MAC is already stored in the ARP table (v4 | v6) the
      * source is copied from ARP table and a flow is installed creating a
      * datapath from the source to the destination through the port where
      * destination is connected. */


     /* stores destination MAC copied with memcpy */
     uint8_t dst_mac[6];
     memcpy ( ( uint8_t* ) &dst_mac,
              ( uint8_t* ) packet_in->data(), 6 );


     // Lock to access the stack.
     this->mutex_->lock();
     // Lock to access the switch ARP-like table.
     //stack_ptr_->at ( event->get_switch_id() )->mutex_.lock();


     /* Get ARP table (v4 | v6) and search for destination MAC; when found
      * the port is copied to out_port. */
     int index;
     if ( event->get_ip_version() ==  derailleur::util::IP::v6 ) {

          derailleur::Arp6Table* arp_table =
               &stack_ptr_->at ( event->get_switch_id() )->arp_table_v6_;

          if ( ( index = search_MAC_in_table (
                              ( uint8_t* ) &dst_mac, arp_table ) ) >=  0 )
               out_port = arp_table->at ( index ).port;

     } else {

          derailleur::Arp4Table* arp_table =
               &stack_ptr_->at ( event->get_switch_id() )->arp_table_v4_;

          if ( ( index = search_MAC_in_table (
                              ( uint8_t* ) &dst_mac, arp_table ) ) >=  0 )
               out_port = arp_table->at ( index ).port;
     }


     /* If destination was found a flow is installed. */
     if ( index >=  0 ) {


          /* get source MAC */
          uint8_t src_mac[6];
          memcpy ( ( uint8_t* ) &src_mac,
                   ( uint8_t* ) packet_in->data() + 6, 6 );


          /* OpenFlow 1.3 */
          if ( event->get_version() ==  fluid_msg::of13::OFP_VERSION ) {

               fluid_msg::of13::FlowMod fm;
               fm.xid ( packet_in->xid() );
               fm.cookie ( 123 );
               fm.cookie_mask ( 0xffffffffffffffff );
               fm.table_id ( 0 );
               fm.command ( fluid_msg::of13::OFPFC_ADD );
               fm.idle_timeout ( idle_timeout );
               fm.hard_timeout ( hard_timeout );
               fm.priority ( 10 );
               fm.buffer_id ( packet_in->buffer_id() );
               fm.out_port ( 0 );
               fm.out_group ( 0 );
               fm.flags ( 0 );
               fluid_msg::of13::EthSrc source ( ( ( uint8_t* ) &src_mac ) );
               fluid_msg::of13::EthDst destination ( ( ( uint8_t* ) &dst_mac ) );
               fm.add_oxm_field ( source );
               fm.add_oxm_field ( destination );
               fluid_msg::of13::OutputAction act ( out_port, 1024 );
               fluid_msg::of13::ApplyActions inst;
               inst.add_action ( act );
               fm.add_instruction ( inst );

               // install flow
               stack_ptr_->at ( event->get_switch_id() )->install_flow ( &fm );

               flow_installed = true;
          }
          /* OpenFlow 1.0 */
          else {

          }

     }
     /* If destination is unknown (not found in ARP tables) the message is
      * flooded trying to find destination. */
     else {
          stack_ptr_->at ( event->get_switch_id() )->flood ( packet_in, in_port );
     }


     //stack_ptr_->at ( event->get_switch_id() )->mutex_.unlock();
     this->mutex_->unlock();

     delete packet_in;

     /* If program reached this point any device was learned. */
     return flow_installed;
}



std::vector< derailleur::Arp4 > derailleur::Application::get_IPv4_neighborhood (
     short int switch_id )
{
     this->mutex_->lock();
     std::vector<Arp4> arp =
          this->stack_ptr_->at ( switch_id )->get_IPv4_neighborhood ();
     this->mutex_->unlock();
     return arp;
}


std::vector< derailleur::Arp6 > derailleur::Application::get_IPv6_neighborhood (
     short int switch_id )
{
     this->mutex_->lock();
     std::vector<Arp6> arp =
          this->stack_ptr_->at ( switch_id )->get_IPv6_neighborhood ();
     this->mutex_->unlock();
     return arp;
}



