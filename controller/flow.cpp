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

#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iomanip>


#include "flow.hpp"

derailleur::Flow::Flow()
{

}

std::string derailleur::Flow::convert_bits_to_MAC ( const uint8_t* data )
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


uint8_t* derailleur::Flow::convert_MAC_to_bits ( const std::string* mac )
{
        uint8_t* array = new uint8_t[MAC_bytes_length];
        short position = 0;
        
        for (short i = 0; i < 6; i++) {
             array[i] = std::stoi( mac->substr(position, 2), nullptr, 16 );
             position += 3;
        }
        
        return array;
}