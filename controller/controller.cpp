
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

#include "controller.hpp"
#include "application.hpp"
#include "event.hpp"



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
     this->application_->set_stack_ptr ( &this->stack_ );
}



void derailleur::Controller::message_callback (
     fluid_base::OFConnection *ofconn,
     uint8_t type,
     void *data,
     size_t length )
{     
     switch ( type ) {

     case 6: // Switch UP: OFTP_FEATURES_REPLAY
          this->log_.message_log ( "Controller", ofconn->get_id(), type );

          //TODO: lock here

          // stack the new switch (New Switch object is instantiated).
          stack_.emplace (
               std::make_pair (
                    int ( ofconn->get_id() ),
                    derailleur::SwitchFactory::create_switch (
                         ofconn->get_version(),
                         ofconn,
                         data ) ) );


          //TODO: unlock here
          break;


     case 19: // Switch sending description: OFTP_MULTIPART_REPLAY
          this->log_.message_log ( "Controller", ofconn->get_id(), type );

          stack_.at ( ofconn->get_id() )->multipart_description_reply (
               new InternalEvent ( this, type, data, length ) );

          this->application_->on_switch_up (
               new Event ( ofconn->get_id(), this, type, data, length ) );
          break;


     default:
          this->log_.message_log ( "Controller", ofconn->get_id(), type );
//           this->application_->message_handler (
//                ofconn->get_id(),
//                new derailleur::Message ( this, type, data, len ) );
          break;
     }
}



void derailleur::Controller::connection_callback (
     fluid_base::OFConnection *ofconn,
     fluid_base::OFConnection::Event type )
{

     if ( type == fluid_base::OFConnection::EVENT_STARTED ) {
          // TODO: log
     } else if ( type == fluid_base::OFConnection::EVENT_ESTABLISHED ) {
          // TODO: log
     } else if ( type == fluid_base::OFConnection::EVENT_FAILED_NEGOTIATION ) {
          // TODO: log
     } else if ( type == fluid_base::OFConnection::EVENT_CLOSED ) {
          // TODO: log
//           this->application_->del_switch ( ofconn->get_id() );
     } else if ( type == fluid_base::OFConnection::EVENT_DEAD ) {
          // TODO: log
//           this->application_->del_switch ( ofconn->get_id() );
     }
}

