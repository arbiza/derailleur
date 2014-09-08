
/**
 *  File: message.hpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  Distributed under the Lesser General Public License v3. 
 *  There is a copy of the license distributed with this software. It is also
 *  available at <https://www.gnu.org/licenses/lgpl.html>
 *
 **/



#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

namespace fluid_base {
    // Forward declaration
    class OFHandler; 
}


namespace derailleur {

// Class comment: describe what it is for and how it should be used.
class Message
{
    
public:
    Message(fluid_base::OFHandler* ofhandler,
	    int type,
	    void* data,
	    size_t len);

    virtual ~Message();

    int get_type();
    
private:
    fluid_base::OFHandler* handler_;
    int type_;
    uint8_t* data_;
    size_t len_;
};

} // namespace derailleur

#endif // _MESSAGE_HPP_
