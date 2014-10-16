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
//typedef std::vector<Flow*> FlowTable;










//           /* Example from libfluid MsgApps.hh */
//           uint8_t* buffer;
//           /*You can also set the message field using
//           class methods which have the same names from
//           the field present on the specification*/
//           fluid_msg::of13::FlowMod fm;
//           //fm.xid(pi.xid());
//           fm.cookie(123);
//           fm.cookie_mask(0xffffffffffffffff);
//           fm.table_id(0);
//           fm.command(fluid_msg::of13::OFPFC_ADD);
//           fm.idle_timeout(5);
//           fm.hard_timeout(10);
//           fm.priority(100);
//           fm.buffer_id(0xffffffff);
//           fm.out_port(0);
//           fm.out_group(0);
//           fm.flags(0);
//           //of13::EthSrc fsrc(((uint8_t*) &src) + 2);
//           //of13::EthDst fdst(((uint8_t*) &dst) + 2);
//           fm.add_oxm_field(fsrc);
//           fm.add_oxm_field(fdst);
//           of13::OutputAction act(out_port, 1024);
//           of13::ApplyActions inst;
//           inst.add_action(act);
//           fm.add_instruction(inst);
//           buffer = fm.pack();
//           ofconn->send(buffer, fm.length());
//           OFMsg::free_buffer(buffer);
//           of13::Match m;
//           of13::MultipartRequestFlow rf(2, 0x0, 0, of13::OFPP_ANY, of13::OFPG_ANY,
//           0x0, 0x0, m);
//           buffer = rf.pack();
//           ofconn->send(buffer, rf.length());
//           OFMsg::free_buffer(buffer);


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
