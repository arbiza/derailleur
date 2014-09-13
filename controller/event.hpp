/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Lucas Arbiza - Inventati <lucas.arbiza@inventati.org>
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

#ifndef EVENT_H
#define EVENT_H

# include <cstdlib>
# include <cstdint>

#include <fluid/OFServer.hh>

namespace derailleur {

class Event {

public:
     Event ( const int switch_id,
             fluid_base::OFHandler* ofhandler,
             const int type,
             const void* data,
             const size_t length ) {
          this->switch_id_ = switch_id;
          this->handler_ = ofhandler;
          this->type_ = type;
          this->data_ = ( uint8_t* ) data;
          this->length_ = length;
     }

     ~Event() {
          this->handler_->free_data ( this->data_ );
     }

     int get_switch_id () {
          return this->switch_id_;
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
     int switch_id_;
     fluid_base::OFHandler* handler_;
     int type_;
     uint8_t* data_;
     size_t length_;
};

} //  namespace derailleur

#endif // EVENT_H
