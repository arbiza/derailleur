
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

#include <vector>

#include <fluid/of13msg.hh>

namespace derailleur {
	
enum {
	MAIN = 0,
	INPUT = 1,
	OUTPUT = 2,
	FIREWALL = 3,
}


// A Table contain OpenFlow flows (rules), this class store and manage that
// flows.
// TODO: how to use.
class Table 
{
    
public:
    // Default constructor
    Table();
    
private:
    
    
};

} // namespace derailleur

#endif // _TABLE_HPP_
