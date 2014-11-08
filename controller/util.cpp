
# include <sstream>
# include <cstring>

#include "util.hpp"


// Link layer functions  =======================================================


const uint16_t derailleur::util::get_link_layer_protocol ( 
     const uint8_t* data )
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


std::string derailleur::util::get_destination_MAC ( const uint8_t* data )
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


std::string derailleur::util::get_source_MAC ( const uint8_t* data )
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



uint8_t* derailleur::util::MAC_converter ( const std::string* mac )
{

     uint8_t* array = new uint8_t[LENGTH::mac];
     short position = 0;

     for ( short i = 0; i < 6; i++ ) {
          array[i] = std::stoi ( mac->substr ( position, 2 ), nullptr, 16 );
          position += 3;
     }

     return array;
}


std::string derailleur::util::MAC_converter ( const uint8_t* mac )
{
     std::stringstream ss;
     ss << std::hex << std::setfill ( '0' );

     uint8_t array[LENGTH::mac];
     memcpy ( array, mac, LENGTH::mac );

     for ( short i = 0; i < 6; i++ ) {
          ss <<  std::setw ( 2 ) << ( int ) array[i];

          if ( i < 5 )
          ss << ":";
       }

     return ss.str();
}



// Network layer functions  ====================================================


short derailleur::util::get_ip_version ( const uint8_t* data )
{

     return 0;                                              // TODO: fix it
}



uint16_t* derailleur::util::ipv6_converter ( const std::string ipv6 )
{
     uint16_t* ip = new uint16_t[LENGTH::ipv6];
     return ip;
}



std::string derailleur::util::ipv6_converter ( const uint16_t* ipv6_array )
{
     std::string ip;
     return ip;
}



uint8_t* derailleur::util::ipv4_converter ( const std::string ipv4 )
{

     uint8_t* ip = new uint8_t[4];
     return ip;
}



std::string derailleur::util::ipv4_converter ( const uint8_t* ipv4_array )
{
     std::string ip;
     return ip;
}



std::string derailleur::util::get_destination_ip ( const uint8_t* data )
{
     std::string ip;
     return ip;
}



uint8_t* derailleur::util::get_source_ipv4 ( const uint8_t* data )
{

     uint8_t* ip = new uint8_t[4];
     return ip;
}



uint16_t* derailleur::util::get_source_ipv6 ( const uint8_t* data )
{

     uint16_t* ip = new uint16_t[4];
     return ip;
}



std::string derailleur::util::get_source_ip ( const uint8_t* data )
{
     std::string ip;

     if ( get_ip_version ( data ) ==  IP::v6 )
          ip = ipv6_converter ( get_source_ipv6 ( data ) );
     else
          ip = ipv4_converter ( get_source_ipv4 ( data ) );

     return ip;
}



uint8_t* derailleur::util::get_destination_ipv4 ( const uint8_t* data )
{

     uint8_t* ip = new uint8_t[4];
     return ip;
}



uint16_t* derailleur::util::get_destination_ipv6 ( const uint8_t* data )
{

     uint16_t* ip = new uint16_t[4];
     return ip;
}

