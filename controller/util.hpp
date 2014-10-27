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

# include <string>
# include <sstream>
# include <cstring>

# include "event.hpp"


namespace derailleur {

/**
 * TODO: rewrite this comment.
 * Namespace flow provides useful functions such as conversions and analyzers.
 * This functions may be also used to handle switches messages translating
 * received data to strings.
 */
namespace util {



enum IP {
     v4 = 4,
     v6 = 6
};

enum LENGTH {
     mac = 6,
     ipv4 = 4,
     ipv6 = 8
};




/**
 * struct l1 contains protocol codes of the Link Layer (first layer of TCP/IP 
 * model stack).
 */
struct l1 {
     const char* arp = "0x0806";
};

/**
 * struct l2 contains protocol codes of the Internet Layer (second layer of 
 * TCP/IP model stack).
 */
struct l2 {
     const char* ipv4 = "0x0800";
     const char* ipv4 = "0x08dd";
};

/**
 * struct l3 contains protocol codes of the Transport Layer (third layer of 
 * TCP/IP model stack).
 */
struct l3 {
     
};

/**
 * struct l4 contains protocol codes of the Application Layer (fourth layer of 
 * TCP/IP model stack).
 */
struct l4 {
     
};

/**
 * struct protocols contains an instance of each of the above layers; this way
 * protocols codes may be used accessing nested instances.
 */
struct protocols {
     const struct l1 link_layer;
     const struct l2 internet_layer;
     const struct l3 transport_layer;
     const struct l4 application_layer;
};

/**
 * Instance of the struct protocols. Protocols codes may be accessed as 
 * following:
 * derailleur::util::Protocols.link_layer.arp;
 */
static const struct protocols Protocols;




/**
 *
 */
short get_protocol_type_from_data ( )
{

     // TODO remove this temporary return
     short i = 0;
     return i;
}


////// Link layer (L2) functions

/**
 * oxm_ofb_match_fields
 */
short get_link_layer_protocol ( const derailleur::Event* event )
{

     short i = 0;

     if ( event->get_version() == fluid_msg::of13::OFP_VERSION ) {

          //uint8_t array[2];
          uint8_t opcode[2];
          memcpy ( opcode, event->get_data() + 12, 1 );
          //memcpy ( opcode + 1, event->get_data() + 13, 1 );

          i = ( short ) opcode[0];
     }

     return i;
}


/**
 * Converts uint8_t* to MAC in hex format.
 * @param data uint8_t* from packet data; the first 6 bytes contain MAC
 * @return string with MAC (xx:xx:xx:xx:xx:xx)
 */
std::string convert_bits_to_MAC ( const uint8_t* data )
{

     std::stringstream ss;
     ss << std::hex << std::setfill ( '0' );

     uint8_t array[LENGTH::mac];
     memcpy ( array, data, LENGTH::mac );

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
uint8_t* convert_MAC_to_bits ( const std::string* mac )
{

     uint8_t* array = new uint8_t[LENGTH::mac];
     short position = 0;

     for ( short i = 0; i < 6; i++ ) {
          array[i] = std::stoi ( mac->substr ( position, 2 ), nullptr, 16 );
          position += 3;
     }

     return array;
}


////// Network layer (L3) functions

/**
 * Returns the IP version of the received packet.
 * @param data Data sent in the packet where physical and network addresses are.
 */
short get_ip_version ( const uint8_t* data )
{
     // TODO remove this temporary return
     short i = 0;
     return i;
}


/**
 *
 */
uint8_t* convert_ipv4_to_bits ( const std::string ipv4 )
{
     uint8_t* ip = new uint8_t[LENGTH::ipv4];

     return ip;
}

/**
 *
 */
uint16_t* convert_ipv6_to_bits ( const std::string ipv6 )
{
     uint16_t* ip = new uint16_t[LENGTH::ipv6];
     return ip;
}


/**
 * // TODO consider L3 protocol (e.g. ARP,  IP, ICMP); may change fields positions.
 */
std::string convert_ipv4_to_string ( const uint8_t* ipv4_array )
{
     std::string ip;
     return ip;
}

/**
 *
 */
std::string convert_ipv6_to_string ( const uint16_t* ipv6_array )
{
     std::string ip;
     return ip;
}




//// Source IP

/**
 * Extracts the IPv4 address bits from data and converts it to uint8_t array.
 * @return uint8_t[4] with fields of IPv4 address
 */
uint8_t* get_source_ipv4_in_bits ( const uint8_t* data )
{
     uint8_t* ip = new uint8_t[LENGTH::ipv4];
     return ip;
}

/**
 * Extracts the IPv6 address bits from data and converts it to uint16_t array.
 * @return uint16_t[8] with fields of IPv6 address
 */
uint16_t* get_source_ipv6_in_bits ( const uint8_t* data )
{
     uint16_t* ip = new uint16_t[LENGTH::ipv6];
     return ip;
}


//// Destination IP

/**
 *
 */
uint8_t* get_destination_ipv4_in_bits ( const uint8_t* data )
{
     uint8_t* ip = new uint8_t[LENGTH::ipv4];
     return ip;
}

/**
 *
 */
uint16_t* get_destination_ipv6_in_bits ( const uint8_t* data )
{
     uint16_t* ip = new uint16_t[LENGTH::ipv6];
     return ip;
}


/**
 * Return the source IP (v4 | v6) address contained in data parameter.
 * @return string with IP address formatted according its version (4 | 6).
 */
std::string get_source_ip ( const uint8_t* data )
{
     std::string ip;

     if ( get_ip_version ( data ) ==  IP::v6 ) {
          uint16_t* ipv6 = get_source_ipv6_in_bits ( data );
          ip = convert_ipv6_to_string ( ipv6 );
     } else {
          uint8_t* ipv4 = get_source_ipv4_in_bits ( data );
          ip = convert_ipv4_to_string ( ipv4 );
     }
     return ip;
}


/**
 *
 */
std::string get_destination_ip ( const uint8_t* data )
{
     std::string ip;
     return ip;
}



////// Data layer functions



}                                                           // namespace util
} // namespace derailleur

#endif // _FLOW_H

