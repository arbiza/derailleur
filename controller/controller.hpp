
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


#include <thread>
#include <mutex>
#include <vector>

#include <fluid/OFServer.hh>

#include "switch.hpp"
#include "log.hpp"


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
                  derailleur::Application* application );



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
          return this->stack_.at ( switch_id ).get_pointer();
     }
     
     
     int get_threads_size () {
          return this->threads_.size();
     }


private:

     std::map<int, derailleur::Switch> stack_;

     derailleur::Application* application_;

     // Threads
     std::vector< std::thread > threads_;
     std::mutex mutex_;


     derailleur::Log log_;

};

} // namespace derailleur



#endif // _CONTROLLER_HPP_
