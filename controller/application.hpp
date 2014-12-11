
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
#include "util.hpp"


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
     virtual void on_switch_down ( derailleur::Switch* switch_down ) = 0;

     /**
      * Pure virtual method triggered by Controller when a packet-in is received.
      * This method MUST be implemented by Application children.
      * @param switch_id Controller sends the switch identifier.
      */
     virtual void on_packet_in ( const derailleur::Event* const event ) = 0;

     /**
      * * Pure virtual method triggered by Controller when a switch sends any
      * message not handled by the tree previous methods.
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
      * Copy the switch identified by switch_id to other pointer. The copy has
      * not a connection pointer. It is not possible to interact with the switch
      * through the copied object.
      * @param switch_id switch identifier.
      * @param other object to where the switch is copied.
      * @return true if a copy is made; false if switch is not yet connected
      * and copy was not made.
      */
     bool get_switch_copy ( short switch_id, derailleur::Switch& other );
     bool get_switch_copy ( short switch_id, derailleur::Switch* other );
     
     
     /**
      * Return a pointer to map container with copies of all connected switches. 
      * Switches copies are pointers to a copy,  but not to the switch object.
      */
     void get_switches_copies ( std::map< int, derailleur::Switch* >* copies );


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
      * @param event Event pointer.
      * @return true if a flow was installed; false if not or if something gone
      * wrong (e.g. packet corrupted).
      * @see Event
      */
     bool learning_switch ( const derailleur::Event* const event,
                            uint16_t hard_timeout, uint16_t idle_timeout );


     /**
      * Returns a copy of the ARP-like table for IPv4 connections of switch
      * identified by parameter switch_id.
      */
     std::vector<derailleur::Arp4> get_IPv4_neighborhood ( short switch_id );


     /**
      * Returns a copy of the ARP-like table for IPv6 connections of switch
      * identified by parameter switch_id.
      */
     std::vector<derailleur::Arp6> get_IPv6_neighborhood ( short switch_id );


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
      * Searches for a MAC address in an ARP-like table.
      * @param mac uint8_t pointer to the MAC to be searched
      * @param table reference to the ARP-like table to be inspected
      * @return index of the found element; if not found returns -1
      * @see Switch
      */
     template <class TypeArp>
     static int search_MAC_in_table ( const uint8_t* mac,
                                      const std::vector<TypeArp>* table ) {

          for ( int i = 0; i < ( int ) table->size(); i++ ) {
               if ( derailleur::util::compare_byte_arrays ( mac,
                         table->at ( i ).mac, 6 ) )
                    return i;
          }

          return -1;
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










