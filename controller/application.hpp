
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

#include <fluid/OFServer.hh>

#include "switch.hpp"


namespace derailleur {

// Forward declarations
class Controller;
class Message;


// It's an interface class 
class Application 
{
    
public:
    Application(const char* address,
                const int port,
                const int n_workers,
                const bool secure);
    

    // TODO: Check if it is correct
    virtual void on_switch_up (int switch_id) {}

    virtual void on_switch_down(int switch_id) {}

    virtual void handler(int switch_id, derailleur::Message* message) {}


    void set_rack_pointer(std::map<int, derailleur::Switch>& switches_rack_ptr){
        this->switches_rack_ptr_ = &switches_rack_ptr;
    }
    
private:
    
    Controller* controller_;
    std::map<int, derailleur::Switch>* switches_rack_ptr_;
};

} // namespace derailleur

#endif // _APPLICATION_HPP_










