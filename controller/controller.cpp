
/**
 *  File: controller.cpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 *
 *  This file is an implementation of the class(es) defined in
 *  controller.hpp header file.
 *
 *  See the header file for documentation and license information.
 *
 **/

#include <string>
#include <utility>
#include <vector>

#include "controller.hpp"
#include "application.hpp"
#include "event.hpp"
#include "log.hpp"



derailleur::Controller::Controller ( const char* address,
                                     const int port,
                                     const int n_workers,
                                     const bool secure,
                                     derailleur::Application* application,
                                     const char* log_path )
     : fluid_base::OFServer ( address,
                              port,
                              n_workers,
                              secure,
                              fluid_base::OFServerSettings().
                              supported_version ( 1 ).
                              supported_version ( 4 ).
                              keep_data_ownership ( false ) )
{
     this->application_ = application;
     this->application_->set_stack_ptr ( &this->stack_ );
     this->application_->set_mutex_ptr ( &this->mutex_ );

     derailleur::Log::Instance()->open_log_file ( log_path );
}


derailleur::Controller::~Controller()
{
     Log::Instance()->log ( "Controller", "CONTROLLER EXECUTION FINISHED." );
}



void derailleur::Controller::message_callback (
     fluid_base::OFConnection *ofconn,
     uint8_t type,
     void* data,
     size_t length )
{

     switch ( type ) {

     case 10: // packet-in
          learn_source_device ( new Event (
                                     this, ofconn->get_id(),
                                     ofconn->get_version(),
                                     type, data, length ) );

          this->application_->on_packet_in ( new Event (
                                                  this, ofconn->get_id(),
                                                  ofconn->get_version(),
                                                  type, data, length ) );

          break;

     case 6: // Switch UP: OFTP_FEATURES_REPLAY
          Log::Instance()->log ( "Controller",
                                 "Message type 6 - OFTP_FEATURES_REPLAY" );

          // Lock preventing ilegal memory access
          this->mutex_.lock();

          // stack the new switch (New Switch object is instantiated).
          stack_.emplace (
               std::make_pair (
                    int ( ofconn->get_id() ),
                    derailleur::SwitchFactory::create_switch (
                         ofconn->get_version(),
                         ofconn,
                         data ) ) );

          this->mutex_.unlock();

          break;


     case 19: // Switch sending description: OFTP_MULTIPART_REPLAY

          Log::Instance()->log ( "Controller",
                                 "Message type 19 - OFTP_MULTIPART_REPLAY" );
          this->mutex_.lock();

          stack_.at ( ofconn->get_id() )->multipart_description_reply (
               new Event ( this, ofconn->get_id(), ofconn->get_version(),
                           type, data, length ) );

          this->mutex_.unlock();

          this->application_->on_switch_up (
               new Event ( this, ofconn->get_id(), ofconn->get_version(),
                           type, data, length ) );
          break;


     default:

          /* If packet was not handled in the previous conditions it triggers
           * the default method (Application child handles the packet). */
          this->application_->message_handler (
               new Event ( this, ofconn->get_id(), ofconn->get_version(),
                           type, data, length ) );
          break;
     }
}



void derailleur::Controller::connection_callback (
     fluid_base::OFConnection *ofconn,
     fluid_base::OFConnection::Event type )
{

     if ( type == fluid_base::OFConnection::EVENT_STARTED ) {

          derailleur::Log::Instance()->log (
               "Controller",
               "Switch attempting to connect: EVENT_STARTED" );

     } else if ( type == fluid_base::OFConnection::EVENT_ESTABLISHED ) {

          derailleur::Log::Instance()->log (
               "Controller",
               "Switch's connection established: EVENT_ESTABLISHED" );

     } else if ( type == fluid_base::OFConnection::EVENT_FAILED_NEGOTIATION ) {

          derailleur::Log::Instance()->log (
               "Controller",
               "Switch's connection attempt failed: EVENT_FAILED_NEGOTIATION" );

     } else if ( type == fluid_base::OFConnection::EVENT_CLOSED ) {

          if ( ofconn->get_version() == fluid_msg::of13::OFP_VERSION ) {
               derailleur::Switch13 s;
               this->application_->get_switch_copy ( ofconn->get_id(), s );
               this->application_->on_switch_down ( &s );

          } else {
               derailleur::Switch10 s;
               this->application_->get_switch_copy ( ofconn->get_id(), s );
               this->application_->on_switch_down ( &s );
          }

          // Delete the switch object corresponding to the connection from the
          // stack.
          this->mutex_.lock();
          stack_.erase ( ofconn->get_id() );
          this->mutex_.unlock();

          derailleur::Log::Instance()->log (
               "Controller",
               "Switch disconnected: EVENT_CLOSED" );

     } else if ( type == fluid_base::OFConnection::EVENT_DEAD ) {

          if ( ofconn->get_version() == fluid_msg::of13::OFP_VERSION ) {
               derailleur::Switch13 s;
               this->application_->get_switch_copy ( ofconn->get_id(), s );
               this->application_->on_switch_down ( &s );

          } else {
               derailleur::Switch10 s;
               this->application_->get_switch_copy ( ofconn->get_id(), s );
               this->application_->on_switch_down ( &s );
          }

          // Delete the switch object corresponding to the connection from the
          // stack.
          this->mutex_.lock();
          stack_.erase ( ofconn->get_id() );
          this->mutex_.unlock();

          derailleur::Log::Instance()->log (
               "Controller",
               "Switch disconnected: EVENT_DEAD" );
     }
}



void derailleur::Controller::learn_source_device (
     const derailleur::Event* const event )
{
     /* Check if the source is already stored in the ARP-like table; if not
      * stores the source. */

     fluid_msg::PacketInCommon* packet_in;

     uint32_t in_port;


     /* Extract device information (MAC, IP, port) */


     /* Port is the first because it depends of the proper version of packet-in */
     if ( event->get_version() ==  fluid_msg::of13::OFP_VERSION ) {

          fluid_msg::of13::PacketIn* p_in13 = new fluid_msg::of13::PacketIn();
          p_in13->unpack ( event->get_data() );

          if ( p_in13->match().in_port() !=  NULL ) {
               in_port = p_in13->match().in_port()->value();
               packet_in = p_in13;
          } else
               return;

     } else {
          fluid_msg::of10::PacketIn* p_in10 = new fluid_msg::of10::PacketIn();
          p_in10->unpack ( event->get_data() );

          in_port = p_in10->in_port();

          packet_in = p_in10;
     }


     /* IPv6 */
     if ( event->get_ip_version() == derailleur::util::IP::v6 ) {

          derailleur::Arp6 source;

          // IP
          memcpy ( ( uint16_t* ) &source.ip,
                   ( uint16_t* ) packet_in->data() + 23, 8 );

          // MAC
          memcpy ( ( uint8_t* ) &source.mac,
                   ( uint8_t* ) packet_in->data() + 6, 6 );

          // Port
          source.port = in_port;


          /* Get the ARP table pointer to search for device. */
          this->mutex_.lock();

          std::vector<derailleur::Arp6>* arp_table =
               &stack_.at ( event->get_switch_id() )->arp_table_v6_;


          /* If device is unknown the new device will be stored in the ARP table. */
          int index;
          if ( ( index = derailleur::Application::search_MAC_in_table (
                              ( uint8_t* ) source.mac, arp_table ) ) < 0 ) {

               arp_table->push_back ( source );
          }
          /* If device is known its entry is updated because it IP or port may
           * changed. */
          else {
               // update IP
               memcpy ( ( uint16_t* ) arp_table->at ( index ).ip,
                        ( uint16_t* ) source.ip, 8 );

               // update port
               arp_table->at ( index ).port = source.port;
          }

          this->mutex_.unlock();
     }
     /* IPv4 */
     else {
          derailleur::Arp4 source;

          // IP
          memcpy ( ( uint8_t* ) &source.ip,
                   ( uint8_t* ) packet_in->data() + 26, 4 );

          // MAC
          memcpy ( ( uint8_t* ) &source.mac,
                   ( uint8_t* ) packet_in->data() + 6, 6 );

          // Port
          source.port = in_port;


          /* Get the ARP table pointer to search for device. */
          this->mutex_.lock();

          std::vector<derailleur::Arp4>* arp_table =
               &stack_.at ( event->get_switch_id() )->arp_table_v4_;


          /* If device is unknown the new device will be stored in the ARP table. */
          int index = derailleur::Application::search_MAC_in_table (
                           ( uint8_t* ) source.mac, arp_table );

          if ( index < 0 ) {
               arp_table->push_back ( source );
          }
          /* If device is known its entry is updated because it IP or port may
           * changed. */
          else {
               // update IP
               memcpy ( ( uint8_t* ) arp_table->at ( index ).ip,
                        ( uint8_t* ) source.ip, 4 );

               // update port
               arp_table->at ( index ).port = source.port;
          }

          this->mutex_.unlock();
     }

     delete packet_in;

     /* Event can not be deleted here because it is a pointer and is used after
      * here in on_packet_in method. */
     // delete event;
}
