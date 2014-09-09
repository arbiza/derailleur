
/**
 *  File: application.h
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  Distributed under the Lesser General Public License v3. 
 *  There is a copy of the license distributed with this software. It is also
 *  available at <https://www.gnu.org/licenses/lgpl.html>
 *
 **/



#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_


#include <map>
#include <string>

#include <fluid/OFServer.hh>

#include "switch.hpp"



namespace derailleur {

// Forward declarations
class Message;


// It's an interface class 
class Application 
{
    
public:
    Application(const std::string name) {
        this->name_ = name;
    }

    // Abstract methods
    virtual void on_switch_up (const int switch_id) {}

    virtual void on_switch_down(const int switch_id) {}

    virtual void message_handler(const int switch_id,
                         derailleur::Message* const message) {}


    void set_rack_pointer(
            std::map<int, derailleur::Switch>* const switches_rack_ptr){
        this->switches_rack_ptr_ = switches_rack_ptr;
    }

    std::string get_name() const { return this->name_; }
    
protected:
	std::map<int, derailleur::Switch>* switches_rack_ptr_;
    
private:
    std::string name_;
};

} // namespace derailleur

#endif // _APPLICATION_HPP_










