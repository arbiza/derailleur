
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
#include <vector>
#include <list>
#include <mutex>

#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>



/** Forward declarations of classes from fluid_base namespace **/
namespace fluid_base {
class OFConnection;
}

/** Forward declarations of classes from fluid_msg namespace **/
namespace fluid_msg {
class FlowModCommon;
class Action;
}




namespace derailleur {

/** Forward declaration of classes from derailleur namespace **/
class Controller;
class Application;
class InternalEvent;
class Event;
class SwitchFactory;


/**
 * Capabilities structures for OpenFlow 1.0 switches.
 */
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


/**
 * Capabilities structures for OpenFlow 1.3 switches.
 */
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


/**
 * Arp6 is an Arp-like structure for IPv6 neiborhood.
 * It is used to provide L2 leaning switching features and may be used to
 * provide an overview of the entire network connected to the switch.
 */
typedef struct arp6 {
     uint16_t ip[8];
     uint8_t mac[6];
} Arp6;

/**
 * Arp4 is an Arp-like structure for IPv4 neiborhood.
 * It is used to provide L2 leaning switching features and may be used to
 * provide an overview of the entire network connected to the switch.
 */
typedef struct arp4 {
     uint8_t ip[4];
     uint8_t mac[6];
} Arp4;



/**
 * Switch class abstracts each OpenFlow switch connected to the controller. This
 * is an abstract class.
 *
 * Controller instantiates and stacks a Switch object in a container when a new
 * switch connects. The container is shared with Application class using a
 * pointer.
 *
 * SwitchFactory class creates Switch objects according to OpenFlow version.
 *
 * @param connection It is a connection pointer; messages are sent through this
 *                      pointer.
 *
 * @see Controller
 * @see Application
 * @see SwitchFactory
 */
class Switch {

public:

     /**
      * Copy constructor.
      * Only objects created using the above constructor passing a connection
      * object as argument have a connection pointer. When a copy is made the
      * copy's connection is set to null; it is because all the communication
      * with switches must be through switches objects in the controller stack;
      * switches copies may represent a switch that is not connected anymore.
      */
     Switch ( const Switch& source );


     /**
      * Why is this getter is not in the Application class? Because
      * methods in the Application class receive as parameter a switch ID;
      * this method aims to be used by switches' copies instances.
      */
     short get_switch_id () const {
          return this->switch_id_;
     }

     //// Switch GETTERS
     std::string get_name () const {
          return this->name_;
     }

     uint8_t get_version () const {
          return this->of_version_;
     }

     std::string get_mac_address () const {
          return this->mac_address_;
     }

     uint64_t get_datapath_id () const {
          return this->datapath_id_;
     }

     uint32_t get_n_buffers () const {
          return this->n_buffers_;
     }

     uint8_t get_n_tables () const {
          return this->n_tables_;
     }

     std::string get_manufacturer () const {
          return this->manufacturer_;
     }

     std::string get_hardware () const {
          return this->hardware_;
     }

     std::string get_software () const {
          return this->software_;
     }

     std::string get_serial_number () const {
          return this->serial_number_;
     }

     std::string get_datapath () const {
          return this->datapath_;
     }

     uint8_t get_of_version () const {
          return this->of_version_;
     }


     /**
      * Install the flow received by parameter in the switch.
      * Children of this class must convert parameters to the proper version.
      * @param flow FlowModCommon pointer
      * @param action action settings pointer
      * @return true on success
      */
     virtual short install_flow ( fluid_msg::FlowModCommon* flow,
                                  fluid_msg::Action* action ) = 0;


     /**
      * Install the flow table received by parameter in the switch.
      * @param table table containing flows to be installed (children must
      *                 convert flows to the prover Flow Mod version)
      * @return true on success
      */
     virtual short install_flow_table ( ) = 0;



     /**
      * Returns a copy of the ARP-like table for IPv4 connections.
      */
     std::list<Arp4> get_IPv4_neighborhood () {
          this->mutex_.lock();
          std::list<Arp4> arp = this->arp_table_v4_;
          this->mutex_.unlock();
          return arp;
     }

     /**
      * Returns a copy of the ARP-like table for IPv6 connections.
      */
     std::list<Arp6> get_IPv6_neighborhood () {
          this->mutex_.lock();
          std::list<Arp6> arp = this->arp_table_v6_;
          this->mutex_.unlock();
          return arp;
     }


protected:

     /**
      * Switch constructor only initializes connection_ attribute.
      * It is protected because it is only used by Controller (friend) class
      * and its children.
      *
      * Classes inheriting from this class MUST implement the following methods:
      * - public: install_flow
      * - public: install_flow_table
      * - protected: install_flow_default
      * - protected: multipart_description_request
      * - protected: multipart_description_reply
      */
     Switch ( fluid_base::OFConnection* connection )
          : connection_ ( connection ),
            switch_id_ ( connection->get_id() ) {}

     Switch () : connection_ ( nullptr ) {}

     /**
      * Parse response received from switches to features request message.
      * This method MUST be implemented by children.
      */
     virtual void set_features_reply ( uint8_t* data ) = 0;

     /**
      * This flow sets a rule for communication between switch and controller.
      * This method MUST be implemented by children.
      */
     virtual void install_flow_default() = 0;

     /**
      * Send Multipart description request message to switch.
      * This method MUST be implemented by children.
      */
     virtual void multipart_description_request () = 0;

     /**
      * Parses Multipart description request answer reveived from switch and
      * stores description fields in attributes.
      * This method MUST be implemented by children.
      */
     virtual void multipart_description_reply (
          const derailleur::InternalEvent* event ) = 0;

     /**
      * Extracts switch MAC from datapath_id.
      * This method receives the 64 bits (8 bytes) variable datapath_id. The
      * last 48 bits represents switch MAC whose are read and translated to
      * hexdecimal format that is returned as a string in MAC format.
      * @param datapath_id datapath_id in a string of bits format.
      * @return MAC as a string (xx:xx:xx:xx:xx:xx).
      */
     std::string convert_bits_to_mac_address ( uint64_t* datapath_id );



     /**
      * Controller to switch connection pointer.
      * It points to a connection handled by fluid_base::OFserver
      */
     fluid_base::OFConnection* connection_;


     /**
      * Each switch instance stores its own libfluid connection ID.
      */
     short switch_id_;

     // The following attributes store information retrieved from the switch
     // through OpenFlow (features_reply, multipart_description_reply).

     std::string name_;
     std::string mac_address_;
     uint8_t of_version_;


     // Features reply attributes:
     uint64_t datapath_id_;
     uint32_t n_buffers_;
     uint8_t n_tables_;

     // Switch description (Multipart) attributes.
     // They are set by multipart_description_reply
     std::string manufacturer_,
         hardware_,
         software_,
         serial_number_,
         datapath_;

     std::list<Arp6> arp_table_v6_;
     std::list<Arp4> arp_table_v4_;


     /**
      * This mutex is used to prevent errors when accessing ARP-like tables.
      */
     std::mutex mutex_;

private:

     /**
      * Switch default constructor is private to prevent users to instantiate
      * switches objects; Switches objects may be created by Controller and may
      * be copied through the copy constructor (who calls this method).
      */
     //Switch ();

//      void set_null_connection_ptr () {
//           this->connection_ = nullptr;
//      }


     /**
      * Checks if there is an entry in the IPv4 ARP-like table for a new device.
      * It is private and is primarily used by Application class.
      * @param entry Arp4 struct containing MAC, IPv4 and port.
      * @return true if a new entry was added; false if not.
      */
     bool set_IPv4_neighbor ( Arp4* entry );


     /**
      * Checks if there is an entry in the IPv6 ARP-like table for a new device.
      * It is private and is primarily used by Application class.
      * @param entry Arp4 struct containing MAC, IPv6 and port.
      * @return true if a new entry was added; false if not.
      */
     bool set_IPv6_neighbor ( Arp6 entry );


     /**
      * Controller is a friend class because they creates switch objects and
      * needs to access some private members.
      */
     friend class derailleur::Controller;

     /**
      * Application class provides methods through which their children access
      * switches' methods; it is a friend, so that Application may set switches'
      * attributes directly without calling, for example, a setter inside a
      * setter or a getter inside a getter.
      */
     friend class derailleur::Application;
};




class Switch10 : public Switch {

public:
     Switch10 () : Switch() {}

     /**
      * Cosntructor calls Switch constructor and sets the version attribute.
      * @see Switch
      */
     Switch10 ( fluid_base::OFConnection* connection )
          : Switch ( connection ) {
          this->of_version_ = fluid_msg::of10::OFP_VERSION;
     }

     /**
      * Overrides install_flow method from Switch class.
      * Install the flow received by parameter in the switch.
      * @param flow FlowModCommon pointer
      * @param action actions to be performade
      * @return true on success
      */
     virtual short install_flow ( fluid_msg::FlowModCommon* flow,
                                  fluid_msg::Action* action ) override;


     /**
      * Install the flow table received by parameter in the switch.
      * @param table table containing flows to be installed (children must
      *                 convert flows to the prover Flow Mod version)
      * @return true on success
      */
     virtual short install_flow_table ( ) override;

private:

     virtual void set_features_reply ( uint8_t* data ) override;

     virtual void install_flow_default() override;

     // Multipart description
     virtual void multipart_description_request () override;
     virtual void multipart_description_reply (
          const derailleur::InternalEvent* event ) override;

     // Features reply attributes:
     Capabilities_10_ capabilities_;

     friend class derailleur::SwitchFactory;
};




class Switch13 : public Switch {

public:

     Switch13 () : Switch() {}

     /**
      * Cosntructor calls Switch construcstor and sets the version attribute.
      * @see Switch
      */
     Switch13 ( fluid_base::OFConnection* connection )
          : Switch ( connection ) {
          this->of_version_ = fluid_msg::of13::OFP_VERSION;
     }


     uint8_t get_auxiliary_id () {
          return this->auxiliary_id_;
     }


     /**
      * Overrides install_flow method from Switch class.
      * Install the flow received by parameter in the switch.
      * @param flow FlowModCommon pointer
      * @param action actions to be performade
      * @return true on success
      */
     virtual short install_flow ( fluid_msg::FlowModCommon* flow,
                                  fluid_msg::Action* action ) override;


     /**
      * Install the flow table received by parameter in the switch.
      * @param table table containing flows to be installed (children must
      *                 convert flows to the prover Flow Mod version)
      * @return true on success
      */
     virtual short install_flow_table ( ) override;


private:

     virtual void set_features_reply ( uint8_t* data ) override;

     virtual void install_flow_default() override;

     // Multipart description
     virtual void multipart_description_request () override;
     virtual void multipart_description_reply (
          const derailleur::InternalEvent* event ) override;


     // Features reply attributes:
     int auxiliary_id_;
     Capabilities_13_ capabilities_;

     friend class derailleur::SwitchFactory;
};








class SwitchFactory {

public:
     static Switch* create_switch ( uint8_t version,
                                    fluid_base::OFConnection* connection,
                                    void *data ) {

          // OpenFlow 1.0 Switch
          if ( version == 1 ) {
               Switch10* s = new Switch10 ( connection );
               s->set_features_reply ( ( uint8_t* ) data );
               s->install_flow_default();
               s->multipart_description_request();
               return s;
          }
          // OpenFlow 1.3 Switch
          else if ( version ==  4 ) {
               Switch13* s = new Switch13 ( connection );
               s->set_features_reply ( ( uint8_t* ) data );
               s->install_flow_default();
               s->multipart_description_request();
               return s;
          }

          return nullptr;
     }


};



} // namespace derailleur

#endif // _SWITCH_HPP_











