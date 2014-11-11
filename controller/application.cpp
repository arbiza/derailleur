
#include <fluid/OFServer.hh>
#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>

#include "application.hpp"


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


derailleur::Switch* derailleur::Application::get_switch_copy ( short int switch_id )
{
     // It is the default return; returned when any error occurs
     derailleur::Switch* s = nullptr;

     // OpenFlow 1.3
     if ( get_switch_version ( switch_id ) ==
               fluid_msg::of13::OFP_VERSION ) {

          this->mutex_->lock();

          derailleur::Switch13* s13_ptr =
               static_cast<derailleur::Switch13*> (
                    this->stack_ptr_->at ( switch_id ) );

          this->mutex_->unlock();

          /**
           * Set connection pointer to nullptr, this way Application children
           * won't be able to access the switch through connection pointer, only
           * through methods provided by this class.
           */
          s13_ptr->set_null_connection_ptr();

          // Copy the content of the pointer to and object
          derailleur::Switch13 s13 = *s13_ptr;
          // s receives a pointer to the copied object (it is not a pointer
          // to the switch object on the stack).
          s = &s13;
     }
     // OpenFlow 1.0
     else if ( get_switch_version ( switch_id ) ==
               fluid_msg::of10::OFP_VERSION ) {

          this->mutex_->lock();

          derailleur::Switch10* s10_ptr =
               static_cast<derailleur::Switch10*> (
                    this->stack_ptr_->at ( switch_id ) );

          this->mutex_->unlock();

          s10_ptr->set_null_connection_ptr();
          derailleur::Switch10 s10 = *s10_ptr;
          s = &s10;
     }

     return s;
}


bool derailleur::Application::learning_switch ( short int switch_id, fluid_msg::PacketInCommon* packet_in )
{

}
