
#include <fluid/OFServer.hh>
#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>

#include "application.hpp"
#include "util.hpp"
#include "event.hpp"


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


void derailleur::Application::get_switch_copy ( short int switch_id,
          derailleur::Switch& other )
{
     this->mutex_->lock();

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

     this->mutex_->unlock();
}



bool derailleur::Application::learning_switch ( short int switch_id,
          const derailleur::Event* const event )
{

     fluid_msg::PacketInCommon* packet_in = nullptr;
     uint16_t port;

     /* First: Stores the input port; how it is done differs depending of
      * OpenFlow version. */
     if ( packet_in->version() ==  fluid_msg::of13::OFP_VERSION ) {
          fluid_msg::of13::PacketIn* p_in13 = new fluid_msg::of13::PacketIn();
          p_in13->unpack ( event->get_data() );
          packet_in = p_in13;
          port = p_in13->match().in_port()->value();

     } else {
          fluid_msg::of10::PacketIn* p_in10 = new fluid_msg::of10::PacketIn();
          p_in10->unpack ( event->get_data() );
          packet_in = p_in10;
          port = p_in10->in_port();
     }
     
     /* Check the link layer protocol (ARP or IPv6) */
     uint16_t link_layer = derailleur::util::get_link_layer_protocol (
          ( uint8_t* ) packet_in->data() );

     /* Second: Extracts MAC and IP address; it differs depending of IP version */

     /* Check if the link layer protocol is IPv6 (NDP/ICMPv6) */
     if ( link_layer == derailleur::util::Protocols.link_layer.ipv6 ) {

     }
     /* Check if the link layer protocol is ARP (used for neighborhood
      * discovering in IPv4). */
     else if ( link_layer == derailleur::util::Protocols.link_layer.arp ) {

          derailleur::Arp4 arp_entry;

          memcpy ( arp_entry.mac, ( uint8_t* ) packet_in->data() + 6, 6 );
          memcpy ( arp_entry.ip, ( uint8_t* ) packet_in->data() + 90, 4 );
          arp_entry.port = port;

          if ( stack_ptr_->at ( switch_id )->set_IPv4_neighbor (
                         &arp_entry ) ) {

          } else
               return false;
     }
     /* If neither ARP of ICMPv6 are used return false because it is not
      * a neighborhood discovering operation. */
     else {
          return false;
     }


//      // OpenFlow 1.3
//      if ( packet_in->version() ==  fluid_msg::of13::OFP_VERSION ) {
//
//           /* TODO:
//            *   - check ip version - OK
//            *   - get switch arp
//            *   - check if source in known (does IP and MAC match?):
//            *     - no: store de source in ARP table
//            *     - yes: check destination (if unknown, store)
//            *   - set flow
//            */
//
//
//           /* At first, it checks if the source is unknown; */
//
//           /* Second, checks if the destination is unknown. If the switch knows
//            * the destination, it does not need to flood to discover the
//            * destination. */
//
//      }
//      // OpenFlow 1.0
//      else if ( packet_in->version() ==  fluid_msg::of10::OFP_VERSION ) {
//
//      }

     return true;
}


std::list< derailleur::Arp4 > derailleur::Application::get_IPv4_neighborhood (
     short int switch_id )
{
     this->mutex_->lock();
     std::list<Arp4> arp =
          this->stack_ptr_->at ( switch_id )->get_IPv4_neighborhood ();
     this->mutex_->unlock();
     return arp;
}


std::list< derailleur::Arp6 > derailleur::Application::get_IPv6_neighborhood (
     short int switch_id )
{
     this->mutex_->lock();
     std::list<Arp6> arp =
          this->stack_ptr_->at ( switch_id )->get_IPv6_neighborhood ();
     this->mutex_->unlock();
     return arp;
}
