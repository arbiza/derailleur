
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


#include <iostream>
#include <vector>
#include <thread>
#include <fluid/OFServer.hh>



// It is used when controller is started (by OFServer). The controller
// will run on only 1 thread, because it will run each switch connection
// in a separated thread.
const int server_n_threads_ = 1;



class thread; // forward declaration


namespace derailleur {    

// This class inherits from fluid_base::OFServer ans implements Derailleur
// controller functionalities.  Usage: ...
class Controller : public fluid_base::OFServer {
    
public:

    // Constructor with parameters
    Controller(const char* address, const int port, bool secure)
    	: fluid_base::OFServer(address, port, server_n_threads_, secure,
    			       fluid_base::OFServerSettings().supported_version(1).
    			       supported_version(4).keep_data_ownership(false)) {};

	  
    
    Controller()
	: fluid_base::OFServer("0.0.0.0", 6653, server_n_threads_, false,
			       fluid_base::OFServerSettings().supported_version(1).supported_version(4).keep_data_ownership(false)) {};



    // Method called when a new message arrives
    // @param
    void message_callback(fluid_base::OFConnection* ofconn,
			  uint8_t type, void* data, size_t len) override;

    

    // This method manages switches connections. It is called when a switch
    // connection state changes.
    // @param ofcon switch connection pointer
    // @param type connection type identifier
    void connection_callback(fluid_base::OFConnection* ofconn,
			     fluid_base::OFConnection::Event type) override;

private:

    // This vector stores the threads over witch the Switches objects are
    // running.
    // TODO: maybe a map is batter than vector (I'll need to release)
    std::vector<std::thread> switches_;


    void new_switch(fluid_base::OFConnection* ofconn,
		    fluid_base::OFHandler* handler);
};
    
} // namespace derailleur


#endif // _CONTROLLER_HPP_






