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

#include <vector>

//TODO remove when Log class is properly implemented.
#include "log.hpp"


// forward declarations of fluid_msg classes
namespace fluid_msg {
class fluid_msg::FlowModCommon;
class fluid_msg::PacketInCommon;
class fluid_msg::Action;
}


namespace derailleur {

// Forward declarations of derailleur classes
class Event;


unsigned const MAC_bytes_length = 6;

/**
 * enum flow_commands provides the same enumerations than of13 and of10
 * namespaces, but using this enum developers do not need to use an specific
 * enumerator for each flow version.
 */
enum flow_mod_commands {
     ADD            = 0,         /* New flow. */
     MODIFY         = 1,        /* Modify all matching flows. */
     MODIFY_STRICT = 2,        /* Modify entry strictly matching wildcards and
                                    priority. */
     DELETE         = 3,        /* Delete all matching flows. */
     DELETE_STRICT = 4,        /* Delete entry strictly matching wildcards and
                                    priority. */
};

enum flow_mod_flags {
     SEND_FLOW_REM = 1 << 0,    /* Send flow removed message when flow
                                   expires or is deleted. */
     CHECK_OVERLAP = 1 << 1,    /* Check for overlapping entries first. */
     RESET_COUNTS  = 1 << 2,    /* Reset flow packet and byte counts. */
     NO_PKT_COUNTS = 1 << 3,    /* Don’t keep track of packet count. */
     NO_BYT_COUNTS = 1 << 4,    /* Don’t keep track of byte count. */
};


/**
 * FlowTable stores Flow class derived objects.
 */
typedef std::vector<Flow*> FlowTable;


class Flow {

public:
     /**
      * Constructor creates a flow with default values for fields common to
      * OpenFlow message versions.
      */
     Flow ( const derailleur::Event* const event );

     /**
      * Sets the command this flow executes on switch(es).
      */

     boot set_command_ADD ( uint16_t priority,
                            derailleur::flow_mod_commands command,
                            derailleur::flow_mod_flags flag );

     bool set_command_MODIFY ();
     bool set_command_MODIFY_STRICT ();
     
     bool set_command_DELETE ();
     bool set_command_DELETE_STRICT ();

private:
     fluid_msg::FlowModCommon* flow_mod_;
     fluid_msg::Action* action_;

     //TODO it must be a pointer.
     derailleur::Log log_;
};





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



} // namespace derailleur

#endif // _FLOW_H
