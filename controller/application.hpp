
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

#include <fluid/OFServer.hh>

#include "switch.hpp"

// namespace fluid_base {
//      // Forward declaration
//      class OFConnection;
// }


namespace derailleur {

// Forward declaration
class Message;
class Controller;


// It's an interface class
class Application {

public:
     Application ( const std::string name ) {
          this->name_ = name;
     }

// Abstract methods
     virtual void on_switch_up ( const int switch_id ) {}

     virtual void on_switch_down ( const int switch_id ) {}

     virtual void message_handler ( const int switch_id,
                                    derailleur::Message* const message ) {}

     std::string get_name() const {
          return this->name_;
     }
     
     derailleur::Switch* get_switch ( const int switch_id ) {
          return &this->switch_stack_.at(switch_id);
     }


private:
     // Methods used by friend class Controller. They need be implemented in the
     // header file otherwise it won't compile.
     virtual bool add_switch ( fluid_base::OFConnection* ofconn ) final {
          switch_stack_.emplace ( std::make_pair (
               int ( ofconn->get_id() ),
               derailleur::Switch ( ofconn ) ) );
          on_switch_up ( ofconn->get_id() );

          return true;
     }
     //virtual void add_switch_multipart_desc(derailleur::Message* message) final;

     virtual bool del_switch ( const int connection_id ) final {
          return true;
     }

     std::string name_;
     std::map<int, derailleur::Switch> switch_stack_;

     friend class derailleur::Controller;
};

} // namespace derailleur

#endif // _APPLICATION_HPP_










