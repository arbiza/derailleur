
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

     // Abstract methods
     virtual void on_switch_up ( const derailleur::Event* const event ) = 0;

     virtual void on_switch_down ( const int switch_id ) = 0;

     /**
      *
      */
     virtual void message_handler ( const derailleur::Event* const event ) = 0;



     std::string get_name() const {
          return this->name_;
     }


     // Switches related methods
     derailleur::Switch* get_switch ( int switch_id ) {
          // TODO: lock here
          return this->stack_ptr_->at ( switch_id );
          // TODO: unlock here
     }

     // I will complete it later.
//      std::vector<derailleur::Switch*> get_switches_list () {
//           std::vector<derailleur::Switch*> switches;
//
//           // TODO: lock
//
//           // TODO: unlock
//      return std::move( switches );
//      }



     // Mutex related methods.
//      void lock () {
//           this->mutex_->lock();
//      }
// 
//      void unlock () {
//           this->mutex_->unlock();
//      }


private:

     // This method is called by controller. It sets a pointer to the siwthes
     // stack.
     void set_stack_ptr ( std::map<int, derailleur::Switch*>* ptr ) {
          this->stack_ptr_ = ptr;
     }


     /*
      * Called by controller to set mutex_ pointer.
      */
     void set_mutex_ptr ( std::mutex* mutex ) {
          this->mutex_ = mutex;
     }


     // This variable stores the application name.
     std::string name_;

     /**
      * This pointer is set by Controller class through set_stack_ptr method.
      * It is a pointer to a switches container. Due to the concorrency, it is
      * private and will be accessible to child classes only through public
      * methods.
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
      * as set_stack_ptr and set_mutex_ptr.
      */
     friend class derailleur::Controller;
};

} // namespace derailleur

#endif // _APPLICATION_HPP_










