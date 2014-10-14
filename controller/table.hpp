
/**
 *  File: table.h
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 *
 *  Distributed under the Lesser General Public License v3.
 *  There is a copy of the license distributed with this software. It is also
 *  available at <https://www.gnu.org/licenses/lgpl.html>
 *
 **/



#ifndef _TABLE_HPP_
#define _TABLE_HPP_

#include <string>

#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>


namespace derailleur {



// A Table contain OpenFlow flows (rules), this class store and manage that
// flows.
// TODO: how to use.
class Table {

public:
     // Default constructor
     Table();

private:


};



/**
 * Namespace flow provides functions to abstract some data manipulation to
 * make easier flows handling.
 */
namespace flow {

std::string bits_to_MAC ( std::string data )
{
     short position = 16;                                   // MAC address starts at position 16 in datapath_id
     size_t length = 4;
     std::string mac;

     for ( short i = 0; i < 12; i++ ) {
          std::string mac_in_bits = datapath_id.substr ( position, length );

          if ( mac_in_bits == "0000" )
               mac += "0";
          else if ( mac_in_bits == "0001" )
               mac += "1";
          else if ( mac_in_bits == "0010" )
               mac += "2";
          else if ( mac_in_bits == "0011" )
               mac += "3";
          else if ( mac_in_bits == "0100" )
               mac += "4";
          else if ( mac_in_bits == "0101" )
               mac += "5";
          else if ( mac_in_bits == "0110" )
               mac += "6";
          else if ( mac_in_bits == "0111" )
               mac += "7";
          else if ( mac_in_bits == "1000" )
               mac += "8";
          else if ( mac_in_bits == "1001" )
               mac += "9";
          else if ( mac_in_bits == "1010" )
               mac += "a";
          else if ( mac_in_bits == "1011" )
               mac += "b";
          else if ( mac_in_bits == "1100" )
               mac += "c";
          else if ( mac_in_bits == "1101" )
               mac += "d";
          else if ( mac_in_bits == "1110" )
               mac += "e";
          else if ( mac_in_bits == "1111" )
               mac += "f";

          position += length;
     }

     return mac;
}

} //  namespace flow

} // namespace derailleur

#endif // _TABLE_HPP_
