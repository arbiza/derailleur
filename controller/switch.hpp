
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


//TODO: remove
#include <string>

#include "message.hpp"


// Forward declaration
namespace fluid_base {
    class OFConnection;
}


namespace derailleur {

    
// Class comment: describe what it is for and how it should be used.
class Switch {

public:
    
    Switch(fluid_base::OFConnection* connection);

    void add_flow_normal();
	
	std::string ok() { return "lslslsls"; }
    
private:
    void add_flow_default();

    fluid_base::OFConnection* connection_;
};

} // namespace derailleur

#endif // _SWITCH_HPP_










