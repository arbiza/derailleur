
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


#include <cstdlib>
#include <cstdint>

#include <fluid/OFServer.hh>

//TODO: remove, at least methods that use OFHandler are implemented in source
// namespace fluid_base {
// // Forward declaration
// class OFHandler;
// }


namespace derailleur {

// Class comment: describe what it is for and how it should be used.
class Message {

public:
     Message ( fluid_base::OFHandler* ofhandler,
               int type,
               void* data,
               size_t length ) {
          this->handler_ = ofhandler;
          this->type_ = type;
          this->data_ = ( uint8_t* ) data;
          this->length_ = length;
     }

     virtual ~Message() {
          this->handler_->free_data ( this->data_ );
     }

     int get_type() const {
          return this->type_;
     }

     uint8_t* get_data () const {
          return this->data_;
     }

     size_t get_length () const {
          return this->length_;
     }


private:
     fluid_base::OFHandler* handler_;
     int type_;
     uint8_t* data_;
     size_t length_;
};

} // namespace derailleur

#endif // _MESSAGE_HPP_
