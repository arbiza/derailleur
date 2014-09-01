
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

#include <memory>
#include <fluid/OFServer.hh>

//#include "project_includes"

namespace derailleur {

// It's an interface class 
class Application 
{
    
public:
    Application();

    // TODO: Check if it is correct
    virtual void on_switch_up (
	std::shared_ptr<fluid_base::OFConnection> ofconn) override;
    virtual void on_switch_down() override;
    virtual void on_packet_in() override;
    
private:
    std::shared_ptr<fluid_base::OFConnection> ofconn_;
    
};

} // namespace derailleur

#endif // _APPLICATION_HPP_
