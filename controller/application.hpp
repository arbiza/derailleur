
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

#include <fluid/OFServer.hh>

#include "switch.hpp"
#include "log.hpp"


namespace derailleur {

// Forward declaration
class InternalEvent;
class Event;
class Controller;
class Switch;


// It's an interface class
class Application {

public:
     Application ( const std::string name ) {
          this->name_ = name;
     }

     // Abstract methods
     virtual void on_switch_up ( const derailleur::Event* const event ) {}

     virtual void on_switch_down ( const int switch_id ) {}

     virtual void message_handler ( const derailleur::Event* const event ) {}



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


protected:
     derailleur::Log log_;

private:

     // This method is called by controller. It sets a pointer to the siwthes
     // stack.
     void set_stack_ptr ( std::map<int, derailleur::Switch*>* ptr ) {
          this->stack_ptr_ = ptr;
     }



     // This variable stores the application name.
     std::string name_;

     // This pointer is set by Controller class through set_stack_ptr method.
     // It is a pointer to a switches container. Due to the concorrency, it is
     // private and will be accessible to child classes only through public
     // methods.
     std::map< int, derailleur::Switch* >* stack_ptr_;

     // Controller class is a friend class to be able to access set_stack and
     // set_locker private methods.
     friend class derailleur::Controller;
};

} // namespace derailleur

#endif // _APPLICATION_HPP_










