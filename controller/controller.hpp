
/** * File: controller.hpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 **/



#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_


#include <mutex>
#include <vector>

#include <fluid/OFServer.hh>

#include "switch.hpp"


namespace derailleur {

// Forward declaration
class Application;

// This class inherits from fluid_base::OFServer ans implements Derailleur
// controller functionalities.  Usage: ...
class Controller : public fluid_base::OFServer {

public:

     // Contructor
     Controller ( const char* address,
                  const int port,
                  const int n_workers,
                  const bool secure,
                  derailleur::Application* application,
                  const char* log_path );



     // Method called when a new message arrives
     // @param
     void message_callback ( fluid_base::OFConnection* ofconn,
                             uint8_t type,
                             void* data,
                             size_t length ) override;



     // This method manages switches connections. It is called when a switch
     // connection state changes.
     // @param ofcon switch connection pointer
     // @param type connection type identifier
     void connection_callback (
          fluid_base::OFConnection* ofconn,
          fluid_base::OFConnection::Event type ) override;



     const derailleur::Switch* get_switch_ptr ( const int switch_id ) {
          return this->stack_.at ( switch_id );
     }


     size_t get_stack_size () {
          this->mutex_.lock();
          size_t aux = this->stack_.size();
          this->mutex_.unlock();
          return aux;
     }


private:
     
     /**
      * This method stores MAC, IP and the port of the source device in the 
      * ARP-like table according to the packet-in IP version. With this method
      * controller knows every device connected to each switch.
      */
     void knows_source_device ( derailleur::Event* event );

     std::map<int, derailleur::Switch*> stack_;

     derailleur::Application* application_;

     /**
      * fluid_base::OFServer provides methods to lock and to unlock program
      * execution, but using it blocks the program when it is not necessary (e.g.
      * to manage containers); this attribute is used to prevent illegal access
      * to the stack_ attribute where switches objects are stacked.
      */
     std::mutex mutex_;

};

} // namespace derailleur



#endif // _CONTROLLER_HPP_
