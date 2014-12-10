
#include <iostream>
#include "derailleur.hpp"
#include "learning_switch.hpp"

void LearningSwitch::on_switch_up ( const derailleur::Event* const event )
{
     derailleur::Switch13 s;
     get_switch_copy ( event->get_switch_id(), s );

     std::stringstream log;
     log << "on switch up - MAC " << s.get_mac_address();
     log << " - connection ID: " << event->get_switch_id();

     derailleur::Log::Instance()->log ( this->get_name().c_str(),
                                        log.str().c_str() );
}



void LearningSwitch::on_switch_down ( derailleur::Switch* switch_down )
{

     std::stringstream log;
     log << "switch ";
     log << switch_down->get_mac_address();
     log << ", ID ";
     log << switch_down->get_switch_id();
     log << " is disconnected.";

     derailleur::Log::Instance()->log ( this->get_name().c_str(),
                                        log.str().c_str() );
}



void LearningSwitch::on_packet_in ( const derailleur::Event* const event )
{


     /* learning_switch inherited method extracts link and internet
      * layers information from packet data, updates switches'
      * ARP-like tables (IPv4 and IPv6) and installs the proper
      * flow in the switch. */
     if ( this->learning_switch ( event, 15, 5 ) ) {

          std::vector<derailleur::Arp4> arp =
               get_IPv4_neighborhood ( event->get_switch_id() );

          std::cout << "\nn MACs: " << arp.size();


          for ( derailleur::Arp4 each : arp )
               std::cout << "\nMAC: "
                         << derailleur::util::MAC_converter ( each.mac )
                         << " IP: "
                         << derailleur::util::ipv4_converter ( each.ip )
                         << " porta: "
                         << ( int ) each.port
                         <<  std::endl;
     }

     derailleur::Log::Instance()->log ( this->get_name().c_str(),
                                        "packet-in" );

     delete event;
}


void LearningSwitch::message_handler ( const derailleur::Event* const event )
{

     derailleur::Log::Instance()->log ( this->get_name().c_str(),
                                        "unknown/unhandled message" );

     delete event;
}
