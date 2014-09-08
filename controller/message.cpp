
/**
 *  File: message.cpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  This file is an implementation of the class(es) defined in
 *  message.hpp header file.
 *
 *  See the header file for documentation and license information.
 *
 **/

#include <fluid/OFServer.hh>

#include "message.hpp"


derailleur::Message::Message(fluid_base::OFHandler* ofhandler,
			     int type,
			     void* data,
			     size_t length) {
    this->handler_ = ofhandler;
    this->type_ = type;
    this->data_ = (uint8_t*) data;
    this->length_ = length;
}



derailleur::Message::~Message() {
    this->handler_->free_data(this->data_);
}



int derailleur::Message::get_type() {
    return this->type_;
}






