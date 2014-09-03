
/**
 *  File: switch.hpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  Distributed under the Lesser General Public License v3. 
 *  There is a copy of the license distributed with this software. It is also
 *  available at <https://www.gnu.org/licenses/lgpl.html>
 *
 **/



#ifndef _SWITCH_HPP_
#define _SWITCH_HPP_


#include <fluid/OFServer.hh>


namespace derailleur {

// Class comment: describe what it is for and how it should be used.
class Switch : public fluid_base::OFHandler {

public:
    Switch(fluid_base::OFConnection* connection,
	   fluid_base::OFHandler* handler) {
	this->connection_ = connection;
	this->handler_ = handler;
	add_flow_default();
    }

    

    // Method called when a new message arrives.
    // Overrides OFHandler connection_callback.
    // @param
    void message_callback(fluid_base::OFConnection* ofconn,
				  uint8_t type, void* data, size_t len) override;

    

    // This method manages switches connections. It is called when a switch
    // connection state changes.
    // Overrides OFHandler connection_callback.
    // @param ofcon switch connection pointer
    // @param type connection type identifier
    void connection_callback(fluid_base::OFConnection* ofconn,
				     fluid_base::OFConnection::Event type) override;

    void add_flow_normal();
    
private:
    void add_flow_default();

    fluid_base::OFConnection* connection_;
    fluid_base::OFHandler* handler_;

    
};

} // namespace derailleur

#endif // _SWITCH_HPP_










