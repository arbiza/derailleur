
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



#include "switch.hpp"


namespace fluid_msg {

// Forward declarations
class PacketInCommon;
}


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
          size_t aux = this->stack_ptr_->size();
          this->mutex_->unlock();
          return aux;
     }

     /**
      * Return a vector with all IDs of stacked switches.
      */
     std::vector<int> get_switches_IDs ();


     //// Switch SETTERS

     void set_switch_name ( short switch_id, std::string name ) {
          this->mutex_->lock();
          this->stack_ptr_->at ( switch_id )->name_ = name;
          this->mutex_->unlock();
     }


     //// Switch GETTERS

     std::string get_switch_name ( short switch_id ) const {
          this->mutex_->lock();
          std::string aux = this->stack_ptr_->at ( switch_id )->name_;
          this->mutex_->unlock();
          return aux;
     }

     uint8_t get_switch_version ( short switch_id ) const {
          this->mutex_->lock();
          uint8_t aux = this->stack_ptr_->at ( switch_id )->of_version_;
          this->mutex_->unlock();
          return aux;
     }

     std::string get_switch_mac_address ( short switch_id ) const {
          this->mutex_->lock();
          std::string aux = this->stack_ptr_->at ( switch_id )->mac_address_;
          this->mutex_->unlock();
          return aux;
     }

     uint64_t get_switch_datapath_id ( short switch_id ) const {
          this->mutex_->lock();
          uint64_t aux = this->stack_ptr_->at ( switch_id )->datapath_id_;
          this->mutex_->unlock();
          return aux;
     }

     uint32_t get_switch_n_buffers ( short switch_id ) const {
          this->mutex_->lock();
          uint32_t aux = this->stack_ptr_->at ( switch_id )->n_buffers_;
          this->mutex_->unlock();
          return aux;
     }

     uint8_t get_switch_n_tables ( short switch_id ) const {
          this->mutex_->lock();
          uint8_t aux = this->stack_ptr_->at ( switch_id )->n_tables_;
          this->mutex_->unlock();
          return aux;
     }

     std::string get_switch_manufacturer ( short switch_id ) const {
          this->mutex_->lock();
          std::string aux = this->stack_ptr_->at ( switch_id )->manufacturer_;
          this->mutex_->unlock();
          return aux;
     }

     std::string get_switch_hardware ( short switch_id ) const {
          this->mutex_->lock();
          std::string aux = this->stack_ptr_->at ( switch_id )->hardware_;
          this->mutex_->unlock();
          return aux;
     }

     std::string get_switch_software ( short switch_id ) const {
          this->mutex_->lock();
          std::string aux = this->stack_ptr_->at ( switch_id )->software_;
          this->mutex_->unlock();
          return aux;
     }

     std::string get_switch_serial_number ( short switch_id ) const {
          this->mutex_->lock();
          std::string aux = this->stack_ptr_->at ( switch_id )->serial_number_;
          this->mutex_->unlock();
          return aux;
     }

     std::string get_switch_datapath ( short switch_id ) const {
          this->mutex_->lock();
          std::string aux = this->stack_ptr_->at ( switch_id )->datapath_;
          this->mutex_->unlock();
          return aux;
     }

     /** Available only in OpenFlow 1.3 switch (possibly 1.3+) */
     uint8_t get_auxiliary_id ( short switch_id );



     /**
      * Returns a copy of the switch without connection pointer. This method
      * copies all values of the switch object, but not the connection pointer
      * to avoid trying attempts to use a connection pointer that may not exist
      * anymore due to a disconnection. Values copied are that common for all
      * OpenFlow versions, the ones included in Switch class.
      */
     void get_switch_copy ( short switch_id, derailleur::Switch& other );


     ////// L2/L3 Switching related methods
     /**
      * This method extracts source IP(v4 | v6) and MAC addresses from the data
      * parameter, check if it is already present in the swith's ARP-like tables;
      * if not a new entry is added in the proper table (v4 | v6); if the entry
      * exists with the same MAC and IP this method does nothing, otherwise
      * overwrites.
      *
      *TODO: comment and implement about installing flow.
      *
      * @param switch_id switch identifier
      * @param data data from an OpenFlow packet; Packet_in class from libfluid
      * provides the data() method that may be used as parameter when calling this
      * method.
      * @return returns true if a new connected device was added to any of the
      * ARP-like tables; otherwise return false.
      * @see Event
      */
     bool learning_switch ( short switch_id,
                            fluid_msg::PacketInCommon* packet_in );
                            
                            
     /**
      * Returns a copy of the ARP-like table for IPv4 connections of switch 
      * identified by parameter switch_id.
      */
     std::list<derailleur::Arp4> get_IPv4_neighborhood ( short switch_id );
     

     /**
      * Returns a copy of the ARP-like table for IPv6 connections of switch 
      * identified by parameter switch_id.
      */
     std::list<derailleur::Arp6> get_IPv6_neighborhood ( short switch_id );


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










