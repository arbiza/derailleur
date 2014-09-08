
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

#include <fluid/OFServer.hh>

#include "controller.hpp"

namespace derailleur {

// It's an interface class 
class Application 
{
    
public:
    Application();

    // TODO: Check if it is correct
    void on_switch_up ();
    void on_switch_down();
    virtual void handler();
    
private:
    //auto switches_rack_;
    //std::shared_ptr<std::map<int, derailleur::Switch>> switches_rack_;

    derailleur::Controller controller;
    
};

} // namespace derailleur

#endif // _APPLICATION_HPP_
