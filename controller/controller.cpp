
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


#include <map>

#include "controller.hpp"
#include "message.hpp"



derailleur::Controller::Controller ( const char* address,
                                     const int port,
                                     const int n_workers,
                                     const bool secure,
                                     derailleur::Application* application )
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
     this->application_->set_rack_pointer ( &this->switches_rack_ );
}



void derailleur::Controller::message_callback (
     fluid_base::OFConnection *ofconn,
     uint8_t type,
     void *data,
     size_t len )
{
     switch ( type ) {

     case 0: // Switch sending description: OFTP_MULTIPART_REPLAY

          switches_rack_.at ( ofconn->get_id() ).
          handle_multipart_description_reply (
               new derailleur::Message ( this, type, data, len ) );
          break;

     case 6:	// Switch UP: OFTP_FEATURES_REPLAY
          //TODO: lock here
          switches_rack_.emplace ( std::make_pair ( int ( ofconn->get_id() ),
                                   derailleur::Switch ( ofconn ) ) );

          this->application_->on_switch_up ( ofconn->get_id() );
          //TODO: unlock here
          break;

     default:
          this->application_->message_handler (
               ofconn->get_id(),
               new derailleur::Message ( this, type, data, len ) );
          break;
     }
}



void derailleur::Controller::connection_callback ( fluid_base::OFConnection
          *ofconn, fluid_base::OFConnection::Event type )
{
     if ( type == fluid_base::OFConnection::EVENT_STARTED ) {
          // TODO: log
     } else if ( type == fluid_base::OFConnection::EVENT_ESTABLISHED ) {
          // TODO: log
     } else if ( type == fluid_base::OFConnection::EVENT_FAILED_NEGOTIATION ) {
          // TODO: log
     } else if ( type == fluid_base::OFConnection::EVENT_CLOSED ) {
          // TODO: log
          // TODO: Delete switch from rack
     } else if ( type == fluid_base::OFConnection::EVENT_DEAD ) {
          // TODO: log
          // TODO: Delete switch from rack
     }
}
