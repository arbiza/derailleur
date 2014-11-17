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

#ifndef _EVENT_H_
#define _EVENT_H_

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <fluid/OFServer.hh>
#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>

#include "util.hpp"

// TODO: remover
#include "log.hpp"

namespace derailleur {

class Event {

public:
     Event ( fluid_base::OFHandler* ofhandler,
             const int switch_id,
             const uint8_t of_version,
             const int type,
             const void* data,
             const size_t length )
          : handler_ ( ofhandler ), switch_id_ ( switch_id ),
            of_version_ ( of_version ), type_ ( type ),
            data_ ( ( uint8_t* ) data ), length_ ( length ) {

//           this->handler_ = ofhandler;
//           this->type_ = type;
//           this->data_ = &data;
//           this->length_ = length;
//           this->of_version_ = of_version;
//           this->switch_id_ = switch_id;

          /* If the packet is a packet-in (10) ip_version_ is set; if not it
           * will not be. It may change in the future, but for now I do not see
           * it as useful but for packet-in handling. */
          if ( this->type_ ==  10 )
               set_ip_version();
          else
               this->ip_version_ = 0;
     }

     ~Event() {
          this->handler_->free_data ( this->data_ );
     }

     int get_type() const {
          return this->type_;
     }

     uint8_t* get_data () const {
          return ( uint8_t* ) this->data_;
     }

     size_t get_length () const {
          return this->length_;
     }

     int get_switch_id () const {
          return this->switch_id_;
     }

     uint8_t get_version () const {
          return this->of_version_;
     }

     uint8_t get_ip_version () const {
          return this->ip_version_;
     }

private:

     /**
      * Set ip_version_ attribute with 4 or 6 corresponding to the version of
      * IP of the packet. It is called only when the received packet is a packet-
      * in (type 10).
      */
     void set_ip_version () {

          fluid_msg::PacketInCommon* packet_in = nullptr;

          if ( this->of_version_ == fluid_msg::of13::OFP_VERSION ) {
               packet_in = new fluid_msg::of13::PacketIn();
               packet_in->unpack ( ( uint8_t* ) this->data_ );
          } else {
               packet_in = new fluid_msg::of10::PacketIn();
               packet_in->unpack ( ( uint8_t* ) this->data_ );
          }

          uint16_t link_layer = derailleur::util::get_link_layer_protocol (
                                     ( uint8_t* ) packet_in->data() );


          if ( link_layer == derailleur::util::Protocols.link_layer.ipv6 )
               this->ip_version_ = 6;
          else if ( link_layer == derailleur::util::Protocols.link_layer.ipv4 ||
                    link_layer ==  derailleur::util::Protocols.link_layer.arp )
               this->ip_version_ = 4;

          delete packet_in;
     }

     fluid_base::OFHandler* handler_;
     int switch_id_;
     uint8_t of_version_;
     int type_;
     uint8_t* data_;
     size_t length_;
     uint8_t ip_version_;
};

} //  namespace derailleur

#endif // _EVENT_H_
