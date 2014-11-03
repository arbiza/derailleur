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

// TODO remove
#include <bitset>
#include <iostream>

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
 * model stack) according to oxm_ofb_match_fields (OpenFlow 1.3).
 */
struct l1 {
     const uint16_t arp = 0x0806;
     
     /**
      * IPv6 also appear here in link layer protocols because in IPv6 networks
      * neighborhood discovery is made by NDP (Neighbor Discovery Protocol) that
      * uses ICMPv6 packets; OpenFlow identifies IPv6 in the first layer.
      */
     const uint16_t ipv6 = 0x08dd;
};

/**
 * struct l2 contains protocol codes of the Internet Layer (second layer of
 * TCP/IP model stack) according to oxm_ofb_match_fields (OpenFlow 1.3).
 */
struct l2 {
     const uint16_t ipv4 = 0x0800;
     const uint16_t ipv6 = 0x08dd;
};

/**
 * struct l3 contains protocol codes of the Transport Layer (third layer of
 * TCP/IP model stack) according to oxm_ofb_match_fields (OpenFlow 1.3).
 */
struct l3 {

};

/**
 * struct l4 contains protocol codes of the Application Layer (fourth layer of
 * TCP/IP model stack) according to oxm_ofb_match_fields (OpenFlow 1.3).
 */
struct l4 {

};

/**
 * struct protocols contains an instance of each of the above layers; this way
 * protocols codes may be used accessing nested instances.
 */
struct protocols {
     struct l1 link_layer;
     struct l2 internet_layer;
     struct l3 transport_layer;
     struct l4 application_layer;
};

/**
 * Instance of the struct protocols. Protocols codes may be accessed as
 * following:
 * derailleur::util::Protocols.link_layer.arp;
 */
static struct protocols Protocols;






////////////////////////////////////////////////////////////////////////////////
////// Link layer functions
////////////////////////////////////////////////////////////////////////////////


/**
 * Return the code of the link layer protocol used in the packet. This method
 * reads the 13th and 14th bytes from packet data where the protocol code is.
 * Even protocol codes are in hex format (e.g. 0x0800) it is stored in the
 * memory in bits, as an interger (e.g. 0x0800 = 2048); this way the returning
 * value may be compared to l1, l2, l3 and l4 structures fields.
 */
const uint16_t get_link_layer_protocol ( const uint8_t* data )
{
     uint8_t bytes[2];

     // Copy the 13th and 14th bytes to the bytes array.
     memcpy ( bytes, data + 12, 2 );

     // 1) copy bytes[0] to the leftmost bits of protocol; 2) does an OR 
     // operation with bytes[1] against the rightmost bits of protocol (all 0)
     // to concatenate both bytes.
     uint16_t protocol = ( ( uint16_t ) bytes[0] <<  8 ) |  bytes[1];

     return protocol;
}


/**
 * Extracts destination MAC from data and return in hex formatted string.
 * @param data uint8_t* from packet data; the first 6 bytes contain MAC
 * @return string with MAC (xx:xx:xx:xx:xx:xx)
 */
std::string get_destination_MAC ( const uint8_t* data )
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
 * Extracts source MAC from data and return in hex formatted string.
 * @param data uint8_t* from packet data; 6 to 12 first bytes contain source MAC
 * @return string with MAC (xx:xx:xx:xx:xx:xx)
 */
std::string get_source_MAC ( const uint8_t* data )
{

     std::stringstream ss;
     ss << std::hex << std::setfill ( '0' );

     uint8_t array[LENGTH::mac];
     memcpy ( array, data + 6, LENGTH::mac );

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
uint8_t* MAC_converter ( const std::string* mac )
{

     uint8_t* array = new uint8_t[LENGTH::mac];
     short position = 0;

     for ( short i = 0; i < 6; i++ ) {
          array[i] = std::stoi ( mac->substr ( position, 2 ), nullptr, 16 );
          position += 3;
     }

     return array;
}






////////////////////////////////////////////////////////////////////////////////
////// Network layer functions
////////////////////////////////////////////////////////////////////////////////



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
uint16_t* ipv6_converter ( const std::string ipv6 )
{
     uint16_t* ip = new uint16_t[LENGTH::ipv6];
     return ip;
}

/**
 *
 */
std::string ipv6_converter ( const uint16_t* ipv6_array )
{
     std::string ip;
     return ip;
}


/**
 *
 */
uint8_t* ipv4_converter ( const std::string ipv4 )
{
     uint8_t* ip = new uint8_t[LENGTH::ipv4];

     return ip;
}


/**
 * // TODO consider L3 protocol (e.g. ARP,  IP, ICMP); may change fields positions.
 */
std::string ipv4_converter ( const uint8_t* ipv4_array )
{
     std::string ip;
     return ip;
}






//// Source IP

/**
 * Extracts the IPv4 address bits from data and converts it to uint8_t array.
 * @return uint8_t[4] with fields of IPv4 address
 */
uint8_t* get_source_ipv4 ( const uint8_t* data )
{
     uint8_t* ip = new uint8_t[LENGTH::ipv4];
     return ip;
}

/**
 * Extracts the IPv6 address bits from data and converts it to uint16_t array.
 * @return uint16_t[8] with fields of IPv6 address
 */
uint16_t* get_source_ipv6 ( const uint8_t* data )
{
     uint16_t* ip = new uint16_t[LENGTH::ipv6];
     return ip;
}


//// Destination IP

/**
 *
 */
uint8_t* get_destination_ipv4 ( const uint8_t* data )
{
     uint8_t* ip = new uint8_t[LENGTH::ipv4];
     return ip;
}

/**
 *
 */
uint16_t* get_destination_ipv6 ( const uint8_t* data )
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
          ip = ipv6_to_string ( ipv6 );
     } else {
          uint8_t* ipv4 = get_source_ipv4_in_bits ( data );
          ip = ipv4_to_string ( ipv4 );
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

