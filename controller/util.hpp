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

#ifndef _UTIL_HPP_
#define _UTIL_HPP_


#include <string>



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
     const uint16_t ipv4 = 0x0800;
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




// Functions interfaces
//==============================================================================


template <class Type>
bool compare_byte_arrays ( const Type* a, const Type* b, size_t length )
{

     Type first[length], second[length];

     memcpy ( first,  a, length );
     memcpy ( second,  b, length );

     for ( size_t i = 0; i < length; i++ )
          if ( first[i] != second[i] )
               return false;

     return true;
}


// Link layer functions  =======================================================


/**
 * Return the code of the link layer protocol used in the packet. This method
 * reads the 13th and 14th bytes from packet data where the protocol code is.
 * Even protocol codes are in hex format (e.g. 0x0800) it is stored in the
 * memory in bits, as an interger (e.g. 0x0800 = 2048); this way the returning
 * value may be compared to l1, l2, l3 and l4 structures fields.
 */
const uint16_t get_link_layer_protocol ( const uint8_t* data );



/**
 * Extracts destination MAC from data and return in hex formatted string.
 * @param data uint8_t* from packet data; the first 6 bytes contain MAC
 * @return string with MAC (xx:xx:xx:xx:xx:xx)
 */
std::string get_destination_MAC ( const uint8_t* data );



/**
 * Extracts source MAC from data and return in hex formatted string.
 * @param data uint8_t* from packet data; 6 to 12 first bytes contain source MAC
 * @return string with MAC (xx:xx:xx:xx:xx:xx)
 */
std::string get_source_MAC ( const uint8_t* data );



/**
 * Converts MAC stored in a string to an uint8_t vector.
 * @param mac string containing MAC (xx:xx:xx:xx:xx:xx)
 * @return uint8_t* each position contains a MAC byte
 */
uint8_t* MAC_converter ( const std::string* mac );


/**
 * Converts MAC stored in a bytes array to a string
 * @param mac MAC stored in a bytes array
 * @return string with a formatted MAC (xx:xx:xx:xx:xx:xx).
 */
std::string MAC_converter ( const uint8_t* mac );


// Network layer functions  ====================================================


/**
 * Returns the IP version of the received packet.
 * @param data Data sent in the packet where physical and network addresses are.
 */
short get_ip_version ( const uint8_t* data );


/**
 *
 */
uint16_t* ipv6_converter ( const std::string ipv6 );



/**
 *
 */
std::string ipv6_converter ( const uint16_t* ipv6_array );


/**
 *
 */
uint8_t* ipv4_converter ( const std::string ipv4 );



/**
 * // TODO consider L3 protocol (e.g. ARP,  IP, ICMP); may change fields positions.
 */
std::string ipv4_converter ( const uint8_t* ipv4_array );


/**
 *
 */
std::string get_destination_ip ( const uint8_t* data );



/**
 * Extracts the IPv4 address from data and converts it to uint8_t array.
 * @return uint8_t[4] with fields of IPv4 address
 */
uint8_t* get_source_ipv4 ( const uint8_t* data );


/**
 * Extracts the IPv6 address bits data and converts it to uint16_t array.
 * @return uint16_t[8] with fields of IPv6 address
 */
uint16_t* get_source_ipv6 ( const uint8_t* data );



/**
 * Return the source IP (v4 | v6) address contained in data parameter.
 * @return string with IP address formatted according its version (4 | 6).
 */
std::string get_source_ip ( const uint8_t* data );


/**
 *
 */
uint8_t* get_destination_ipv4 ( const uint8_t* data );


/**
 *
 */
uint16_t* get_destination_ipv6 ( const uint8_t* data );



//==============================================================================

} // namespace util
} // namespace derailleur

#endif // _UTIL_HPP_

