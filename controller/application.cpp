
// TODO: remove
#include <sstream>

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

     bool learned = false;
     fluid_msg::PacketInCommon* packet_in = nullptr;
     uint8_t* dst_mac;
     uint32_t port;


     /* First: Stores the input port; it differs from one OpenFlow version from
      * other. */
     if ( event->get_version() ==  fluid_msg::of13::OFP_VERSION ) {
          fluid_msg::of13::PacketIn* p_in13 = new fluid_msg::of13::PacketIn();
          p_in13->unpack ( event->get_data() );

          if ( p_in13->match().in_port() ==  NULL )
               return false;
          else
               port = p_in13->match().in_port()->value();

          packet_in = p_in13;

     } else {
          fluid_msg::of10::PacketIn* p_in10 = new fluid_msg::of10::PacketIn();
          p_in10->unpack ( event->get_data() );

          port = p_in10->in_port();

          packet_in = p_in10;
     }


     /* Check the link layer protocol. If ARP it is an IPv4 packet; if IPv6
      * it is an ICMPv6 packet (Neighbor Discovery Protocol). */
     uint16_t link_layer = derailleur::util::get_link_layer_protocol (
                                ( uint8_t* ) packet_in->data() );


     /* ************* IPv6 (ICMPv6) ************* */

     if ( link_layer == derailleur::util::Protocols.link_layer.ipv6 ) {




     }
     /* ************* IPv4 (ARP) ************* */
     else if ( link_layer == derailleur::util::Protocols.link_layer.arp ) {

          derailleur::Arp4 arp_entry;

          // Set fields
          memcpy ( ( uint8_t* ) &dst_mac,
                   ( uint8_t* ) packet_in->data(), 6 );
          memcpy ( ( uint8_t* ) &arp_entry.mac,
                   ( uint8_t* ) packet_in->data() + 6, 6 );
          memcpy ( ( uint8_t* ) &arp_entry.ip,
                   ( uint8_t* ) packet_in->data() + 28, 4 );
          arp_entry.port = port;



          // Lock to access the stack.
          this->mutex_->lock();
          // Lock to access the switch ARP-like table.
          //stack_ptr_->at ( switch_id )->mutex_.lock();



          std::vector<derailleur::Arp4> table =
               stack_ptr_->at ( switch_id )->arp_table_v4_;


          /* SOURCE */

          /* Check if the source is already stored in the ARP-like table; if not
           * stores the source. */
          derailleur::Log::Instance()->log ( "Application", "4.0" );
          if ( searche_MAC_in_table ( arp_entry.mac, table ) == -1 ) {
               stack_ptr_->at ( switch_id )->arp_table_v4_.push_back ( arp_entry );
               learned = true;
          }
          derailleur::Log::Instance()->log ( "Application", "4.1" );


          /* DESTINATION */

          /* If destination is broadcast (ff:ff:ff:ff:ff:ff) the message is
           * flooded through all switch ports. */
          uint8_t* broadcast = new uint8_t[6];
          for ( short i = 0; i < 6; i++ )
               broadcast[0] = 255;                           // ff

          if ( derailleur::util::compare_byte_arrays (
                         ( uint8_t* ) &dst_mac,
                         ( uint8_t* ) broadcast,
                         6 ) ) {

               // TODO remover
               derailleur::Log::Instance()->log ( "Application", "1.0" );
               stack_ptr_->at ( switch_id )->flood ( packet_in, port );
               derailleur::Log::Instance()->log ( "Application", "1.1" );

          }
          /* Search for destination in ARP-like table. If destination is known
           * a flow is installed; if not the message is flooded. */
          else {
               derailleur::Log::Instance()->log ( "Application", "5.0" );
               //  TODO problem aqui! talvez dst_mac esteja vazio
               int index = searche_MAC_in_table ( dst_mac, table );
               derailleur::Log::Instance()->log ( "Application", "5.1" );

               if ( index != -1 ) {

                    // TODO remover
                    derailleur::Log::Instance()->log ( "Application", "2.0" );

                    /* OpenFlow 1.3 */
                    if ( event->get_version() ==  fluid_msg::of13::OFP_VERSION ) {

                         fluid_msg::of13::FlowMod fm;
                         fm.xid ( packet_in->xid() );
                         fm.cookie ( 123 );
                         fm.cookie_mask ( 0xffffffffffffffff );
                         fm.table_id ( 0 );
                         fm.command ( fluid_msg::of13::OFPFC_ADD );
                         fm.idle_timeout ( 5 );
                         fm.hard_timeout ( 10 );
                         fm.priority ( 10 );
                         fm.buffer_id ( packet_in->buffer_id() );
                         fm.out_port ( 0 );
                         fm.out_group ( 0 );
                         fm.flags ( 0 );
                         fluid_msg::of13::EthSrc fsrc (
                              ( ( uint8_t* ) &arp_entry.mac ) );
                         fluid_msg::of13::EthDst fdst (
                              ( ( uint8_t* ) &table[index].mac ) );
                         fm.add_oxm_field ( fsrc );
                         fm.add_oxm_field ( fdst );
                         fluid_msg::of13::OutputAction act ( port, 1024 );
                         fluid_msg::of13::ApplyActions inst;
                         inst.add_action ( act );
                         fm.add_instruction ( inst );

                         // install flow
                         stack_ptr_->at ( switch_id )->install_flow ( &fm );
                         derailleur::Log::Instance()->log ( "Application", "2.1" );

                    }
                    /* OpenFlow 1.0 */
                    else {

                    }

               } else {
                    // TODO remover
                    derailleur::Log::Instance()->log ( "Application", "3.0" );
                    // floods discovery message (ARP) to discover the destination
                    stack_ptr_->at ( switch_id )->flood ( packet_in, port );
                    derailleur::Log::Instance()->log ( "Application", "3.1" );
               }
          } /* ends else */
     } /* ends IPv4 (ARP) */


     //stack_ptr_->at ( switch_id )->mutex_.unlock();
     this->mutex_->unlock();

     /* If program reached this point any device was learned. */
     return learned;
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
