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

class InternalEvent {

public:
     InternalEvent ( fluid_base::OFHandler* ofhandler,
                     const int type,
                     const void* data,
                     const size_t length ) {
          this->handler_ = ofhandler;
          this->type_ = type;
          this->data_ = ( uint8_t* ) data;
          this->length_ = length;
     }

     ~InternalEvent() {
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

protected:
     fluid_base::OFHandler* handler_;
     int type_;
     uint8_t* data_;
     size_t length_;
};



class Event : public InternalEvent {

public:
     Event ( const int switch_id,
             const uint8_t of_version,
             fluid_base::OFHandler* ofhandler,
             const int type,
             const void* data,
             const size_t length )
          : InternalEvent ( ofhandler,
                            type,
                            data,
                            length ) {
          this->of_version_ = of_version;
          this->switch_id_ = switch_id;
     }

     ~Event() {
          this->handler_->free_data ( this->data_ );
     }

     int get_switch_id () const {
          return this->switch_id_;
     }

     uint8_t get_version () const {
          return this->of_version_;
     }

private:
     int switch_id_;
     uint8_t of_version_;
};


} //  namespace derailleur

#endif // EVENT_H
