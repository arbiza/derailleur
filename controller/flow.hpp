/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Lucas Arbiza <lucas@pop-rs.rnp.br>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _FLOW_H
#define _FLOW_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>


namespace derailleur {


/**
 * Namespace flow is a new abstraction layer to handle flow objects from libguid.
 * Functions provided handle flows of any OpenFlow version transparently.
 */
namespace flow {


unsigned const MAC_bytes_length = 6;

/**
 * FlowTable stores Flow class derived objects.
 */
typedef std::vector<fluid_msg::FlowModCommon*> FlowTable;



/**
 * Create a basic flow with some default values set. The flow is created
 * according OpenFlow version.
 *
 * @param version OpenFlow version
 * @param command OpenFlow command enumerated in ofp_flow_mod_command (described
 *                      bellow); I used enumerator from of13 because it is the 
 *                      same as of10.
 * @return fluid_msg::FlowModCommon* pointer; you need to static_cast the
 *              returning pointer to a pointer of flow mod of the version you 
 *              are using; you may also use auto ptr* (recommended).
 *
 * enum ofp_flow_mod_command {
 *      OFPFC_ADD = 0,          // New flow.
 *      OFPFC_MODIFY = 1,       // Modify all matching flows.
 *      OFPFC_MODIFY_STRICT = 2,// Modify entry strictly matching wildcards
 *                              // and priority.
 *      OFPFC_DELETE = 3,       // Delete all matching flows.
 *      OFPFC_DELETE_STRICT = 4,// Delete entry strictly matching wildcards and
 *                              //priority.
 * };
 */

static fluid_msg::FlowModCommon* create_flow_from_packet_in (
     uint8_t version, 
     const fluid_msg::of13::PacketIn* packet_in )
{

     fluid_msg::FlowModCommon* flow;

     // OpenFlow 1.3 messages
     if ( version == fluid_msg::of13::OFP_VERSION ) {
          
          fluid_msg::of13::FlowMod* flow13 = new fluid_msg::of13::FlowMod;
          fluid_msg::of13::PacketIn* packet_in = new fluid_msg::of13::PacketIn();
                                   
          packet_in->unpack ( event->get_data() );
          
          flow13->xid( packet_in->xid() );
          
          
          // //fm.xid(pi.xid());
          // fm.cookie ( 123 );
          // fm.cookie_mask ( 0xffffffffffffffff );
          // fm.table_id ( 0 );
          // fm.command ( fluid_msg::of13::OFPFC_ADD );
          // fm.idle_timeout ( 5 );
          // fm.hard_timeout ( 10 );
          // fm.priority ( 100 );
          // fm.buffer_id ( 0xffffffff );
          // fm.out_port ( 0 );
          // fm.out_group ( 0 );
          // fm.flags ( 0 );
          // //of13::EthSrc fsrc(((uint8_t*) &src) + 2);
          // //of13::EthDst fdst(((uint8_t*) &dst) + 2);
          // fm.add_oxm_field ( fsrc );
          // fm.add_oxm_field ( fdst );
          // of13::OutputAction act ( out_port, 1024 );
          // of13::ApplyActions inst;
          // inst.add_action ( act );
          // fm.add_instruction ( inst );

          flow = flow13;
     }
     // OpenFlow 1.0 messages
     else if ( version == fluid_msg::of10::OFP_VERSION ) {

          fluid_msg::of10::FlowMod* flow10 = new fluid_msg::of10::FlowMod;

          flow = flow10;
     }
     else {
          flow = nullptr;
     }

     return flow;
}





/**
 * Namespace util provides useful functions such as conversions and analyzers.
 * This functions may be also used to handle switches messages translating
 * received data to strings.
 */
namespace util {


/**
 * Converts uint8_t* to MAC in hex format.
 * @param data uint8_t* from packet data; the first 6 bytes contain MAC
 * @return string with MAC (xx:xx:xx:xx:xx:xx)
 */
static std::string convert_bits_to_MAC ( const uint8_t* data )
{

     std::stringstream ss;
     ss << std::hex << std::setfill ( '0' );

     uint8_t array[MAC_bytes_length];
     std::memcpy ( array, data, MAC_bytes_length );

     for ( short i = 0; i < 6; i++ ) {
          ss <<  std::setw ( 2 ) << ( int ) array[i];

          if ( i < 5 )
               ss << ":";
     }

     return ss.str();
}


/**
 * Converts MAC stored in a string to an uint8_t vector.
 * @param mac string containing MAC (xx:xx:xx:xx:xx:xx)
 * @return uint8_t* each position contains a MAC byte
 */
static uint8_t* convert_MAC_to_bits ( const std::string* mac )
{

     uint8_t* array = new uint8_t[MAC_bytes_length];
     short position = 0;

     for ( short i = 0; i < 6; i++ ) {
          array[i] = std::stoi ( mac->substr ( position, 2 ), nullptr, 16 );
          position += 3;
     }

     return array;
}



} // namespace util


} // namespace flow
} // namespace derailleur

#endif // _FLOW_H
