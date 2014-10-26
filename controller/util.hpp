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


namespace derailleur {


#include <string>
#include <sstream>
#include <cstring>


enum IP {
     v4 = 4,
     v6 = 6,
};

enum LENGTH {
     mac = 6,
     ipv4 = 4,
     ipv6 = 6,
};


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
//typedef std::vector<Flow*> FlowTable;



/**
 * TODO: rewrite this comment.
 * Namespace flow provides useful functions such as conversions and analyzers.
 * This functions may be also used to handle switches messages translating
 * received data to strings.
 */
namespace util {


////// L2 functions


/**
 * Converts uint8_t* to MAC in hex format.
 * @param data uint8_t* from packet data; the first 6 bytes contain MAC
 * @return string with MAC (xx:xx:xx:xx:xx:xx)
 */
static std::string convert_bits_to_MAC ( const uint8_t* data )
{

     std::stringstream ss;
     ss << std::hex << std::setfill ( '0' );

     uint8_t array[LENGTH.mac];
     memcpy ( array, data, LENGTH.mac );

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

     uint8_t* array = new uint8_t[LENGTH.mac];
     short position = 0;

     for ( short i = 0; i < 6; i++ ) {
          array[i] = std::stoi ( mac->substr ( position, 2 ), nullptr, 16 );
          position += 3;
     }

     return array;
}


////// L3 functions

/**
 * Returns the IP version of the received packet.
 * @param data Data sent in the packet where physical and network addresses are.
 */
static short get_ip_version ( uint8_t* data )
{

}


/**
 *
 */
static std::string convert_ipv4_to_string ( uint8_t* ipv4_array )
{

}

/**
 *
 */
static std::string convert_ipv6_to_string ( uint16_t* ipv6_array )
{

}

/**
 * 
 */
static uint8_t* convert_ipv4_to_bits ( std::string ipv4 ) {
     
}

/**
 * 
 */
static uint16_t* convert_ipv6_to_bits ( std::string ipv6 ) {

  }


//// Source IP

/**
 * Return the source IP (v4 | v6) address contained in data parameter.
 * @return string with IP address formatted according its version (4 | 6).
 */
static std::string get_source_ip_from_data ( uint8_t* data )
{
     std::string ip;
     
     if ( get_ip_version ( data ) ==  IP.ipv6 ) {
          uint16_t ipv6[8] = get_source_ipv6_in_bits ( data );
          ip = convert_ipv6_to_string( ipv6 );
     } else {
          uint8_t ipv4[4] = get_source_ipv4_in_bits ( data );
          ip = convert_ipv4_to_string( ipv4 );
     }
     return ip;
}


/**
 * Extracts the IPv4 address bits from data and converts it to uint8_t array.
 * @return uint8_t[4] with fields of IPv4 address
 */
static uint8_t* get_source_ipv4_from_data_in_bits ( uint8_t* data )
{

}

/**
 * Extracts the IPv6 address bits from data and converts it to uint16_t array.
 * @return uint16_t[8] with fields of IPv6 address
 */
static uint16_t* get_source_ipv6_from_data_in_bits ( uint8_t* data )
{

}

//// Destination IP

/**
 *
 */
static std::string get_destination_ip_from_data ( uint8_t* data )
{

}

/**
 *
 */
static uint8_t* get_destination_ipv4_from_data_in_bits ( uint8_t* data )
{

}

/**
 *
 */
static uint16_t* get_destination_ipv6_from_data_in_bits ( uint8_t* data )
{

}




} // namespace util
} // namespace derailleur

#endif // _FLOW_H
