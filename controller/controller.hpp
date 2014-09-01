
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

#include <fluid/OFServer.hh>
#include <fluid/of13msg.hh>

#include "application.hpp"

namespace derailleur {

// This class inherits from fluid_base::OFServer ans implements Derailleur
// controller functionalities.  Usage: ...
class Controller : public fluid_base::OFServer {
    
public:

    // TODO: remove if not used!
    // Controller(const char* address = "0.0.0.0", const int port = 6653,
    // 	       const int n_workers = 4, bool secure = false)
    // 	: fluid_base::OFServer(address, port, n_workers, secure,
    // 			       fluid_base::OFServerSettings().supported_version(1).
    // 			       supported_version(4).keep_data_ownership(false))
    // {
    // 	this->running_ = true;
    // }

    //Controller(&Application Application)
    Controller()
	: fluid_base::OFServer("0.0.0.0", 6653, 4, false,
			       fluid_base::OFServerSettings().supported_version(1).supported_version(4).keep_data_ownership(false)) {};


    /* The following two methods override methods defined in
       fluid_base::OFServer class. They were defined in
       fluid_base::OFHandler which is inherited by fluid_base::OFServer.
    */


    // Method called when a new message arrives
    // @param
    virtual void message_callback(fluid_base::OFConnection* ofconn,
				  uint8_t type, void* data, size_t len);

    

    // This method manages switches connections. It is called when a switch
    // connection state changes.
    // @param ofcon switch connection pointer
    // @param type connection type identifier
    virtual void connection_callback(fluid_base::OFConnection* ofconn,
				     fluid_base::OFConnection::Event type);

                                                                      

    void teste();

private: bool running_;
    
};
    
} // namespace derailleur


#endif // _CONTROLLER_HPP_

