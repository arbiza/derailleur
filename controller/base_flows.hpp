
/**
 *  File: base_flows.hpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  Distributed under the Lesser General Public License v3. 
 *  There is a copy of the license distributed with this software. It is also
 *  available at <https://www.gnu.org/licenses/lgpl.html>
 *
 **/

#include <fluid/OFServer.hh>
#include <fluid/of13msg.hh>


namespace derailleur {

inline

namespace flows {

    // Normal flow - makes the OpenFlow switch act as a common switch.
    void normal(fluid_base::OFConnection* ofconn) {
	uint8_t* buffer;
	fluid_msg::of13::FlowMod fm(42, 0, 0xffffffffffffffff, 0,
				    fluid_msg::of13::OFPFC_ADD, 0, 0, 0,
				    0xffffffff, 0, 0, 0);
	fluid_msg::of13::OutputAction *act =
	    new fluid_msg::of13::OutputAction(fluid_msg::of13::OFPP_CONTROLLER,
					      fluid_msg::of13::OFPCML_NO_BUFFER);
	fluid_msg::of13::ApplyActions *inst =
	    new fluid_msg::of13::ApplyActions();
        inst->add_action(act);
	fm.add_instruction(inst);
	buffer = fm.pack();
        ofconn->send(buffer, fm.length());
	fluid_msg::OFMsg::free_buffer(buffer);
    }
    
} // namespace flows
} // namespace derailleur
