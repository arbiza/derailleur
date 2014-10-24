
/**
*  File: application.h
*
*  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
*
*  Distributed under the Lesser General Public License v3.
*  There is a copy of the license distributed with this software. It is also
*  available at <https://www.gnu.org/licenses/lgpl.html>
*
**/



#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_


#include <map>
#include <string>
#include <vector>
#include <utility>
#include <mutex>

#include <fluid/OFServer.hh>
#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>

#include "switch.hpp"
#include "log.hpp"



namespace derailleur {

// Forward declarations
class InternalEvent;
class Event;
class Controller;
class Switch;

/**
 *
 * TODO About the virtual methods, there must be a comment instructing to use
 * threads for long operation avoiding to block message handling; this comment
 * must include an example.
 */
class Application {

public:
     Application ( const std::string name ) {
          this->name_ = name;
     }

     std::string get_name() const {
          return this->name_;
     }


     ////// (Pure) Abstract methods

     /**
      * Pure virtual method triggered by Controller when a switch connection
      * is established. This method MUST be implemented by Application children.
      * @param event Event object containing data sent by switch.
      * @see Event
      */
     virtual void on_switch_up ( const derailleur::Event* const event ) = 0;

     /**
      * Pure virtual method triggered by Controller when a switch disconnects.
      * This method MUST be implemented by Application children.
      * @param switch_id Controller sends the switch identifier.
      */
     virtual void on_switch_down ( const int switch_id ) = 0;

     /**
      * * Pure virtual method triggered by Controller when a switch send any
      * message but some messages handled by controller to create switch objects
      * with information retrieved from each connected switch.
      * This method MUST be implemented by Application children.
      * @param event Event object containing data to be used in message handling.
      * @see Event
      * @see Controller
      */
     virtual void message_handler ( const derailleur::Event* const event ) = 0;


     ////// Switches related methods

     /**
      * Return the number of switches on the stack.
      */
     size_t get_number_of_switches () {
          this->mutex_->lock();
          return this->stack_ptr_->size();
     }

     /**
      * Return a vector with all IDs of stacked switches.
      */
     std::vector<int> get_switches_IDs () {

          std::vector<int> ids;

          this->mutex_->lock();
          for ( auto& s : *this->stack_ptr_ )
               ids.push_back ( s.first );
          return ids;
     }


     //// Switch SETTERS
     void set_switch_name ( short switch_id, std::string name ) {
          this->mutex_->lock();
          this->stack_ptr_->at ( switch_id )->name_ = name;
          this->mutex_->unlock();
     }


     //// Switch GETTERS
     std::string get_switch_name ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->name_;
          this->mutex_->unlock();
     }

     int get_switch_version ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->of_version_;
          this->mutex_->unlock();
     }

     std::string get_switch_mac_address ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->mac_address_;
          this->mutex_->unlock();
     }

     uint64_t get_switch_datapath_id ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->datapath_id_;
          this->mutex_->unlock();
     }

     int get_switch_n_buffers ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->n_buffers_;
          this->mutex_->unlock();
     }

     int get_switch_n_tables ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->n_tables_;
          this->mutex_->unlock();
     }

     std::string get_switch_manufacturer ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->manufacturer_;
          this->mutex_->unlock();
     }

     std::string get_switch_hardware ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->hardware_;
          this->mutex_->unlock();
     }

     std::string get_switch_software ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->software_;
          this->mutex_->unlock();
     }

     std::string get_switch_serial_number ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->serial_number_;
          this->mutex_->unlock();
     }

     std::string get_switch_datapath ( short switch_id ) const {
          this->mutex_->lock();
          return this->stack_ptr_->at ( switch_id )->datapath_;
          this->mutex_->unlock();
     }

     /** Available only in OpenFlow 1.3 switch (possibly 1.3+) */
     uint8_t get_auxiliary_id ( short switch_id ) {

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

     derailleur::Switch* get_switch_copy ( short switch_id ) {

          // It is the default return; returned when any error occurs
          derailleur::Switch* s = nullptr;

          // OpenFlow 1.3
          if ( get_switch_version ( switch_id ) ==
                    ( int ) fluid_msg::of13::OFP_VERSION ) {

               derailleur::Switch13* s13_ptr =
                    static_cast<derailleur::Switch13*> (
                         this->stack_ptr_->at ( switch_id ) );

               /**
                * Set connection pointer to nullptr, this way Application children
                * won't be able to access the switch through connection pointer, only
                * through methods provided by this class.
                */
               s13_ptr->set_null_connection_ptr();

               derailleur::Switch13 s13 = *s13_ptr;
               return *s13;
          }
          // OpenFlow 1.0
          else if ( get_switch_version ( switch_id ) ==
                    ( int ) fluid_msg::of10::OFP_VERSION ) {

               derailleur::Switch10 s10 =
                    *static_cast<derailleur::Switch10> (
                         this->stack_ptr_->at ( switch_id ) );

               /**
                * Set connection pointer to nullptr, this way Application children
                * won't be able to access the switch through connection pointer, only
                * through methods provided by this class.
                */
               s10.set_null_connection_ptr();
               return &s10;
          }

          return s;
     }


private:

     /**
      * This method is called by controller. It sets a pointer to the siwthes
      * stack.
      */
     void set_stack_ptr ( std::map<int, derailleur::Switch*>* ptr ) {
          this->stack_ptr_ = ptr;
     }


     /*
      * Called by controller to set mutex_ pointer.
      */
     void set_mutex_ptr ( std::mutex* mutex ) {
          this->mutex_ = mutex;
     }


     /**
      * It in just the application name.
      */
     std::string name_;

     /**
      * This pointer is set by Controller class through set_stack_ptr method.
      * It is a pointer to a switches container. Due to the concorrency, it is
      * private and will be accessible to child classes only through public
      * methods.
      *
      * May occur an access attempt to an object just removed from the stack by
      * controller; std::map is exception safe so that it is not necessary to
      * worry about exceptions on map access.
      */
     std::map< int, derailleur::Switch* >* stack_ptr_;

     /*
      * Application class provides methods to access switches objects stacked
      * and managed by controller; mutex_ pointer avoid an switch being deleted
      * by controller while they are accessed by Application derived class.
      */
     std::mutex* mutex_;

     /**
      * Controller is a friend class to be able to access private methods such
      * as set_stack_ptr used to share stack pointer with this class.
      */
     friend class derailleur::Controller;
};

} // namespace derailleur

#endif // _APPLICATION_HPP_










