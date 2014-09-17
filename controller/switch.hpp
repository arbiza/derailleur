
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

#include "log.hpp"


// Forward declaration
namespace fluid_base {
// Forward declaration
class OFConnection;
}


namespace derailleur {

// Forward declarations
class Controller;
class InternalEvent;
class Event;



// Capabilities structures

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

typedef struct capabilities_13 {

     /* enum ofp_capabilities {
     * OFPC_FLOW_STATS   = 1 << 0, // Flow statistics., of_version_ ( "1.3" )
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






// Class comment: describe what it is for and how it should be used.
// TODO: This class MUST use exceptions, because the major part of operations won't
class Switch {

public:

     Switch ( fluid_base::OFConnection* connection,
              derailleur::InternalEvent* event );

     //virtual bool install_flow ( fluid_msg::FlowModCommon flow );

     void set_name ( std::string name ) {
          this->name_ = name;
     }

     virtual void set_features_reply ( uint8_t* data ) {}


     std::string get_name() {
          return this->name_;
     }

     std::string get_version () {
          return this->of_version_;
     }

     std::string get_mac_address () {
          return this->mac_address_;
     }

     // The following methods return attributes from features reply
     uint64_t get_datapath_id () {
          return this->datapath_id_;
     }

     int get_n_buffers () {
          return this->n_buffers_;
     }

     int get_n_tables () {
          return this->n_tables_;
     }


     // The following methods return attributes from switch_description_ object
     std::string get_manufacturer () {
          return this->manufacturer_;
     }

     std::string get_hardware () {
          return this->hardware_;
     }

     std::string get_software () {
          return this->software_;
     }

     std::string get_serial_number () {
          return this->serial_number_;
     }

     std::string get_datapath () {
          return this->datapath_;
     }

     // Return a pointer to this Switch instance
     const Switch* get_pointer() {
          return const_cast< const Switch* > ( this );
     }


protected:

     //  This flow sets a rule for communication between switch and controller
     virtual void install_flow_default() {}


     // Multipart description
     virtual void multipart_description_request () {}
     virtual void multipart_description_reply (
          const derailleur::InternalEvent* event ) {}


     virtual void set_version () {}


     std::string convert_bits_to_mac_address ( std::string datapath_id );



     fluid_base::OFConnection* connection_;

     // Switches details
     std::string name_;
     std::string mac_address_;
     std::string of_version_;


     // Features reply attributes:
     uint64_t datapath_id_;
     int n_buffers_;
     int n_tables_;

     // Switch description (Multipart) attributes.
     // They are set by multipart_description_reply
     std::string manufacturer_,
         hardware_,
         software_,
         serial_number_,
         datapath_;


     derailleur::Log log_;

private:
     friend class derailleur::Controller;
};




class Switch13 : public Switch {

public:

     Switch13 ( fluid_base::OFConnection* connection,
                derailleur::InternalEvent* event )
          : Switch ( connection, event ) {}

     virtual void set_features_reply ( uint8_t* data ) override;
     
     virtual void set_version () override {
          this->of_version_ = "1.3.2 (0x04)";
     }


     int get_auxiliary_id () {
          return this->auxiliary_id_;
     }

private:

     virtual void install_flow_default() override;

     // Multipart description
     virtual void multipart_description_request () override;
     virtual void multipart_description_reply (
          const derailleur::InternalEvent* event ) override;


     // Features reply attributes:
     int auxiliary_id_;
     Capabilities_13_ capabilities_;
};




class Switch10 : public Switch {

public:

     Switch10 ( fluid_base::OFConnection* connection,
                derailleur::InternalEvent* event )
          : Switch ( connection, event ) {
          this->of_version_ = "1.0.0 (0x01)";
     }

private:
     // Features reply attributes:
     Capabilities_10_ capabilities_;
};



class SwitchFactory {

public:
     static Switch* create_switch ( uint8_t version,
                                    fluid_base::OFConnection* connection,
                                    derailleur::InternalEvent* event ) {

          if ( version == 1 )
               return new Switch10 ( connection,  event );
          else if ( version ==  4 )
               return new Switch13 ( connection,  event );
     }
};



} // namespace derailleur

#endif // _SWITCH_HPP_










