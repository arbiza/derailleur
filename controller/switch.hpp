
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


#include <string>

#include <fluid/of13msg.hh>

#include "message.hpp"


// Forward declaration
namespace fluid_base {
class OFConnection;
}

// namespace fluid_msg {
// class SwitchDesc;
// }


namespace derailleur {

// Forward declaration
class Controller;


// Class comment: describe what it is for and how it should be used.
// TODO: This class MUST use exceptions, because the major part of operations won't 
class Switch {

public:

     Switch ( fluid_base::OFConnection* connection );

     bool install_flow ( fluid_msg::FlowModCommon flow );

     std::string get_name() {
          return this->name_;
     }

     // The following methods return attributes from switch_description_ object
     std::string get_manufacturer () const {
          return this->switch_description_.mfr_desc();
     }

     std::string get_hardware () const {
          return this->switch_description_.hw_desc();
     }

     std::string get_software () const {
          return this->switch_description_.sw_desc();
     }

     std::string get_serial_number () const {
          return this->switch_description_.serial_num();
     }

     std::string get_datapath () const {
          return this->switch_description_.dp_desc();
     }


private:

     // handlers:
     bool handle_multipart_description_reply ( derailleur::Message* message );

     //TODO: this method must return a flow to be installed
     void add_flow_default();

     fluid_base::OFConnection* connection_;
     fluid_msg::SwitchDesc switch_description_;

     std::string name_;

     friend class derailleur::Controller;
};

} // namespace derailleur

#endif // _SWITCH_HPP_










