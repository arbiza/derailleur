
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


namespace derailleur {

    class Controller;


// It's an interface class 
class Application 
{
    
public:
    Application(const char* address,
		const int port,
		const int n_workers,
		const bool secure);
    
    ~Application();

    // TODO: Check if it is correct
    virtual void on_switch_up ();
    virtual void on_switch_down();
    virtual void handler();
    
private:

    void set_rack_pointer();
    
    Controller* controller_;
    //auto switches_rack_;
};

} // namespace derailleur

#endif // _APPLICATION_HPP_










