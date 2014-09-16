
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
#include <fluid/of10msg.hh>

#include "log.hpp"


// Forward declaration
namespace fluid_base {
// Forward declaration
class OFConnection;
}


namespace derailleur {

// Forward declaration
class Controller;
class InternalEvent;
class Event;



// Capabilities structures
typedef struct capabilities_13 {
     
     /* enum ofp_capabilities {
      * OFPC_FLOW_STATS   = 1 << 0, // Flow statistics.
      * OFPC_TABLE_STATS  = 1 << 1, // Table statistics.
      * OFPC_PORT_STATS   = 1 << 2, // Port statistics.
      * OFPC_GROUP_STATS  = 1 << 3, // Group statistics.
      * OFPC_IP_REASM     = 1 << 5, // Can reassemble IP fragments.
      * OFPC_QUEUE_STATS  = 1 << 6, // Queue statistics.
      * OFPC_PORT_BLOCKED = 1 << 8 // Switch will block looping ports.
      */
     
     bool OFPC_FLOW_STATS,
          OFPC_TABLE_STATS,
          OFPC_PORT_STATS,
          OFPC_GROUP_STATS,
          OFPC_IP_REASM,
          OFPC_QUEUE_STATS,
          OFPC_PORT_BLOCKED;
          
} Capabilities_13_;


typedef struct capabilities_10 {

     /* enum ofp_capabilities {
      * OFPC_FLOW_STATS   = 1 << 0, // Flow statistics.
      * OFPC_TABLE_STATS  = 1 << 1, // Table statistics.
      * OFPC_PORT_STATS   = 1 << 2, // Port statistics.
      * OFPC_STP          = 1 << 3, // 802.1d spanning tree.
      * OFPC_RESERVED     = 1 << 4, // Reserved, must be zero.
      * OFPC_IP_REASM     = 1 << 5, // Can reassemble IP fragments.
      * OFPC_QUEUE_STATS  = 1 << 6, // Queue statistics.
      * OFPC_ARP_MATCH_IP = 1 << 7  // Match IP addresses in ARP pkts.
      */

     bool OFPC_FLOW_STATS,
          OFPC_TABLE_STATS,
          OFPC_PORT_STATS,
          OFPC_STP,
          OFPC_RESERVED,
          OFPC_IP_REASM,
          OFPC_QUEUE_STATS,
          OFPC_ARP_MATCH_IP;

} Capabilities_10_;



// Class comment: describe what it is for and how it should be used.
// TODO: This class MUST use exceptions, because the major part of operations won't
class Switch {

public:

     Switch ( fluid_base::OFConnection* connection,
              derailleur::InternalEvent* event );

     virtual bool install_flow ( fluid_msg::FlowModCommon flow );

     void set_name ( std::string name ) {
          this->name_ = name;
     }

     std::string get_name() {
          return this->name_;
     }

     float get_version () {
          return this->of_version;
     }

     std::string get_mac_address () {
          return this->mac_address_;
     }

     // The following methods return attributes from features reply
     uint64_t get_datapath_id () {
          return this->features_reply_.datapath_id();
     }

     int get_n_buffers () {
          return this->features_reply_.n_buffers();
     }

     int get_n_tables () {
          return this->features_reply_.n_tables();
     }

     int get_auxiliary_id () {
          return this->features_reply_.auxiliary_id();
     }

     std::string get_capabilities () {
          return this->cap_temp;
     }

     // The following methods return attributes from switch_description_ object
     std::string get_manufacturer () {
          return switch_description_.mfr_desc();
     }

     std::string get_hardware () {
          return this->switch_description_.hw_desc();
     }

     std::string get_software () {
          return this->switch_description_.sw_desc();
     }

     std::string get_serial_number () {
          return this->switch_description_.serial_num();
     }

     std::string get_datapath () {
          return this->switch_description_.dp_desc();
     }

     // Return a pointer to this Switch instance
     const Switch* get_pointer() {
          return const_cast< const Switch* > ( this );
     }




protected:

     //TODO: this method must return a flow to be installed
     virtual void install_flow_default() {}

     
     // Multipart description
     virtual void multipart_description_request ();
     virtual void multipart_description_reply (
          const derailleur::InternalEvent* event );

     
     
     std::string convert_bits_to_mac_address ( std::string datapath_id );


     fluid_base::OFConnection* connection_;


     // Switches details
     std::string name_;
     std::string mac_address_;
     float of_version;

     fluid_msg::SwitchDesc switch_description_;

     derailleur::Log log_;

     friend class derailleur::Controller;
};




class Switch13 : public Switch {

public:

     Switch13 ( fluid_base::OFConnection* connection,
                derailleur::InternalEvent* event );

private:
     
     virtual void install_flow_default() override;
     
     fluid_msg::of13::FeaturesReply features_reply_;
     Capabilities_13_ capabilities;

};




class Switch10 : public Switch {

public:

     Switch10 ( fluid_base::OFConnection* connection,
                derailleur::InternalEvent* event );

private:
     fluid_msg::of10::FeaturesReply features_reply_;



};


} // namespace derailleur

#endif // _SWITCH_HPP_










