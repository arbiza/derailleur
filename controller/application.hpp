
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

//#include <system_includes>

//#include "project_includes"

namespace derailleur {

// It's an interface class 
class Application 
{
    
public:
    // Default constructor
    Application();

    virtual void on_switch_up() override;
    virtual void on_switch_down() override;
    virtual void on_packet_in() override;
    
private:
    
    
};

} // namespace derailleur

#endif // _APPLICATION_HPP_
