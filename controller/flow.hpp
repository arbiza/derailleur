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
#include <string>

namespace derailleur {

unsigned const MAC_bytes_length = 6;

// Forward declaration
class Flow;

/**
 * FlowTable stores Flow class derived objects.
 */
typedef std::vector<Flow*> FlowTable;



class Flow {

public:
     Flow();

     /**
      * Converts uint8_t* to MAC in hex format.
      * @param data uint8_t* from packet data; the first 6 bytes contain MAC
      * @return string with MAC (xx:xx:xx:xx:xx:xx)
      */
     static std::string convert_bytes_to_MAC ( uint8_t* data );
     
};

} // namespace derailleur

#endif // _FLOW_H
