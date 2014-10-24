
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
     this->application_->set_mutex_ptr( &this->mutex_ );

     derailleur::Log::Instance()->open_log_file ( log_path );
}



void derailleur::Controller::message_callback (
     fluid_base::OFConnection *ofconn,
     uint8_t type,
     void *data,
     size_t length )
{
     switch ( type ) {

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
               new InternalEvent ( this, type, data, length ) );
          
          this->mutex_.unlock();

          this->application_->on_switch_up (
               new Event ( ofconn->get_id(), ofconn->get_version(), this,
                           type, data, length ) );
          break;


     default:

          this->application_->message_handler (
               new Event ( ofconn->get_id(), ofconn->get_version(), this,
                           type, data, length )
          );
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

          derailleur::Log::Instance()->log (
               "Controller",
               "Switch disconnected: EVENT_CLOSED" );
          // Delete the switch object corresponding to the connection from the
          // stack.
          this->mutex_.lock();
          stack_.erase ( ofconn->get_id() );
          this->mutex_.unlock();

     } else if ( type == fluid_base::OFConnection::EVENT_DEAD ) {

          derailleur::Log::Instance()->log (
               "Controller",
               "Switch disconnected: EVENT_DEAD" );
          // Delete the switch object corresponding to the connection from the
          // stack.
          this->mutex_.lock();
          stack_.erase ( ofconn->get_id() );
          this->mutex_.unlock();
     }
}

